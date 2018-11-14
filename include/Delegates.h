#pragma once

#include "skse64/gamethreads.h"  // TaskDelegate
#include "skse64/Hooks_UI.h"  // UIDelegate_v1
#include "skse64/PluginAPI.h"  // SKSETaskInterface

#include <vector>  // vector

class GFxValue;


namespace QuickLootRE
{
	class SetPlatforUIDelegate : public UIDelegate_v1
	{
	public:
		virtual void Run() override;
		virtual void Dispose() override;
	};


	class SetupUIDelegate : public UIDelegate_v1
	{
	public:
		virtual void Run() override;
		virtual void Dispose() override;
	};


	class OpenContainerUIDelegate : public UIDelegate_v1
	{
	public:
		virtual void Run() override;
		virtual void Dispose() override;

	private:
		void AddContainerChanges();
	};


	class SetContainerUIDelegate : public UIDelegate_v1
	{
	public:
		virtual void Run() override;
		virtual void Dispose() override;
	};


	class UpdateButtonsUIDelegate : public UIDelegate_v1
	{
		virtual void Run() override;
		virtual void Dispose() override;
	};


	class CloseContainerUIDelegate : public UIDelegate_v1
	{
	public:
		virtual void Run() override;
		virtual void Dispose() override;
	};


	class SetSelectedIndexUIDelegate : public UIDelegate_v1
	{
	public:
		virtual void Run() override;
		virtual void Dispose() override;
	};


	class GFxValueDeallocTaskDelegate : public TaskDelegate
	{
	public:
		virtual void Run() override;
		virtual void Dispose() override;


		std::vector<GFxValue*> heapAllocVals;
	};


	class DelayedUpdater : public TaskDelegate
	{
	public:
		virtual void Run() override;
		virtual void Dispose() override;

		static void Register();
	};


	extern SKSETaskInterface* g_task;
}
