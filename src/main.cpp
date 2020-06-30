#include "Events/Events.h"
#include "HUDHandler.h"
#include "Input/Input.h"
#include "Loot.h"
#include "Scaleform/LootMenu.h"
#include "Scaleform/Scaleform.h"
#include "version.h"

class InputHandler :
	public RE::BSTEventSink<RE::InputEvent*>
{
public:
	static InputHandler* GetSingleton()
	{
		static InputHandler singleton;
		return std::addressof(singleton);
	}

	static void Register()
	{
		auto input = RE::BSInputDeviceManager::GetSingleton();
		input->AddEventSink(GetSingleton());
		_MESSAGE("Registered InputHandler");
	}

protected:
	using EventResult = RE::BSEventNotifyControl;

	EventResult ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>*) override
	{
		using InputType = RE::INPUT_EVENT_TYPE;
		using Keyboard = RE::BSWin32KeyboardDevice::Key;

		if (!a_event) {
			return EventResult::kContinue;
		}

		auto uiStr = RE::InterfaceStrings::GetSingleton();
		auto ui = RE::UI::GetSingleton();
		if (ui->IsMenuOpen(uiStr->console)) {
			return EventResult::kContinue;
		}

		for (auto event = *a_event; event; event = event->next) {
			if (event->eventType != InputType::kButton) {
				continue;
			}

			auto button = static_cast<RE::ButtonEvent*>(event);
			if (!button->IsDown() || button->device != RE::INPUT_DEVICE::kKeyboard) {
				continue;
			}

			auto loot = Loot::GetSingleton();
			switch (button->idCode) {
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

private:
	InputHandler() = default;
	InputHandler(const InputHandler&) = delete;
	InputHandler(InputHandler&&) = delete;

	~InputHandler() = default;

	InputHandler& operator=(const InputHandler&) = delete;
	InputHandler& operator=(InputHandler&&) = delete;
};

void MessageHandler(SKSE::MessagingInterface::Message* a_msg)
{
	switch (a_msg->type) {
	case SKSE::MessagingInterface::kDataLoaded:
		InputHandler::Register();
		HUDHandler::Install();
		Events::Register();
		Scaleform::Register();
		Input::HookControlMap();
		break;
	}
}

extern "C" DLLEXPORT bool APIENTRY SKSEPlugin_Query(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_info)
{
	SKSE::Logger::OpenRelative(FOLDERID_Documents, L"\\My Games\\Skyrim Special Edition\\SKSE\\QuickLootRE.log");
	SKSE::Logger::SetPrintLevel(SKSE::Logger::Level::kDebugMessage);
	SKSE::Logger::SetFlushLevel(SKSE::Logger::Level::kDebugMessage);
	SKSE::Logger::UseLogStamp(true);

	_MESSAGE("QuickLootRE v%s", QKLT_VERSION_VERSTRING);

	a_info->infoVersion = SKSE::PluginInfo::kVersion;
	a_info->name = "QuickLootRE";
	a_info->version = QKLT_VERSION_MAJOR;

	if (a_skse->IsEditor()) {
		_FATALERROR("Loaded in editor, marking as incompatible!\n");
		return false;
	}

	auto ver = a_skse->RuntimeVersion();
	if (ver < SKSE::RUNTIME_1_5_39) {
		_FATALERROR("Unsupported runtime version %s!\n", ver.GetString().c_str());
		return false;
	}

	return true;
}

extern "C" DLLEXPORT bool APIENTRY SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
	_MESSAGE("QuickLootRE loaded");

	if (!SKSE::Init(a_skse)) {
		return false;
	}

	auto message = SKSE::GetMessagingInterface();
	if (!message->RegisterListener(MessageHandler)) {
		return false;
	}

	return true;
}
