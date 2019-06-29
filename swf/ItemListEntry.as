import skyui.components.list.ScrollingList;
import skyui.components.list.ListState;
import skyui.components.list.BasicListEntry;


class ItemListEntry extends BasicListEntry
{
	// PRIVATE VARIABLES
	private var _iconLabel: String;
	private var _iconSource: String;
	

	// STAGE ELMENTS
  	public var itemIcon: MovieClip;
	public var stolenIcon: MovieClip;
	public var enchantIcon: MovieClip;
	public var readIcon: MovieClip;
	public var textField: TextField;
	public var pickPocketChance: TextField;
  	public var selectIndicator: MovieClip;


	// @override BasicListEntry
	public function initialize(a_index:Number, a_state:ListState): Void
	{
		super.initialize();

		_iconSource = "skyui/icons_item_psychosteve.swf";

		var iconLoader = new MovieClipLoader();
		iconLoader.addListener(this);
		iconLoader.loadClip(_iconSource, itemIcon);
		
		itemIcon._visible = false;
	}
	

	// @implements MovieClipLoader
	private function onLoadInit(a_icon:MovieClip): Void
	{
		a_icon.gotoAndStop(_iconLabel);
	}
	
	
	// @override BasicListEntry
	public function setEntry(a_entryObject:Object, a_state:ListState): Void
	{
		var isAssigned = a_entryObject == a_state.assignedEntry;
		var isSelected = a_entryObject == a_state.list.selectedEntry || isAssigned;
		
		var groupIndex = a_state.activeGroupIndex;
		
		enabled = a_state.assignedEntry == null || isAssigned;
		_alpha = enabled ? 100 : 25;
		
		if (selectIndicator != undefined) {
			selectIndicator._visible = isSelected;
		}
		
		// set text
		textField.autoSize = "left";
		if (a_entryObject.text == undefined) {
			textField.text = " ";
		} else {
			var text = a_entryObject.text;
			
			if (a_entryObject.count > 1) {
				text += " (" + a_entryObject.count.toString() + ")";
			}
			
			var maxTextLength: Number = 32;
			if (text.length > maxTextLength) {
				textField.text = text.substr(0, maxTextLength - 3) + "...";
			} else {
				textField.text = text;
			}
		}

		var iconPosX = textField._x + textField._width + 6;
		
		stolenIcon._visible = (a_entryObject.isStolen != undefined && a_entryObject.isStolen);
		if (stolenIcon._visible) {
			stolenIcon._x = iconPosX;
			iconPosX += stolenIcon._width + 2;
		}
		
		enchantIcon._visible = (a_entryObject.isEnchanted != undefined && a_entryObject.isEnchanted);
		if (enchantIcon._visible) {
			enchantIcon._x = iconPosX;
			iconPosX += enchantIcon._width + 2;
		}
		
		readIcon._visible = (a_entryObject.isRead != undefined && a_entryObject.isRead);
		if (readIcon._visible) {
			readIcon._x = iconPosX;
			iconPosX += readIcon._width + 2;
		}
		
		pickPocketChance.autoSize = "right";
		if (a_entryObject.itemChance < 0){
			pickPocketChance.text = "";
		} else {
			var chance = a_entryObject.itemChance.toString();
			chance += "%";
			pickPocketChance.text = chance;
		}

		itemIcon._visible = true;

		_iconLabel = a_entryObject.iconLabel != undefined ? a_entryObject.iconLabel : "default_misc";
		itemIcon.gotoAndStop(_iconLabel);
	
		var iconAlpha = isSelected ? 90 : 50;
		itemIcon._alpha = iconAlpha;
	}
}
