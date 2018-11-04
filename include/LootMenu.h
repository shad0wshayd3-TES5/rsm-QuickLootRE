#pragma once

#undef PlaySound

#include "skse64/GameTypes.h"  // BSFixedString

#include "RE/IMenu.h"  // IMenu
#include "RE/MenuEventHandler.h"  // MenuEventHandler
#include "RE/TESObjectREFR.h"  // TESObjectREFR

class InputEvent;
class TESObjectREFR;
class UIMessage;

namespace RE
{
	class ButtonEvent;
}


namespace QuickLootRE
{
	class LootMenuCreator
	{
	public:
		static RE::IMenu* Create();

	private:
		LootMenuCreator();
	};


	class LootMenu :
		public RE::IMenu,
		public RE::MenuEventHandler
	{
		friend class LootMenuCreator;
		friend class SetPlatforUIDelegate;
		friend class SetupUIDelegate;
		friend class OpenContainerUIDelegate;
		friend class SetContainerUIDelegate;
		friend class CloseContainerUIDelegate;
		friend class SetSelectedIndexUIDelegate;
	private:
		enum Platform : UInt32
		{
			kPlatform_PC = 0,
			kPlatform_Other = 2
		};

	public:
		enum Scaleform : UInt32
		{
			kScaleform_SetPlatform,
			kScaleform_Setup,
			kScaleform_OpenContainer,
			kScaleform_SetContainer,
			kScaleform_CloseContainer,
			kScaleform_SetSelectedIndex
		};


		LootMenu(const char* a_swfPath);
		virtual ~LootMenu();

		static LootMenu*			GetSingleton();
		static BSFixedString		GetName();

		static void					SetContainerRef(TESObjectREFR* a_ref);
		static void					SetContainerRef(RE::TESObjectREFR* a_ref);
		static RE::TESObjectREFR*	GetContainerRef();
		static void					ClearContainerRef(bool a_playAnimation = true);

		static bool					CanOpen(RE::TESObjectREFR* a_ref, bool a_isSneaking);
		static bool					IsOpen();

		static void					Register(Scaleform a_reg);

		// IMenu
		virtual UInt32				ProcessMessage(UIMessage* a_message) override;
		virtual void				Render() override;

		// MenuEventHandler
		virtual bool				CanProcess(InputEvent* a_event) override;
		virtual bool				ProcessButton(RE::ButtonEvent* a_event) override;

		void						OnMenuOpen();
		void						OnMenuClose();
		void						TakeItem();
		void						ModSelectedIndex(SInt32 a_indexOffset);

	private:
		bool						SingleLootEnabled();
		void						PlayAnimation(const char* fromName, const char* toName);
		void						PlayAnimationOpen();
		void						PlayAnimationClose();
		void						PlaySound(TESForm* a_item);


		static LootMenu*			_singleton;
		static SInt32				_selectedIndex;
		static RE::TESObjectREFR*	_containerRef;
		static bool					_isOpen;
		static Platform				_platform;
	};
}
