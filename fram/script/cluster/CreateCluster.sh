#!/bin/bash

. ./common.sh

if [ $# -lt 2 ] 
then
	echo "Usage $0 [Master] [Node1] [Node2]..."
	exit 9
fi

if [ `id -u` -ne 0 ] 
then
    echo "Permission denied"
    exit 9
fi

DefaultUsr="root"
DefaultPwd="root"

LocalName=`hostname`
MasterName=$1
if [[ $MasterName != $LocalName ]]
then
	_Error "Must run the script on master node that you specify[$MasterName]"
	exit 9
fi

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

./InstallCluster.sh $@

sleep 2

./ModifyHosts.sh $@

for node in $@
do
	User=$DefaultUsr
	Password=$DefaultPwd

	_Debug "Base configure on node [$node]"
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
				send \"ufw disable\n\"
			}
		}
		expect {
			\"*#\" {
				send \"systemctl enable pcsd\n\"
			}
		}
		expect {
			\"*#\" {
				send \"systemctl start pcsd\n\"
			}
		}
		expect {
			\"*#\" {
				send \"pcs cluster destroy\n\"
			}
		}
		expect {
			\"*#\" {
				send \"mkdir /var/log/cluster\n\"
			}
		}
		expect {
			\"*#\" {
				send \"exit\n\"
			}
		}
		expect eof
	"
	_Ok "Base configure on node [$node] complete"
done

_Info "Configure on master node..."
_Info "Auth each node..."
pcs cluster auth $@ -u hacluster -p hacluster

if [ $? -ne 0 ]
then
	_Error "Auth each node error"
	exit 9
else
	_Ok "Auth each node success"
fi

ClsName="testcls"

_Info "Create cluster[$ClsName]..."
pcs cluster setup --name $ClsName $@

if [ $? -ne 0 ]
then
	_Error "Create cluster error"
	exit 9
else
	_Ok "Create cluster success"
fi

_Info "Start cluster..."
pcs cluster start --all

if [ $? -ne 0 ]
then
	_Error "Start cluster error"
	exit 9
else
	_Ok "Start cluster success"
fi





