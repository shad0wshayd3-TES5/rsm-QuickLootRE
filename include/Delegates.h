#pragma once

#include "skse64/gamethreads.h"  // TaskDelegate
#include "skse64/Hooks_UI.h"  // UIDelegate_v1

#include <string>  // string


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


class SetKeyMappingsDelegate : public UITaskDelegateBase
{
public:
	virtual void Run() override;
};


class SetPlatformDelegate : public UITaskDelegateBase
{
public:
	virtual void Run() override;
};


class SetSelectedIndexDelegate : public UITaskDelegateBase
{
public:
	virtual void Run() override;
};


class SetupDelegate : public UITaskDelegateBase
{
public:
	virtual void Run() override;
};


class SetContainerDelegate : public UITaskDelegateBase
{
public:
	virtual void Run() override;
};


class OpenContainerDelegate : public UITaskDelegateBase
{
public:
	virtual void Run() override;

	void DebugContents();
};


class CloseContainerDelegate : public UITaskDelegateBase
{
public:
	virtual void Run() override;
};


class UpdateButtonsDelegate : public UITaskDelegateBase
{
public:
	virtual void Run() override;
};


class HideButtonsDelegate : public UITaskDelegateBase
{
public:
	virtual void Run() override;
};


class SwitchStyleDelegate : public UITaskDelegateBase
{
public:
	virtual void Run() override;
};


class DelayedUpdater : public TaskDelegateBase
{
public:
	virtual void Run() override;
};
