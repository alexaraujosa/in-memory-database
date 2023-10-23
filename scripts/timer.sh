#!/bin/bash
# For debug purposes: printf "'%s'\n" "$@"
timeStart=$(date +%s.%N)
/bin/bash "$@"
timeEnd=$(date +%s.%N)
dt=$(echo "$timeEnd - $timeStart" | bc)
dd=$(echo "$dt/86400" | bc)
dt2=$(echo "$dt-86400*$dd" | bc)
dh=$(echo "$dt2/3600" | bc)
dt3=$(echo "$dt2-3600*$dh" | bc)
dm=$(echo "$dt3/60" | bc)
ds=$(echo "$dt3-60*$dm" | bc)

LC_NUMERIC=C printf "Total runtime: %d:%02d:%02d:%02.4f\n" $dd $dh $dm $ds