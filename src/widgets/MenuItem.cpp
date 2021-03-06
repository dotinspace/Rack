#include "widgets.hpp"


namespace rack {


#define RIGHT_PADDING 10.0
#define BND_LABEL_FONT_SIZE 13

float MenuItem::computeMinWidth(NVGcontext *vg) {
	return MenuEntry::computeMinWidth(vg) + RIGHT_PADDING + bndLabelWidth(vg, -1, rightText.c_str());
}

void MenuItem::draw(NVGcontext *vg) {
	// Get state
	BNDwidgetState state = (gHoveredWidget == this) ? BND_HOVER : BND_DEFAULT;
	Menu *parentMenu = dynamic_cast<Menu*>(parent);
	if (parentMenu && parentMenu->activeEntry == this) {
		state = BND_ACTIVE;
	}

	bndMenuItem(vg, 0.0, 0.0, box.size.x, box.size.y, state, -1, text.c_str());

	float x = box.size.x - bndLabelWidth(vg, -1, rightText.c_str());
	NVGcolor rightColor = (state == BND_DEFAULT) ? bndGetTheme()->menuTheme.textColor : bndGetTheme()->menuTheme.textSelectedColor;
	bndIconLabelValue(vg, x, 0.0, box.size.x, box.size.y, -1, rightColor, BND_LEFT, BND_LABEL_FONT_SIZE, rightText.c_str(), NULL);
}

void MenuItem::onMouseEnter() {
	Menu *parentMenu = dynamic_cast<Menu*>(parent);
	if (!parentMenu)
		return;

	parentMenu->activeEntry = NULL;

	// Try to create child menu
	Menu *childMenu = createChildMenu();
	if (childMenu) {
		parentMenu->activeEntry = this;
		childMenu->box.pos = parent->box.pos.plus(box.getTopRight());
	}
	parentMenu->setChildMenu(childMenu);
}

void MenuItem::onDragDrop(Widget *origin) {
	if (origin != this)
		return;

	onAction();
	// deletes `this`
	gScene->setOverlay(NULL);
}


} // namespace rack
