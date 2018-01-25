#!/bin/bash

SAPwd="Auto@sql"

LANGID=$($SHELLHOME/script/mssql-conf/getlangopt.sh)
export TEXTDOMAIN=mssql-conf

#Str1=`gettext -e -s "Do you accept the license terms? [Yes/No]:"`
Str1="Do you accept the license terms? [Yes/No]:"
Str4=`gettext -e -s "Enter the SQL Server system administrator password: "`
Str5=`gettext -e -s "Confirm the SQL Server system administrator password: "`

Str1=${Str1//\[/\\\\\\\[}
Str1=${Str1//\]/\\\\\\\]}
Str3=`printf "$Str3" "Express" "Evaluation" "Developer" "Developer"`
Str3=${Str3//\"/\\\\\\\"}

expect -c "
	spawn sudo /opt/mssql/bin/mssql-conf setup
	set timeout 3
	expect {
		\"\\\\\[sudo\\\\\] \" {send \"$ROOTPWD\r\"}
		timeout { }
	 	eof { exit 9 }
	}
	expect {
		\"$Str1\" { send \"YES\n\" }
		timeout { }
		eof { exit 9 }
	}
	set timeout -1
	expect {
		\"$Str4\" { send \"$SAPwd\n\" }
		timeout {
			send_user \"Except [password] on screen, But timeout!\"
			exit 9
		}
		eof { exit 9 }
	}
	expect {
		\"$Str5\" { send \"$SAPwd\n\" }
		timeout {
			send_user \"Except [confirm the password] on screen, But timeout!\"
			exit 9
		}
		eof { exit 9 }
	}
	expect {
		\"*\\\\\$\" { }
	}
"
