#!/bin/bash

rsync -avu --delete --exclude '*.dropbox*' ./src/ ~/dev/git/mscsim/trunk/src/

notify-send "Syncing finished."
read -p "Press any key to continue... " -n1 -s
