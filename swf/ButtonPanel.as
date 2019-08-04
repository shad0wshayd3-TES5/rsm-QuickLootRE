class ButtonPanel extends MovieClip
{
	/* STAGE ELMENTS */

	public var icon: MovieClip;
	public var name: TextField;
	

	/* INITIALIZATION */

	public function ButtonPanel()
	{
		super();
	}


	/* PUBLIC FUNCTIONS */

	public function setButtonText(a_name:String):Void
	{
		name.textAutoSize = "shrink";
		name.autoSize = "left";
		name.htmlText = a_name;
	}


	public function updateButtonIcon(a_key:Number):Void
	{
		icon.gotoAndStop(a_key);
		name._x = icon._x + icon._width + 7;
	}


	public function setVisible(a_visible:Boolean):Void
	{
		icon._visible = a_visible;
		name._visible = a_visible;
	}
}
