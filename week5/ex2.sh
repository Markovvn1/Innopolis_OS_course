#!/bin/bash

OUTPUT_FILE="ex2.out.txt"

if [ ! -f "$OUTPUT_FILE" ]; then
	echo 1 > "$OUTPUT_FILE"
fi

# to simulate slow computer (I want that output file will not be huge)
random_sleep()
{
	sleep $(echo "$RANDOM/327680" | bc -l)
}

mutex_lock()
{
	if [ $# -ne 1 ]; then
		return 1
	fi

	while ! ln "$1" "$1.lock" 2> /dev/null
	do
		random_sleep
	done
}

mutex_unlock()
{
	if [ $# -ne 1 ]; then
		return 1
	fi

	rm -f "$1.lock"
}

while :
do
	random_sleep
	mutex_lock "$OUTPUT_FILE"
		random_sleep
		out=$(tail -n1 "$OUTPUT_FILE")
		random_sleep
		expr "$out" + 1 >> "$OUTPUT_FILE"
		# echo $1 >> "$OUTPUT_FILE"  # to check that this system works
		random_sleep
	mutex_unlock "$OUTPUT_FILE"
	random_sleep
done