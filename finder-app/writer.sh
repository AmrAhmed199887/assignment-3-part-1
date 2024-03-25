#!/bin/bash
writefile=$1
writestr=$2
if [ $# -eq 2 ]
then
	if [ -d `dirname $writefile` ]
	then
		touch $writefile
        	if [ -e $writefile ]
        	then
        	echo "$writestr" > $writefile
        	else
        	echo "can\'t create the file"
        	(exit 1)
        	fi
	else
		mkdir -p `dirname $writefile`
        	touch $writefile
        	if [ -e $writefile ]
        	then
        	echo "$writestr" > $writefile
        	else
        	echo "can\'t create the file"
        	(exit 1)
        	fi
	fi
else
echo "too few arguments"
(exit 1)
fi
