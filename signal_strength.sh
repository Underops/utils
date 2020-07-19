[ $(cat /sys/class/net/wlp4s0/operstate) = up ] &&
    iwlist wlp4s0 scan | grep dBm | cut -c 49-55 | tr '\n' '\0' ||
    printf "down"
