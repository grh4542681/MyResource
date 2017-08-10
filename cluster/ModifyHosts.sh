#!/bin/bash

. ./common.sh

if [ $# -lt 1 ] 
then
	echo "Usage $1 [-v|] [Node1] [Node2] ...."
	exit 9
fi

Platform="Ubuntu"
DefaultUsr="root"
DefaultPwd="root"
VerifyFlag=0

if [[ $Platform == "Ubuntu" ]]
then
	HostFile="/etc/hosts"
elif [[ $Platform == "RedHat" ]]
then
	HostFile="/etc/hosts"
else
	echo "Unknow Platform [$Platform]"
	exit 9
fi

while getopts ":v" opt
do
	case $opt in
		v)  
			VerifyFlag=1
			;;  
		?)  
			echo "Unknow option [$opt]!"
			exit 9
	esac
done
shift $(($OPTIND-1))

if [[ $VerifyNode == 1 ]]
then
	#Verify each node
	_Info "Verify each node..."
	for node in $@
	do
		AddressLine=`host $node|grep "address"`
		if [ $? -ne 0 ] 
		then
			_Error "Node [$node] con't resolve"
			exit 9
		else
			Address=`echo $AddressLine | awk -F ' ' '{print $4}'`
			_Debug "Node [$node] maping address [$Address]"
		fi
	done
	_Ok "Each node verify complete"
fi

_Info "Configuration each node hosts file..."
for node in $@
do
	User=$DefaultUsr
	Password=$DefaultPwd

	_Debug "On node [$node]..."

	expect -c "
		spawn ssh -oStrictHostKeyChecking=no $User@$node
		expect {
			\"password\" {
				send \"$Password\n\"
			}
		}
		set timeout -1
		expect {
			\"*#\" {
				send \"sed -i \\\"/^127.0/{/$node/{s/^/#&/g}}\\\" $HostFile\n\"
			}
		}
		expect {
			\"*#\" {
				send \"exit\n\"
			}
		}
		expect eof
	"
	sleep 1
	_Ok "On node [$node] install complete"
	sleep 2
done





