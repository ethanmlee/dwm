#!/bin/sh

echo $$ > /tmp/dwmbarpid.tmp # pid in file for other scripts

while :
do
  TIME=$(cat /tmp/curTime.tmp)
  VOLUME=$(pamixer --get-volume-human)
  UPOWER=$(upower -i /org/freedesktop/UPower/devices/battery_BAT0 | grep -E "state" | awk '{ print $NF }')
  STATE=↓; [ "$UPOWER" = "charging" ] && STATE=↑
  BATTERY=$(cat /sys/class/power_supply/BAT?/capacity)%
  MEMORY=$(free -h | grep 'Mem' | awk '{ print $3 }')
  CPU=$(sensors acpitz-acpi-0 -u | grep temp1_input | awk '{ print $2 }' | sed 's/\..*//g')C

  xsetroot -name " vol:$VOLUME | bat:$BATTERY$STATE | mem:$MEMORY | temp:$CPU | $TIME"
  sleep 15s
done
