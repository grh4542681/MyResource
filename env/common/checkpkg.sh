#!/bin/bash

if [ $# -ne 2 ]
then
	echo "Usage: $0 [Ubuntu|Red Hat|...] [PackageName]"
	exit 9
fi

if [[ $1 == "Ubuntu" ]]
then
	Debinfo=`dpkg-query -l $2|grep "$2"|grep -v grep`
	if [ $? -ne 0 ]
	then
		exit 1
	else
        _Status=`echo $Debinfo|awk '{print $1}'`
        if [[ $_Status == "ii" ]]
        then
            echo "Deb $2 has been installed"
            echo "[$Debinfo]"
            exit 0
        else
            exit 1
        fi
	fi
elif [[ $1 == "Red Hat" ]]
then
	echo "Red Hat Not support!"
	exit 9
else
	echo "Unknow Linux Issue [$1]"
	exit 9
fi
