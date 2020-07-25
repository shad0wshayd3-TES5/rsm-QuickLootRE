#pragma once

#include "Animation/Animation.h"
#include "Input/InputDisablers.h"
#include "Input/InputListeners.h"

class ViewHandler :
	public RE::BSTEventSink<RE::MenuOpenCloseEvent>,
	public Animation::IEventSink
{
public:
	ViewHandler() = delete;
	ViewHandler(const ViewHandler&) = default;
	ViewHandler(ViewHandler&&) = default;

	inline ViewHandler(observer<RE::IMenu*> a_menu, RE::ActorHandle a_dst) :
		_menu(a_menu),
		_view(a_menu ? a_menu->uiMovie : nullptr),
		_dst(a_dst)
	{
		assert(_menu != nullptr);
		assert(_view != nullptr);

		SetVisible(false);
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

	inline void OnAnimationEvent() override { Evaluate(); }

private:
	enum class Priority : std::size_t
	{
		kDefault,
		kLowest
	};

	inline void Register()
	{
		auto menuSrc = RE::UI::GetSingleton();
		if (menuSrc) {
			menuSrc->AddEventSink(this);
		}
	}

	inline void Unregister()
	{
		auto menuSrc = RE::UI::GetSingleton();
		if (menuSrc) {
			menuSrc->RemoveEventSink(this);
		}
	}

	inline void Evaluate()
	{
		const auto controlMap = RE::ControlMap::GetSingleton();
		const auto menuControls = RE::MenuControls::GetSingleton();
		const auto dst = _dst.get();
		if (controlMap && menuControls) {
			const auto& priorityStack = controlMap->contextPriorityStack;
			if (dst->IsInKillMove() ||
				dst->GetOccupiedFurniture() ||
				menuControls->InBeastForm() ||
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
				SetVisible(true);
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
				SetVisible(false);
				_disablers.Disable();
				_listeners.Disable();
				_enabled = false;
			}
		});
	}

	inline void SetVisible(bool a_visible)
	{
		if (_view) {
			const auto prev = _view->GetVisible();
			if (prev != a_visible) {
				_view->SetVisible(a_visible);

				if (a_visible) {
					RefreshInventory();	 // TODO: Queue full UI refresh
				}
			}
		} else {
			assert(false);
		}
	}

	void AdjustPriority(Priority a_priority);
	void RefreshInventory();

	observer<RE::IMenu*> _menu;
	RE::GPtr<RE::GFxMovieView> _view;
	Input::Disablers _disablers;
	Input::Listeners _listeners;
	RE::ActorHandle _dst;
	bool _enabled{ false };
};
