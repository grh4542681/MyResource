#!/bin/bash

expect -c "
    spawn sudo $SHELLHOME/script/mssql-conf/mssql-conf-setup_env.sh
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
