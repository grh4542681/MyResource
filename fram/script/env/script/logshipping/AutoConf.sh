#!/bin/bash

. $SHELLHOME/common/common.sh

Host=`hostname`

MapHost=`awk '/^'$Host'/ {print $2}' $SHELLHOME/script/logshipping/MachineMap.ini` 
if [ -z $MapHost ]
then
	_Warn "Can't find Maphost by master host[$Host] in MachineMap.ini"
	_Warn "Maybe [$Host] is not master"
	exit 0
fi

_Info "$Host ----------- $MapHost"

echo "$ROOTPWD"|sudo -S $SHELLHOME/script/logshipping/ConfShareDir.sh $Host $MapHost

