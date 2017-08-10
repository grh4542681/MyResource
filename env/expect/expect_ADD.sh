#!/bin/bash

if [ $# -gt 3 ]
then 
	echo "Usage $0 [Ubuntu|Red Hat|...] [Net|Local] [ |PKGPATH]"
fi

if [[ $1 == "Ubuntu" ]]
then
	if [[ $2 == "Net" ]]
	then
		echo "$ROOTPWD"|sudo -S apt-get install -y expect 
	elif [[ $2 == "Local" ]]
	then
		echo "$ROOTPWD"|sudo -S dpkg -i $3
	else
		echo "Unknow Channel[$2] for install"
	fi
elif [[ $1 == "Red Hat" ]]
then 
	echo "Not support yet!"
	exit 9
else
	exit 9
fi

