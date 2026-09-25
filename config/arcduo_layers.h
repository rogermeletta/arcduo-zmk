// Layer indices, in one place, for everything that needs to name a layer.
//
// This is a header rather than a block of #defines at the top of the keymap
// because the trackball input-listener children in
// boards/shields/arcduo/arcduo.dtsi name layers too, and that file belongs to
// the shield -- it is preprocessed whether or not the keymap is in the picture.
// Before this, those three `layers = <N>` properties were the only copies of
// the layer numbering living outside config/arcduo.keymap, in a file you would
// not think to open when adding a layer.
//
// The `L_` prefix is load-bearing, not decoration. A bare `#define BASE 0`
// would also rewrite the `BASE { ... }` layer *node* into `0 { ... }` and break
// the devicetree -- which is exactly why these defines sat commented out in
// config/arcduo.keymap for as long as they did. Prefixing them sidesteps the
// collision and leaves the node names (and so ZMK Studio, and the diagram's
// display-name headings) untouched.
//
// The include guard is required, not habit: Zephyr preprocesses every
// devicetree fragment of a build in a single pass, so this file can legitimately
// be reached twice -- once via the shield, once via the keymap.
//
// If you add or move a layer, this file and the keymap's layer *order* are the
// only two things that have to agree. See docs/UHK-PARITY.md for what each
// layer is for, and the note in README.md about ZMK Studio's persisted layer
// order, which does NOT follow a renumbering on its own.

#ifndef ARCDUO_LAYERS_H
#define ARCDUO_LAYERS_H

#define L_BASE    0
#define L_NUM     1   // the UHK's Fn layer
#define L_NAV     2   // the UHK's Mod layer
#define L_SYM     3
#define L_EXTRAS  4   // the UHK's window-manager chords
#define L_MOUSE   5

#endif
