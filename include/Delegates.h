#pragma once

#include "skse64/gamethreads.h"  // TaskDelegate
#include "skse64/Hooks_UI.h"  // UIDelegate_v1
#include "skse64/PluginAPI.h"  // SKSETaskInterface


class UIDelegateBase : public UIDelegate_v1
{
public:
	UIDelegateBase() = default;
	virtual ~UIDelegateBase() = default;

	virtual void Dispose() override;
};


class TaskDelegateBase : public TaskDelegate
{
public:
	TaskDelegateBase() = default;
	virtual ~TaskDelegateBase() = default;

	virtual void Dispose() override;
};


class SetKeyMappingsUIDelegate : public UIDelegateBase
{
public:
	SetKeyMappingsUIDelegate() = default;
	virtual ~SetKeyMappingsUIDelegate() = default;

	virtual void Run() override;
};


class SetPlatformUIDelegate : public UIDelegateBase
{
public:
	SetPlatformUIDelegate() = default;
	virtual ~SetPlatformUIDelegate() = default;

	virtual void Run() override;
};


class SetSelectedIndexUIDelegate : public UIDelegateBase
{
public:
	SetSelectedIndexUIDelegate() = default;
	virtual ~SetSelectedIndexUIDelegate() = default;

	virtual void Run() override;
};


class SetupUIDelegate : public UIDelegateBase
{
public:
	SetupUIDelegate() = default;
	virtual ~SetupUIDelegate() = default;

	virtual void Run() override;
};


class SetContainerUIDelegate : public UIDelegateBase
{
public:
	SetContainerUIDelegate() = default;
	virtual ~SetContainerUIDelegate() = default;

	virtual void Run() override;
};


class OpenContainerUIDelegate : public UIDelegateBase
{
public:
	OpenContainerUIDelegate() = default;
	virtual ~OpenContainerUIDelegate() = default;

	virtual void Run() override;

	void DebugContents();
};


class CloseContainerUIDelegate : public UIDelegateBase
{
public:
	CloseContainerUIDelegate() = default;
	virtual ~CloseContainerUIDelegate() = default;

	virtual void Run() override;
};


class UpdateButtonsUIDelegate : public UIDelegateBase
{
public:
	UpdateButtonsUIDelegate() = default;
	virtual ~UpdateButtonsUIDelegate() = default;

	virtual void Run() override;
};


class HideButtonsUIDelegate : public UIDelegateBase
{
public:
	HideButtonsUIDelegate() = default;
	virtual ~HideButtonsUIDelegate() = default;

	virtual void Run() override;
};


class SwitchStyleTaskDelegate : public UIDelegateBase
{
public:
	SwitchStyleTaskDelegate() = default;
	virtual ~SwitchStyleTaskDelegate() = default;

	virtual void Run() override;
};


class DelayedUpdater : public TaskDelegateBase
{
public:
	DelayedUpdater() = default;
	virtual ~DelayedUpdater() = default;

	virtual void Run() override;
};
