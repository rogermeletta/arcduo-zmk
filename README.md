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
| On **Num** | horizontal scroll is inverted | unchanged |
| On **Mouse** | unchanged | **snipe**: a further 1/4 → ~200 CPI for pixel work |

Snipe mode is why the mouse buttons on the MOUSE layer sit under the *left*
hand: hold the right inner thumb to slow the right ball down, and you can
still click while sniping.

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

Flash all three parts after any keymap change. If the halves and dongle will
not pair, flash `firmware_reset_*.uf2` to each first to clear stored bonds,
then reflash — pairing the **left** half before the right, since the dongle
screen orders the battery widgets by pairing order.

## Layers

| # | Name | Reached by |
| --- | --- | --- |
| 0 | Base | default |
| 1 | Num | hold Tab or Backspace thumb |
| 2 | Nav | hold Space thumb |
| 3 | Symbols | hold `G` |
| 4 | Extra | hold left inner thumb, or the both-inner-thumbs combo |
| 5 | Mouse | hold right inner thumb |

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
