#include "Delegates.h"

#include <exception>  // exception
#include <memory>  // unique_ptr
#include <string>  // string

#include "Forms.h"
#include "InventoryList.h"  // g_invList
#include "LootMenu.h"  // LootMenu
#include "Settings.h"  // Settings
#include "Utility.h"  // IsValidPickPocketTarget

#include "RE/Skyrim.h"


namespace
{
	LocalizationStrings::LocalizationStrings() :
		take(""),
		steal(""),
		takeAll(""),
		search("")
	{
		auto collection = RE::GameSettingCollection::GetSingleton();
		auto setting = collection->GetSetting("sTake");
		if (setting) {
			take = setting->GetString();
		}

		setting = collection->GetSetting("sSteal");
		if (setting) {
			steal = setting->GetString();
		}

		setting = collection->GetSetting("sTakeAll");
		if (setting) {
			takeAll = setting->GetString();
		}

		setting = collection->GetSetting("sSearch");
		if (setting) {
			search = setting->GetString();
		}
	}
}


void TaskDelegateBase::Dispose()
{
	delete this;
}


void UITaskDelegateBase::Dispose()
{
	delete this;
}


void SetKeyMappingsDelegate::Run()
{
	RE::GFxValue args[3];

	args[0].SetString(LootMenu::GetTakeMapping());
	args[1].SetString(LootMenu::GetTakeAllMapping());
	args[2].SetString(LootMenu::GetSearchMapping());

	LootMenu::GetSingleton()->view->Invoke("_root.Menu_mc.SetKeyMappings", 0, args, 3);
}


void SetPlatformDelegate::Run()
{
	LootMenu* loot = LootMenu::GetSingleton();
	if (!loot->IsOpen()) {
		return;
	}

	RE::GFxValue args[2];

	args[0].SetNumber(to_underlying(loot->GetPlatform()));
	args[1].SetBoolean(false);

	loot->view->Invoke("_root.Menu_mc.SetPlatform", 0, args, 2);
}


void SetSelectedIndexDelegate::Run()
{
	LootMenu* loot = LootMenu::GetSingleton();
	if (!loot->IsOpen()) {
		return;
	}

	RE::GFxValue args[1];

	args[0].SetNumber(loot->GetSelectedIndex());

	loot->view->Invoke("_root.Menu_mc.SetSelectedIndex", 0, args, 1);
}


void SetupDelegate::Run()
{
	LootMenu* loot = LootMenu::GetSingleton();

	RE::GFxValue args[4];

	RE::GFxMovieDef* def = loot->view->GetMovieDef();

	double x = Settings::positionX;
	double y = Settings::positionY;
	double scale = Settings::scale;
	double opacity = Settings::opacity;

	x = (0 <= x && x <= 100) ? (x * def->GetWidth() * 0.01) : -1;
	y = (0 <= y && y <= 100) ? (y * def->GetHeight() * 0.01) : -1;
	if (scale >= 0) {
		if (scale < 25) {
			scale = 25;
		} else if (scale > 400) {
			scale = 400;
		}
	}
	if (opacity >= 0) {
		if (opacity > 100) {
			opacity = 100;
		}
	}

	args[0].SetNumber(x);
	args[1].SetNumber(y);
	args[2].SetNumber(scale);
	args[3].SetNumber(opacity);

	Settings::isApplied = true;

	loot->view->Invoke("_root.Menu_mc.Setup", 0, args, 4);
}


void SetContainerDelegate::Run()
{
	static LocalizationStrings locStrings;

	LootMenu* loot = LootMenu::GetSingleton();
	RE::TESObjectREFR* ref = loot->GetContainerRef();
	if (!ref) {
		return;
	}

	RE::GFxValue args[6];

	const char* takeText;
	if (IsValidPickPocketTarget(ref, RE::PlayerCharacter::GetSingleton()->IsSneaking()) || ref->IsOffLimits()) {
		takeText = locStrings.steal.c_str();
	} else {
		takeText = locStrings.take.c_str();
	}

	const char* searchText = Settings::disableActiTextHook ? locStrings.search.c_str() : loot->GetActiText();

	args[0].SetNumber(ref->formID);
	args[1].SetString(ref->GetReferenceName());
	args[2].SetString(takeText);
	args[3].SetString(locStrings.takeAll.c_str());
	args[4].SetString(searchText);
	args[5].SetNumber(loot->GetSelectedIndex());

	loot->view->Invoke("_root.Menu_mc.SetContainer", 0, args, 6);
}


void OpenContainerDelegate::Run()
{
	LootMenu* loot = LootMenu::GetSingleton();
	if (!loot) {
		return;
	}

	RE::GFxValue args[1];
	loot->view->CreateArray(&args[0]);
	InventoryList& invList = loot->GetInventoryList();
	SInt32 size = (invList.size() < Settings::itemLimit) ? invList.size() : Settings::itemLimit;

	std::unique_ptr<RE::GFxValue[]> item(new RE::GFxValue[size]);
	std::unique_ptr<RE::GFxValue[]> text(new RE::GFxValue[size]);
	std::unique_ptr<RE::GFxValue[]> count(new RE::GFxValue[size]);
	std::unique_ptr<RE::GFxValue[]> isStolen(new RE::GFxValue[size]);
	std::unique_ptr<RE::GFxValue[]> isEnchanted(new RE::GFxValue[size]);
	std::unique_ptr<RE::GFxValue[]> isRead(new RE::GFxValue[size]);
	std::unique_ptr<RE::GFxValue[]> itemChance(new RE::GFxValue[size]);
	std::unique_ptr<RE::GFxValue[]> iconLabel(new RE::GFxValue[size]);

	SInt32 displaySize = 0;
	for (SInt32 i = 0; i < size; ++i) {
		if (!invList[i].canPickPocket()) {
			continue;
		}

		loot->view->CreateObject(&item[i]);

		text[i].SetString(invList[i].name());
		count[i].SetNumber(invList[i].count());
		isStolen[i].SetBoolean(invList[i].isStolen());
		isEnchanted[i].SetBoolean(invList[i].isEnchanted());
		isRead[i].SetBoolean(invList[i].isRead());
		itemChance[i].SetNumber(invList[i].pickPocketChance());
		iconLabel[i].SetString(invList[i].icon());

		item[i].SetMember("text", text[i]);
		item[i].SetMember("count", count[i]);
		item[i].SetMember("isStolen", isStolen[i]);
		item[i].SetMember("isEnchanted", isEnchanted[i]);
		item[i].SetMember("isRead", isRead[i]);
		item[i].SetMember("itemChance", itemChance[i]);
		item[i].SetMember("iconLabel", iconLabel[i]);

		args[0].PushBack(item[i]);
		++displaySize;
	}
	loot->SetDisplaySize(displaySize);
	loot->ModSelectedIndex(0);

	if (Settings::disableIfEmpty && displaySize <= 0) {
		loot->Close();
	} else {
		if (!Settings::disableOnActiDispatch) {
			RE::ScriptEventSourceHolder* sourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
			RE::TESObjectREFRPtr target(loot->GetContainerRef());
			RE::TESObjectREFRPtr caster(RE::PlayerCharacter::GetSingleton());
			sourceHolder->SendActivateEvent(target, caster);
		}
		loot->view->Invoke("_root.Menu_mc.OpenContainer", 0, args, 1);
	}

#if ITEM_DATA_DEBUG_ENABLED
	DebugContents();
#endif
}


void OpenContainerDelegate::DebugContents()
{
	UInt32 i = 0;
	bool div = false;
	_DMESSAGE("");
	InventoryList& invList = LootMenu::GetSingleton()->GetInventoryList();
	for (auto& item : invList) {
		if (!div && !item.canPickPocket()) {
			_DMESSAGE("========== HIDDEN ITEMS ==========");
			div = true;
		}
		item.dbgDumpType(i);
		++i;
	}
}


void CloseContainerDelegate::Run()
{
	LootMenu::GetSingleton()->view->Invoke("_root.Menu_mc.CloseContainer", 0, 0, 0);
}


void UpdateButtonsDelegate::Run()
{
	LootMenu* loot = LootMenu::GetSingleton();
	if (loot->IsOpen()) {
		loot->view->Invoke("_root.Menu_mc.UpdateButtons", 0, 0, 0);
	}
}


void HideButtonsDelegate::Run()
{
	LootMenu* loot = LootMenu::GetSingleton();
	if (loot->IsOpen()) {
		RE::GFxValue args[3];

		args[0].SetBoolean(false);
		args[1].SetBoolean(true);
		args[2].SetBoolean(false);

		loot->view->Invoke("_root.Menu_mc.HideButtons", 0, args, 3);
	}
}


void SwitchStyleDelegate::Run()
{
	RE::GFxValue args[1];

	LootMenu* loot = LootMenu::GetSingleton();
	args[0].SetNumber(loot->GetStyle());

	loot->view->Invoke("_root.Menu_mc.SwitchStyle", 0, args, 1);
}


void DelayedUpdater::Run()
{
	LootMenu* loot = LootMenu::GetSingleton();
	if (loot->IsVisible()) {
		if (loot->GetContainerRef()) {
			loot->ParseInventory();
			loot->Register(LootMenu::Scaleform::kOpenContainer);
		}
	}
}
