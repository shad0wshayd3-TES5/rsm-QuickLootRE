class LootMenu.ListItemRenderer extends gfx.controls.ListItemRenderer
{
	/* PRIVATE VARIABLES */

	private var chargeX: Number = 0;


	/* STAGE ELEMENTS */

	public var charge: MovieClip;
	public var chargeOutline: MovieClip;


	/* INITIALIZATION */

	// @override gfx.controls.ListItemRenderer
	public function ListItemRenderer(a_obj: Object)
	{
		super();
		chargeX = charge._x;
		var chargeMask: MovieClip = charge.duplicateMovieClip("chargeMask", charge.getDepth() + 1);
		chargeMask._width += 1;
		charge.setMask(chargeMask);
	}


	/* PUBLIC FUNCTIONS */

	/**
	 * @override gfx.controls.ListItemRenderer
	 *
	 * @param a_data
	 * 	displayName: String
	 * 	count: Number
	 * 	[enchantmentCharge: Number]
	 */
	public function setData(a_data: Object): Void
	{
		super.setData(a_data);

		charge._x = chargeX - charge._width;
		charge._visible = false;
		chargeOutline._visible = false;
		if (data != null) {
			var displayName: String = data.displayName;
			var count: Number = data.count;

			if (count > 1) {
				displayName += " (" + count.toString() + ")";
			}
			label = displayName;

			var enchantmentCharge: Number = data.enchantmentCharge;
			if (enchantmentCharge != null) {
				var offset: Number = clamp(charge._width * (enchantmentCharge / 100), 0, charge._width);
				charge._x += offset;
				charge._visible = true;
				chargeOutline._visible = true;
			}
		}
	}


	/* PRIVATE FUNCTIONS */

	private function clamp(a_val: Number, a_lo: Number, a_hi: Number): Number
	{
		return Math.min(Math.max(a_lo, a_val), a_hi);
	}
}
