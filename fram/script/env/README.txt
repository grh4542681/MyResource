How to use it：
	./envprepare.sh [config file]
	If no configuration file be specified, './etc/default.ini' is used by default
	
Directory structure introduced:
	common:
		Common script directory
	etc:
		System Configuration Files and Default Environment Initialize the configuration file directory
	script:
		Other scripts store directories
	locale:
		Localized language file directory
	
	Other directory for the application installation script directory, naming rules:
		[application name]_[version] or [application name]
		The directory name must be the same as the application name specified in the environment configuration file
		
Examples of environment profiles:
	[STEP1]				(Step name)
	DESCRIPT=Expect package install
	TYPE=Install		(The configuration type contains Install,Uninstall or Configuration)
	PACKAGE=expect		(application name)
	CHANNEL=Net			(The installation channel contains Net or Local)
	PKGPATH=./visual-studio-code/code.deb		(If the channel is local, use this path for package installation)
	_SCRIPT=./script/1.sh		(If the configuration type is configuration, execute this script)
	PARAMETER=			(Script parameters)
	INTERVALS=5			(And the interval between the previous steps)

	
./etc/sysconf.ini:
	[SYSTEM]
	LINUXISSUE=Ubuntu			(system version Ubuntu|Red Hat|...)
	ROOTPWD=@WSX3edc			(The current user password)
	DEBUGFLAG=0					(Whether to print debugging information, 1--yes, 0--no)
	DEFAULTCFGFILE=./etc/default.ini	(Default Environment Initializes the configuration file)
