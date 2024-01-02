#include "Animation/Animation.h"
#include "Events/Events.h"
#include "Hooks.h"
#include "Input/Input.h"
#include "Loot.h"
#include "Scaleform/Scaleform.h"

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

namespace
{
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

	void InitializeLog()
	{
		auto path = logger::log_directory();
		if (!path)
		{
		stl::report_and_fail("Failed to find standard logging directory"sv);
		}

		*path /= fmt::format(FMT_STRING("{}.log"), Version::PROJECT);
		auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);

		auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));

		log->set_level(spdlog::level::info);
		log->flush_on(spdlog::level::info);

		spdlog::set_default_logger(std::move(log));
		spdlog::set_pattern("[%^%l%$] %v"s);

		logger::info(FMT_STRING("{:s} v{:s}"), Version::PROJECT, Version::NAME);
	}
}

extern "C" DLLEXPORT constinit auto SKSEPlugin_Version = []()
{
	SKSE::PluginVersionData v;
	v.PluginVersion(Version::MAJOR);
	v.PluginName(Version::PROJECT);
	v.AuthorName("shad0wshayd3");
	v.UsesAddressLibrary();
	v.UsesUpdatedStructs();
	v.CompatibleVersions({ SKSE::RUNTIME_LATEST });

	return v;
}();

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
	InitializeLog();
	logger::info(FMT_STRING("{:s} loaded"), Version::PROJECT);

	Settings::load();

	SKSE::Init(a_skse);
	SKSE::AllocTrampoline(1 << 6);

	auto message = SKSE::GetMessagingInterface();
	if (!message->RegisterListener(MessageHandler)) {
		return false;
	}

	Hooks::Install();

	return true;
}
