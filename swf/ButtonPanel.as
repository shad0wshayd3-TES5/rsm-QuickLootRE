class ButtonPanel extends MovieClip
{
	public var icon: MovieClip;
	public var name: TextField;
	

	public function ButtonPanel()
	{
		super();
	}


	public function SetButtonText(a_name:String): Void
	{
		name.textAutoSize = "shrink";
		name.autoSize = "left";
		name.htmlText = a_name;
	}


	public function UpdateButtonIcon(a_key:Number): Void
	{
		icon.gotoAndStop(a_key);
	}


	public function SetVisible(a_visible:Boolean): Void
	{
		icon._visible = a_visible;
		name._visible = a_visible;
	}
}
