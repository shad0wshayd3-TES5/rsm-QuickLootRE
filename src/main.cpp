#include "skse64/PluginAPI.h"  // SKSEMessagingInterface, SKSEInterface, PluginInfo
#include "skse64_common/BranchTrampoline.h"  // g_branchTrampoline
#include "skse64_common/skse_version.h"  // RUNTIME_VERSION

#include <ShlObj.h>  // CSIDL_MYDOCUMENTS

#include "Events.h"  // CrosshairRefEventHandler, InputEventHandler, MenuOpenCloseEventHandler, TESCombatEventHandler, TESContainerChangedEventHandler, TESMagicEffectApplyEventHandler
#include "Hooks.h"  // InstallHooks
#include "ItemData.h"  // SetCompareOrder
#include "LootMenu.h"  // LootMenuCreator
#include "Settings.h"  // Settings
#include "SKSEInterface.h"  // SKSE
#include "version.h"  // HOOK_SHARE_API_VERSION_MAJOR

#include "HookShare.h"  // RegisterForCanProcess_t

#include "RE/Skyrim.h"


namespace
{
	void HooksReady(SKSEMessagingInterface::Message* a_msg)
	{
		using HookShare::RegisterForCanProcess_t;

		switch (a_msg->type) {
		case HookShare::kType_CanProcess:
			if (a_msg->dataLen == HOOK_SHARE_API_VERSION_MAJOR) {
				auto _RegisterForCanProcess = static_cast<RegisterForCanProcess_t*>(a_msg->data);
				Hooks::InstallHooks(_RegisterForCanProcess);
				_MESSAGE("[MESSAGE] Hooks registered");
			} else {
				_FATALERROR("[FATAL ERROR] An incompatible version of Hook Share SSE was loaded! Expected (%i), found (%i)!\n", HOOK_SHARE_API_VERSION_MAJOR, a_msg->type);
				LootMenu::QueueMessage(LootMenu::Message::kHookShareIncompatible);
			}
			break;
		}
	}


	void MessageHandler(SKSEMessagingInterface::Message* a_msg)
	{
		switch (a_msg->type) {
		case SKSEMessagingInterface::kMessage_PostPostLoad:
			{
				auto messaging = SKSE::GetMessagingInterface();
				if (messaging->RegisterListener(SKSE::GetPluginHandle(), "HookShareSSE", HooksReady)) {
					_MESSAGE("[MESSAGE] Registered HookShareSSE listener");
				} else {
					_FATALERROR("[FATAL ERROR] Failed to register HookShareSSE listener!\n");
					LootMenu::QueueMessage(LootMenu::Message::kHookShareMissing);
				}
			}
			break;
		case SKSEMessagingInterface::kMessage_InputLoaded:
			{
				auto messaging = SKSE::GetMessagingInterface();
				auto crosshairRefDispatcher = SKSE::GetCrosshairRefEventSource();
				crosshairRefDispatcher->AddEventSink(Events::CrosshairRefEventHandler::GetSingleton());
				_MESSAGE("[MESSAGE] Crosshair ref event handler sinked");

				auto inputManager = RE::InputManager::GetSingleton();
				inputManager->AddEventSink(Events::InputEventHandler::GetSingleton());
				_MESSAGE("[MESSAGE] Input event handler sinked");

				auto mm = RE::MenuManager::GetSingleton();
				mm->GetMenuOpenCloseEventSource()->AddEventSink(Events::MenuOpenCloseEventHandler::GetSingleton());
				_MESSAGE("[MESSAGE] Menu open/close event handler sinked");

				auto sourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
				sourceHolder->combatEventSource.AddEventSink(Events::TESCombatEventHandler::GetSingleton());
				_MESSAGE("[MESSAGE] Combat event handler sinked");

				sourceHolder->containerChangedEventSource.AddEventSink(Events::TESContainerChangedEventHandler::GetSingleton());
				_MESSAGE("[MESSAGE] Container changed event handler sinked");

				mm->Register("LootMenu", []() -> RE::IMenu*
				{
					return LootMenu::GetSingleton();
				});
				_MESSAGE("[MESSAGE] LootMenu registered");

				ItemData::setCompareOrder();
				_MESSAGE("[MESSAGE] Settings applied");
			}
			break;
		case SKSEMessagingInterface::kMessage_DataLoaded:
			{
				auto dataHandler = RE::TESDataHandler::GetSingleton();
				if (dataHandler->LookupModByName("SkyUI_SE.esp")) {
					_MESSAGE("[MESSAGE] SkyUI is installed");
				} else {
					_FATALERROR("[FATAL ERROR] SkyUI is not installed!\n");
				}

				LootMenu::GetSingleton();
			}
			break;
		}
	}
}


extern "C" {
	bool SKSEPlugin_Query(const SKSEInterface* a_skse, PluginInfo* a_info)
	{
		gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\Skyrim Special Edition\\SKSE\\QuickLootRE.log");
		gLog.SetPrintLevel(IDebugLog::kLevel_DebugMessage);
		gLog.SetLogLevel(IDebugLog::kLevel_DebugMessage);

		_MESSAGE("QuickLootRE v%s", QUICKLOOTRE_VERSION_VERSTRING);

		a_info->infoVersion = PluginInfo::kInfoVersion;
		a_info->name = "QuickLootRE";
		a_info->version = 1;

		if (!SKSE::Init(a_skse)) {
			return false;
		}

		if (a_skse->isEditor) {
			_FATALERROR("[FATAL ERROR] Loaded in editor, marking as incompatible!\n");
			return false;
		} else  if (a_skse->runtimeVersion != RUNTIME_VERSION_1_5_73) {
			_FATALERROR("[FATAL ERROR] Unsupported runtime version %08X!\n", a_skse->runtimeVersion);
			return false;
		}

		return true;
	}


	bool SKSEPlugin_Load(const SKSEInterface* a_skse)
	{
		_MESSAGE("[MESSAGE] QuickLootRE loaded");

		if (g_branchTrampoline.Create(1024 * 8)) {
			_MESSAGE("[MESSAGE] Branch trampoline creation successful");
		} else {
			_FATALERROR("[FATAL ERROR] Branch trampoline creation failed!\n");
			return false;
		}

		auto messaging = SKSE::GetMessagingInterface();
		if (messaging->RegisterListener(SKSE::GetPluginHandle(), "SKSE", MessageHandler)) {
			_MESSAGE("[MESSAGE] Registered SKSE listener");
		} else {
			_FATALERROR("[FATAL ERROR] Failed to register SKSE listener!\n");
			return false;
		}

		if (Settings::loadSettings()) {
			_MESSAGE("[MESSAGE] Settings successfully loaded");
		} else {
			_FATALERROR("[FATAL ERROR] Settings failed to load!\n");
			return false;
		}

		return true;
	}
};
