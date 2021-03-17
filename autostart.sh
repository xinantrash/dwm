#!/bin/bash

xrandr --auto --output HDMI1 --same-as eDP1 --size 1920x1080
/bin/bash ~/dwm_sh/scripts/dwm-status.sh &
/bin/bash ~/dwm_sh/scripts/wp-autochange.sh &
compton &
nm-applet &
/bin/bash ~/dwm_sh/scripts/setxmodmap-qwerty.sh &
xfce4-power-manager &
/usr/lib/gsd-xsettings &
/bin/bash ~/dwm_sh/scripts/autostart_wait.sh &
