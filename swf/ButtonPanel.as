
class ButtonPanel extends MovieClip
{
	public var icon: MovieClip;
	public var name: TextField;
	
	public function ButtonPanel()
	{
		super();
	}

	public function setButton(a_key:Number, a_name:String): Void
	{
		icon.gotoAndStop(a_key);
		name.textAutoSize = "shrink";
		name.autoSize = "left";
		name.htmlText = a_name;
	}
}
