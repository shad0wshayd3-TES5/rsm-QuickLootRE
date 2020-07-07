import gfx.controls.ListItemRenderer;


class LootMenuRenderer extends ListItemRenderer
{
	/* PRIVATE VARIABLES */

	private var itemChargeX: Number = 0;

	/* STAGE ELEMENTS */

	public var itemCharge: MovieClip;
	public var itemChargeOutline: MovieClip;


	/* INITIALIZATION */

	public function LootMenuRenderer()
	{
		super();

		itemChargeX = itemCharge._x;
		var itemChargeMask: MovieClip = itemCharge.duplicateMovieClip("itemChargeMask", itemCharge.getDepth() + 1);
		itemCharge.setMask(itemChargeMask);
	}


	/* PUBLIC FUNCTIONS */

	public function setData(data: Object): Void
	{
		super.setData(data);

		itemCharge._x = itemChargeX - itemCharge._width;
		itemChargeOutline._visible = false;
		if (data != null) {
			var displayName: String = this.data.displayName;
			var count: Number = this.data.count;

			if (count > 1) {
				displayName += " (" + count.toString() + ")";
			}
			this.label = displayName;

			var enchantmentCharge: Number = this.data.enchantmentCharge;
			if (enchantmentCharge != null) {
				itemCharge._x += itemCharge._width * (enchantmentCharge / 100);
				itemChargeOutline._visible = true;
			}
		}
	}
}
