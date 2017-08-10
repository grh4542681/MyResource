#!/bin/bash

if [ -d /var/opt/mssql ]
then
	expect -c "
		spawn sudo rm -rf /var/opt/mssql
		set timeout 3
	    expect {
    	    \"\\\\\[sudo\\\\\] \" {send \"$ROOTPWD\r\"}
       		timeout { }
       		eof { exit 9 }
   		}
		expect {
			\"*\\\\\$\" { }
		}
	"
fi


