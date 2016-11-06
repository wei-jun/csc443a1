#!/bin/bash
echo "Experiment for writing starts now!"
for i in $(seq 100 300000 3145728)
do
	create_random_file $i 10000000 $i | tail -2
	echo "------------------------------------"
done
echo "Experiment done!"
