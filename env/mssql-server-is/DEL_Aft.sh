#!/bin/bash

if [ -d /var/opt/ssis ]
then
	expect -c "
		spawn sudo rm -rf /var/opt/ssis
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


