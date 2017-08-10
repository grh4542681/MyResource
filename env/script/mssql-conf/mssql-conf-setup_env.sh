#!/bin/bash

MssqlConf_setup="/opt/mssql/bin/mssql-conf setup"

Locale=`echo $LANG|awk -F '.' '{print $1}'`

Line=`grep ${Locale} /home/helsinki/env/script/mssql-conf/language.ini`

if [ $? -ne 0 ] 
then
    _Error "unknow locale [$Locale]"
    exit 9
fi

LangID=`echo $Line|awk -F ' ' '{print $2}'`
echo $LangID

#export ACCEPT_EULA=Y
#export MSSQL_SA_PASSWORD=Auto@sql
#export MSSQL_LCID=$LangID
#export MSSQL_PID=Developer

sudo ACCEPT_EULA=Y MSSQL_SA_PASSWORD=Auto@sql MSSQL_LCID=$LangID MSSQL_PID=Developer $MssqlConf_setup

