#!/bin/bash

echo $PATH|grep "/opt/mssql/bin"
if [ $? -ne 0 ]
then
	echo "export PATH=\$PATH:/opt/mssql/bin" >> ~/.bashrc
	. ~/.bashrc
fi
