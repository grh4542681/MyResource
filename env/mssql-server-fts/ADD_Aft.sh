#!/bin/bash

expect -c "
    spawn sudo systemctl restart mssql-server
    set timeout 3
    expect {
            \"\\\\\[sudo\\\\\] \" {send \"$ROOTPWD\r\"}
            timeout { }
            eof { exit 9 }
    }
    expect {
        \"*$\" { exit 0 }
    }
"

