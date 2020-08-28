#include "Animation/Animation.h"
#include "Events/Events.h"
#include "Hooks.h"
#include "Input/Input.h"
#include "Loot.h"
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
		logger::info("Registered InputHandler"sv);
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

		auto intfcStr = RE::InterfaceStrings::GetSingleton();
		auto ui = RE::UI::GetSingleton();
		if (ui->IsMenuOpen(intfcStr->console)) {
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

			auto& loot = Loot::GetSingleton();
			switch (button->idCode) {
			case Keyboard::kNum0:
				loot.Enable();
				break;
			case Keyboard::kNum9:
				loot.Disable();
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
#ifndef NDEBUG
		InputHandler::Register();
#endif

		Animation::AnimationManager::Install();

		Events::Register();
		Scaleform::Register();
		break;
	}
}

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_info)
{
#ifndef NDEBUG
	auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
#else
	auto path = logger::log_directory();
	if (!path) {
		return false;
	}

	*path /= "QuicklootRE.log"sv;
	auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
#endif

	auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));

#ifndef NDEBUG
	log->set_level(spdlog::level::trace);
#else
	log->set_level(spdlog::level::info);
	log->flush_on(spdlog::level::warn);
#endif

	spdlog::set_default_logger(std::move(log));
	spdlog::set_pattern("%g(%#): [%^%l%$] %v"s);

	logger::info("QuickLootRE v{}"sv, QKLT_VERSION_VERSTRING);

	a_info->infoVersion = SKSE::PluginInfo::kVersion;
	a_info->name = "QuickLootRE";
	a_info->version = QKLT_VERSION_MAJOR;

	if (a_skse->IsEditor()) {
		logger::critical("Loaded in editor, marking as incompatible"sv);
		return false;
	}

	const auto ver = a_skse->RuntimeVersion();
	if (ver < SKSE::RUNTIME_1_5_39) {
		logger::critical("Unsupported runtime version {}"sv, ver.string());
		return false;
	}

	return true;
}

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
	logger::info("QuickLootRE loaded"sv);
	Settings::load();

	SKSE::Init(a_skse);
	SKSE::AllocTrampoline(1 << 4);

	auto message = SKSE::GetMessagingInterface();
	if (!message->RegisterListener(MessageHandler)) {
		return false;
	}

	Hooks::Install();

	return true;
}
