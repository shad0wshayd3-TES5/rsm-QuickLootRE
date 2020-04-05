#include "Loot.h"

#include <memory>

#include "RE/Skyrim.h"
#include "SKSE/API.h"

#include "LootMenu.h"


Loot* Loot::GetSingleton()
{
	static Loot singleton;
	return std::addressof(singleton);
}


void Loot::Close()
{
	auto msgQ = RE::UIMessageQueue::GetSingleton();
	if (msgQ) {
		msgQ->AddMessage(LootMenu::MenuName(), RE::UI_MESSAGE_TYPE::kHide, nullptr);
	}
}


void Loot::Open()
{
	auto msgQ = RE::UIMessageQueue::GetSingleton();
	if (msgQ) {
		msgQ->AddMessage(LootMenu::MenuName(), RE::UI_MESSAGE_TYPE::kShow, nullptr);
	}
}


void Loot::Process(Scaleform::LootMenu& a_menu)
{
	if (!_taskQueue.empty()) {
		for (auto& task : _taskQueue) {
			task(a_menu);
		}
		_taskQueue.clear();
	}
}


void Loot::ModSelectedIndex(double a_mod)
{
	AddTask([a_mod](Scaleform::LootMenu& a_menu) {
		a_menu.ModSelectedIndex(a_mod);
	});
}


void Loot::SetContainer(RE::TESObjectREFRPtr a_container)
{
	AddTask([a_container](Scaleform::LootMenu& a_menu) {
		a_menu.ProcessRef(a_container);
	});
}


void Loot::AddTask(Tasklet a_task)
{
	auto task = SKSE::GetTaskInterface();
	task->AddUITask([this, a_task]() {
		auto menu = GetMenu();
		if (menu) {
			a_task(*menu);
		} else {
			Open();
			_taskQueue.push_back(std::move(a_task));
		}
	});
}


auto Loot::GetMenu()
	-> RE::GPtr<LootMenu>
{
	auto ui = RE::UI::GetSingleton();
	return ui ? ui->GetMenu<LootMenu>(LootMenu::MenuName()) : nullptr;
}
