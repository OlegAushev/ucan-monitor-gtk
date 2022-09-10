#!/bin/bash
#
if [ -z $1 ] || [ -z $2 ]
then
	echo "Invalid arguments."
	exit 1
fi
if ip addr ls dev $1 > /dev/null 2>&1
then
	echo "Device $1 found."
	if ip link set $1 down
	then
		ip link set $1 type can bitrate $2 restart-ms 100
		ip link set $1 up
		echo "SocketCAN interface $1 (bitrate = $2) enabled."
		exit 0
	else
		echo "Configuring and enabling SocketCAN interface $1 failed."
		exit 3
	fi
else
	echo "Device $1 not found."
	exit 2
fi
