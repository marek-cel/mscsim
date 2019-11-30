#!/bin/bash

################################################################################

APP_NAME=mscsim
APP_VER=0.2

################################################################################

APP_DIR=${APP_NAME}_${APP_VER}_bionic_amd64

sudo rm -R $APP_DIR
sudo rm $APP_DIR.deb

mkdir $APP_DIR
mkdir $APP_DIR/DEBIAN
mkdir $APP_DIR/usr
mkdir $APP_DIR/usr/bin

cp deb_mscsim_amd64.control $APP_DIR/DEBIAN/control
cp bin/mscsim $APP_DIR/usr/bin/

sudo chown root:root -R $APP_DIR
sudo chmod 0755 $APP_DIR/usr/bin/mscsim

dpkg -b $APP_DIR/

################################################################################

read -p "Press any key to continue..." -n1 -s
