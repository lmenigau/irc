#!/bin/bash

for i in {1..2000}
do
	echo "a"
	nc ::1 9999 &
done
