#!/bin/bash

set -e

out="xbox_one_controller"
tmp_dir="$(mktemp -d -t xbox_one_controller)"
echo "building in '$tmp_dir'..."

mkdir "$tmp_dir/xbox_one_controller"
sudo cp -R /Users/lloeki/Library/Developer/Xcode/DerivedData/xbox_one_controller-*/Build/Products/Debug/xbox_one_controller.kext "$tmp_dir/xbox_one_controller"
sudo cp install.sh "$tmp_dir/xbox_one_controller/Install.command"

cd "$tmp_dir"
zip -r "$out.zip" xbox_one_controller
cd -
mv "$tmp_dir/$out.zip" .
