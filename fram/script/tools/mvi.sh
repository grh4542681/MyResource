#!/bin/bash

RootDir="home"
TopDir="workspace"
CurrDir=`pwd`
echo "$CurrDir"

TempDir=$CurrDir
while :; do
    if [[ ${TempDir##*/} == $TopDir ]] || [[ ${TempDir##*/} == $RootDir ]] || [[ "x${TempDir##*/}" == "x" ]]
    then
        break
    fi
    if [ -f "$TempDir"/tags ]
    then
        echo "$TempDir"/tags
        sed -i -e 's%tags=.*$%tags='$TempDir'/tags%g' $HOME/.vimrc
        break
    fi
    TempDir=${TempDir%/*}
done

vim $@
