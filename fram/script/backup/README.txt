Features:
	Backup files by current time

Useage:
	./backup.sh

backup.ini:
	The script executes the parameter configuration file
	example:
		[helsinki]														----------------- backup file main name
		TYPE=tar														----------------- backup file type
		BACKUPTARGET=/home/helsinki/cluster 							----------------- backup files or directorys (Separated by spaces)
		USER=helsinki													----------------- The user who use this script
		GROUP=helsinki													----------------- user's group
		PASSWORD=@WSX3edc												----------------- user's password
		BACKUPPATH=/home/helsinki/backup								----------------- backup file storage path
		SYNCFLAG=YES													----------------- Whether to sync to other locations
		SYNCDIRPLAT=windows												----------------- The system type of the synchronous address
		SYNCDIRTYPE=share												----------------- Synchronous address type
		SYNCMNTPOINT=/home/helsinki/backup/mount						----------------- If the type is share,Specify the mount point
		SYNCDIRPATH=//MS-SQL-HVSRV23/Project/SQL17/Helsinki/BackUp		----------------- Synchronous address
		SYNCUSER=svcdploc												----------------- Synchronous address user
		SYNCPASSWORD=A9K@Pti16L											----------------- Synchronous address password

Notice:
	The path in the configuration file must be an absolute path!
