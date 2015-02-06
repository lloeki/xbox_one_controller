#!/bin/bash

sync
sudo kextunload /tmp/xbox_one_controller.kext;
sleep 1;
sudo cp -R /Users/lloeki/Library/Developer/Xcode/DerivedData/xbox_one_controller-*/Build/Products/Debug/xbox_one_controller.kext /tmp/ &&
sudo kextload /tmp/xbox_one_controller.kext
