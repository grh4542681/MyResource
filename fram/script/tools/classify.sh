#!/bin/bash

if [ $# -ne 1 ]
then
	echo "Usege $1 [foldr] (must Absolute path)"
	exit 9
fi

if [[ ${1%%/*} == '.' ]]
then
	echo "Must Absolute path"
	exit 9
fi

AllFiles=`find $1 -type f -name "ID_*"`

for File in $AllFiles
do
	BaseFile=${File##*/}
	CaseID=`echo $BaseFile|cut -b 4-12`
	BasePath=${File%/*}
	BasepPath=${BasePath##*/}
	
#	echo $CaseID $BasepPath
	if [[ $CaseID != $BasepPath ]]
	then
		Flag=0
		for WorkDir in $AllWorkDir
		do
			if [[ $WorkDir == $BasePath ]]
			then
				Flag=1
				break
			fi
		done
		if [[ $Flag == 0 ]]
		then
			AllWorkDir=$AllWorkDir" "$BasePath
		fi
	fi
done

#AllWorkDir=`echo $AllWorkDir|uniq`
	
for WorkDir in $AllWorkDir
do
	cd $WorkDir
	AllCase=`ls | cut -b 4-12|uniq`
	for CaseID in $AllCase
	do  
		mkdir $CaseID
		mv "ID_"${CaseID}* $CaseID
	done
done

