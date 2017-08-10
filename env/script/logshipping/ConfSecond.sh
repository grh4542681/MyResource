#!/bin/bash

if [ `id -u` -ne 0 ] 
then
    echo "Permission denied"
    exit 9
fi

MasterNode=$1
ShareDir="/var/opt/mssql/tlogs"

mount -l|grep "$MasterNode/tlogs"|grep "$ShareDir"
if [ $? -eq 0 ]
then
	exit 0
else
	if [ -d $ShareDir ]
	then
		rm -d $ShareDir
	fi
	mkdir $ShareDir

	chown root:root $ShareDir /var/opt/mssql/tlogcreds
	chmod 0550 $ShareDir
	chmod 0660 /var/opt/mssql/tlogcreds

	sed -i -e "/$MasterNode\/tlogs/d" /etc/fstab
	echo "//$MasterNode/tlogs /var/opt/mssql/tlogs cifs credentials=/var/opt/mssql/tlogcreds,ro,uid=mssql,gid=mssql 0 0" >> /etc/fstab

	mount -a
fi
	

