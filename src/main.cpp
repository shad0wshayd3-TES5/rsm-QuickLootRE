#include "Animation/Animation.h"
#include "Events/Events.h"
#include "Hooks.h"
#include "Input/Input.h"
#include "Loot.h"
#include "Scaleform/Scaleform.h"

class InputHandler :
	public RE::BSTEventSink<RE::InputEvent*>,
	public REX::TSingleton<InputHandler>
{
public:
	static void Register()
	{
		auto input = RE::BSInputDeviceManager::GetSingleton();
		input->AddEventSink(GetSingleton());
		REX::INFO("Registered InputHandler"sv);
	}

protected:
	using EventResult = RE::BSEventNotifyControl;

	EventResult ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>*) override
	{
		using InputType = RE::INPUT_EVENT_TYPE;
		using Keyboard = RE::BSWin32KeyboardDevice::Key;

		if (!a_event)
		{
			return EventResult::kContinue;
		}

		auto intfcStr = RE::InterfaceStrings::GetSingleton();
		auto ui = RE::UI::GetSingleton();
		if (ui->IsMenuOpen(intfcStr->console))
		{
			return EventResult::kContinue;
		}

		for (auto event = *a_event; event; event = event->next)
		{
			if (event->eventType != InputType::kButton)
			{
				continue;
			}

			auto button = static_cast<RE::ButtonEvent*>(event);
			if (!button->IsDown() || button->device != RE::INPUT_DEVICE::kKeyboard)
			{
				continue;
			}

			auto loot = Loot::GetSingleton();
			switch (button->idCode)
			{
			case Keyboard::kNum0:
				loot->Enable();
				break;
			case Keyboard::kNum9:
				loot->Disable();
				break;
			default:
				break;
			}
		}

		return EventResult::kContinue;
	}
};

namespace
{
	void MessageHandler(SKSE::MessagingInterface::Message* a_msg)
	{
		switch (a_msg->type)
		{
		case SKSE::MessagingInterface::kPostLoad:
		{
			Hooks::Install();
			break;
		}
		case SKSE::MessagingInterface::kDataLoaded:
		{
#ifndef NDEBUG
			InputHandler::Register();
#endif

			Animation::AnimationManager::Install();

			Events::Register();
			Scaleform::Register();
			break;
		}
		default:
			break;
		}
	}
}

SKSE_PLUGIN_LOAD(const SKSE::LoadInterface* a_skse)
{
	Settings::Load();

	SKSE::Init(a_skse, { .trampoline = true, .trampolineSize = 64 });
	SKSE::GetMessagingInterface()->RegisterListener(MessageHandler);

	return true;
}
