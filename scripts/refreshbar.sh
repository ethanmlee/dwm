#!/bin/sh
cd "${0%"/"*}" # sets relative path

kill -44 $(cat /tmp/dwmbarpid.tmp) # kills dwmbar.sh
nohup ~/.scripts/dwmbar.sh > /dev/null & exit # starts dwmbar.sh
