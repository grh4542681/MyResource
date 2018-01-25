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
InsPkgs="pacemaker pcs fence-agents resource-agents"
VerifyFlag=0

if [[ $Platform == "Ubuntu" ]]
then
	InsCmd="apt-get install -y "
	UpdCmd="apt-get update"
elif [[ $Platform == "RedHat" ]]
then
	InsCmd="yum -y install "
	UpdCmd=""
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

#Verify each node
if [[ $VerifyFlag == 1 ]]
then
	_Info "Verify ecah node..."
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

_Info "Install packages on each node..."
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
				send \"$InsCmd $InsPkgs\n\"
			}
		}
		expect {
			\"*#\" {
				send \"passwd hacluster\n\"
			}
		}
		expect {
			\"UNIX\" {
				send \"hacluster\n\"
			}
		}
		expect {
			\"UNIX\" {
				send \"hacluster\n\"
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
