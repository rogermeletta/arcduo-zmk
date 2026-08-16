# UHK → ArcDuo parity

This branch reshapes the keymap after the Ultimate Hacking Keyboard v1 config in
`UserConfiguration.json` (Agent v9.0.2, firmware v16.0.0), default keymap
**MAC — "QWERTY for Mac"**, so that moving between the two boards costs as
little as possible.

The UHK has **72 keys** — 34 on the right half, 32 on the left, 6 on the left
key-cluster module and 2 on the right trackball module. The ArcDuo has **36**,
plus two trackballs. Half the keys are simply not there.

## The mapping rule

Layers are mapped **by base-layer legend, not by physical position**. Whatever
the UHK's Mod layer puts on its `Q` key is what `NAV` puts on `Q`; whatever Fn
puts on `V` is what `NUM` puts on `V`.

This is the whole trick, and it is worth stating explicitly because the obvious
alternative — walk the UHK's key positions in order and pour them into the
ArcDuo's 36 — produces a keymap that is wrong in a way that is hard to see. The
muscle memory being moved is *"Mod plus C closes the tab"*. The finger does not
know or care that the UHK's `C` is the fifth key of the fourth row and the
ArcDuo's is the third key of the third; it knows it is the left middle finger,
one row down. Map by position and every one of those pairings silently shifts.

Under this rule **28 of the 30 alpha positions on `NAV` come straight across**,
and the two that do not are the two the UHK leaves empty. The Fn layer's numpad
lands on the same nine right-hand keys it occupies on the UHK. Almost nothing
had to be invented.

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
| mod | `NAV` (2) | hold **left inner** thumb, or the right **Space** thumb |
| fn | `NUM` (1) | hold **left outer** thumb (Tab), or right outer thumb (Bspc) |
| mouse | `MOUSE` (5) | **toggle**: hold `EXTRAS` (right inner), tap the left-click thumb |
| — | `EXTRAS` (4) | hold right **inner** thumb — radios, brightness, transport |
| — | `SYM` (3) | hold `G` (type right) or `H` (type left) |

Both of the layers carried over from the UHK are reachable with the **left
thumb**, because that is how they are held on the UHK: Fn and Mod are both left
thumb keys there, with Fn outboard of Mod, and the ArcDuo matches that order. It
matters more than it looks. The UHK's Mod layer puts window and tab management
under the left fingers and the arrows under the right hand; holding Mod with
*only* the right thumb, as an earlier draft of this branch did, meant driving
the arrows with the same hand that was holding the layer down.

`NAV` is on the right **Space** thumb as well, which the UHK has no equivalent
of. Held there, the layer and the arrows are on the same hand, so the right hand
can navigate alone. It gets the Space thumb rather than the inner one on
frequency: `NAV` is reached constantly and `EXTRAS` almost never, so `EXTRAS`
takes the inner key.

The window-manager chord family (`Opt+Cmd+<digit>` and `Ctrl+Opt+Cmd+<key>`) was
briefly exiled to `EXTRAS`, on the theory that nine chords could not fit beside
the arrows. That was an artefact of the by-position mapping. By legend they fit
exactly where the UHK has them — `N M , . /` and `V B` and `G` — with no
crowding at all, so `EXTRAS` no longer carries a copy.

---

## Where each UHK function landed

### Carried over

**Mod layer → `NAV`.** Read this as the mapping rule in action: the left column
is the key's *base* legend on both boards, and the two right columns are what
the UHK's Mod layer and this layer put on it.

| Key | UHK Mod | ArcDuo `NAV` | |
| --- | --- | --- | --- |
| `Q` | Escape | Escape | |
| `W` | `Opt+Cmd+←` | same | previous tab |
| `E` | `Cmd+T` | same | new tab |
| `R` | `Opt+Cmd+→` | same | next tab |
| `T` | `Ctrl+Shift+Cmd+F` | same | fullscreen |
| `A` | Caps Lock | same, **hold = Cmd** | |
| `S` | `Cmd+←` | same, **hold = Opt** | back |
| `D` | `Cmd+Tab` | same, **hold = Ctrl** | app switcher |
| `F` | `Cmd+→` | same, **hold = Shift** | forward |
| `G` | `Opt+Cmd+6` | same | |
| `Z` | — | `Shift+Cmd+5` | *deviation, see below* |
| `X` | — | ``Cmd+` `` | *deviation, see below* |
| `C` | `Cmd+W` | same | close tab |
| `V` | `Ctrl+Opt+Cmd+V` | same | |
| `B` | `Ctrl+Opt+Cmd+B` | same | |
| `Y` | PgUp | same | |
| `U` | Home | same | |
| `I` | ↑ | same | |
| `O` | End | same | |
| `P` | Delete | same | |
| `H` | PgDn | same | |
| `J` `K` `L` | ← ↓ → | same | |
| `;` | Insert | `Ctrl+Opt+Cmd+T` | *deviation, see below* |
| `N` `M` `,` | `Opt+Cmd+1/2/3` | same | |
| `.` `/` | `Ctrl+Opt+Cmd+4/5` | same | |

Thumbs, which have no positional correspondence at all — the UHK has five per
side plus a bottom modifier row, this board has three:

| Function | Position |
| --- | --- |
| 1Password (`Shift+Cmd+Space`) | 33 |
| Delete word (`Opt+Bspc`) | 35 — the UHK's `delete word (mac)` macro |
| Left click, Tab, Return, Space | pass through from `BASE` |

### The three deviations

**`Z` and `X`.** The UHK leaves both empty on Mod. They take the two left-hand
functions whose UHK keys do not exist here: `Shift+Cmd+5` (screenshot, on the
UHK's `Tab`) and ``Cmd+` `` (cycle windows of the front app, on the UHK's ISO
key). Both stay on the left hand, which is the hand that has them on the UHK.

**`;`.** The UHK has Insert here. Insert does nothing on macOS. It becomes
`Ctrl+Opt+Cmd+T` — AirPods noise cancelling via AirBuddy — which the UHK has on
`RShift`. A 36-key board has no `RShift`, and the right pinky home is the
closest thing to it.

**The home row holds.** The UHK's Mod layer keeps LShift, LCtrl, LCmd and
LOption on dedicated keys in its bottom row, so `Shift+←` still selects text
there. This board's modifiers *are* the home row, and the UHK also puts Caps,
`Cmd+←`, `Cmd+Tab` and `Cmd+→` on that same home row — so positions 10-13 are
`&hml` rather than `&kp`: tap for the UHK function, hold for the modifier.
`hold-trigger-key-positions` on `&hml` is the right hand only, which is exactly
where the arrows are, and `hold-trigger-on-release` lets several chord at once,
so `Cmd+Shift+←` works.

Escape appears twice by different routes and that is deliberate: `NAV` has it on
`Q` per the rule, and `BASE` has it as the `Q`+`W` combo. The combo is scoped to
`BASE` only — armed on `NAV` it would stack a second Escape on the real one and
misfire on Escape-then-`Opt+Cmd+←`, which is Escape followed by the very next
key over while switching tabs.

**Fn layer → `NUM`.**

| Key | UHK Fn | ArcDuo `NUM` | |
| --- | --- | --- | --- |
| `V` | `Shift+Cmd+V` | same | clipboard history |
| `B` | — | `Cmd+Opt+V` | sanitised paste, not a UHK binding |
| `U` `I` `O` | 7 8 9 | same | |
| `J` `K` `L` | 4 5 6 | same | |
| `N` `M` `,` | 1 2 3 | **`M` `,` `.`** | see below |
| `Z` `X` `C` | — | ä ö ü | UHK has these on `[` `;` `'` |
| Backspace thumb | Backspace | same | |
| — | — | 1–0 with F1–F10 on hold | row 0; this board has no number row |

**The numpad is the one place the legend rule is deliberately broken.** The UHK
puts 1 2 3 on `N M ,`. Doing that here would put the numpad's three rows in the
columns `U`/`J`/`N` — and on this board `N` is one column left of `J`, so 7, 4
and 1 would not line up. On the UHK's row-staggered layout they physically do.
A numpad whose columns do not line up is not a numpad, so 1 2 3 sit on `M` `,`
`.` instead, directly under 4 5 6 and 7 8 9.

Fn is reachable from **either** outer thumb — left (Tab) or right (Backspace).
The umlauts and both clipboard chords are on the left hand, so for those hold Fn
with the **right** thumb and type with the left; for the numpad hold it with the
left thumb and type with the right. Same one-hand-holds, other-hand-types shape
either way.

**Elsewhere.**

| Function | Chord | Where |
| --- | --- | --- |
| Raycast | `Cmd+Space` | combo, left-click + Space thumbs (80 ms window) |
| Mouse move, scroll, left and middle click | | MOUSE layer |
| Right click | | combo, `F` + left-click thumb — on BASE and MOUSE |
| Transport (prev, play-pause, next) | | EXTRAS 16, 17, 18 |

The window-manager chords are deliberately left as chords rather than given
names. `Ctrl+Opt+Cmd+<key>` and `Opt+Cmd+<digit>` are the binding style of
third-party window managers (Rectangle, Moom, BetterTouchTool and friends);
which one, and what each does, is not knowable from the UHK config, and guessing
in a comment would be worse than saying nothing.

### Not carried over, and why

| UHK function | Why not |
| --- | --- |
| **Caret mode on the trackballs** | Not possible in *upstream* ZMK at the pinned SHA — but an out-of-tree module does it. See below. |
| Print Screen, Scroll Lock, Pause/Break | UHK `[`, `]`, `\` on Mod. None of the three does anything on macOS, and none of those keys exists here. |
| `Cmd+Tab` cycling | `&kp LG(TAB)` on `D` presses and releases Cmd, so it only ever flips to the previous app — you cannot hold and step through the list. The UHK has the same limitation. A tri-state behaviour fixes it; see the note at the end. |
| **ä ö ü on the right hand** | The Fn right hand is the numpad, which is staying. The umlauts are on the Fn *left* hand instead; the gesture is still thumb-holds / fingers-type, just on one hand. Freeing three right-hand positions means giving up either the numpad's `1 2 3` (duplicated on the top row anyway) or its `-` `=` `/` operators — a small edit if it turns out to matter. |
| Numeric keypad `KP_*` scancodes | The digits are the ordinary number keycodes, not the keypad ones. macOS treats them identically outside a handful of apps. |
| **Double-tap-to-toggle on Mod and Mouse** | The UHK sets `holdAndDoubleTapToggle` on six of its layer keys, so Mod and Mouse can be *locked* for a long selection or a long scroll rather than held. Here both are hold-only. Adding it is a small change — make position 32 a tap-dance `bindings = <&lt 2 RETURN>, <&tog 2>`, and the same for 33 with layer 5, giving the toggled layer an exit key. It is left out for now because a tap-dance re-introduces a tapping-term delay on that key (see the note in `config/behaviors.dtsi` about why three of them were just removed), and because holding has been enough so far. Say the word. |
| ~~`'` and `\`~~ | **Fixed.** Both, plus `"` and `|`, were untypeable on this board on any layer — `\` existed only inside the shrug macro, and `"` and `|` are the shifted forms of `'` and `\`, so they died with them. All four are now on `SYM`'s home row, on both halves. |
| Keymap switching (MAC / PC / GAM / COL) | ZMK has no runtime keymap switching. `&to` can swap base layers if you ever want a gaming or Windows base — say the word and it is a small change. |
| Scroll Lock, Pause/Break | No function on macOS. |
| `Alt+Tab` | Not a macOS shortcut; `Cmd+Tab` is on `NAV`'s `D`. |
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

### Caret mode needs an out-of-tree module

Worth stating plainly, because it looks like it should be built in. ZMK's
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

Nothing else at this SHA converts pointer motion into key presses.

What *does* work is an out-of-tree input processor:
[`zmk-input-processor-keybind`](https://github.com/te9no/zmk-input-processor-keybind).
It quantises motion into discrete key taps — `bindings = <&kp RIGHT>, <&kp LEFT>,
<&kp UP>, <&kp DOWN>` in `+X, −X, +Y, −Y` order, with `threshold` and `tick`
controlling how far the ball travels per keypress — which is exactly the UHK's
Caret mode. It drops into a per-layer override on the listener nodes the same way
snipe mode does.

It is not wired up here, for one reason worth quoting rather than paraphrasing.
From its own README: *"This module is currently under active development and
somewhat works. Use at your own risk."* Adding it also means a fourth pinned
west dependency, in a repo whose entire eight-month outage was caused by
unpinned ones. Until then, arrows on `NAV` (`I`, `J`, `K`, `L`) are the
substitute.

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
   divisor on the `scroll` node in the same file. Its *direction* is settled —
   `INPUT_TRANSFORM_Y_INVERT` was dropped from that node because vertical scroll
   came out the wrong way round. Horizontal is still `X_INVERT`; the two flags
   are independent, but note both are evaluated *after* `XY_SWAP`.
3. **The Raycast combo** (thumbs 31+34 — left click and right Space). Thumb
   combos can be awkward; if it misfires or is hard to hit, it can move to any
   free position. Note a miss is not a no-op: position 31 is left click, so a
   failed combo clicks wherever the pointer is and then types a space. That is
   why its window is 80 ms rather than ZMK's default 50.
4. **The `&hml` taps on `NAV`'s home row.** Tap `A` for Caps Lock, hold it for
   Cmd. If the taps feel like they need too long a press, `tapping-term-ms` on
   `hml` in `config/behaviors.dtsi` is the knob — it is currently 190 ms, and
   urob's timerless setup (which the rest of this config already follows) uses
   280. Longer is *safer* against false taps here, not slower.

Flash all three parts: the halves for the sensor, the dongle for the keymap and
the listener chains.

---

## Two things worth adding later

**`Cmd+Tab` cannot cycle.** `&kp LG(TAB)` presses and releases Cmd in one go, so
it always flips to the previous app and never steps through the list. You cannot
work around it with the Cmd home-row mod either: `hml`'s
`hold-trigger-key-positions` does not include `D`, so holding `A` and pressing
`D` resolves `A` as a tap. The fix is a tri-state behaviour —
`bindings = <&kt LGUI>, <&kp TAB>, <&kt LGUI>` — which holds Cmd across repeated
taps and releases it on the next unrelated key. It is not upstream (PR #1366
closed for inactivity) but exists as
[`caksoylar/zmk-smart-toggle`](https://github.com/caksoylar/zmk-smart-toggle) and
[`dhruvinsh/zmk-tri-state`](https://github.com/dhruvinsh/zmk-tri-state). The UHK
has the same limitation, so this would be an upgrade rather than parity.

**Double-tap-to-toggle**, described in the table above. No new dependency, and
it is the only `holdAndDoubleTapToggle` behaviour from the UHK that is missing.
