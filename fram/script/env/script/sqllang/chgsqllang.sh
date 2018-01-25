#!/bin/bash

. $SHELLHOME/common/common.sh

SqlCmd=/opt/mssql-tools/bin/sqlcmd
SqlScript=$SHELLHOME/script/sqllang/chglang.sql

Locale=`echo $LANG|awk -F '.' '{print $1}'`

Line=`grep ${Locale} $SHELLHOME/script/sqllang/language.ini`

if [ $? -ne 0 ] 
then
    _Error "unknow locale [$Locale]"
    exit 9
fi

LangID=`echo $Line|awk -F ' ' '{print $2}'`
echo $LangID

echo "sp_configure 'default language',$LangID;" > $SqlScript
echo "go" >> $SqlScript
echo "RECONFIGURE" >> $SqlScript
echo "go" >> $SqlScript

$SqlCmd -S localhost -U sa -P Auto@sql -i $SqlScript
if [ $? -ne 0 ]
then
	_Error "Execute Sql script error"
	exit 9
fi

expect -c "
	spawn sudo systemctl restart mssql-server
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
