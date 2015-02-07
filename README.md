# Xbox One Controller driver for Mac OS X

## Installation

Until I obtain an Apple Developer license and certificate, the driver is unsigned and has to be installed with kext dev mode enabled on Yosemite.

Obtain the kext either from the [releases page](https://github.com/lloeki/xbox_one_controller/releases) or by building it in Xcode. Place it in `/Library/Extensions/` and make sure permissions and ownership are correct.

Easy way, in a terminal:

```
sudo nvram boot-args=kext-dev-mode=1                                # disable driver signature checks
sudo kextunload /Library/Extensions/xbox_one_controller.kext        # just to be sure
sudo rm -rf /Library/Extensions/xbox_one_controller.kext            # just to be sure, too
sudo cp -R ~/Downloads/or/whatever/xbox_one_controller.kext /tmp    # this should set the ownership and permissions straight
sudo cp -R /tmp/xbox_one_controller.kext /Library/Extensions        # third-party kexts are autoloaded from there on Yosemite
```

An installer automating those steps will be built soon.

## Usage

Gamepad pops up as a USB HID device, so it's readily usable by most games.

- Steam: games supporting controllers will work fine once you set up the controller via Big Picture's controller settings. I've uploaded the settings to Steam's servers so it may be shared with anyone and you may not even need to set it up at some point.
- OpenEmu: until mappings have been added, simply map the buttons yourself in the settings page.

## Buttons are all mixed up!

This driver makes no attempt to emulate another gamepad and stays as true as possible to the hardware. Solutions abound to map joystick buttons, and game developers are expected to add support for this controller, as they already are for PS3, PS4, RumblePad... which already differ in significant ways. The only exception is that the driver does reverse vertical axes to match pre-cited common gamepads. Axes roles being arbitrary, they are made to match oft-used gamepads such as PS4. A possible future addition may be to add synthetic buttons on triggers in addition to trigger axes (similarly to what the PS4 controller does).

## Where is button 1?

That's the pairing button.

## Where is button 2?

There's none, but it may be used by an accessory or something.

## The controller keeps blinking/wakes my Xbox One when unplugged!

This is by (Microsoft's) design. When the controller is unplugged, it starts looking for a wireless connection. This may wake up a nearby Xbox One console if it has been associated already, or if Kinect's IR sensor is in the controller's line of sight (due to Kinect's smart pairing system). Just keep press the Xbox button pressed until it fades away and the controller shuts down, or simply run without batteries if it's a controller dedicated to your computer.

## Bugs

Please report all bugs via [GitHub issues](https://github.com/lloeki/xbox_one_controller/issues).

## License

3-clause BSD. See attached LICENSE file.
