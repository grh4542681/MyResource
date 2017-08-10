#!/bin/bash

. $SHELLHOME/common/common.sh

Sqlconf="/opt/mssql/bin/mssql-conf"
ClnCmd="/opt/mssql/bin/mssql-conf set-collation"

Locale=`echo $LANG|awk -F '.' '{print $1}'`

Line=`grep ${Locale} $SHELLHOME/script/collation/collation.ini`

if [ $? -ne 0 ]
then
	_Error "unknow locale [$Locale]"
	exit 9
fi

Collation=`echo $Line|awk -F ' ' '{print $2}'`
echo $Collation

expect -c "
	spawn sudo systemctl stop mssql-server
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
	

expect -c "
	spawn $ClnCmd
	set timeout 3
    expect {
        \"\\\\\[sudo\\\\\] \" {send \"$ROOTPWD\r\"}
        timeout { }
        eof { exit 9 }
    }
	set timeout -1
	expect {
		\"collation\" {send \"$Collation\n\"}
        timeout {
            send_user \"Except [password] on screen, But timeout!\"
            exit 9
        }
        eof { exit 9 }
	}
    expect {
        \"*$\" { }
    }
"

expect -c "
	spawn sudo systemctl start mssql-server
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

