# Xbox One Controller driver for Mac OS X

## Installation

Obtain the driver from the [releases page](https://github.com/lloeki/xbox_one_controller/releases). Unzip, then run `Install.command` (right click, then choose 'Open') and follow instructions.

Note: while I obtained an Apple Developer license and certificate, the driver is unsigned and has to be installed with kext dev mode enabled on Yosemite beacause Apple refuses to grant kext signing ability to random FOSS developers (see [issue #2](https://github.com/lloeki/xbox_one_controller/issues/2)). The install script will inform you of this fact and will assist in setting the boot arguments.

## Usage

Gamepad pops up as a USB HID device, so it's readily usable by most games.

- Steam: games supporting controllers will work fine once you set up the controller via Big Picture's controller settings. I've uploaded the settings to Steam's servers so it may be shared with anyone and you may not even need to set it up at some point.
- OpenEmu: until mappings have been added, simply map the buttons yourself in the settings page.

## FAQ

### Does this driver support multiple controllers at the same time?

Yes.

### How are buttons and axes mapped?

Buttons are mapped in the order the hardware reports them:

- Button 1: "Pair" (wireless)
- Button 3: "Menu" (start)
- Button 4: "Window" (select)
- Button 5: A
- Button 6: B
- Button 7: X
- Button 8: Y
- Button 9: D-pad up
- Button 10: D-pad down
- Button 11: D-pad left
- Button 12: D-pad right
- Button 13: LB (shoulder left)
- Button 14: RB (shoulder right)
- Button 15: Left Stick click
- Button 16: Right Stick click
- Button 17: Xbox button

Axes are mapped the same as for DualShock 4:

- Axis 1: LS (left-right)
- Axis 2: LS (up-down)
- Axis 3: RS (left-right)
- Axis 6: RS (up-down)
- Axis 4: Left Trigger
- Axis 5: Right Trigger

You can easily view and test all of them using software such as [Joystick Show](https://itunes.apple.com/fr/app/joystick-show/id515886877).

### Where is button 1 located?

That's the pairing button, on top of the controller, next to the USB port.

### Where is button 2 located?

There's none, but it may be used by an accessory or something.

### Buttons are all mixed up in my favorite game!

This driver makes no attempt to emulate another gamepad and stays as true as possible to the hardware, therefore buttons are reported in hardware order. Be sure to set up the mappings correctly, or beg your favorite game developer to add button mappings for this controller.

### I'm a game developer and want to add support for this controller

If you already have HID gamepad support, chances are it already works! With this driver, it's just a HID Class gamepad, just as any other game controller such as Logitech RumblePad 2, DualShock 3 or DualShock 4. If you want to match the hardware to add a button mapping, look for Vendor ID:Product ID 0x02d1:0x045e and/or Vendor Name/Product Name "Microsoft"/"Xbox One Controller".

### The controller keeps blinking/wakes my Xbox One when unplugged

This is by (Microsoft's) design. When the controller is unplugged, it starts looking for a wireless connection. This may wake up a nearby Xbox One console if it has been associated already, or if Kinect's IR sensor is in the controller's line of sight (due to Kinect's smart pairing system). Just keep press the Xbox button pressed until it fades away and the controller shuts down, or simply run without batteries if it's a controller dedicated to your computer.

### It's not working wirelessly!

It can't, as Xbox 360 and One controllers all use non-standard radio hardware.

### How do I uninstall this?

- Make sure that no controller is plugged
- `sudo kextunload /Library/Extensions/xbox_one_controller.kext` (optional, as this gets unloaded automatically after some time)
- `sudo rm -rf /Library/Extensions/xbox_one_controller.kext`
- `sudo nvram boot-args=""` (but first make sure there are no other flags with `nvram boot-args`)
- reboot

## Bugs

Please report all bugs via [GitHub issues](https://github.com/lloeki/xbox_one_controller/issues).

## License

3-clause BSD. See attached LICENSE file.
