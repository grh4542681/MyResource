#include "log.h"

void Log( char *pcFileName, int iLine, char *pcLogStr,int mode )
{
	char    acFileName[ 201 ];
	char	acDirName[ 201 ];
	char	acTrsFile[ 20 ];
	char	acLogStr[ 4096 ];
	char 	acTimeStr[ 31 ];
	int		i,j,iReturn;
	struct timeval tp ;
	struct 	tm *ts;
	FILE   *fp;

	memset( acLogStr	, 0x00, sizeof(	acLogStr	) );
	memset( acFileName	, 0x00, sizeof(	acFileName	) );
	memset( acDirName	, 0x00, sizeof(	acDirName	) );
	memset( acTrsFile	, 0x00, sizeof(	acTrsFile	) );
	memset( acTimeStr	, 0x00, sizeof(	acTimeStr	) );

	gettimeofday( &tp, NULL ) ;
	ts = localtime( &tp.tv_sec );

	j = 0 ;
	for ( i = 0 ; pcFileName[i] != '\0'; i++ )
	{
		//if ( pcFileName[i] == '.' ) break ;	

		acTrsFile[j++] = pcFileName[i] ; 

		if ( pcFileName[i] == '/' ) 
		{
			j = 0 ;
			memset( acTrsFile, 0x00, sizeof( acTrsFile ) ) ;
		}
	}
	acTrsFile[j] = 0x00 ;

	sprintf( acTimeStr	, "%02d-%02d %02d:%02d:%02d:%06ld",
			 ts->tm_mon+1, ts->tm_mday, ts->tm_hour, 
			 ts->tm_min, ts->tm_sec, tp.tv_usec );

	sprintf( acLogStr	, "%d|%s|%4d|%-9s|%s|\n", 
			  getpid(),acTimeStr, iLine, acTrsFile,pcLogStr );

	if(mode==LEVEL_SYS)
	{
		sprintf( acFileName, "%s/sys.log",SYSLOGDIR);
	}
	else if(mode==LEVEL_APP)
	{
		sprintf( acFileName,"%s/app.log",APPLOGDIR);
	}
	else
	{}

	if ( access( acFileName, W_OK ) )
	{
		umask(111);
		iReturn = creat( acFileName, 0777 );
		close( iReturn );
		umask(022);
	}


	if( (fp = fopen( acFileName, "a+" )) == NULL )
	{
		fprintf( stderr, "打开日志文件[%s]失败!\n", acFileName );
		return;
	}

	fprintf( fp, "%s", acLogStr );
	fflush( fp );
	fclose( fp );

	return;
}