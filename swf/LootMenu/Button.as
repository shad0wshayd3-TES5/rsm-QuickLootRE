class LootMenu.Button extends gfx.controls.Button
{
	/* PRIVATE VARIABLES */

	private var _xPos: Number = 0;
	private var _yPos: Number = 0;

	/* STAGE ELEMENTS */

	public var icon: MovieClip;


	/* INITIALIZATION */

	/**
	 * @override gfx.controls.Button
	 *
	 * @param a_obj
	 * 	toggle: Boolean
	 * 	focusTarget: ButtonBar
	 * 	tabEnabled: Boolean
	 * 	autoSize: String
	 */
	public function Button(a_obj: Object)
	{
		super();

		disableConstraints = true;
		textField.autoSize = "left";

		_xPos = textField._x;
		_yPos = textField._y;
	}


	/* PRIVATE FUNCTIONS */

	// @override UIComponent
	private function configUI(): Void
	{
		super.configUI();

		trace("configUI: " + textField.text);

		var index: Number = data.index != null ? data.index : 282;
		icon.gotoAndStop(index);
		icon._x = _xPos;
		icon._y = _yPos;

		var scale: Number = textField.textHeight / icon._height * 100;
		icon._xscale = scale;
		icon._yscale = scale;

		textField._x += icon._width + 7;

		var w: Number = textField._x - icon._x + textField.textWidth;
		var h: Number = Math.max(icon._height, textField.textHeight);
		setSize(w, h);
	}
}
