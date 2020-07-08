import gfx.controls.Button;
import gfx.controls.ButtonBar;


class LootMenuButton extends Button
{
	/* PRIVATE VARIABLES */

	private var _focusTarget: ButtonBar = null;

	/* STAGE ELEMENTS */

	public var icon: MovieClip;


	/* INITIALIZATION */

	/**
	 * @override Button
	 *
	 * @param a_obj
	 * 	toggle: Boolean
	 * 	focusTarget: ButtonBar
	 * 	tabEnabled: Boolean
	 * 	autoSize: String
	 */
	public function LootMenuButton(a_obj: Object)
	{
		super();

		disableConstraints = true;
		textField.autoSize = "left";
	}


	/* PRIVATE FUNCTIONS */

	// @override UIComponent
	private function configUI(): Void
	{
		super.configUI();

		var index: Number = data.index != null ? data.index : 282;
		icon.gotoAndStop(index);
		icon._x = textField._x;
		icon._y = textField._y;

		var scale: Number = textField.textHeight / icon._height * 100;
		icon._xscale = scale;
		icon._yscale = scale;

		textField._x += icon._width + 7;

		var w: Number = textField._x - icon._x + textField.textWidth;
		var h: Number = Math.max(icon._height, textField.textHeight);
		setSize(w, h);
	}


	// @override UIComponent
	private function draw(): Void
	{
		super.draw();
	}
}
