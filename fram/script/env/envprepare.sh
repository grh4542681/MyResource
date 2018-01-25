#!/bin/bash

. ./common/common.sh

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
                        if(a==1&&$1~/'$Item'/){
                                print $2;
                        }
                }
        }' $IniFile`
        echo ${ItemValue}
}

SysconfFile="./etc/sysconf.ini"
if [ ! -f $SysconfFile ]
then
	_Error "Profile[$SysconfFile] not found."
	exit 9
fi
_SliptLine
_Info "The environment installation script initialization starts."
LinuxIssue=$(_readIniItem "$SysconfFile" "SYSTEM" "LINUXISSUE")
CfgFile=$(_readIniItem "$SysconfFile" "SYSTEM" "DEFAULTCFGFILE")
ROOTPWD=$(_readIniItem "$SysconfFile" "SYSTEM" "ROOTPWD")
DebugFlag=$(_readIniItem "$SysconfFile" "SYSTEM" "DEBUGFLAG")
if [ -z $LinuxIssue ]
then
	_Error "Failed to get Linux version."
	exit 9
fi
if [ -z $CfgFile ]
then
	_Error "Failed to get Default env config file."
	exit 9
fi
if [ -z $ROOTPWD ]
then
	_Error "Failed to get root password."
	exit 9
fi

export ROOTPWD
export SHELLHOME=`pwd`
export TEXTDOMAINDIR=$SHELLHOME/locale

if [ $# -eq 1 ]
then
	CfgFile=$1
fi
if [ ! -f $CfgFile ]
then
	_Error "Configuration file [$CfgFile] exists!"
	exit 9
fi

_Ok "The environment installation script initialization success."

for _Section in $(_readIniSection $CfgFile)
do
	_Package=
	_Repo=
	_Descript=
	_Type=
	_SliptLine
	_Info  "Section [$(_green $_Section)] Begin!"
	_Descript=$(_readIniItem "$CfgFile" "$_Section" "DESCRIPT")
	_Info "Descript [$_Descript]"

	_Intervals=
	_Intervals=$(_readIniItem "$CfgFile" "$_Section" "INTERVALS")
	if [ ! -z $_Intervals ]
	then
		_Info "Wait $_Intervals seconds to ensure script stability"
		sleep $_Intervals
	fi

	_Type=$(_readIniItem "$CfgFile" "$_Section" "TYPE")
	if [[ $_Type == "Install" ]]
	then
		_Package=$(_readIniItem "$CfgFile" "$_Section" "PACKAGE")
		_Repo=$(_readIniItem "$CfgFile" "$_Section" "REPO")
		_Version=$(_readIniItem "$CfgFile" "$_Section" "VERSION")
		_PackageDir=$_Package

		if [ -z $_Repo ]
		then
			_Repo="DEFAULT"
		fi

		_Info "Check package[$_Package] status..."
		#sleep 1
		./common/checkpkg.sh $LinuxIssue $_Package
		_Ret=$?
		if [ $_Ret -eq 1 ]
		then 
			_Info "Begin install package[$_Package]..."
			_Channel=$(_readIniItem "$CfgFile" "$_Section" "CHANNEL")
			_Path=$(_readIniItem "$CfgFile" "$_Section" "PKGPATH")
			_Script="./$_PackageDir/""$_Package"_ADD.sh
			if [ -f $_Script ]
			then
				`echo "$_Script $LinuxIssue $_Channel $_Path $_Repo $_Version"`
				_Ret=$?
				if [[ $_Ret -ne 0 ]]
				then
					_Error "Package[$_Package] install error!" 
					exit 9
				else
					_Ok "Package[$_Package] install done!"
				fi
			else
				_Error "Script[$_Script] not exist!"
				exit 9
			fi
		elif [ $_Ret -eq 0 ]
		then
			_Ok "Package has been installed!"
		else
			_Error "Check Package[$_Package] error!"
			exit 9
		fi
	elif [[ $_Type == "Uninstall" ]]
	then
		_Package=$(_readIniItem "$CfgFile" "$_Section" "PACKAGE")
		_PackageDir=$_Package

		_Info "Check package[$_Package] status..."
		./common/checkpkg.sh $LinuxIssue $_Package
		_Ret=$?
		if [ $_Ret -eq 0 ]
		then 
			_Info "Begin uninstall package[$_Package]..."
			_Path=$(_readIniItem "$CfgFile" "$_Section" "PKGPATH")
			_Channel=$(_readIniItem "$CfgFile" "$_Section" "CHANNEL")
			_Script="./$_PackageDir/""$_Package"_DEL.sh
			if [ -f $_Script ]
			then
				`echo "$_Script $LinuxIssue $_Channel"`
				_Ret=$?
				if [[ $_Ret -ne 0 ]]
				then
					_Error "Package[$_Package] uninstall error!" 
					exit 9
				else
					_Ok "Package[$_Package] uninstall done!"
				fi
			else
				_Error "Script[$_Script] not exist!"
				exit 9
			fi
		elif [ $_Ret -eq 1 ]
		then
			_Ok "Package has not been installed!"
		else
			_Error "Check Package[$_Package] error!"
			exit 9
		fi
	elif [[ $_Type == "Configure" ]]
	then
		_Script=$(_readIniItem "$CfgFile" "$_Section" "_SCRIPT")
		if [ -f $_Script ]
		then
			Parameter=$(_readIniItem "$CfgFile" "$_Section" "PARAMETER")
			echo $_Script $Parameter
			$_Script $Parameter
			_Ret=$?
			if [[ $_Ret -ne 0 ]]
			then
				_Error "Configure[$_Script] execute error."
				exit 9
			else
				_Ok "Configure[$_Script] execute done."
			fi
		else
			_Error "Script[$_Script] not exist."
			exit 9
		fi
	fi
	_Ok "Section [$(_green $_Section)] Done!"
	_SliptLine
done
