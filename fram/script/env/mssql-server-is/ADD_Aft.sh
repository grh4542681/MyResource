#!/bin/bash

echo $PATH|grep "/opt/ssis/bin"
if [ $? -ne 0 ]
then
	echo "export PATH=\$PATH:/opt/ssis/bin" >> ~/.bashrc
	. ~/.bashrc
fi

echo "$ROOTPWD"|sudo -k -S gpasswd -a helsinki ssis
if [ $? -ne 0 ] 
then
    exit 9
fi

