import skyui.components.list.BasicEnumeration;
import skyui.components.list.ScrollingList;
import ButtonPanel;


class LootMenu extends MovieClip
{
	// private
	private var _platform: Number;
	private var _refID: Number;
	private var _takeKey: Number;
	private var _takeMapping: String;
	private var _takeAllKey: Number;
	private var _takeAllMapping: String;
	private var _searchKey: Number;
	private var _searchMapping: String;
	

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
		
		_takeKey = skse.GetMappedKey("Activate", 0, 0);
		_takeAllKey = skse.GetMappedKey("Toggle POV", 0, 0);
		_searchKey = skse.GetMappedKey("Ready Weapon", 0, 0);
	}


	// @API
	public function InitExtensions(): Void
	{
	}
	

	public function SetKeyMappings(a_takeMapping:String, a_takeAllMapping:String, a_searchMapping:String): Void
	{
		_takeMapping = a_takeMapping;
		_takeAllMapping = a_takeAllMapping;
		_searchMapping = a_searchMapping;
	}


	// @API
	public function SetPlatform(a_platform:Number, a_bPS3Switch:Boolean): Void
	{
		_platform = a_platform;

		var device: Number;
		if (a_platform == 0) {
			device = 0;
		} else {
			device = 2;
		}

		_takeKey = skse.GetMappedKey(_takeMapping, device, 0);
		_takeAllKey = skse.GetMappedKey(_takeAllMapping, device, 0);
		_searchKey = skse.GetMappedKey(_searchMapping, device, 0);
	}
	
	
	public function SetSelectedIndex(idx: Number): Void
	{
		itemList.selectedIndex = idx;
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

		background.gotoAndStop("Default");
	}
	

	public function SetContainer(refID:Number, title:String, sTake:String, sSearch:String, sTakeAll:String, selectedIndex:Number): Void
	{
		itemList.selectedIndex = selectedIndex;
		
		_refID = refID;
		titleText.htmlText = title;
		
		buttonTake.setButton(_takeKey, sTake);
		buttonTakeAll.setButton(_takeAllKey, sTakeAll);
		buttonSearch.setButton(_searchKey, sSearch);
	}
	
	
	public function OpenContainer(items:Array): Void
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
	
	
	public function CloseContainer(): Void
	{
		itemList.clearList();
		_visible = false;
	}


	public function UpdateButtons(): Void
	{
		buttonTake.updateButton(_takeKey);
		buttonTakeAll.updateButton(_takeAllKey);
		buttonSearch.updateButton(_searchKey);
	}


	public function SwitchStyle(style: Number): Void
	{
		switch(style){
			case 0:
				background.gotoAndStop("Default");
				itemList.scrollbar._x = itemList.width - itemList.scrollbar.width;
				itemList.leftBorder = 0
				break
			case 1:
				background.gotoAndStop("Dialogue");
				itemList.scrollbar._x = 0;
				itemList.leftBorder += itemList.scrollbar.width;
				break
		};
	}
}
