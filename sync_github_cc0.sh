#!/bin/bash

rsync -avu --delete --exclude '*.dropbox*' ./src_cc0/ ~/dev/git/mscsim-cc0/trunk/src/

notify-send "Syncing finished."
read -p "Press any key to continue... " -n1 -s
