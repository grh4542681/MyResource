#!/bin/bash

CurDir=$SHELLHOME/script/dataprep
MntDir=$CurDir/temp

RemoteDir="//ms-sql-hvsrv17/DPGLocAutomation/DPGLocTestData/SQLServer/SQLServer2017/Helsinki"
WinUsr="v-rongao"
WinUsrPwd="Grh4542681~"
MntUsr="helsinki"
MntGrp="helsinki"


if [ -d $MntDir ]
then
	mount -l |grep $RemoteDir
	if [ $? -eq 0 ]
	then
		expect -c "
			spawn sudo -k umount $MntDir
			set timeout 3
			expect {
				\"\\\\\[sudo\\\\\] \" {send \"$ROOTPWD\r\"}
				timeout { }
				eof { exit 9 }
			}
			set timeout -1
			expect {
				\"*$\" { }
			}
		"
	fi
else
	mkdir $MntDir
fi

expect -c "
	spawn sudo -k $CurDir/mntwsfs -u $MntUsr -g $MntGrp -U $WinUsr -P $WinUsrPwd $RemoteDir $MntDir
	set timeout 3
	expect {
		\"\\\\\[sudo\\\\\] \" {send \"$ROOTPWD\r\"}
		timeout { }
		eof { exit 9 }
	}
	set timeout -1
	expect {
		\"*$\" { }
	}
"
	
if [ -d ~/SqlScript ]
then
	rm -rf ~/SqlScript
fi

cp -r $MntDir/Data/Mount/SqlScript ~/SqlScript

cp -r $MntDir/Data/ToLinux/* $SHELLHOME/data

expect -c "
	spawn sudo -k umount $MntDir
	set timeout 3
	expect {
		\"\\\\\[sudo\\\\\] \" {send \"$ROOTPWD\r\"}
		timeout { }
		eof { exit 9 }
	}
	set timeout -1
	expect {
		\"*$\" { }
	}
"

