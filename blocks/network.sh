#/bin/bash
declare lastTotal=(0 0)
while true; do
    declare wltotal=($(grep "wlp" /proc/net/dev | awk '{printf "%d %d", $2/1024, $10/1024}'))
    declare ethtotal=($(grep "enp" /proc/net/dev | awk '{printf "%d %d", $2/1024, $10/1024}'))
    declare total=($((${wltotal[0]}+${ethtotal[0]})) $((${wltotal[1]}+${ethtotal[1]})))
    printf "^c#3EC13F^▾^d^ %4dkB | ^c#FE522C^▴^d^ %4dkB" $((${total[0]}-${lastTotal[0]})) $((${total[1]}-${lastTotal[1]})) > /tmp/network
    lastTotal=(${total[@]})
    sleep 1
done
