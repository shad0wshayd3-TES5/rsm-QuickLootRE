import gfx.controls.ListItemRenderer;


class LootMenuRenderer extends ListItemRenderer
{
	public function LootMenuRenderer()
	{
		super();
	}


	public function setData(data: Object): Void
	{
		super.setData(data);

		if (data != null) {
			var displayName: String = this.data.displayName;
			var count: Number = this.data.count;

			if (count > 1) {
				displayName += " (" + count.toString() + ")";
			}
			this.label = displayName;
		}
	}
}
