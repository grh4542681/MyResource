#!/bin/bash

#Feartures:
#   Compile the po file to a mo file

if [ $# -ne 1 ]
then
	echo "Usage $0 [file|directory]"
	exit 9
fi

if [ -d $1 ]
then
	Pofilelist=`find $1 -type f -name "*.po"`
	for Pofile in $Pofilelist
	do
		Pofilename=${Pofile##*/}
		Pofilepath=${Pofile%/*}
		Mofilename=${Pofilename/%po/mo}
		Mofile=${Pofilepath}"/"${Mofilename}
	
		msgfmt $Pofile -o $Mofile
		if [ $? -ne 0 ]
		then
			echo "Generate $Mofile error"
		else
			echo $Mofile
		fi
	done
elif [ -f $1 ]
then
	Pofile=$1
	Mofile=${Pofile/%po/mo}
	msgfmt $Pofile -o $Mofile
	if [ $? -ne 0 ]
	then
		echo "Generate $Mofile error"
	else
		echo $Mofile
	fi
else
	echo "[$1]not file or directory"
fi

