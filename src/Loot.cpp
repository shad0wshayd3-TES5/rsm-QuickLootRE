#include "Loot.h"

#include "HUDHandler.h"
#include "Scaleform/LootMenu.h"

void Loot::Close()
{
	if (IsOpen()) {
		auto msgQ = RE::UIMessageQueue::GetSingleton();
		if (msgQ) {
			msgQ->AddMessage(LootMenu::MenuName(), RE::UI_MESSAGE_TYPE::kHide, nullptr);
			auto hud = HUDHandler::GetSingleton();
			hud->Disable();
		}
	}
}

void Loot::Open()
{
	if (_enabled && !IsOpen()) {
		auto msgQ = RE::UIMessageQueue::GetSingleton();
		if (msgQ) {
			msgQ->AddMessage(LootMenu::MenuName(), RE::UI_MESSAGE_TYPE::kShow, nullptr);
			auto hud = HUDHandler::GetSingleton();
			hud->Enable();
		}
	}
}

void Loot::Process(LootMenu& a_menu)
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
	AddTask([a_mod](LootMenu& a_menu) {
		a_menu.ModSelectedIndex(a_mod);
	});
}

void Loot::SetContainer(RE::TESObjectREFRPtr a_container)
{
	AddTask([a_container](LootMenu& a_menu) {
		a_menu.ProcessRef(a_container);
	});
}

void Loot::TakeStack()
{
	AddTask([](LootMenu& a_menu) {
		a_menu.TakeStack();
	});
}

void Loot::AddTask(Tasklet a_task)
{
	Open();
	auto task = SKSE::GetTaskInterface();
	task->AddUITask([this, a_task]() {
		auto menu = GetMenu();
		if (menu) {
			a_task(*menu);
		} else {
			_taskQueue.push_back(std::move(a_task));  // thread safe
		}
	});
}

bool Loot::IsOpen() const
{
	return static_cast<bool>(GetMenu());
}

auto Loot::GetMenu() const
	-> RE::GPtr<LootMenu>
{
	auto ui = RE::UI::GetSingleton();
	return ui ? ui->GetMenu<LootMenu>(LootMenu::MenuName()) : nullptr;
}
