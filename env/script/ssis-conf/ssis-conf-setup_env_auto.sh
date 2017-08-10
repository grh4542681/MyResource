#!/bin/bash

expect -c "
    spawn sudo $SHELLHOME/script/ssis-conf/ssis-conf-setup_env.sh
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
