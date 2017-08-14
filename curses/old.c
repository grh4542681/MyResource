#include <curses.h>
#include <string.h>
#include <stdlib.h>
#include <panel.h>

#define WLINE 26
#define LINENUM 200
#define CLOSNUM 120
#define MAXRECLEN 1024

char showline[LINENUM][CLOSNUM];
int currcols;
int currline;
int commond;
int loop;
int sline;

int main(int argc, char const *argv[])
{
	if(argc!=2)
	{
		printf("Bad arguments\nUsage $1 <filename>\n");
		return -1;
	}
	initscr();

	char filename[32];
	char oneline[LINENUM][MAXRECLEN];
	FILE* fp;
	WINDOW *mainwindow;
	WINDOW *subwindow;
	PANEL *mainpanel;
	PANEL *subpanel;
	mainwindow=newwin(WLINE+2+2,CLOSNUM+2,0,0);
	mainpanel=new_panel(mainwindow);
	

	memset(filename,0x00,sizeof(filename));
	strcpy(filename,argv[1]);

	fp=fopen(filename,"r");
	/** Init stdscr */
	keypad(mainwindow,TRUE);
	noecho();
	
	for(loop=0;loop<LINENUM;loop++)
	{
		fgets(oneline[loop],sizeof(oneline),fp);
	}
	do
	{
		
		wclear(mainwindow);
		
		memset(showline,0x00,sizeof(LINENUM*CLOSNUM));
		for(loop=0;loop<WLINE;loop++)
		{
			sline=loop+currline;
			strncpy(showline[loop],oneline[sline]+currcols,CLOSNUM);
			mvwaddstr(mainwindow,loop+1,1,showline[loop]);
		}
		//wrefresh(mainwindow);
		box(mainwindow,0,0);
		mvwhline(mainwindow,WLINE+1,1,0,CLOSNUM);
		update_panels();
		doupdate();

		commond=wgetch(mainwindow);
		switch(commond)
		{
			case 'K':
			case 'k':
			case KEY_UP:
				//up one LINENUM
				if(currline>0)
				{
					currline--;
				}
				break;
			case 'J':
			case 'j':
			case KEY_DOWN:
				//down one LINENUM
				currline++;
				break;
			case 'L':
			case 'l':
			case KEY_RIGHT:
				//right
				if(currcols<MAXRECLEN)
				{
					currcols++;
				}
				break;
			case 'H':
			case 'h':
			case KEY_LEFT:
				//left
				if(currcols>0)
				{
					currcols--;
				}
				break;
			case 't':
				subwindow=derwin(mainwindow,(WLINE+2)/2,(CLOSNUM+2)/2,(WLINE+2)/4,(CLOSNUM+2)/4);
				subpanel=new_panel(subwindow);
				wclear(subwindow);
				top_panel(subpanel);
				box(subwindow,0,0);
				update_panels();
				doupdate();
				sleep(5);
				del_panel(subpanel);
				delwin(subwindow);
				break;
		}
	}while(1);

EXIT:
	endwin();
	fclose(fp);
	return 0;
}

/*Tab2Space( char * str )
{
	int i = 0, j = 0 ;
	int n ;
	char acTmpStr[ MAXCOL + 1 ] ;

	memset( acTmpStr, 0x00, sizeof( acTmpStr ) ) ;

	for	( i = 0 ; str[i] != 0x00 ; i++ )
	{
		switch ( str[ i ] )
		{
			case '\t':
				for ( n = 0 ;n < 4 ; n++ )
					acTmpStr[ j++ ] = ' ' ;
				break ;
			case '\n':
				acTmpStr[ j++ ] = 0x00 ;
				break ;
			default:
				acTmpStr[ j++ ] = str[ i ] ;
		}
	}
	acTmpStr[ j ] = 0x00 ;

	strcpy( str, acTmpStr ) ;

	return 0;
}*/
