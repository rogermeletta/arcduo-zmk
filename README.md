# ArcDuo

A wireless, split, 36-key column-staggered keyboard with an integrated
trackball in **each** half, driven by a dongle.

![ArcDuo](https://github.com/user-attachments/assets/2eb6655a-ef4d-4e0f-bca6-efd063d21898)

## Hardware

| Part | Board | Notes |
| --- | --- | --- |
| Left half | nice!nano v2 | 5x3 matrix + 3 thumb keys, PMW3610 trackball (scroll) |
| Right half | nice!nano v2 | 5x3 matrix + 3 thumb keys, PMW3610 trackball (cursor) |
| Dongle | Seeed XIAO nRF52840 | Split central, ST7789V screen + APDS9960 light sensor |

Both halves are BLE **peripherals**; the dongle is the split **central**. The
dongle is therefore required — it is the part that runs the keymap, and the
halves will not act as a keyboard without it.

## Trackballs

Both balls appear on the keymap diagram below as the two circles facing each
other across the split, at the inner edge of each half. The halves are mirror
images, balls included.

| | Left ball | Right ball |
| --- | --- | --- |
| Function | Scroll | Cursor |
| Sensor | PMW3610 @ 200 CPI | PMW3610 @ 1200 CPI |
| Every layer | scrolls, scaled 1/16 | moves the pointer, scaled 2/3 → ~800 CPI |
| On **Nav** | **moves the pointer**, scaled 4× → ~800 CPI | **scrolls**, scaled 1/96 |
| On **Mouse** | unchanged | **snipe**: a further 1/4 → ~200 CPI for pixel work |

The two balls swapping roles on Nav is carried over from the UHK, whose left key
cluster and right trackball are configured `navigationModeModLayer = Cursor` and
`= Scroll` respectively. Both scaling factors put the two balls at the same
effective speed as each other — ~800 CPI for the pointer, one scroll tick per
2 mm of travel.

MOUSE is a **toggle**, not a hold: hold Extras (right inner thumb) and tap the
left-click thumb to lock it on, then tap that same inner thumb to leave. Both
inner thumbs are Nav, which is what displaced it — and a toggle suits snipe
mode better anyway, since it leaves every thumb free to click while you are
lining up a pixel.

There is **no auto-mouse layer**. A `zip_temp_layer` used to switch to MOUSE on
any ball motion, but it fires on any input event at all — including the
single-count jitter a sensitive sensor picks up from desk vibration — so the
layer changed at random. It was removed rather than desensitising the sensor;
jitter is now harmless sub-pixel cursor noise.

Neither ball does anything by itself: the halves only publish raw sensor data
over `zmk,input-split`, and every processor above runs on the **dongle**. That
is why a change to a ball's behaviour means reflashing the dongle, and a change
to the sensor itself means reflashing the halves.

## Building

Firmware is built by GitHub Actions on every push; download the artifact from
the **Actions** tab. To build locally you need Docker and nothing else:

```bash
git clone https://github.com/rogermeletta/arcduo-zmk && cd arcduo-zmk
mkdir -p /tmp/arcduo-ws && cp -R config /tmp/arcduo-ws/config
docker run --rm -v /tmp/arcduo-ws:/ws -v "$PWD":/repo -w /ws zmkfirmware/zmk-build-arm:stable bash -c "git config --global --add safe.directory '*' && west init -l /ws/config && west update && west zephyr-export && west build -s zmk/app -d /ws/build -b 'nice_nano/nrf52840/zmk' -- -DZMK_CONFIG=/ws/config -DSHIELD=arcduo_left -DZMK_EXTRA_MODULES=/repo"
```

The resulting `.uf2` lands in `/tmp/arcduo-ws/build/zephyr/zmk.uf2`.

### Board targets

ZMK moved to Zephyr 4.1 hardware-model-v2 board targets in December 2025, so
the old names no longer work:

| Old | Current |
| --- | --- |
| `nice_nano_v2` | `nice_nano/nrf52840/zmk` |
| `seeeduino_xiao_ble` | `xiao_ble/nrf52840/zmk` |

### Pinned dependencies

`config/west.yml` pins ZMK and both modules to **exact commits** rather than
tracking `main`. Tracking `main` is what silently broke this repo for eight
months when ZMK moved to Zephyr 4.1 and the PMW3610 driver renamed its
devicetree compatible. Bump one revision at a time and rebuild.

Note there is no tagged ZMK release with Zephyr 4.1 yet — v0.3.0 predates it
and v0.4.0 is still pending — so ZMK is pinned to a `main` commit.

## Flashing

1. Double-tap the reset button on the board to mount it as a USB drive.
2. Copy the matching `.uf2` onto it. It reboots automatically.

**Keymap changes need the dongle only.** The dongle is the split central: it
runs the keymap, the combos, the behaviors and both trackball input-listener
chains. The halves only scan their matrix and publish raw sensor data. Editing
`config/*.keymap` or `*.dtsi`, or a per-layer trackball override, produces
byte-identical half firmware — verified by building both halves either side of
such a change and comparing the `.uf2`.

Flash the halves when something they actually compile changes: the shield
overlays under `boards/shields/arcduo/`, `config/arcduo_{left,right}.conf`, or
a `config/west.yml` bump that moves the PMW3610 driver or ZMK itself.

If the halves and dongle will not pair, flash `firmware_reset_*.uf2` to each
first to clear stored bonds, then reflash — pairing the **left** half before
the right, since the dongle screen orders the battery widgets by pairing order.

## Layers

| # | Name | Reached by | |
| --- | --- | --- | --- |
| 0 | Base | default | |
| 1 | Num | hold left outer (Tab) or right outer (Bspc) thumb | the UHK's Fn layer |
| 2 | Nav | hold left inner thumb, or the right Space thumb | the UHK's Mod layer |
| 3 | Symbols | hold `G` (type right) or `H` (type left) | mirrored — same symbol, mirrored finger |
| 4 | Extra | hold right inner thumb | F13–F21 pad, radios, screen, transport |
| 5 | Mouse | toggle: hold Extra, tap the left-click thumb | the UHK's Mouse layer |

Both layers carried over from the UHK are held with the **left** thumb, matching
how they are held there — which leaves the right hand free for the arrows on Nav
and keeps tab and window management under the left fingers.

Nav is on the right Space thumb as well, which is the one place this deliberately
departs from the UHK. Holding it there puts the layer and the arrows on the same
hand, so the right hand alone can navigate with the left off the board entirely.
It has the Space thumb because Nav is reached constantly and Extra almost never.

Extra's left hand is `F13`–`F21` — nine keycodes macOS defines and binds to
nothing, which is what Raycast, Karabiner, BetterTouchTool and Hammerspoon
want as triggers. They replaced five text macros. `&bootloader` and
`&sys_reset` sit on Extra's two left thumbs; before that neither was bound
anywhere, so the bootloader meant finding the physical reset button. Note it
only reaches the dongle — the halves do not run the keymap.

Symbols is mirrored: the same symbol sits on the mirrored finger of both halves,
so there is one set to learn and it is always typeable by whichever hand is not
on a trackball. It carries only what has no home elsewhere — the six brackets
are Base combos, `-` `+` `=` are on Num, `;` is on Base — which is what makes
room for `\ | ' "`, none of which could be typed on this board at all before.

The layer bodies are shaped after Roger's Ultimate Hacking Keyboard v1 "QWERTY
for Mac" keymap, so that switching between the two boards costs as little as
possible. [docs/UHK-PARITY.md](docs/UHK-PARITY.md) has the full mapping table,
including what did not fit and why.

![keymap images](keymap-drawer/arcduo.svg)

The keymap image is regenerated automatically by
[keymap-drawer](https://github.com/caksoylar/keymap-drawer) whenever
`config/arcduo.keymap` changes.

The two round shapes are the trackballs. keymap-drawer only knows about keys,
so they are faked in three places that have to stay in step: two extra
positions in `config/arcduo.json`, two bindings per layer guarded by
`#ifdef KEYMAP_DRAWER` in `config/arcduo.keymap` (a define that only the
drawer's preprocessor ever sees — the firmware build drops the block), and the
`&ball_*` legends plus the round styling in `keymap-drawer/config.yaml`. Their
real behaviour lives in the input listeners in
`boards/shields/arcduo/arcduo.dtsi`; if you change those, update the legends.

## Credits & Inspiration

This project is inspired by and builds upon the following:

- [levels-zmk firmware](https://github.com/Good-Great-Grand-Wonderful/levels-zmk)
- [charybdis-wireless-mini-zmk-firmware](https://github.com/280Zo/charybdis-wireless-mini-zmk-firmware)
- [Skeletyl Low Trackball Mod 3D Model](https://makerworld.com/en/models/802589-skeletyl-low-trackball-mod-for-single-switch-pcbs)
- [zmk-pmw3610-driver](https://github.com/badjeff/zmk-pmw3610-driver) by badjeff
- [YADS — Yet Another Dongle Screen](https://github.com/janpfischer/zmk-dongle-screen) by janpfischer

Special thanks to the authors and contributors of these projects and resources
for their valuable work and inspiration.

## License

MIT — see [LICENSE](LICENSE).
