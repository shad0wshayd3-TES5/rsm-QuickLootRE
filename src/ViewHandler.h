#pragma once

#include "Input/InputDisablers.h"
#include "Input/InputListeners.h"

class ViewHandler :
	RE::BSTEventSink<RE::MenuOpenCloseEvent>
{
private:
	using super = RE::BSTEventSink<RE::MenuOpenCloseEvent>;

public:
	ViewHandler() = delete;
	ViewHandler(const ViewHandler&) = default;
	ViewHandler(ViewHandler&&) = default;

	inline ViewHandler(RE::GPtr<RE::GFxMovieView> a_view) :
		super(),
		_view(std::move(a_view)),
		_disablers(),
		_listeners(),
		_enabled(false)
	{
		assert(_view != nullptr);
		Register();
		Evaluate();
	}

	inline ~ViewHandler() { Unregister(); }

	ViewHandler& operator=(const ViewHandler&) = default;
	ViewHandler& operator=(ViewHandler&&) = default;

protected:
	using EventResult = RE::BSEventNotifyControl;

	inline EventResult ProcessEvent(const RE::MenuOpenCloseEvent*, RE::BSTEventSource<RE::MenuOpenCloseEvent>*) override
	{
		Evaluate();
		return EventResult::kContinue;
	}

private:
	inline void Register()
	{
		auto source = RE::UI::GetSingleton();
		if (source) {
			source->AddEventSink(this);
		}
	}

	inline void Unregister()
	{
		auto source = RE::UI::GetSingleton();
		if (source) {
			source->RemoveEventSink(this);
		}
	}

	inline void Evaluate()
	{
		auto ui = RE::UI::GetSingleton();
		if (ui) {
			if (ui->GameIsPaused()) {
				Disable();
			} else {
				Enable();
			}
		}
	}

	inline void Enable()
	{
		if (!_enabled) {
			_view->SetVisible(true);
			_disablers.Enable();
			_listeners.Enable();
			_enabled = true;
		}
	}

	inline void Disable()
	{
		if (_enabled) {
			_view->SetVisible(false);
			_disablers.Disable();
			_listeners.Disable();
			_enabled = false;
		}
	}

	RE::GPtr<RE::GFxMovieView> _view;
	Input::Disablers _disablers;
	Input::Listeners _listeners;
	bool _enabled;
};
