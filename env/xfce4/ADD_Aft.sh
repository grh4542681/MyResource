#!/bin/bash

cd ~ 
echo xfce4-session > .xsession 
if [ $? -ne 0 ]
then
	exit 9
fi
