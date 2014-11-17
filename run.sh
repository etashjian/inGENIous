#! /bin/bash

for queue in {1..10}
do
	for window in {1..10}
	do
		./bin/Client -n 100 -q $queue -w $window 2> packets_100.queue_$queue.window_$window.data
	done
done
