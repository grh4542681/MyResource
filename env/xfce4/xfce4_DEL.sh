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

if [ -f $DIR/DEL_Bef.sh ]
then
	_Info "Start the pre-uninstallation script."
	$DIR/DEL_Bef.sh
	if [ $? -ne 0 ]
	then
		_Error "Pre-uninstallation script execution failed."
		exit 9
	fi
	_Ok "The pre-uninstallation script execution is complete."
fi

_Info "Start the uninstallation script."
$DIR/DEL.exp $1 $2 $3
if [ $? -ne 0 ]
then 
	_Error "Uninstallation script execution failed."
	exit 9
fi
_Ok "The uninstallation script execution is complete."

if [ -f $DIR/DEL_Aft.sh ]
then
	_Info "Start the post-uninstallation script."
	$DIR/DEL_Aft.sh
	if [ $? -ne 0 ]
	then
		_Error "Post-uninstallation script execution failed."
		exit 9
	fi
fi
_Ok "The post-uninstallation script execution is complete."

exit 0

