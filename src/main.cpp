#include "RE/Skyrim.h"
#include "SKSE/API.h"

#include "Events.h"
#include "LootMenu.h"
#include "Scaleform.h"
#include "version.h"


namespace
{
	class InputHandler : public RE::BSTEventSink<RE::InputEvent*>
	{
	public:
		using EventResult = RE::BSEventNotifyControl;


		static InputHandler* GetSingleton()
		{
			static InputHandler singleton;
			return &singleton;
		}


		static void Register()
		{
			auto input = RE::BSInputDeviceManager::GetSingleton();
			input->AddEventSink(GetSingleton());
			_MESSAGE("Registered InputHandler");
		}


		virtual EventResult ProcessEvent(RE::InputEvent* const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::InputEvent*>* a_eventSource) override
		{
			using InputType = RE::INPUT_EVENT_TYPE;
			using Key = RE::BSKeyboardDevice::Key;
			using RemoveType = RE::ITEM_REMOVE_REASON;

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

				switch (button->idCode) {
				case Key::kNum0:
					//Scaleform::LootMenu::Open();
					break;
				case Key::kNum9:
					//Scaleform::LootMenu::Close();
					break;
				}
			}

			return EventResult::kContinue;
		}

	private:
		InputHandler() = default;
		InputHandler(const InputHandler&) = delete;
		InputHandler(InputHandler&&) = delete;
		virtual ~InputHandler() = default;

		InputHandler& operator=(const InputHandler&) = delete;
		InputHandler& operator=(InputHandler&&) = delete;
	};


	void MessageHandler(SKSE::MessagingInterface::Message* a_msg)
	{
		switch (a_msg->type) {
		case SKSE::MessagingInterface::kDataLoaded:
			//InputHandler::Register();
			Events::Register();
			Scaleform::Register();
			break;
		}
	}
}


extern "C"
{
	bool SKSEPlugin_Query(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_info)
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


	bool SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
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
};
