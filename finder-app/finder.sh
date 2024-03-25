#!/bin/bash

filesdir=$1

searchstr=$2


if [ $# -eq 2 ]
then 

	if [ -d $filesdir ]
	then

	echo "The number of files are `ls $filesdir | wc -l` and the number of matching lines are `for f in ${filesdir}/* ; do cat "${f}";echo ;done |grep "$searchstr" |wc -l`"

	else
	echo 'no such directory'
	(exit 1)
	fi

else

echo ' too few arguments '
(exit 1)

fi
