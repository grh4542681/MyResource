#!/bin/bash

echo $PATH|grep "/opt/mssql-tools/bin"
if [ $? -ne 0 ]
then
	echo "export PATH=\$PATH:/opt/mssql-tools/bin" >> ~/.bashrc
	. ~/.bashrc
fi
