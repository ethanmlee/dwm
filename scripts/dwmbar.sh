#!/bin/sh
echo $$ > /tmp/dwmbarpid.tmp

while :
do
  TIME=$(cat /tmp/curTime.tmp)
  VOLUME=$(pamixer --get-volume-human)
  UPOWER=$(upower -i /org/freedesktop/UPower/devices/battery_BAT0 | grep -E "state" | awk '{ print $NF }')
  CAPACITY=$(cat /sys/class/power_supply/BAT?/capacity)%; CHARGE=↓; [ "$UPOWER" = "charging" ] && STATE=↑
  BATTERY=$CAPACITY$CHARGE; [ "$UPOWER" = "fully-charged" ] && BATTERY=100%
  MEMORY=$(free -h | grep 'Mem' | awk '{ print $3 }')
  CPU=$(sensors acpitz-acpi-0 -u | grep temp1_input | awk '{ print $2 }' | sed 's/\..*//g')C

  xsetroot -name " vol:$VOLUME | bat:$BATTERY | mem:$MEMORY | temp:$CPU | $TIME"

  sleep 15s
done
