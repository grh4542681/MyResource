#!/bin/bash

BACKHOME=/home/helsinki/backup
. $BACKHOME/common.sh

IniFile=$BACKHOME/backup.ini
CurrTime=`date +%y%m%d%H%M%S`
Date=`date`
_Info "Backup time [$Date]"

for _Section in $(_readIniSection $IniFile)
do
	_Info "Backup Section [$_Section]"
	_Type=$(_readIniItem "$IniFile" "$_Section" "TYPE")
	if [ -z $_Type ]
	then
		_Type=tar
	fi

	_BackupTarget=$(_readIniItem "$IniFile" "$_Section" "BACKUPTARGET")
	_BackupTarget=($_BackupTarget)
	_BackupPath=$(_readIniItem "$IniFile" "$_Section" "BACKUPPATH")"/$_Section"
	BackupFile=$_Section.$CurrTime.$_Type
	_Info "Backup File [$_BackupPath/$BackupFile]"

	if [[ $_Type == "tar" ]]
	then
		TargetNum=${#_BackupTarget[@]}
		if [[ $TargetNum == 1 ]]
		then
			TarDir=${_BackupTarget%/*}
			TarObj=${_BackupTarget##*/}
			PackCommand="tar -C $TarDir -cvf $_BackupPath/$BackupFile $TarObj"
		else
			SpliDir=(${_BackupTarget[0]//\// })
			Loop=0
			for SubDir in ${SpliDir[@]}
			do
				for SubObj in ${_BackupTarget[@]}
				do
					SubObj=(${SubObj//\// })
					if [[ $SubDir != ${SubObj[$Loop]} ]]
					then
						flag=1
						break
					fi
				done
				if [[ $flag == 1 ]]
				then
					TarObj=${_BackupTarget[@]//${TarDir}"/"/ }
					break
				else
					TarDir=$TarDir/$SubDir
					Loop=$((Loop+1))
				fi
			done
			PackCommand="tar -C $TarDir -cvf $_BackupPath/$BackupFile $TarObj"
		fi
	elif [[ $_Type == "zip" ]]
	then
		PackCommand="zip -r $_BackupPath/$BackupFile ${_BackupTarget[@]}"
	else
		_Error "Unknow Compress type[$_Type]"
		exit 9
	fi

	if [ ! -d $_BackupPath ]
	then
		mkdir -p $_BackupPath
	fi

	($PackCommand)

	_Ok "Backup complete [$_Section]"

	_User=$(_readIniItem "$IniFile" "$_Section" "USER")
	_Group=$(_readIniItem "$IniFile" "$_Section" "GROUP")
	_Password=$(_readIniItem "$IniFile" "$_Section" "PASSWORD")

	_SyncFlag=$(_readIniItem "$IniFile" "$_Section" "SYNCFLAG")
	_SyncFlag=`echo $_SyncFlag|tr '[:upper:]' '[:lower:]'`
	if [[ $_SyncFlag == yes ]]
	then
		_SyncDirPlat=$(_readIniItem "$IniFile" "$_Section" "SYNCDIRPLAT")
		_SyncDirPlat=`echo $_SyncDirPlat|tr '[:upper:]' '[:lower:]'`
		if [[ $_SyncDirPlat == "windows" ]]
		then
			_SyncDirPath=$(_readIniItem "$IniFile" "$_Section" "SYNCDIRPATH")
			_SyncUser=$(_readIniItem "$IniFile" "$_Section" "SYNCUSER")
			_SyncPassword=$(_readIniItem "$IniFile" "$_Section" "SYNCPASSWORD")
			_SyncMntPoint=$(_readIniItem "$IniFile" "$_Section" "SYNCMNTPOINT")

			SyncMntCommand="sudo mount -t cifs -o username=$_SyncUser,password=$_SyncPassword,uid=$_User,gid=$_Group,vers=2.0 $_SyncDirPath $_SyncMntPoint"
			SyncUmntCommand="sudo umount $_SyncMntPoint"
			
			if [ ! -d $_SyncMntPoint ]
			then
				mkdir -p $_SyncMntPoint
			fi

			expect -c "
				spawn $SyncMntCommand
				expect {
					\"\\\\\[sudo\\\\\] \" {send \"$_Password\r\"}
					timeout { }
					eof { exit 9 }
				}
				expect {
					\"*#\" { send \"exit\n\" }
					\"*\\\\\$\" { send \"exit\n\" }
				}
			"
			
			_Info "Synchronize files to [$_SyncDirPath]..."
			sleep 2
			cp $_BackupPath/$BackupFile $_SyncMntPoint
			sleep 2
			_Ok "Synchronize files to [$_SyncDirPath] Success"

			expect -c "
				spawn $SyncUmntCommand
				expect {
					\"\\\\\[sudo\\\\\] \" {send \"$_Password\r\"}
					timeout { }
					eof { exit 9 }
				}
				expect {
					\"*#\" { send \"exit\n\" }
					\"*\\\\\$\" { send \"exit\n\" }
				}
			"
		elif [[ $_SyncDirPlat == "linux" ]]
		then
			echo "Not Support yet!"
			exit 9
		else
			_Error "Unknow SyncDirPlat[$_SyncDirPlat]"
			exit 9
		fi
	fi	

done

