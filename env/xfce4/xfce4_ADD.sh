#!/bin/bash

. $SHELLHOME/common/common.sh

if [ $# -lt 2 ]
then
	_Error "Usage: $0 [Ubuntu|Red Hat|...] [Net|Loacl|...] [ |Path]"
	exit 9
fi

if [[ $2 == "Local" ]] && [ $# -lt 3 ]
then 
	_Error "Usage: $0 [Ubuntu|Red Hat|...] [Net|Loacl|...] [ |Path]"
	exit 9
fi

DIR=$SHELLHOME/xfce4

if [ -f $DIR/ADD_Bef.sh ]
then
	_Info "Start the pre-installation script."
	$DIR/ADD_Bef.sh
	if [ $? -ne 0 ]
	then
		_Error "Pre-installation script execution failed."
		exit 9
	fi
	_Ok "The pre-installation script execution is complete."
fi

_Info "Start the installation script."
$DIR/ADD.exp $1 $2 $3
if [ $? -ne 0 ]
then 
	_Error "Installation script execution failed."
	exit 9
fi
_Ok "The installation script execution is complete."

if [ -f $DIR/ADD_Aft.sh ]
then
	_Info "Start the post-installation script."
	$DIR/ADD_Aft.sh
	if [ $? -ne 0 ]
	then
		_Error "Post-installation script execution failed."
		exit 9
	fi
fi
_Ok "The post-installation script execution is complete."

exit 0

