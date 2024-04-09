#!/bin/bash 
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:`pwd`/lib
for((i=1;i<=100;i=i+1))
do
	./client -i 127.0.0.1 -p 30321 -t 3 &
	sleep 0.05
done
