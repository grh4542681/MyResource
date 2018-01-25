#!/bin/bash

if [ $# -lt 2 ]
then
    echo "Usage: $0 MasterNode SecondNode"
    exit 9
fi

MasterNode=$1
SecondNode=$2

BaseDir=`pwd`
TemplateDir=$BaseDir"/template"

Hostname=`hostname`

if [[ $Hostname != $MasterNode ]]
then
	echo "Must run on MasterNode[$MasterNode] LocalNode[$Hostname]!"
	exit 9
fi

cd tsql

#Generate master node t-sql script
if [ -d $MasterNode ]
then
	rm -rf $MasterNode
fi
mkdir $MasterNode

cd $MasterNode
cat $TemplateDir/master_create_db.sql > CreateSampleDB.sql
cat $TemplateDir/master_backup_db.sql > master_1.sql
sed "s/<ip_address_of_secondary_server>/$SecondNode/g" $TemplateDir/master_create_job.sql > master_2.sql
cat $TemplateDir/master_test_job.sql > master_test.sql
cat $TemplateDir/drop_db.sql > DropSampleDB.sql
cd ..

#Generate second node t-sql script
if [ -d $SecondNode ]
then
	rm -rf $SecondNode
fi
mkdir $SecondNode

cd $SecondNode
cat $TemplateDir/second_restore_db.sql > second_1.sql
sed "s/<ip_address_of_primary_server>/$MasterNode/g" $TemplateDir/second_create_job.sql > second_2.sql
cat $TemplateDir/second_test_job.sql > second_test.sql
cat $TemplateDir/drop_db.sql > DropSampleDB.sql
cd ..

cd ..





