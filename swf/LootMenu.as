import skyui.components.list.BasicEnumeration;
import skyui.components.list.ScrollingList;
import ButtonPanel;

class LootMenu extends MovieClip
{
	// private
	private var _platform: Number;
	private var _activateKey: Number;
	private var _weaponKey: Number;
	private var _povKey: Number;
	private var _refID: Number;
	
	// stage elements
	public var background: MovieClip;
	public var itemList: ScrollingList;
	public var titleText: TextField;
	public var buttonTake: ButtonPanel;
	public var buttonSearch: ButtonPanel;
	public var buttonTakeAll: ButtonPanel;
	
	public function LootMenu()
	{
		super();

		// default position
		_x += 384;
		_y += 0;
		
		_visible = false;
		
		_activateKey = skse.GetMappedKey("Activate", 0, 0);
		_povKey = skse.GetMappedKey("Toggle POV", 0, 0);
		_weaponKey = skse.GetMappedKey("Ready Weapon", 0, 0);
	}

	// @API
	public function InitExtensions(): Void
	{
	}

	// @API
	public function SetPlatform(a_platform: Number, a_bPS3Switch: Boolean): Void
	{
		_platform = a_platform;

		var device: Number;
		if (a_platform == 0) {
			device = 0;
		} else {
			device = 2;
		}

		_activateKey = skse.GetMappedKey("Activate", device, 0);
		_povKey = skse.GetMappedKey("Toggle POV", device, 0);
		_weaponKey = skse.GetMappedKey("Ready Weapon", device, 0);
	}
	
	// @API
	public function Setup(a_positionX:Number, a_positionY:Number, a_scale:Number, a_opacity:Number) :Void
	{
		if (0 <= a_positionX) {
			_x = a_positionX;
		}
		
		if (0 <= a_positionY) {
			_y = a_positionY;
		}
		
		if (a_scale >= 0) {
			if (a_scale < 25) {
				a_scale = 25;
			} else if (a_scale > 400) {
				a_scale = 400;
			}
			_xscale = _yscale = a_scale;
		}
		
		if (a_opacity >= 0) {
			if (a_opacity > 100) {
				a_opacity = 100;
			}
			background._alpha = a_opacity;
		}
	}
	
	public function openContainer(items:Array): Void
	{
		itemList.clearList();
		itemList.listEnumeration = new BasicEnumeration(itemList.entryList);
		for (var i=0; i<items.length; i++) {
			var entry = items[i]
			itemList.entryList.push(entry)
		}
		itemList.InvalidateData();
		_visible = true;
	}
	
	public function setContainer(refID:Number, title:String, sTake:String, sSearch:String, sTakeAll:String, selectedIndex:Number): Void
	{
		itemList.selectedIndex = selectedIndex;
		
		_refID = refID;
		titleText.htmlText = title;
		
		buttonTake.setButton(_activateKey, sTake);
		buttonTakeAll.setButton(_povKey, sTakeAll);
		buttonSearch.setButton(_weaponKey, sSearch);
	}
	
	public function closeContainer(): Void
	{
		itemList.clearList();
		_visible = false;
	}
	
	public function setSelectedIndex(idx: Number): Void
	{
		itemList.selectedIndex = idx;
	}
}
