#!/bin/bash

MssqlConf_setup="/opt/ssis/bin/ssis-conf setup"

Locale=`echo $LANG|awk -F '.' '{print $1}'`

Line=`grep ${Locale} /home/helsinki/env/script/ssis-conf/language.ini`

if [ $? -ne 0 ] 
then
    _Error "unknow locale [$Locale]"
    exit 9
fi

LangID=`echo $Line|awk -F ' ' '{print $2}'`
echo $LangID

export ACCEPT_EULA=Y
export MSSQL_LCID=$LangID
export SSIS_PID=Developer

$MssqlConf_setup

