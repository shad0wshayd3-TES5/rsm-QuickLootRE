import gfx.controls.ButtonBar;


class LootMenuButtonBar extends ButtonBar
{
	/* INITIALIZATION */

	// @override ButtonBar
	public function LootMenuButtonBar()
	{
		super();
	}


	/* PRIVATE FUNCTIONS */

	/**
	 * @override ButtonBar
	 * implements a horizontal, centered layout
	 */
	private function drawLayout(): Boolean
	{
		// If the (last) renderer is not yet ready, invalidate to force a redraw.
		if (renderers.length > 0 && !renderers[renderers.length - 1].initialized) {
			reflowing = true;
			invalidate();
			return false;
		}
		reflowing = false;

		var totalW: Number = 0;
		for (var i: Number = 0; i < renderers.length; ++i) {
			var renderer: MovieClip = renderers[i];
			// Manually size the renderer
			if (_autoSize == "none" && _buttonWidth > 0) {
				renderer.width = _buttonWidth;
			}

			totalW += renderer.width;
		}

		totalW += _spacing * (renderers.length - 1);

		var calcY: Function = function(a_height: Number): Number {
			if (a_height < this.height) {
				return (this.height - a_height) / 2;
			} else {
				return 0;
			}
		};

		var midpoint: Number = width / 2;
		var w: Number = midpoint - totalW / 2;
		for (var i: Number = 0; i < renderers.length; ++i) {
			var renderer: MovieClip = renderers[i];
			renderer._x = w;
			renderer._y = calcY(renderer.height);
			renderer._visible = true;
			w += renderer.width + _spacing;
		}

		return true;
	}


	// @override ButtonBar
	private function createRenderer(a_index: Number): MovieClip
	{
		var renderer: MovieClip = super.createRenderer(a_index);
		renderer._visible = false;	// avoid visible snapping into place
		return renderer;
	}
}
