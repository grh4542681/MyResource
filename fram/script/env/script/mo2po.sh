#!/bin/bash

if [ $# -ne 1 ]
then
	echo "Usage $0 [file|directory]"
	exit 9
fi

if [ -d $1 ]
then
	Mofilelist=`find $1 -type f -name "*.mo"`
	for Mofile in $Mofilelist
	do
		Mofilename=${Mofile##*/}
		Mofilepath=${Mofile%/*}
		Pofilename=${Mofilename/%mo/po}
		Pofile=${Mofilepath}"/"${Pofilename}
	
		msgunfmt $Mofile -o $Pofile
		if [ $? -ne 0 ]
		then
			echo "Generate $Pofile error"
		else
			echo $Pofile
		fi
	done
elif [ -f $1 ]
then
	Mofile=$1
	Pofile=${Mofile/%mo/po}
	msgunfmt $Mofile -o $Pofile
	if [ $? -ne 0 ]
	then
		echo "Generate $Pofile error"
	else
		echo $Pofile
	fi
else
	echo "[$1]not file or directory"
fi

