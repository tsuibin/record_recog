#!/bin/bash 

for i in `xlsclients -l | grep "^ *Name" | awk '{print $2}'`; do
	echo -n "$i "
	xprop -name "$i" | grep PID | awk '{print $3}'
done
