#include "common/IDebugLog.h"  // IDebugLog
#include "skse64/GameEvents.h"  // EventDispatcherList
#include "skse64/GameInput.h"  // InputEventDispatcher
#include "skse64/GameMenus.h"  // MenuManager
#include "skse64/PluginAPI.h"  // PluginHandle, SKSEMessagingInterface, SKSETaskInterface, SKSEInterface, PluginInfo
#include "skse64_common/BranchTrampoline.h"  // g_branchTrampoline
#include "skse64_common/skse_version.h"  // RUNTIME_VERSION

#include <ShlObj.h>  // CSIDL_MYDOCUMENTS

#include "Delegates.h"  // g_task
#include "Events.h"  // g_crosshairRefEventHandler, g_containerChangedEventHandler
#include "Hooks.h"  // installHooks()
#include "Keywords.h"  // initializeKeywords()
#include "LootMenu.h"  // LootMenuCreator
#include "Settings.h"  // Settings

#include "RE/EventDispatcherList.h"  // RE::EventDispatcherList
#include "RE/MenuManager.h"  // RE::MenuManager


static PluginHandle	g_pluginHandle = kPluginHandle_Invalid;
static SKSEMessagingInterface* g_messaging = 0;


void MessageHandler(SKSEMessagingInterface::Message* a_msg)
{
	switch (a_msg->type) {
	case SKSEMessagingInterface::kMessage_InputLoaded:
	{
		EventDispatcher<SKSECrosshairRefEvent>* crosshairRefDispatcher = (EventDispatcher<SKSECrosshairRefEvent>*)g_messaging->GetEventDispatcher(SKSEMessagingInterface::kDispatcher_CrosshairEvent);
		crosshairRefDispatcher->AddEventSink(&QuickLootRE::g_crosshairRefEventHandler);
		_MESSAGE("[MESSAGE] Crosshair ref event handler sinked");

		RE::EventDispatcherList* dispatcherList = RE::EventDispatcherList::GetEventDispatcherList();
		dispatcherList->containerChangedDispatcher.AddEventSink(&QuickLootRE::g_containerChangedEventHandler);
		_MESSAGE("[MESSAGE] Container changed event handler sinked");

		RE::MenuManager::GetSingleton()->Register("LootMenu", QuickLootRE::LootMenuCreator::Create);
		_MESSAGE("[MESSAGE] LootMenu registered");

		QuickLootRE::Settings::loadSettings();
		_MESSAGE("[MESSAGE] Settings loaded");

		break;
	}
	case SKSEMessagingInterface::kMessage_DataLoaded:
		if (!QuickLootRE::initalizeKeywords()) {
			_FATALERROR("[FATAL ERROR] Failed to initialize keywords!\n");
		} else {
			_MESSAGE("[MESSAGE] Keywords initialized\n");
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

		_MESSAGE("QuickLootRE");

		// populate info structure
		a_info->infoVersion = PluginInfo::kInfoVersion;
		a_info->name = "QuickLootRE";
		a_info->version = 1;

		// store plugin handle so we can identify ourselves later
		g_pluginHandle = a_skse->GetPluginHandle();

		if (a_skse->isEditor) {
			_FATALERROR("[FATAL ERROR] Loaded in editor, marking as incompatible!");
			return false;
		} else if (a_skse->runtimeVersion != RUNTIME_VERSION_1_5_53) {
			_FATALERROR("[FATAL ERROR] Unsupported runtime version %08X!", a_skse->runtimeVersion);
			return false;
		}

		// supported runtime version
		return true;
	}


	bool SKSEPlugin_Load(const SKSEInterface* a_skse)
	{
		_MESSAGE("[MESSAGE] QuickLootRE loaded");

		g_messaging = (SKSEMessagingInterface*)a_skse->QueryInterface(kInterface_Messaging);
		if (g_messaging->RegisterListener(g_pluginHandle, "SKSE", MessageHandler)) {
			_MESSAGE("[MESSAGE] Messaging interface registration successful!");
		} else {
			_FATALERROR("[FATAL ERROR] Messaging interface registration failed!");
			return false;
		}

		QuickLootRE::g_task = (SKSETaskInterface*)a_skse->QueryInterface(kInterface_Task);
		if (QuickLootRE::g_task) {
			_MESSAGE("[MESSAGE] Task interface query successful!");
		} else {
			_FATALERROR("[FATAL ERROR] Task interface query failed!");
			return false;
		}

		Hooks::installHooks();

		return true;
	}
};
