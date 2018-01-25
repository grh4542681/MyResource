#!/bin/bash
RED='\033[5;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
CLREND='\033[0m'

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
                echo "[ $(_blue "Debug") ] $1"
        fi
}

function _Info(){
        echo "[ $(_blue "Info") ] $1"
}

function _Warn(){
	echo "[ $(_yellow "Warn") ] $1"
}

function _Error(){
        echo "[ $(_red "Error") ] $1"
}

function _Ok(){
        echo "[ $(_green "Ok") ] $1"
}

function _SliptLine(){
        echo "-----------------------------------------------------------------"
}

