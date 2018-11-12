#include "Delegates.h"

#include <exception>  // exception

#include "Events.h"  // getInventoryList()
#include "Exceptions.h"  // bad_gfx_value_interface
#include "Forms.h"
#include "InventoryList.h"  // g_invList
#include "LootMenu.h"  // LootMenu
#include "Settings.h"  // Settings

#include "RE/GFxMovieDef.h"  // GFxMovieDef
#include "RE/GFxMovieView.h"  // GFxMovieView
#include "RE/MenuControls.h"  // MenuControls
#include "RE/MenuEventHandler.h"  // MenuEventHandler
#include "RE/PlayerCharacter.h"  // PlayerCharacter
#include "RE/TESObjectREFR.h"  // TESObjectREFR


namespace QuickLootRE
{
	void SetPlatforUIDelegate::Run()
	{
		if (LootMenu::IsOpen()) {
			GFxValue args[2];

			args[0].SetNumber(LootMenu::GetPlatform());
			args[1].SetBool(false);

			LootMenu::GetSingleton()->view->Invoke("_root.Menu_mc.SetPlatform", 0, args, 2);
		}
	}


	void SetPlatforUIDelegate::Dispose()
	{
		if (this) {
			Heap_Free(this);
		}
	}


	void SetupUIDelegate::Run()
	{
		if (LootMenu::IsOpen()) {
			LootMenu* loot = LootMenu::GetSingleton();

			GFxValue args[4];

			RE::GFxMovieDef* def = loot->view->GetMovieDef();

			double x = Settings::positionX;
			double y = Settings::positionY;
			double scale = Settings::scale;
			double opacity = Settings::opacity;

			x = (0 <= x && x <= 100) ? (x * def->GetWidth() * 0.01) : -1;
			y = (0 <= y && y <= 100) ? (y * def->GetHeight() * 0.01) : -1;
			if (scale >= 0) {
				if (scale < 25)
					scale = 25;
				else if (scale > 400)
					scale = 400;
			}
			if (opacity >= 0) {
				if (opacity > 100)
					opacity = 100;
			}

			args[0].SetNumber(x);
			args[1].SetNumber(y);
			args[2].SetNumber(scale);
			args[3].SetNumber(opacity);

			Settings::isApplied = true;

			loot->view->Invoke("_root.Menu_mc.Setup", 0, args, 4);
		}
	}


	void SetupUIDelegate::Dispose()
	{
		if (this) {
			Heap_Free(this);
		}
	}


	void OpenContainerUIDelegate::Run()
	{
		static RE::PlayerCharacter* player = reinterpret_cast<RE::PlayerCharacter*>(*g_thePlayer);

		if (LootMenu::IsOpen()) {
			try {
				LootMenu* loot = LootMenu::GetSingleton();
				AddContainerChanges();

				GFxValue args[1];

				loot->view->CreateArray(&args[0]);
				if (!args[0].objectInterface) {
					throw bad_gfx_value_interface();
				}

				bool hasMisdirection = player->HasPerk(Misdirection);
				bool hasPerfectTouch = player->HasPerk(PerfectTouch);

				SInt32 size = (g_invList.size() < Settings::itemLimit) ? g_invList.size() : Settings::itemLimit;

				GFxValue* item = (GFxValue*)ScaleformHeap_Allocate(sizeof(GFxValue) * size);
				new (item)GFxValue[size];
				GFxValue* text = (GFxValue*)ScaleformHeap_Allocate(sizeof(GFxValue) * size);
				new (text)GFxValue[size];
				GFxValue* count = (GFxValue*)ScaleformHeap_Allocate(sizeof(GFxValue) * size);
				new (count)GFxValue[size];
				GFxValue* isStolen = (GFxValue*)ScaleformHeap_Allocate(sizeof(GFxValue) * size);
				new (isStolen)GFxValue[size];
				GFxValue* isEnchanted = (GFxValue*)ScaleformHeap_Allocate(sizeof(GFxValue) * size);
				new (isEnchanted)GFxValue[size];
				GFxValue* isRead = (GFxValue*)ScaleformHeap_Allocate(sizeof(GFxValue) * size);
				new (isRead)GFxValue[size];
				GFxValue* itemChance = (GFxValue*)ScaleformHeap_Allocate(sizeof(GFxValue) * size);
				new (itemChance)GFxValue[size];
				GFxValue* iconLabel = (GFxValue*)ScaleformHeap_Allocate(sizeof(GFxValue) * size);
				new (iconLabel)GFxValue[size];

				SInt32 displaySize = 0;
				for (SInt32 i = 0; i < size; ++i) {
					if (g_invList[i].isEquipped()) {
						if (g_invList[i].form()->formType == kFormType_Weapon) {
							if (!hasMisdirection) {
								continue;
							}
						} else {
							if (!hasPerfectTouch) {
								continue;
							}
						}
					}

					loot->view->CreateObject(&item[i]);

					text[i].SetString(g_invList[i].name());
					count[i].SetNumber(g_invList[i].count());
					isStolen[i].SetBool(g_invList[i].isStolen());
					isEnchanted[i].SetBool(g_invList[i].isEnchanted());
					isRead[i].SetBool(g_invList[i].isRead());
					itemChance[i].SetNumber(g_invList[i].pickPocketChance());
					iconLabel[i].SetString(g_invList[i].icon());

					item[i].SetMember("text", &text[i]);
					item[i].SetMember("count", &count[i]);
					item[i].SetMember("isStolen", &isStolen[i]);
					item[i].SetMember("isEnchanted", &isEnchanted[i]);
					item[i].SetMember("isRead", &isRead[i]);
					item[i].SetMember("itemChance", &itemChance[i]);
					item[i].SetMember("iconLabel", &iconLabel[i]);

					args[0].PushBack(&item[i]);
					++displaySize;
				}
				LootMenu::SetDisplaySize(displaySize);
				LootMenu::ModSelectedIndex(0);

				if (Settings::disableIfEmpty && displaySize <= 0) {
					LootMenu::Close();
				} else {
					loot->view->Invoke("_root.Menu_mc.openContainer", 0, args, 1);
				}

				GFxValueDeallocTaskDelegate* dlgt = (GFxValueDeallocTaskDelegate*)Heap_Allocate(sizeof(GFxValueDeallocTaskDelegate));
				new (dlgt)GFxValueDeallocTaskDelegate;
				dlgt->heapAllocVals.push_back(item);
				dlgt->heapAllocVals.push_back(text);
				dlgt->heapAllocVals.push_back(count);
				dlgt->heapAllocVals.push_back(isStolen);
				dlgt->heapAllocVals.push_back(isEnchanted);
				dlgt->heapAllocVals.push_back(isRead);
				dlgt->heapAllocVals.push_back(itemChance);
				dlgt->heapAllocVals.push_back(iconLabel);
				g_task->AddTask(dlgt);
			} catch (std::exception& e) {
				_ERROR(e.what());
			}
		}
	}


	void OpenContainerUIDelegate::Dispose()
	{
		if (this) {
			Heap_Free(this);
		}
	}


	void OpenContainerUIDelegate::AddContainerChanges()
	{
		// Containers don't have ExtraContainerChanges if the player hasn't opened them yet, so we must add them ourselves
		RE::TESObjectREFR* ref = LootMenu::GetContainerRef();
		RE::ExtraContainerChanges* xContainerChanges = static_cast<RE::ExtraContainerChanges*>(ref->extraData.GetByType(kExtraData_ContainerChanges));
		RE::ExtraContainerChanges::Data* changes = xContainerChanges ? xContainerChanges->data : 0;
		if (!changes) {
			RE::ExtraContainerChanges::Data* changes = (RE::ExtraContainerChanges::Data*)Heap_Allocate(sizeof(RE::ExtraContainerChanges::Data));
			new (changes)RE::ExtraContainerChanges::Data(ref);
			ref->extraData.SetInventoryChanges(changes);
			changes->InitContainer();
		}
	}


	void SetContainerUIDelegate::Run()
	{
		static RE::PlayerCharacter*	player		= reinterpret_cast<RE::PlayerCharacter*>(*g_thePlayer);
		static const char*			sTake		= (*g_gameSettingCollection)->Get("sTake")->data.s;
		static const char*			sSteal		= (*g_gameSettingCollection)->Get("sSteal")->data.s;
		static const char*			sSearch		= (*g_gameSettingCollection)->Get("sSearch")->data.s;
		static const char*			sTakeAll	= (*g_gameSettingCollection)->Get("sTakeAll")->data.s;

		GFxValue args[6];

		const char* takeType;
		RE::TESObjectREFR* ref = LootMenu::GetContainerRef();
		if (ref->baseForm->formType == kFormType_NPC) {
			takeType = !ref->IsDead(true) && player->IsSneaking() ? sSteal : sTake;
		} else {
			takeType = ref->IsOffLimits() ? sSteal : sTake;
		}

		args[0].SetNumber(ref->formID);
		args[1].SetString(ref->GetReferenceName());
		args[2].SetString(takeType);
		args[3].SetString(sSearch);
		args[4].SetString(sTakeAll);
		args[5].SetNumber(LootMenu::GetSelectedIndex());

		LootMenu::GetSingleton()->view->Invoke("_root.Menu_mc.setContainer", 0, args, 6);
	}


	void SetContainerUIDelegate::Dispose()
	{
		if (this) {
			Heap_Free(this);
		}
	}


	void CloseContainerUIDelegate::Run()
	{
		LootMenu::GetSingleton()->view->Invoke("_root.Menu_mc.closeContainer", 0, 0, 0);
	}


	void CloseContainerUIDelegate::Dispose()
	{
		if (this) {
			Heap_Free(this);
		}
	}


	void SetSelectedIndexUIDelegate::Run()
	{
		if (LootMenu::IsOpen()) {
			GFxValue args[1];

			args[0].SetNumber(LootMenu::GetSelectedIndex());

			LootMenu::GetSingleton()->view->Invoke("_root.Menu_mc.setSelectedIndex", 0, args, 1);
		}
	}


	void SetSelectedIndexUIDelegate::Dispose()
	{
		if (this) {
			Heap_Free(this);
		}
	}


	void GFxValueDeallocTaskDelegate::Run()
	{
		for (auto& val : heapAllocVals) {
			ScaleformHeap_Free(val);
			val = 0;
		}
	}


	void GFxValueDeallocTaskDelegate::Dispose()
	{
		if (this) {
			Heap_Free(this);
		}
	}


	SKSETaskInterface* g_task = 0;
}
