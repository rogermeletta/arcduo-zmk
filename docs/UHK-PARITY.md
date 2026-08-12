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
| mod | `NAV` (2) | hold right thumb (Space) |
| fn | `NUM` (1) | hold left outer thumb (Tab) or right outer thumb (Bspc) |
| mouse | `MOUSE` (5) | hold right inner thumb |
| — | `EXTRAS` (4) | hold left inner thumb — the window-manager chords |
| — | `SYM` (3) | hold `G` |

The UHK's mod layer is its workhorse: arrows, window management and tab
navigation all on one held thumb. `NAV` already was the ArcDuo's arrow layer, so
it absorbs the rest of that role. The one place the correspondence breaks is the
window-manager chord family (`Ctrl+Opt+Cmd+…`), which is ten chords on its own
and does not fit next to the arrows — it lives on `EXTRAS` instead.

---

## Where each UHK function landed

### Carried over

| UHK function | Chord | ArcDuo |
| --- | --- | --- |
| Screenshot / recording tool | `Shift+Cmd+5` | NAV 0 |
| Cycle windows of current app | `Cmd+\`` | NAV 1 |
| App switcher | `Cmd+Tab` | NAV 2 |
| Close window / tab | `Cmd+W` | NAV 3 |
| New tab | `Cmd+T` | NAV 4 |
| Sleep displays | `Opt+Cmd+Eject` | NAV 14 |
| Back / line start | `Cmd+←` | NAV 20 |
| Forward / line end | `Cmd+→` | NAV 21 |
| Previous tab | `Opt+Cmd+←` | NAV 22 |
| Next tab | `Opt+Cmd+→` | NAV 23 |
| Fullscreen | `Ctrl+Shift+Cmd+F` | NAV 24 |
| Caps Lock | `CAPS` | NAV 6 |
| Print Screen / Insert / Delete | | NAV 5 / 8 / 9 |
| Arrows, Home, End, PgUp, PgDn | | NAV 7, 16-18, 26-29 |
| Play-pause, mute, prev, next | | NAV 15, 25, 30, 31 |
| Delete word | `Opt+Bspc` | NAV 35 |
| Spotlight | `Cmd+Space` | combo, thumbs 30+31 |
| ä ö ü | `Opt+U` then vowel | NUM 20, 21, 22 |
| F1–F12 | | NUM row 0, on hold |
| Window manager | `Opt+Cmd+1/2/3` | EXTRAS 20, 21, 22 |
| Window manager | `Ctrl+Opt+Cmd+4/5` | EXTRAS 23, 24 |
| Window manager | `Opt+Cmd+6` | EXTRAS 3 |
| Window manager | `Ctrl+Opt+Cmd+T` | EXTRAS 15 |
| Window manager | `Ctrl+Opt+Cmd+V` | EXTRAS 30 |
| Window manager | `Ctrl+Opt+Cmd+B` | EXTRAS 31 |
| Window manager | `Shift+Cmd+V` | EXTRAS 34 |
| Mouse move / scroll / clicks | | MOUSE layer, unchanged |

The window-manager chords are deliberately left as chords rather than given
names. `Ctrl+Opt+Cmd+<key>` and `Opt+Cmd+<digit>` are the binding style of
third-party window managers (Rectangle, Moom, BetterTouchTool and friends);
which one, and what each does, is not knowable from the UHK config, and guessing
in a comment would be worse than saying nothing.

### Not carried over, and why

| UHK function | Why not |
| --- | --- |
| **Caret mode on the trackballs** | Not possible in ZMK at the pinned SHA. See below. |
| Numeric keypad (`KP_0`–`KP_9`, `KP_.`) | Needs a 3×3 block plus thumbs; the number row on `NUM` already covers the digits, and macOS treats keypad digits identically outside a handful of apps. |
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
