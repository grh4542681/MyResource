#!/bin/bash
. $SHELLHOME/common/common.sh

RepoFileDir=`dirname $0`
RepoFile=$RepoFileDir"/mssqlrepo.list"
Repo=`awk -F ' ' '/^'"$1"'/ {for (i=2;i<=NF;i++)printf("%s ", $i);}' $RepoFile`
_Info "$Repo"

if [ -z "$Repo" ]
then
	echo "Unknow REPO[$1]"
	exit 9
fi

if [ -d "/var/opt/mssql" ]
then
	expect -c "
	spawn sudo rm -rf /var/opt/mssql
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
			send \"rm -rf /etc/apt/sources.list.d/*\n\"
		}
	}
	expect {
		\"*#\" {
			send \"curl -qsf https://repo.corp.microsoft.com/ubuntu/dpgswdist.v1.asc | sudo apt-key add -\n\"
		}
	}
	expect {
		\"*#\" {
			send \"echo \\\"$Repo\\\" > /etc/apt/sources.list.d/mssql.list\n\"
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
