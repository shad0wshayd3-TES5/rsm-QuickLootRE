#pragma once

#include <queue>  // queue
#include <string>  // string

#include "InventoryList.h"  // InventoryList

#include "RE/Skyrim.h"


class ItemData;


class LootMenu :
	public RE::IMenu,
	public RE::MenuEventHandler
{
public:
	using Result = RE::IMenu::Result;
	using GRefCountBaseStatImpl::operator new;
	using GRefCountBaseStatImpl::operator delete;


	enum class Platform : UInt32
	{
		kPC = 0,
		kOther = 2
	};


	enum class Scaleform : UInt32
	{
		kSetKeyMappings,
		kSetPlatform,
		kSetSelectedIndex,
		kSetup,
		kSetContainer,
		kOpenContainer,
		kCloseContainer,
		kUpdateButtons,
		kHideButtons,
		kSwitchStyle
	};


	enum class Message : UInt32
	{
		kNoInputLoaded,
		kHookShareMissing,
		kHookShareIncompatible,
		kMissingDependencies,
		kLootMenuToggled
	};


	enum Style : UInt32
	{
		kDefault = 0,
		kDialogue = 1
	};


	// IMenu
	virtual Result	ProcessMessage(RE::UIMessage* a_message) override;
	virtual void	Render() override;

	// MenuEventHandler
	virtual bool	CanProcess(RE::InputEvent* a_event) override;
	virtual bool	ProcessButton(RE::ButtonEvent* a_event) override;

	static LootMenu*				GetSingleton();
	static void						Free();
	static const RE::BSFixedString&	GetName();
	static bool						IsConstructed();
	static const char*				GetSingleLootMapping();
	static void						SetSingleLootMapping(const char* a_singLootMapping);
	static const char*				GetTakeMapping();
	static void						SetTakeMapping(const char* a_takeStr);
	static const char*				GetTakeAllMapping();
	static void						SetTakeAllMapping(const char* a_takeAllStr);
	static const char*				GetSearchMapping();
	static void						SetSearchMapping(const char* a_searchStr);
	static void						QueueMessage(Message a_msg);

	SInt32				GetSelectedIndex() const;
	void				ModSelectedIndex(SInt32 a_indexOffset);
	void				SetDisplaySize(SInt32 a_size);
	bool				ShouldSkipNextInput() const;
	void				SkipNextInput();
	void				NextInputSkipped();
	RE::TESObjectREFR*	GetContainerRef() const;
	void				ClearContainerRef();
	bool				IsOpen() const;
	bool				IsVisible() const;
	bool				CanProcessInventoryChanges() const;
	bool				GetEnabled() const;
	void				SetEnabled(bool a_enabled);
	void				ToggleEnabled();
	Platform			GetPlatform() const;
	const char*			GetActiText() const;
	void				SetActiText(const char* a_actiText);
	void				Open() const;
	void				Close() const;
	void				SetVisible(bool a_visible);
	void				SetContainerRef(RE::TESObjectREFR* a_ref);
	RE::TESObjectREFR*	CanOpen(RE::TESObjectREFR* a_ref, bool a_isSneaking) const;
	void				Register(Scaleform a_reg) const;
	Style				GetStyle() const;
	void				OnMenuOpen();
	void				OnMenuClose();
	void				TakeItemStack();
	void				TakeAllItems();
	InventoryList&		GetInventoryList();
	void				ParseInventory();

protected:
	LootMenu() = delete;
	LootMenu(const LootMenu&) = delete;
	LootMenu(LootMenu&&) = delete;
	explicit LootMenu(const char* a_swfPath);
	virtual ~LootMenu();

	LootMenu& operator=(const LootMenu&) = delete;
	LootMenu& operator=(LootMenu&&) = delete;

	static bool	IsEnabled();
	static void	ProcessMessageQueue();

	bool	SingleLootEnabled() const;
	void	PlayAnimation(const char* a_fromName, const char* a_toName) const;
	void	PlayAnimationOpen();
	void	PlayAnimationClose();
	bool	TakeItem(ItemData& a_item, UInt32 a_numItems, bool a_playAnim, bool a_playSound);
	bool	TryToPickPocket(ItemData& a_item, RE::TESObjectREFR::RemoveType& a_lootMode) const;
	void	DispellWornItemEnchantments() const;
	UInt32	GetSingleLootKey(RE::DeviceType a_deviceType) const;


	static LootMenu*				_singleton;
	static std::string				_singleLootMapping;
	static std::string				_takeMapping;
	static std::string				_takeAllMapping;
	static std::string				_searchMapping;
	static std::queue<const char*>	_messageQueue;
	RE::TESObjectREFR*				_containerRef;
	InventoryList					_invList;
	std::string						_actiText;
	Platform						_platform;
	SInt32							_selectedIndex;
	SInt32							_displaySize;
	SInt32							_skipInputCount;
	bool							_isContainerOpen;
	bool							_isMenuOpen;
	bool							_canProcessInvChanges;
	bool							_isRegistered;
	bool							_isEnabled;
};
