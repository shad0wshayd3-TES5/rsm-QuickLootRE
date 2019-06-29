#pragma once

#include "skse64/gamethreads.h"  // TaskDelegate
#include "skse64/Hooks_UI.h"  // UIDelegate_v1

#include <string>

#include "Utility.h"

#include "SKSE/API.h"


namespace
{
	struct LocalizationStrings
	{
		LocalizationStrings();

		std::string take;
		std::string steal;
		std::string takeAll;
		std::string search;
	};
}


class TaskDelegateBase : public TaskDelegate
{
public:
	virtual ~TaskDelegateBase() = default;

	virtual void Dispose() override;
};


class UITaskDelegateBase : public UIDelegate_v1
{
public:
	virtual ~UITaskDelegateBase() = default;

	virtual void Dispose() override;
};


class SetSelectedIndexDelegate : public UITaskDelegateBase
{
public:
	SetSelectedIndexDelegate(SInt32 a_idx);

	virtual void Run() override;

private:
	enum
	{
		kIdx,
		kTotal
	};

	double _idx;
};


class SetupDelegate : public UITaskDelegateBase
{
public:
	virtual void Run() override;

private:
	enum
	{
		kPositionX,
		kPositionY,
		kScale,
		kOpacity,
		kTotal
	};
};


class SetContainerDelegate : public UITaskDelegateBase
{
public:
	SetContainerDelegate(SInt32 a_selectedIndex);

	virtual void Run() override;

private:
	enum
	{
		kTitle,
		kTake,
		kTakeAll,
		kSearch,
		kSelectedIndex,
		kTotal
	};

	double _selectedIndex;
};


class OpenContainerDelegate : public UITaskDelegateBase
{
public:
	virtual void Run() override;

private:
	enum
	{
		kItems,
		kTotal
	};

	void DebugContents();
};


class CloseContainerDelegate : public UITaskDelegateBase
{
public:
	virtual void Run() override;
};


class UpdateButtonIconsDelegate : public UITaskDelegateBase
{
public:
	UpdateButtonIconsDelegate(UInt32 a_take, UInt32 a_takeAll, UInt32 a_search);

	virtual void Run() override;

private:
	enum
	{
		kTake,
		kTakeAll,
		kSearch,
		kTotal
	};

	double _take;
	double _takeAll;
	double _search;
};


class SetVisibleButtonsDelegate : public UITaskDelegateBase
{
public:
	SetVisibleButtonsDelegate(bool a_take, bool a_takeAll, bool a_search);

	virtual void Run() override;

private:
	enum
	{
		kTake,
		kTakeAll,
		kSearch,
		kTotal
	};

	bool _take;
	bool _takeAll;
	bool _search;
};


class SwitchStyleDelegate : public UITaskDelegateBase
{
public:
	SwitchStyleDelegate(Style a_style);

	virtual void Run() override;

private:
	enum
	{
		kStyle,
		kTotal
	};

	double _style;
};


class DelayedUpdater : public TaskDelegateBase
{
public:
	virtual void Run() override;
};


template <class Delegate, class... Args>
void Dispatch(Args&&... a_args)
{
	auto task = SKSE::GetTaskInterface();
	task->AddUITask(new Delegate(std::forward<Args>(a_args)...));
}
