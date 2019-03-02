#include "skse64/PluginAPI.h"  // PluginHandle, SKSEMessagingInterface, SKSETaskInterface, SKSEInterface, PluginInfo
#include "skse64_common/BranchTrampoline.h"  // g_branchTrampoline
#include "skse64_common/skse_version.h"  // RUNTIME_VERSION

#include <ShlObj.h>  // CSIDL_MYDOCUMENTS

#include "Delegates.h"  // g_task
#include "Events.h"  // CrosshairRefEventHandler, InputEventHandler, MenuOpenCloseEventHandler, TESCombatEventHandler, TESContainerChangedEventHandler, TESMagicEffectApplyEventHandler
#include "Hooks.h"  // InstallHooks
#include "ItemData.h"  // SetCompareOrder
#include "LootMenu.h"  // LootMenuCreator
#include "Settings.h"  // Settings
#include "version.h"  // HOOK_SHARE_API_VERSION_MAJOR

#include "HookShare.h"  // _RegisterHook_t

#include "RE/InputManager.h"  // InputManager
#include "RE/MenuManager.h"  // MenuManager
#include "RE/ScriptEventSourceHolder.h"  // ScriptEventSourceHolder
#include "RE/TESDataHandler.h"  // TESDataHandler


static PluginHandle	g_pluginHandle = kPluginHandle_Invalid;
static SKSEMessagingInterface* g_messaging = 0;


void HooksReady(SKSEMessagingInterface::Message* a_msg)
{
	using HookShare::_RegisterForCanProcess_t;

	switch (a_msg->type) {
	case HookShare::kType_CanProcess:
		if (a_msg->dataLen == HOOK_SHARE_API_VERSION_MAJOR) {
			_RegisterForCanProcess_t* _RegisterForCanProcess = static_cast<_RegisterForCanProcess_t*>(a_msg->data);
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
		if (g_messaging->RegisterListener(g_pluginHandle, "HookShareSSE", HooksReady)) {
			_MESSAGE("[MESSAGE] Registered HookShareSSE listener");
		} else {
			_FATALERROR("[FATAL ERROR] HookShareSSE not loaded!\n");
			LootMenu::QueueMessage(LootMenu::Message::kHookShareMissing);
		}
		break;
	case SKSEMessagingInterface::kMessage_InputLoaded:
		{
			EventDispatcher<SKSECrosshairRefEvent>* crosshairRefDispatcher = (EventDispatcher<SKSECrosshairRefEvent>*)g_messaging->GetEventDispatcher(SKSEMessagingInterface::kDispatcher_CrosshairEvent);
			crosshairRefDispatcher->AddEventSink(Events::CrosshairRefEventHandler::GetSingleton());
			_MESSAGE("[MESSAGE] Crosshair ref event handler sinked");

			RE::InputManager::GetSingleton()->AddEventSink(Events::InputEventHandler::GetSingleton());
			_MESSAGE("[MESSAGE] Input event handler sinked");

			RE::MenuManager::GetSingleton()->GetMenuOpenCloseEventSource()->AddEventSink(Events::MenuOpenCloseEventHandler::GetSingleton());
			_MESSAGE("[MESSAGE] Menu open/close event handler sinked");

			RE::ScriptEventSourceHolder* sourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
			sourceHolder->combatEventSource.AddEventSink(Events::TESCombatEventHandler::GetSingleton());
			_MESSAGE("[MESSAGE] Combat event handler sinked");

			sourceHolder->containerChangedEventSource.AddEventSink(Events::TESContainerChangedEventHandler::GetSingleton());
			_MESSAGE("[MESSAGE] Container changed event handler sinked");

			RE::MenuManager::GetSingleton()->Register("LootMenu", []() -> RE::IMenu*
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
			RE::TESDataHandler* dataHandler = RE::TESDataHandler::GetSingleton();
			if (dataHandler->GetLoadedModIndex("SkyUI_SE.esp") != 0xFF) {
				_MESSAGE("[MESSAGE] SkyUI is loaded");
			} else {
				_FATALERROR("[FATAL ERROR] SkyUI is not loaded!\n");
			}

			LootMenu::GetSingleton();
		}
		break;
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

		g_pluginHandle = a_skse->GetPluginHandle();

		if (a_skse->isEditor) {
			_FATALERROR("[FATAL ERROR] Loaded in editor, marking as incompatible!\n");
			return false;
		}

		if (a_skse->runtimeVersion != RUNTIME_VERSION_1_5_62) {
			_FATALERROR("[FATAL ERROR] Unsupported runtime version %08X!\n", a_skse->runtimeVersion);
			return false;
		}

		if (g_branchTrampoline.Create(1024 * 8)) {
			_MESSAGE("[MESSAGE] Branch trampoline creation successful");
		} else {
			_FATALERROR("[FATAL ERROR] Branch trampoline creation failed!\n");
			return false;
		}

		return true;
	}


	bool SKSEPlugin_Load(const SKSEInterface* a_skse)
	{
		_MESSAGE("[MESSAGE] QuickLootRE loaded");

		g_messaging = (SKSEMessagingInterface*)a_skse->QueryInterface(kInterface_Messaging);
		if (g_messaging->RegisterListener(g_pluginHandle, "SKSE", MessageHandler)) {
			_MESSAGE("[MESSAGE] Messaging interface registration successful");
		} else {
			_FATALERROR("[FATAL ERROR] Messaging interface registration failed!\n");
			return false;
		}

		g_task = (SKSETaskInterface*)a_skse->QueryInterface(kInterface_Task);
		if (g_task) {
			_MESSAGE("[MESSAGE] Task interface query successful");
		} else {
			_FATALERROR("[FATAL ERROR] Task interface query failed!\n");
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
