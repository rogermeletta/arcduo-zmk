# UHK → ArcDuo parity

This branch reshapes the keymap after the Ultimate Hacking Keyboard v1 config in
`UserConfiguration.json` (Agent v9.0.2, firmware v16.0.0), default keymap
**MAC — "QWERTY for Mac"**, so that moving between the two boards costs as
little as possible.

The UHK has **72 keys** — 34 on the right half, 32 on the left, 6 on the left
key-cluster module and 2 on the right trackball module. The ArcDuo has **36**,
plus two trackballs. Half the keys are simply not there, so this is a mapping of
*functions*, not of positions. What actually transfers between the two boards is
the chord your fingers send to macOS, not where your fingers were.

---

## What the UHK config actually contains

Decoded from the raw JSON rather than from the Agent UI:

| | |
| --- | --- |
| Keymaps | `MAC` (default), `PC`, `GAM`, `COL` |
| Layers per keymap | base, mod, fn, mouse |
| Macros | 9, of which 6 are bound on MAC |
| Modules | left key cluster (mini trackball + 3 keys), right trackball |
| Secondary roles | **none used** — no home-row mods anywhere |
| Layer switching | `hold`, and `holdAndDoubleTapToggle` for mod/mouse |
| Host connections | USB only; no BLE hosts paired |

Two things in there are dead weight and were not carried over: a macro named
`copy` whose body is the literal text `just text`, and a macro named `@` that
types `@` — useful on a Swiss layout, redundant on the US QWERTY this keymap
actually sends.

---

## Layer mapping

| UHK layer | ArcDuo layer | Reached by |
| --- | --- | --- |
| base | `BASE` (0) | default |
| mod | `NAV` (2) | hold **left inner** thumb |
| fn | `NUM` (1) | hold **left outer** thumb (Tab), or right outer thumb (Bspc) |
| mouse | `MOUSE` (5) | hold right inner thumb |
| — | `EXTRAS` (4) | hold right middle thumb (Space) — window-manager chords |
| — | `SYM` (3) | hold `G` |

Both of the layers carried over from the UHK are held with the **left thumb**,
because that is how they are held on the UHK: Fn and Mod are both left thumb
keys there, with Fn outboard of Mod, and the ArcDuo now matches that order. It
matters more than it looks. The UHK's Mod layer puts window and tab management
under the left fingers and the arrows under the right hand; holding Mod with the
right thumb, as an earlier draft of this branch did, meant driving the arrows
with the same hand that was holding the layer down.

`EXTRAS` moved to the Space thumb to make room. Its own left-hand contents are
unaffected — the thumb that holds it is on the other hand.

The one place the correspondence breaks is the window-manager chord family
(`Opt+Cmd+<digit>` and `Ctrl+Opt+Cmd+<key>`), which is nine chords on its own and
does not fit next to the arrows. It lives on `EXTRAS`.

---

## Where each UHK function landed

### Carried over

**Mod layer (NAV) — left hand.** Tab and window management, all reachable with
the left fingers while the left thumb holds the layer, which is how it is used
on the UHK.

| Function | Chord | Position |
| --- | --- | --- |
| New tab | `Cmd+T` | 0 |
| Close tab / window | `Cmd+W` | 1 |
| Previous tab | `Opt+Cmd+←` | 2 |
| Next tab | `Opt+Cmd+→` | 3 |
| Cycle windows of current app | ``Cmd+` `` | 4 |
| App switcher | `Cmd+Tab` | 14 |
| AirPods noise cancelling (AirBuddy) | `Ctrl+Opt+Cmd+T` | 20 |
| AirPods force connect | `Ctrl+Enter` | 21 |
| Screenshot / recording tool | `Shift+Cmd+5` | 22 |
| Fullscreen | `Ctrl+Shift+Cmd+F` | 23 |
| Sleep displays | `Opt+Cmd+Eject` | 24 |

**Mod layer (NAV) — right hand.** Navigation, driven by the hand that is not
holding the layer.

| Function | Position |
| --- | --- |
| Arrows (inverted T) | 7, 16, 17, 18 |
| Line start / end, back / forward (`Cmd+←/→`) | 15, 25 |
| Home, PgDn, PgUp, End | 26, 27, 28, 29 |
| Print Screen, Caps Lock, Insert, Delete | 5, 6, 8, 9 |
| 1Password | `Shift+Cmd+Space` | 33 |
| Delete word (`Opt+Bspc`) | 35 |

Positions 10-13 stay transparent so the home-row modifiers still work — that is
what makes `Shift+←` select text on this layer.

**Fn layer (NUM).**

| Function | Chord | Position |
| --- | --- | --- |
| Clipboard history | `Shift+Cmd+V` | 23 |
| Sanitised paste | `Cmd+Opt+V` | 24 |
| ä ö ü | `Opt+U` then vowel | 20, 21, 22 |
| Backspace | | 14 |
| Numbers, F1–F12 | | row 0, F-keys on hold |
| Numpad and operators | | right hand, unchanged |

**Window-manager family (EXTRAS).**

| Chord | Position |
| --- | --- |
| `Opt+Cmd+1` / `2` / `3` | 20, 21, 22 |
| `Ctrl+Opt+Cmd+4` / `5` | 23, 24 |
| `Opt+Cmd+6` | 3 |
| `Ctrl+Opt+Cmd+V` / `B` | 30, 31 |

**Elsewhere.**

| Function | Chord | Where |
| --- | --- | --- |
| Raycast | `Cmd+Space` | combo, left-click + Space thumbs |
| Mouse move / scroll / clicks | | MOUSE layer, unchanged |
| Transport (prev, play-pause, next) | | EXTRAS 16, 17, 18 |

The window-manager chords are deliberately left as chords rather than given
names. `Ctrl+Opt+Cmd+<key>` and `Opt+Cmd+<digit>` are the binding style of
third-party window managers (Rectangle, Moom, BetterTouchTool and friends);
which one, and what each does, is not knowable from the UHK config, and guessing
in a comment would be worse than saying nothing.

### Not carried over, and why

| UHK function | Why not |
| --- | --- |
| **Caret mode on the trackballs** | Not possible in ZMK at the pinned SHA. See below. |
| **ä ö ü on the right hand** | The Fn right hand is the numpad, which is staying. The umlauts are on the Fn *left* hand instead; the gesture is still thumb-holds / fingers-type, just on one hand. Freeing three right-hand positions means giving up either the numpad's `1 2 3` (duplicated on the top row anyway) or its `-` `=` `/` operators — a small edit if it turns out to matter. |
| Numeric keypad `KP_*` scancodes | The digits are the ordinary number keycodes, not the keypad ones. macOS treats them identically outside a handful of apps. |
| Keymap switching (MAC / PC / GAM / COL) | ZMK has no runtime keymap switching. `&to` can swap base layers if you ever want a gaming or Windows base — say the word and it is a small change. |
| Scroll Lock, Pause/Break | No function on macOS. |
| `Alt+Tab` | Not a macOS shortcut; `Cmd+Tab` is on NAV 2. |
| Macros `@` and `copy` | Dead weight, see above. |
| Mouse accelerate / decelerate | Partially — see below. |

---

## The trackballs

The UHK gives each pointing module a **navigation mode per layer**. Roger's
config:

| Module | base | mod | fn |
| --- | --- | --- | --- |
| Left key cluster | Scroll | Cursor | Caret |
| Right trackball | Cursor | Scroll | Caret |

The first two columns map directly onto ZMK's per-layer input-processor
overrides, and now do: on `NAV` **both balls swap roles**, left becomes the
cursor and right becomes scroll, exactly as on the UHK.

The scroll rate on the right ball is `1/96`, not the UHK's `scrollSpeedDivisor`
of 8. The divisor is meaningless across different hardware; what was matched
instead is the *feel* of the left ball, which scrolls one tick per 2.0 mm of
travel (200 CPI ÷ 16). At 1200 CPI the same 2 mm is 96 counts.

### Caret mode is not possible here

Worth stating plainly, because it looks like it should be. ZMK's
`zmk,input-processor-behaviors` maps input codes to behaviors, so
`INPUT_REL_X → &kp LEFT` looks like the obvious way to build caret mode. It does
not work, for two independent reasons visible in
`app/src/pointing/input_processor_behaviors.c`:

1. It calls `zmk_behavior_invoke_binding(binding, event, event->value)`, whose
   third parameter is `bool pressed`. For a relative motion event `value` is a
   signed delta, so every motion event is a **press** and no release is ever
   sent — the arrow key would latch down.
2. It matches on `event->code` only, never on the sign of `event->value`, so it
   cannot tell left from right, or up from down.

Nothing else at this SHA converts pointer motion into key presses. Doing it
properly needs an out-of-tree input processor. Until then, arrows on `NAV`
(positions 7, 16, 17, 18) are the substitute.

### Mouse accelerate / decelerate

The UHK has momentary speed keys on its mouse layer. ZMK has no runtime
equivalent: `&mmv` and `&msc` take their acceleration from
`time-to-max-speed-ms` and `acceleration-exponent`, which are properties of the
behavior node, not of the binding.

What *is* possible, and already present, is the layer-scoped scaler override —
the existing snipe mode on `MOUSE`, which drops the cursor ball to a quarter
speed. A "fast" counterpart would be another override with a multiplier above 1.
Additional `zmk,behavior-input-two-axis` nodes with different acceleration
settings would give the mouse *keys* a fast and slow variant, if that turns out
to be missed.

---

## Things to check on hardware

1. **Left-ball cursor direction on NAV.** The transform there is derived, not
   measured — the two sensors are mounted mirrored and their drivers already
   differ (`invert-x` is set on the right sensor only). If the pointer moves
   mirrored, add or drop one `INPUT_TRANSFORM_*` flag on the `cursor` node in
   `boards/shields/arcduo/arcduo.dtsi`; nothing else changes.
2. **Right-ball scroll rate on NAV.** `1/96` is calculated, not felt. Adjust the
   divisor on the `scroll` node in the same file.
3. **The Spotlight combo** (thumbs 30+31). Thumb combos can be awkward; if it
   misfires or is hard to hit, it can move to any free position.

Flash all three parts: the halves for the sensor, the dongle for the keymap and
the listener chains.
