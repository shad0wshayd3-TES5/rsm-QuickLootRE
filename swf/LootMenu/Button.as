class LootMenu.Button extends gfx.controls.Button
{
	/* PRIVATE VARIABLES */

	static private var INVALID: Number = 282;

	private var _xVal: Number = 0;
	private var _yVal: Number = 0;
	private var _needsUpdate: Boolean = true;


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

		_xVal = textField._x;
		_yVal = textField._y;
	}


	/* PRIVATE FUNCTIONS */

	// @override UIComponent
	private function configUI(): Void
	{
		super.configUI();
		doUpdate();
	}


	// @override UIComponent
	private function draw(): Void
	{
		super.draw();
		doUpdate();
	}


	// @override gfx.controls.Button
	private function updateAfterStateChange(): Void
	{
		_needsUpdate = true;
		super.updateAfterStateChange();
	}


	private function doUpdate(): Void
	{
		if (_needsUpdate) {
			icon._x = _xVal;
			icon._y = _yVal;
			textField._x = _xVal;
			textField._y = _yVal;

			var index: Number = data.index != null ? data.index : INVALID;
			icon.gotoAndStop(index);

			var scale: Number = textField.textHeight / icon._height;
			icon._width *= scale;
			icon._height *= scale;

			textField._x += icon._width + 7;

			var w: Number = textField._x - icon._x + textField.textWidth;
			var h: Number = Math.max(icon._height, textField.textHeight);
			setSize(w, h);

			_needsUpdate = false;
		}
	}
}
