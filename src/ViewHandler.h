#pragma once

#include "Animation/Animation.h"
#include "Input/InputDisablers.h"
#include "Input/InputListeners.h"

class ViewHandler :
	public RE::BSTEventSink<RE::MenuOpenCloseEvent>,
	public Animation::IEventSink
{
private:
	using super = RE::BSTEventSink<RE::MenuOpenCloseEvent>;

public:
	ViewHandler() = delete;
	ViewHandler(const ViewHandler&) = default;
	ViewHandler(ViewHandler&&) = default;

	inline ViewHandler(observer<RE::IMenu*> a_menu) :
		_menu(a_menu),
		_view(a_menu->uiMovie)
	{
		assert(_menu != nullptr);
		assert(_view != nullptr);

		_view->SetVisible(false);
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

	inline void ProcessEvent(Animation::Type a_type) override
	{
		switch (a_type) {
		case Animation::Type::kKillMoveStart:
			_activeAnimations.set(kKillMove, true);
			break;
		case Animation::Type::kKillMoveEnd:
			_activeAnimations.set(kKillMove, false);
			break;
		default:
			assert(false);
			break;
		}

		Evaluate();
	}

private:
	enum class Priority : std::size_t
	{
		kDefault,
		kLowest
	};

	enum Anim : std::size_t
	{
		kKillMove,
		kTotal
	};

	inline void Register()
	{
		auto menuSrc = RE::UI::GetSingleton();
		if (menuSrc) {
			menuSrc->AddEventSink(this);
		}

		auto animSrc = Animation::AnimationManager::GetSingleton();
		animSrc->SetEventSink(this);
	}

	inline void Unregister()
	{
		auto menuSrc = RE::UI::GetSingleton();
		if (menuSrc) {
			menuSrc->RemoveEventSink(this);
		}

		auto animSrc = Animation::AnimationManager::GetSingleton();
		animSrc->SetEventSink(nullptr);
	}

	inline void Evaluate()
	{
		auto controlMap = RE::ControlMap::GetSingleton();
		if (controlMap) {
			const auto& priorityStack = controlMap->contextPriorityStack;
			if (_activeAnimations.any() ||
				priorityStack.empty() ||
				priorityStack.back() != RE::UserEvents::INPUT_CONTEXT_ID::kGameplay) {
				Disable();
			} else {
				Enable();
			}
		}
	}

	inline void Enable()
	{
		RE::GPtr safety{ _menu };
		auto task = SKSE::GetTaskInterface();
		task->AddUITask([this, safety]() {
			if (!_enabled) {
				AdjustPriority(Priority::kDefault);
				_view->SetVisible(true);
				_disablers.Enable();
				_listeners.Enable();
				_enabled = true;
			}
		});
	}

	inline void Disable()
	{
		RE::GPtr safety{ _menu };
		auto task = SKSE::GetTaskInterface();
		task->AddUITask([this, safety]() {
			if (_enabled) {
				AdjustPriority(Priority::kLowest);
				_view->SetVisible(false);
				_disablers.Disable();
				_listeners.Disable();
				_enabled = false;
			}
		});
	}

	void AdjustPriority(Priority a_priority);

	observer<RE::IMenu*> _menu;
	RE::GPtr<RE::GFxMovieView> _view;
	std::bitset<Anim::kTotal> _activeAnimations;
	Input::Disablers _disablers;
	Input::Listeners _listeners;
	bool _enabled{ false };
};
