let "temps = $(wc -l <<< $(ls /sys/devices/platform/coretemp.0/hwmon/hwmon?/temp*_input))"
let "total = 0"
let "i = 1"
while [ $i -lt $temps ]; do
    let "temp = $(cat /sys/devices/platform/coretemp.0/hwmon/hwmon?/temp$i\_input)"
    let "total = $total + $temp"
    let "i = i + 1"
done
let "temp = total / ($i-1) / 1000"
printf "$temp°C"
