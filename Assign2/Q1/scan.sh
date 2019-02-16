
#!/bin/bash

####################
# First Name:  Juan Luis
# Last Name:   de Reiset
# Student ID:  30050167
# Course:      CPSC 457
# Tutorial:    T02
# Assignment:  2
# Question:    Q1
# File name: scan.sh
#############################


if [ $2 -lt 0 ]
then
echo Hey i cant print less than 0 files!.

else

find . -name "*$1" -printf "%p %s\n" | sort -k2nr  | head -$2 | awk '{print $1 " " $2; total += $2} END { print "Total size: " total }'


fi
