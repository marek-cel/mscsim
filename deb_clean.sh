#!/bin/bash

################################################################################

APP_NAME=mscsim

################################################################################

APP_DIR=${APP_NAME}_*_focal_amd64

sudo rm -R $APP_DIR
sudo rm $APP_DIR.deb

################################################################################

DATA_DIR=${APP_NAME}-data_*_focal_all

sudo rm -R $DATA_DIR
sudo rm $DATA_DIR.deb

################################################################################

DATA_TERRAIN_DIR=${APP_NAME}-data-terrain_*_focal_all

sudo rm -R $DATA_TERRAIN_DIR
sudo rm $DATA_TERRAIN_DIR.deb

################################################################################

read -p "Press any key to continue..." -n1 -s
echo
