#pragma once

#include "skse64/gamethreads.h"  // TaskDelegate
#include "skse64/Hooks_UI.h"  // UIDelegate_v1
#include "skse64/PluginAPI.h"  // SKSETaskInterface

#include <vector>  // vector

#include "RE/GFxValue.h"  // GFxValue
#include "RE/Memory.h"  // TES_HEAP_REDEFINE_NEW


class SetKeyMappingsUIDelegate : public UIDelegate_v1
{
public:
	virtual void Run() override;
	virtual void Dispose() override;

	TES_HEAP_REDEFINE_NEW();
};


class SetPlatformUIDelegate : public UIDelegate_v1
{
public:
	virtual void Run() override;
	virtual void Dispose() override;

	TES_HEAP_REDEFINE_NEW();
};


class SetSelectedIndexUIDelegate : public UIDelegate_v1
{
public:
	virtual void Run() override;
	virtual void Dispose() override;

	TES_HEAP_REDEFINE_NEW();
};


class SetupUIDelegate : public UIDelegate_v1
{
public:
	virtual void Run() override;
	virtual void Dispose() override;

	TES_HEAP_REDEFINE_NEW();
};


class SetContainerUIDelegate : public UIDelegate_v1
{
public:
	virtual void Run() override;
	virtual void Dispose() override;

	TES_HEAP_REDEFINE_NEW();
};


class OpenContainerUIDelegate : public UIDelegate_v1
{
public:
	virtual void Run() override;
	virtual void Dispose() override;

	void DebugContents();

	TES_HEAP_REDEFINE_NEW();
};


class CloseContainerUIDelegate : public UIDelegate_v1
{
public:
	virtual void Run() override;
	virtual void Dispose() override;

	TES_HEAP_REDEFINE_NEW();
};


class UpdateButtonsUIDelegate : public UIDelegate_v1
{
public:
	virtual void Run() override;
	virtual void Dispose() override;

	TES_HEAP_REDEFINE_NEW();
};


class HideButtonsUIDelegate : public UIDelegate_v1
{
public:
	virtual void Run() override;
	virtual void Dispose() override;

	TES_HEAP_REDEFINE_NEW();
};


class SwitchStyleTaskDelegate : public UIDelegate_v1
{
public:
	virtual void Run() override;
	virtual void Dispose() override;

	TES_HEAP_REDEFINE_NEW();
};


class GFxValueDeallocTaskDelegate : public TaskDelegate
{
public:
	virtual void Run() override;
	virtual void Dispose() override;

	TES_HEAP_REDEFINE_NEW();


	std::vector<RE::GFxValue*> heapAllocVals;
};


class DelayedUpdater : public TaskDelegate
{
public:
	virtual void Run() override;
	virtual void Dispose() override;

	static void Register();

	TES_HEAP_REDEFINE_NEW();

private:
	DelayedUpdater();
	DelayedUpdater(const DelayedUpdater&) = delete;
	DelayedUpdater(DelayedUpdater&&) = delete;
	~DelayedUpdater();

	DelayedUpdater& operator=(const DelayedUpdater&) = delete;
	DelayedUpdater& operator=(DelayedUpdater&&) = delete;
};


extern SKSETaskInterface* g_task;
