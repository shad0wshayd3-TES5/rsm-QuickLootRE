class ButtonPanel extends MovieClip
{
	public var icon: MovieClip;
	public var name: TextField;
	

	public function ButtonPanel()
	{
		super();
	}


	public function SetButton(a_key:Number, a_name:String): Void
	{
		icon.gotoAndStop(a_key);
		name.textAutoSize = "shrink";
		name.autoSize = "left";
		name.htmlText = a_name;
	}


	public function UpdateButton(a_key:Number): Void
	{
		icon.gotoAndStop(a_key);
	}
	
	
	public function HideButton(): Void
	{
		icon.gotoAndStop("Hidden");
	}
}
