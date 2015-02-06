# Xbox One Controller driver for Mac OS X

## Installation

Until I obtain an Apple Developer license and certificate, the driver is unsigned and has to be installed with kext dev mode enabled on Yosemite. If you don't know what that means it's probably not a good idea to go further.

Obtain the kext either from the [releases page](https://github.com/lloeki/xbox_one_controller/releases) or by building it in Xcode. Copy it into /Library/Extensions. Ensure it's chown'd root:wheel and chmod'd correctly.

## Usage

Gamepad pops up as a USB HID device, so it's readily usable by most games.

## Buttons are all mixed up!

This driver makes no attempt to emulate another gamepad and stays as true as possible to the hardware. Solutions abound to map joystick buttons, and game developers are expected to add support for this controller, as they already are for PS3, PS4, RumblePad... which already differ in significant ways. The only exception is that the driver does reverse vertical axes to match pre-cited common gamepads. Axes roles being arbitrary, they are made to match PS4. A possible future addition may be to add synthetic buttons on triggers in addition to trigger axes (similarly to what the PS4 controller does).

## Bugs

This driver is still rough on the edges.

Please report all bugs via [GitHub issues](https://github.com/lloeki/xbox_one_controller/issues).

## License

3-clause BSD. See attached LICENSE file.
