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
				charge._x += charge._width * (enchantmentCharge / 100);
				chargeOutline._visible = true;
			}
		}
	}
}
