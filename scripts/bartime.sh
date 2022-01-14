#!/bin/sh

#cd "${0%"/"*}" #sets relative path

echo $(date '+%a %d.%m.%+4Y %I:%M %p %Z') > /tmp/curTime.tmp
nohup ~/.scripts/dwmbar.sh > /dev/null & # start dwmbar.sh script
while :
do
  sleep $((60 - $(date +%S) )) && # sleep untill next full min
  echo $(date '+%a %d.%m.%+4Y %I:%M %p %Z') > /tmp/curTime.tmp # save current time format to /tmp/curTime.tmp
  ./refreshbar.sh # refresh dwmbar after updating time in file
done
