#!/bin/bash
RED='\033[5;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
CLREND='\033[0m'

export DebugFlag=1

function _red() {
        echo -e ${RED}$1${CLREND}
}

function _green(){
        echo -e ${GREEN}$1${CLREND}
}

function _yellow(){
        echo -e ${YELLOW}$1${CLREND}
}

function _blue(){
        echo -e ${BLUE}$1${CLREND}
}

function _Debug(){
        if [[ $DebugFlag = 1 ]]
        then
                echo -e "[ $(_yellow "Debug") ] $1"
        fi
}

function _Info(){
        echo -e "[ $(_blue "Info") ] $1"
}

function _Warn(){
	echo -e "[ $(_yellow "Warn") ] $1"
}

function _Error(){
        echo -e "[ $(_red "Error") ] $1"
}

function _Ok(){
        echo -e "[ $(_green "Ok") ] $1"
}

function _SliptLine(){
        echo "-----------------------------------------------------------------"
}

function _readIniSection() {
    IniFile=$1
    Section=`awk -F '\\\[|\\\]' '{if($0!~/^\#/&&$0~/\[*.\]/) print $2}' $IniFile`
    echo ${Section}
}

function _readIniItem() {
    IniFile=$1; Section=$2; Item=$3
        ItemValue=`awk -F '=' '{
                if($0!~/\#/&&$0!~/^$/){
                        if($0~/\['$Section'\]/){
                                a=1;
                                next;
                        }
                        if(a==1&&$0~/^[\[]/){
                                exit;
                        }
                        if(a==1&&$1~/^'$Item'/){
                                print $2;
				exit;
                        }
                }
        }' $IniFile`
        echo ${ItemValue}
}
