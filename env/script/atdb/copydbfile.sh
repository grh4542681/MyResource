#!/bin/bash
SHELLHOME=/home/helsinki/env
DataBaseFile=$SHELLHOME/script/atdb/dbfile.ini

DbFilePath=/var/opt/mssql/data

cat $DataBaseFile|while read line
do
	
	DbFile=$line
	RDbFile=$DbFilePath/$DbFile
	
	if [ ! -f $RDbFile ]
	then
		if [ ! -f $SHELLHOME/data/$DbFile ]
		then
			echo "con't find db file [$DbFile]"
			exit 9
		fi
		cp $SHELLHOME/data/$DbFile $RDbFile
	fi

	chown mssql:mssql $RDbFile
	
done

