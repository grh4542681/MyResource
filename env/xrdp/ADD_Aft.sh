#!/bin/bash

cd /etc/xrdp
echo "$ROOTPWD"|sudo -S cp xrdp.ini xrdp.ini.orgi
if [ $? -ne 0 ]
then
	exit 9
fi

