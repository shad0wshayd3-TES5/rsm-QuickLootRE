#include "skse64/GameEvents.h"  // EventDispatcherList
#include "skse64/GameInput.h"  // InputEventDispatcher
#include "skse64/GameMenus.h"  // MenuManager
#include "skse64/PluginAPI.h"  // PluginHandle, SKSEMessagingInterface, SKSETaskInterface, SKSEInterface, PluginInfo
#include "skse64_common/BranchTrampoline.h"  // g_branchTrampoline
#include "skse64_common/skse_version.h"  // RUNTIME_VERSION

#include <ShlObj.h>  // CSIDL_MYDOCUMENTS

#include "Delegates.h"  // g_task
#include "Events.h"  // g_crosshairRefEventHandler, g_containerChangedEventHandler
#include "Forms.h"  // initalizeForms()
#include "Hooks.h"  // installHooks()
#include "ItemData.h"  // SetCompareOrder()
#include "LootMenu.h"  // LootMenuCreator
#include "Settings.h"  // Settings
#include "version.h"

#include "HookShare.h"  // _RegisterHook_t

#include "RE/EventSourceList.h"  // EventSourceList
#include "RE/InputEventDispatcher.h"  // InputEventDispatcher
#include "RE/MenuManager.h"  // MenuManager


static PluginHandle	g_pluginHandle = kPluginHandle_Invalid;
static SKSEMessagingInterface* g_messaging = 0;


void HooksReady(SKSEMessagingInterface::Message* a_msg)
{
	using HookShare::_RegisterHook_t;
	using QuickLootRE::LootMenu;

	if (a_msg->type == HOOK_SHARE_API_VERSION_MAJOR) {
		_RegisterHook_t* _RegisterHook = static_cast<_RegisterHook_t*>(a_msg->data);
		Hooks::InstallHooks(_RegisterHook);
		_MESSAGE("[MESSAGE] Hooks registered");
	} else {
		_FATALERROR("[FATAL ERROR] An incompatible version of Hook Share SSE was loaded! Expected (%i), found (%i)!\n", HOOK_SHARE_API_VERSION_MAJOR, a_msg->type);
		LootMenu::QueueMessage(LootMenu::kMessage_HookShareIncompatible);
	}
}


void MessageHandler(SKSEMessagingInterface::Message* a_msg)
{
	using QuickLootRE::LootMenu;

	switch (a_msg->type) {
	case SKSEMessagingInterface::kMessage_PostPostLoad:
		if (g_messaging->RegisterListener(g_pluginHandle, "HookShareSSE", HooksReady)) {
			_MESSAGE("[MESSAGE] Registered HookShareSSE listener");
		} else {
			_FATALERROR("[FATAL ERROR] HookShareSSE not loaded!\n");
			LootMenu::QueueMessage(LootMenu::kMessage_HookShareMissing);
		}
		break;
	case SKSEMessagingInterface::kMessage_InputLoaded:
	{
		EventDispatcher<SKSECrosshairRefEvent>* crosshairRefDispatcher = (EventDispatcher<SKSECrosshairRefEvent>*)g_messaging->GetEventDispatcher(SKSEMessagingInterface::kDispatcher_CrosshairEvent);
		crosshairRefDispatcher->AddEventSink(&QuickLootRE::g_crosshairRefEventHandler);
		_MESSAGE("[MESSAGE] Crosshair ref event handler sinked");

		RE::InputEventDispatcher::GetSingleton()->AddEventSink(&QuickLootRE::g_inputEventHandler);
		_MESSAGE("[MESSAGE] Input event handler sinked");

		RE::MenuManager::GetSingleton()->GetMenuOpenCloseEventSource()->AddEventSink(&QuickLootRE::g_menuOpenCloseEventHandler);
		_MESSAGE("[MESSAGE] Menu open/close event handler sinked");

		RE::EventSourceList* sourceList = RE::EventSourceList::GetEventSourceList();
		sourceList->combatEventSource.AddEventSink(&QuickLootRE::g_combatEventHandler);
		_MESSAGE("[MESSAGE] Combat event handler sinked");

		sourceList->containerChangedEventSource.AddEventSink(&QuickLootRE::g_containerChangedEventHandler);
		_MESSAGE("[MESSAGE] Container changed event handler sinked");

		RE::MenuManager::GetSingleton()->Register("LootMenu", QuickLootRE::LootMenuCreator::Create);
		_MESSAGE("[MESSAGE] LootMenu registered");

		QuickLootRE::ItemData::setCompareOrder();
		_MESSAGE("[MESSAGE] Settings applied");

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

		_MESSAGE("QuickLootRE v%i.%i.%i.%i", QUICKLOOTRE_VERSION_MAJOR, QUICKLOOTRE_VERSION_MINOR, QUICKLOOTRE_VERSION_PATCH, QUICKLOOTRE_VERSION_BETA);

		a_info->infoVersion = PluginInfo::kInfoVersion;
		a_info->name = "QuickLootRE";
		a_info->version = 1;

		g_pluginHandle = a_skse->GetPluginHandle();

		if (a_skse->isEditor) {
			_FATALERROR("[FATAL ERROR] Loaded in editor, marking as incompatible!");
			return false;
		}

		if (a_skse->runtimeVersion != RUNTIME_VERSION_1_5_53) {
			_FATALERROR("[FATAL ERROR] Unsupported runtime version %08X!", a_skse->runtimeVersion);
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

		QuickLootRE::g_task = (SKSETaskInterface*)a_skse->QueryInterface(kInterface_Task);
		if (QuickLootRE::g_task) {
			_MESSAGE("[MESSAGE] Task interface query successful");
		} else {
			_FATALERROR("[FATAL ERROR] Task interface query failed!\n");
			return false;
		}

		if (QuickLootRE::Settings::loadSettings()) {
			_MESSAGE("[MESSAGE] Settings successfully loaded");
		} else {
			_FATALERROR("[FATAL ERROR] Settings failed to load!\n");
			return false;
		}

		return true;
	}
};
