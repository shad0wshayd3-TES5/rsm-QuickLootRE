#pragma once

#include "skse64/gamethreads.h"  // TaskDelegate

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


class SetKeyMappingsDelegate : public TaskDelegateBase
{
public:
	virtual void Run() override;
};


class SetPlatformDelegate : public TaskDelegateBase
{
public:
	virtual void Run() override;
};


class SetSelectedIndexDelegate : public TaskDelegateBase
{
public:
	virtual void Run() override;
};


class SetupDelegate : public TaskDelegateBase
{
public:
	virtual void Run() override;
};


class SetContainerDelegate : public TaskDelegateBase
{
public:
	virtual void Run() override;
};


class OpenContainerDelegate : public TaskDelegateBase
{
public:
	virtual void Run() override;

	void DebugContents();
};


class CloseContainerDelegate : public TaskDelegateBase
{
public:
	virtual void Run() override;
};


class UpdateButtonsDelegate : public TaskDelegateBase
{
public:
	virtual void Run() override;
};


class HideButtonsDelegate : public TaskDelegateBase
{
public:
	virtual void Run() override;
};


class SwitchStyleDelegate : public TaskDelegateBase
{
public:
	virtual void Run() override;
};


class DelayedUpdater : public TaskDelegateBase
{
public:
	virtual void Run() override;
};
