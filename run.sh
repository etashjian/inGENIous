#! /bin/bash

for packets in 1000
do
	echo "packets "$packets
	for queue in 1 2 3
	do
		echo "  queue size "$queue
		for window in 1 2 3
		do
			echo "    window size "$window
			./bin/Client -n $packets -q $queue -w $window 1> analysis/output/packets_$packets.queue_$queue.window_$window.output 2> analysis/data/packets_100.queue_$queue.window_$window.data
		done
	done
done
