free --mebi | awk '(NR==2){ printf("RAM: %.2f GB / %.2f GB", $3/1024, $2/1024) }'
