#!/bin/bash

if [ $# -lt 2 ] 
then
	echo "Usage: $0 MasterNode SecondNode ..."
	exit 9
fi

MasterNode=$1
Hostname=`hostname`

if [[ $Hostname != $MasterNode ]]
then
	echo "Must run on MasterNode[$MasterNode] LocalNode[$Hostname]!"
	exit 9
fi

if [ `id -u` -ne 0 ] 
then
	echo "Permission denied"
	exit 9
fi

ShareDir="/var/opt/mssql/tlogs"
RedsFile="/home/helsinki/env/script/logshipping/tlogcreds"
RootUsr="root"
RootPwd="root"

if [ -d $SahreDir ]
then
	rm -rf $ShareDir
fi

mkdir $ShareDir
chown mssql:mssql $ShareDir
chmod 0700 $ShareDir

SambaConf="/etc/samba/smb.conf"
#SambaConf="/home/helsinki/2.txt"

sed -i -e "/\[tlogs\]/,/\(^$\|^\[\)/d" $SambaConf

echo "
[tlogs]
path=/var/opt/mssql/tlogs
available=yes
read only=yes
browsable=yes
public=yes
writable=no
" >> $SambaConf


expect -c "
	spawn smbpasswd -a mssql
	set timeout -1
	expect {
		\"SMB\" { send \"mssql\n\"}
	}
	expect {
		\"SMB\" { send \"mssql\n\"}
	}
	expect {
		\"*#\" { }
	}
"

systemctl restart smbd.service nmbd.service


shift 1
for SecondNode in $@
do
	expect -c "
		spawn scp -oStrictHostKeyChecking=no $RedsFile $RootUsr@$SecondNode:/var/opt/mssql
		expect {
			\"password\" {
				send \"$RootPwd\n\"
			}
		} 
		expect {
			\"*#\" {}
		}
	"
	
	expect -c "
	spawn ssh -oStrictHostKeyChecking=no $RootUsr@$SecondNode
	expect {
		\"password\" {
			send \"$RootPwd\n\"
		}
	}
	set timeout 3
	expect {
		\"password\" {
			exit 9
		}
	}
	set timeout -1
	expect {
		\"*#\" {
			send \"/home/helsinki/env/script/logshipping/ConfSecond.sh $MasterNode\n\"
		}
	}
	expect {
		\"*#\" {  }
	}
	"
done








