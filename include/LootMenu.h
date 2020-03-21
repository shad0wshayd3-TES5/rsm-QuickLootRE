#pragma once

#include <queue>
#include <string>
#include <string_view>
#include <unordered_map>

#include "Delegates.h"
#include "InventoryList.h"
#include "Utility.h"

#include "RE/Skyrim.h"


class ItemData;


namespace
{
	class StyleMap : public std::unordered_map<std::string, Style>
	{
	public:
		StyleMap();
	};
}


class LootMenu :
	public RE::IMenu,
	public RE::MenuEventHandler
{
public:
	using MenuBase = RE::IMenu;
	using HandlerBase = RE::MenuEventHandler;
	using Result = MenuBase::Result;
	using GRefCountBaseStatImpl::operator new;
	using GRefCountBaseStatImpl::operator delete;


	enum class Message : UInt32
	{
		kNoInputLoaded,
		kHookShareMissing,
		kHookShareIncompatible,
		kMissingDependencies,
		kLootMenuToggled
	};


	// IMenu
	virtual Result ProcessMessage(RE::UIMessage* a_message) override;
	virtual void PostDisplay() override;

	// MenuEventHandler
	virtual bool CanProcess(RE::InputEvent* a_event) override;
	virtual bool ProcessButton(RE::ButtonEvent* a_event) override;

	static LootMenu* GetSingleton();
	static void Free();
	static constexpr std::string_view Name();
	static const char* GetSingleLootMapping();
	static void SetSingleLootMapping(const char* a_singLootMapping);
	static const char* GetTakeMapping();
	static void SetTakeMapping(const char* a_takeStr);
	static const char* GetTakeAllMapping();
	static void SetTakeAllMapping(const char* a_takeAllStr);
	static const char* GetSearchMapping();
	static void SetSearchMapping(const char* a_searchStr);
	static void QueueMessage(Message a_msg);

	void ModSelectedIndex(SInt32 a_indexOffset);
	void SetDisplaySize(SInt32 a_size);
	bool ShouldSkipNextInput() const;
	void SkipNextInput();
	void NextInputSkipped();
	RE::TESObjectREFR* GetContainerRef() const;
	void ClearContainerRef();
	bool IsOpen() const;
	bool IsVisible() const;
	bool CanProcessInventoryChanges() const;
	void SetEnabled(bool a_enabled);
	void ToggleEnabled();
	const char* GetActiText() const;
	void SetActiText(const char* a_actiText);
	void Open() const;
	void Close() const;
	void SetVisible(bool a_visible);
	void SetContainerRef(RE::TESObjectREFR* a_ref);
	RE::TESObjectREFR* CanOpen(RE::TESObjectREFR* a_ref, bool a_isSneaking) const;
	void TakeItemStack();
	void TakeAllItems();
	InventoryList& GetInventoryList();
	void ParseInventory();

protected:
	enum
	{
		kInvalidButton = RE::ControlMap::kInvalid,
		kESC = 1,
		kKeyboardOffset = 0,
		kMouseOffset = 256,
		kGamepadOffset = 266,
		kPS3Offset = 302
	};


	enum class ControlMethod
	{
		kPC,
		kController
	};


	LootMenu();
	LootMenu(const LootMenu&) = delete;
	LootMenu(LootMenu&&) = delete;
	virtual ~LootMenu();

	LootMenu& operator=(const LootMenu&) = delete;
	LootMenu& operator=(LootMenu&&) = delete;

	static void ProcessMessageQueue();

	void OnMenuOpen();
	void OnMenuClose();
	Style GetStyle() const;
	void UpdateButtonIcons(bool a_controller) const;
	void GetGamepadButtonID(UInt32& a_key, const std::string_view& a_mapping) const;
	void GetPCButtonID(UInt32& a_key, const std::string_view& a_mapping) const;
	bool IsSingleLootEnabled() const;
	void PlayAnimation(std::string_view a_fromName, std::string_view a_toName) const;
	void PlayAnimationOpen();
	void PlayAnimationClose();
	bool TakeItem(ItemData& a_item, SInt32 a_numItems, bool a_playAnim, bool a_playSound);
	bool TryToPickPocket(ItemData& a_item, RE::ITEM_REMOVE_REASON& a_lootMode) const;
	void DispellWornItemEnchantments() const;
	UInt32 GetSingleLootKey(RE::INPUT_DEVICE a_deviceType) const;
	bool IsEnabled() const;
	void ResetInputTimer();
	bool MeetsInputThreshold(float a_timer);


	static constexpr float INPUT_THRESHOLD = 0.11;
	static constexpr char SWF_NAME[] = "LootMenu";


	static LootMenu* _singleton;
	static std::string _singleLootMapping;
	static std::string _takeMapping;
	static std::string _takeAllMapping;
	static std::string _searchMapping;
	static std::queue<const char*> _messageQueue;
	InventoryList _invList;
	std::string _actiText;
	RE::TESObjectREFR* _containerRef;
	ControlMethod _controlMethod;
	SInt32 _selectedIndex;
	SInt32 _displaySize;
	SInt32 _skipInputCount;
	float _lastInputTimer;
	bool _isContainerOpen;
	bool _isMenuOpen;
	bool _canProcessInvChanges;
	bool _isRegistered;
	bool _isEnabled;
};


inline constexpr std::string_view LootMenu::Name()
{
	return "LootMenu";
}
