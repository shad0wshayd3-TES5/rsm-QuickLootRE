#include "ViewHandler.h"

#include "Scaleform/LootMenu.h"

void ViewHandler::AdjustPriority(Priority a_priority)
{
	auto ui = RE::UI::GetSingleton();
	if (!ui) {
		assert(false);
		return;
	}

	switch (a_priority) {
	case Priority::kDefault:
		_menu->depthPriority =
			Scaleform::LootMenu::SortPriority();
		break;
	case Priority::kLowest:
		_menu->depthPriority = -1;
		break;
	default:
		assert(false);
		break;
	}

	std::sort(
		ui->menuStack.begin(),
		ui->menuStack.end(),
		[](auto&& a_lhs, auto&& a_rhs) {
			return a_lhs->depthPriority < a_rhs->depthPriority;
		});
}
