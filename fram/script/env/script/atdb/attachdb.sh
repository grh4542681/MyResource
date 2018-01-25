#!/bin/bash

CurDir=$SHELLHOME/script/atdb


expect -c "
	spawn sudo -k $CurDir/copydbfile.sh
	set timeout 3
	expect {
		\"\\\\\[sudo\\\\\] \" {send \"$ROOTPWD\r\"}
		timeout { }
		eof { exit 9 }
	}
	set timeout -1
	expect {
		\"*$\" { }
	}
"
SqlCmd=/opt/mssql-tools/bin/sqlcmd 
DataBaseFile=$CurDir/database.ini

cat $DataBaseFile|while read line
do
	
	DbName=`echo $line|awk -F ' ' '{print $1}'`
	DbFile=`echo $line|awk -F ' ' '{print $2}'`
	
	echo "USE master" > $CurDir/$DbName.sql
	echo "go" >> $CurDir/$DbName.sql
	echo "" >> $CurDir/$DbName.sql
	echo "exec sp_attach_db $DbName,'C:\\var\\opt\\mssql\\data\\$DbFile'" >> $CurDir/$DbName.sql
	echo "go" >> $CurDir/$DbName.sql

	$SqlCmd -S localhost -U sa -P Auto@sql -i $CurDir/$DbName.sql
	
done

	
