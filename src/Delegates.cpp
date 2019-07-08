#include "Delegates.h"

#include <memory>
#include <string>

#include "Forms.h"
#include "InventoryList.h"
#include "LootMenu.h"
#include "Settings.h"
#include "Utility.h"

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


SetSelectedIndexDelegate::SetSelectedIndexDelegate(SInt32 a_idx) :
	_idx(static_cast<double>(a_idx))
{}


void SetSelectedIndexDelegate::Run()
{
	auto loot = LootMenu::GetSingleton();
	if (!loot->IsOpen()) {
		return;
	}

	RE::GFxValue args[kTotal];

	args[kIdx].SetNumber(_idx);

	loot->view->Invoke("_root.Menu_mc.SetSelectedIndex", 0, args, kTotal);
}


void SetupDelegate::Run()
{
	RE::GFxValue args[kTotal];

	auto loot = LootMenu::GetSingleton();
	auto def = loot->view->GetMovieDef();

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

	args[kPositionX].SetNumber(x);
	args[kPositionY].SetNumber(y);
	args[kScale].SetNumber(scale);
	args[kOpacity].SetNumber(opacity);

	loot->view->Invoke("_root.Menu_mc.Setup", 0, args, kTotal);
}


SetContainerDelegate::SetContainerDelegate(SInt32 a_selectedIndex) :
	_selectedIndex(static_cast<double>(a_selectedIndex))
{}


void SetContainerDelegate::Run()
{
	static LocalizationStrings locStrings;

	auto loot = LootMenu::GetSingleton();
	auto ref = loot->GetContainerRef();
	if (!ref) {
		return;
	}

	RE::GFxValue args[kTotal];

	auto player = RE::PlayerCharacter::GetSingleton();
	const char* takeText;
	if (IsValidPickPocketTarget(ref, player->IsSneaking()) || ref->IsOffLimits()) {
		takeText = locStrings.steal.c_str();
	} else {
		takeText = locStrings.take.c_str();
	}

	auto searchText = Settings::disableActiTextHook ? locStrings.search.c_str() : loot->GetActiText();

	args[kTitle].SetString(ref->GetReferenceName());
	args[kTake].SetString(takeText);
	args[kTakeAll].SetString(locStrings.takeAll.c_str());
	args[kSearch].SetString(searchText);
	args[kSelectedIndex].SetNumber(_selectedIndex);

	loot->view->Invoke("_root.Menu_mc.SetContainer", 0, args, kTotal);
}


void OpenContainerDelegate::Run()
{
	RE::GFxValue args[kTotal];

	auto loot = LootMenu::GetSingleton();
	loot->view->CreateArray(&args[0]);
	auto& invList = loot->GetInventoryList();
	std::size_t size = (invList.size() < Settings::itemLimit) ? invList.size() : Settings::itemLimit;

	std::unique_ptr<RE::GFxValue[]> item(new RE::GFxValue[size]);
	std::unique_ptr<RE::GFxValue[]> text(new RE::GFxValue[size]);
	std::unique_ptr<RE::GFxValue[]> count(new RE::GFxValue[size]);
	std::unique_ptr<RE::GFxValue[]> isStolen(new RE::GFxValue[size]);
	std::unique_ptr<RE::GFxValue[]> isEnchanted(new RE::GFxValue[size]);
	std::unique_ptr<RE::GFxValue[]> isRead(new RE::GFxValue[size]);
	std::unique_ptr<RE::GFxValue[]> itemChance(new RE::GFxValue[size]);
	std::unique_ptr<RE::GFxValue[]> iconLabel(new RE::GFxValue[size]);

	SInt32 displaySize = 0;
	for (std::size_t i = 0; i < size; ++i) {
		if (!invList[i].GetCanPickPocket()) {
			continue;
		}

		loot->view->CreateObject(&item[i]);

		text[i].SetString(invList[i].GetName());
		count[i].SetNumber(invList[i].GetCount());
		isStolen[i].SetBoolean(invList[i].GetIsStolen());
		isEnchanted[i].SetBoolean(invList[i].GetIsEnchanted());
		isRead[i].SetBoolean(invList[i].GetIsRead());
		itemChance[i].SetNumber(invList[i].GetPickPocketChance());
		iconLabel[i].SetString(invList[i].GetIcon());

		item[i].SetMember("text", text[i]);
		item[i].SetMember("count", count[i]);
		item[i].SetMember("isStolen", isStolen[i]);
		item[i].SetMember("isEnchanted", isEnchanted[i]);
		item[i].SetMember("isRead", isRead[i]);
		item[i].SetMember("itemChance", itemChance[i]);
		item[i].SetMember("iconLabel", iconLabel[i]);

		args[kItems].PushBack(item[i]);
		++displaySize;
	}
	loot->SetDisplaySize(displaySize);
	loot->ModSelectedIndex(0);

	if (Settings::disableIfEmpty && displaySize <= 0) {
		loot->Close();
	} else {
		if (!Settings::disableOnActiDispatch) {
			auto sourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
			RE::TESObjectREFRPtr target(loot->GetContainerRef());
			RE::TESObjectREFRPtr caster(RE::PlayerCharacter::GetSingleton());
			sourceHolder->SendActivateEvent(target, caster);
		}
		loot->view->Invoke("_root.Menu_mc.OpenContainer", 0, args, kTotal);
	}

#if ITEM_DATA_DEBUG_ENABLED
	DebugContents();
#endif
}


void OpenContainerDelegate::DebugContents()
{
	auto logStamp = SKSE::Logger::UseLogStamp(false);
	bool timeStamp;
	bool hourFormat;
	std::tie(timeStamp, hourFormat) = SKSE::Logger::UseTimeStamp(false);

	std::size_t i = 0;
	bool div = false;
	SKSE::Logger::Print("");
	auto& invList = LootMenu::GetSingleton()->GetInventoryList();
	for (auto& item : invList) {
		if (!div && !item.GetCanPickPocket()) {
			SKSE::Logger::Print("========== HIDDEN ITEMS ==========");
			div = true;
		}
		item.DBGDumpType(i);
		++i;
	}

	SKSE::Logger::UseLogStamp(logStamp);
	SKSE::Logger::UseTimeStamp(timeStamp, hourFormat);
}


void CloseContainerDelegate::Run()
{
	auto loot = LootMenu::GetSingleton();
	loot->view->Invoke("_root.Menu_mc.CloseContainer", 0, 0, 0);
}


UpdateButtonIconsDelegate::UpdateButtonIconsDelegate(UInt32 a_take, UInt32 a_takeAll, UInt32 a_search) :
	_take(static_cast<double>(a_take)),
	_takeAll(static_cast<double>(a_takeAll)),
	_search(static_cast<double>(a_search))
{}


void UpdateButtonIconsDelegate::Run()
{
	auto loot = LootMenu::GetSingleton();
	if (!loot->IsOpen()) {
		return;
	}

	RE::GFxValue args[kTotal];

	args[kTake].SetNumber(_take);
	args[kTakeAll].SetNumber(_takeAll);
	args[kSearch].SetNumber(_search);

	loot->view->Invoke("_root.Menu_mc.UpdateButtonIcons", 0, args, kTotal);
}


SetVisibleButtonsDelegate::SetVisibleButtonsDelegate(bool a_take, bool a_takeAll, bool a_search) :
	_take(a_take),
	_takeAll(a_takeAll),
	_search(a_search)
{}


void SetVisibleButtonsDelegate::Run()
{
	auto loot = LootMenu::GetSingleton();
	if (!loot->IsOpen()) {
		return;
	}

	RE::GFxValue args[kTotal];

	args[kTake].SetBoolean(_take);
	args[kTakeAll].SetBoolean(_takeAll);
	args[kSearch].SetBoolean(_search);

	loot->view->Invoke("_root.Menu_mc.SetVisibleButtons", 0, args, kTotal);
}


SwitchStyleDelegate::SwitchStyleDelegate(Style a_style) :
	_style(static_cast<double>(a_style))
{}


void SwitchStyleDelegate::Run()
{
	RE::GFxValue args[kTotal];

	args[kStyle].SetNumber(_style);

	auto loot = LootMenu::GetSingleton();
	loot->view->Invoke("_root.Menu_mc.SwitchStyle", 0, args, kTotal);
}


void DelayedUpdater::Run()
{
	auto loot = LootMenu::GetSingleton();
	if (!loot->IsVisible()) {
		return;
	}

	if (loot->GetContainerRef()) {
		loot->ParseInventory();
		Dispatch<OpenContainerDelegate>();
	}
}
