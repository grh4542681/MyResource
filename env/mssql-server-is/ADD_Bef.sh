#!/bin/bash

if [ -d "/var/opt/ssis" ]
then
	expect -c "
	spawn sudo rm -rf /var/opt/ssis
	set timeout 2
	expect {
		\"\\\\\\[sudo\\\\\\] \" {send \"$ROOTPWD\r\"}
		timeout { } 
		eof { exit 9 } 
	}   
	expect {
		\"\\\\\\[sudo\\\\\\] \" { 
			 send_user \"Wrong Password!\" 
			 exit 9
		}   
		timeout { } 
		eof { exit 9 } 
	} 
	expect {
		\"*$\" {
			
		}
	}
	"
fi

expect -c "
	spawn sudo su
	set timeout 2
	expect {
		\"\\\\\\[sudo\\\\\\] \" {send \"$ROOTPWD\r\"}
		timeout { } 
		eof { exit 9 } 
	}   
	expect {
		\"\\\\\\[sudo\\\\\\] \" { 
			 send_user \"Wrong Password!\" 
			 exit 9
		}   
		timeout { } 
		eof { exit 9 } 
	} 
	set timeout -1
	expect {
		\"*#\" {
			send \"curl https://packages.microsoft.com/keys/microsoft.asc | sudo apt-key add -\n\"
		}
	}
	expect {
		\"*#\" {
			send \"curl https://packages.microsoft.com/config/ubuntu/16.04/mssql-server.list | sudo tee /etc/apt/sources.list.d/mssql-server.list\n\"
		}
	}
	expect {
		\"*#\" {
			
		}
	}
"
if [ $? -ne 0 ]
then
	exit 9
fi

echo "$ROOTPWD"|sudo -S apt-get update
if [ $? -ne 0 ]
then
	exit 9
fi
