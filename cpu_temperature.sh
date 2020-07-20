let "temp0 = $(cat /sys/devices/platform/coretemp.0/hwmon/hwmon1/temp1_input)"
let "temp1 = $(cat /sys/devices/platform/coretemp.0/hwmon/hwmon1/temp2_input)"
let "temp = ($temp0 + $temp1) / 2000"
printf "$temp°C"
