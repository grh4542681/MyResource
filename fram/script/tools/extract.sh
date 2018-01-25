#!/bin/bash

if [ $# -ne 3 ]
then
	echo "Usage $1 [file] [WorkDir] [OutDir]"
	exit 9
fi

ExtFile=$1
ExtDir=$2
OutDir=$3

cat $ExtFile|while read Line
do
	CaseID=`echo $Line|cut -b 1-9`
	echo $CaseID
	AllDir=`find $ExtDir -type d -name ${CaseID}`
	for CaseDir in $AllDir
	do
		RCaseDir=${CaseDir/$ExtDir//}
		OCaseDir=$OutDir$RCaseDir
		if [ ! -d $OCaseDir ]
		then
		 	mkdir -p $OCaseDir
		fi
		cp -r $CaseDir/* $OCaseDir/
	done
done
