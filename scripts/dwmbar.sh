#!/bin/sh

echo $$ > /tmp/dwmbarpid.tmp # pid in file for other scripts

while :
do
  TIME=$(cat /tmp/curTime.tmp)
  VOLUME=$(pamixer --get-volume-human)
  BATTERY=$(cat /sys/class/power_supply/BAT?/capacity)%
  MEMORY=$(free -h | grep 'Mem' | awk '{ print $3 }')
  CPU=$(sensors acpitz-acpi-0 -u | grep temp1_input | awk '{ print $2 }' | sed 's/\..*//g')C

  xsetroot -name " vol:$VOLUME | bat:$BATTERY | mem:$MEMORY | temp:$CPU | $TIME"
  sleep 15s
done
