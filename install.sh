#!/bin/bash

set -e
set -o nounset

function finish () {
    if [[ $? -eq 0 ]]; then
        echo "Installation complete! Reboot to finish."
    else
        echo "Installation failed!"
    fi
    read -p "-- Press a key --" -n 1 -r
}
trap finish EXIT

# check source
source_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && echo "$PWD")"
kext_name='xbox_one_controller.kext'
kext_to_install="$source_dir/$kext_name"
if [[ -d "$kext_to_install" ]]; then
    echo "I will install '$kext_to_install' into /Library/Extensions"
else
    echo "Error: kext not found '$kext_to_install'"
    exit 1
fi

# check dev mode
if nvram -p | grep 'boot-args' | grep -q 'kext-dev-mode=1'; then
    echo "kext developer mode detected"
else
    echo "Warning: kext developer mode NOT detected"
    read -p "Add it to kernel boot flags? [y/N] " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        echo "OK, exiting..."
        exit 1
    else
        sudo nvram boot-args=kext-dev-mode=1
    fi
fi

# unload driver
if kextstat | grep -q com.lloeki.xbox-one-controller; then
    if sudo kextunload /Library/Extensions/xbox_one_controller.kext; then
        echo "Previous kext unloaded"
    else
        echo "Could not unload driver, please unplug any controller and rerun"
        exit 1
    fi
fi

# uninstall
echo "Removing previous version..."
sync
sudo rm -rf /Library/Extensions/xbox_one_controller.kext

# install
echo "Installing new version..."
sync
sudo cp -R "$kext_to_install" /tmp
sudo mv /tmp/xbox_one_controller.kext /Library/Extensions
