import skyui.components.list.BasicEnumeration;
import skyui.components.list.ScrollingList;
import ButtonPanel;


class LootMenu extends MovieClip
{
	// stage elements
	public var background: MovieClip;
	public var itemList: ScrollingList;
	public var titleText: TextField;
	public var buttonTake: ButtonPanel;
	public var buttonTakeAll: ButtonPanel;
	public var buttonSearch: ButtonPanel;
	

	public function LootMenu()
	{
		super();

		// default position
		_x += 384;
		_y += 0;
		
		_visible = false;
		
		background.gotoAndStop("Default");
	}
	

	public function Setup(a_positionX:Number, a_positionY:Number, a_scale:Number, a_opacity:Number): Void
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
	
	
	public function OpenContainer(a_items:Array): Void
	{
		itemList.clearList();
		itemList.listEnumeration = new BasicEnumeration(itemList.entryList);
		for (var i = 0; i < a_items.length; i++) {
			var entry = a_items[i]
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


	public function UpdateButtonIcons(a_take:Number, a_takeAll:Number, a_search:Number): Void
	{
		buttonTake.UpdateButtonIcon(a_take);
		buttonTakeAll.UpdateButtonIcon(a_takeAll);
		buttonSearch.UpdateButtonIcon(a_search);
	}


	public function SwitchStyle(a_style:Number): Void
	{
		switch(a_style) {
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


	public function SetContainer(a_title:String, a_take:String, a_takeAll:String, a_search:String, a_selectedIndex:Number): Void
	{
		itemList.selectedIndex = a_selectedIndex;
		
		titleText.htmlText = a_title;
		
		buttonTake.SetButtonText(a_take);
		buttonTakeAll.SetButtonText(a_takeAll);
		buttonSearch.SetButtonText(a_search);
	}


	public function SetSelectedIndex(a_idx:Number): Void
	{
		itemList.selectedIndex = a_idx;
	}
	
	
	public function SetVisibleButtons(a_take:Boolean, a_takeAll:Boolean, a_search:Boolean): Void
	{
		buttonTake.SetVisible(a_take);
		buttonTakeAll.SetVisible(a_takeAll);
		buttonSearch.SetVisible(a_search);
	}
}
