#include "showfile.h"

char taitle[MAXRECLEN];
char htail[MAXRECLEN];
char atail[MAXRECLEN];

int reclen;

void INIT_LIST_HEAD(struct list_head *list)
{
	list->next = list;
	list->prev = list;
}

void _list_add(struct list_head *new,struct list_head *prev,struct list_head *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

void list_add(struct list_head *new, struct list_head *head)
{
	_list_add(new, head, head->next);
}

void list_add_tail(struct list_head *new, struct list_head *head)
{
	_list_add(new, head->prev, head);
}


void _list_del(struct list_head * prev, struct list_head * next)
{
	next->prev = prev;
	prev->next = next;
}

void list_del(struct list_head *del)
{
	_list_del(del->prev,del->next);
}

int list_empty(const struct list_head *head)
{
	return head->next == head;
}

void reclist_init(struct reclist *preclist)
{
	INIT_LIST_HEAD(&(preclist->r_list));
}

int reclist_add(struct list_head *head,struct reclist *preclist)
{
	struct reclist *new;
	new=malloc(sizeof(struct reclist));
	if(!new)
	{
		return -1;
	}
	memcpy(new,preclist,sizeof(struct reclist));
	list_add(&(new->r_list),head);
	return 0;
}

void reclist_del(struct reclist *preclist)
{
	list_del(&(preclist->r_list));
	free(preclist);
}

void reclist_destroy(struct list_head *head)
{
	struct reclist *pcurr;
	list_for_each_entry(pcurr, head, r_list)
	{
		reclist_del(pcurr);
	}
}

/**
 * [LoadF2List description]
 * Load file in list
 * @param  filename [Filename]
 * @param  rec_head [Head of list]
 * @return          [Success return rec num,Failed return -1]
 */
int LoadF2List(char *filename,struct list_head *prec_head)
{
	char rectmp[MAXRECLEN+1];
	struct reclist streclist;
	int num=0;
	if(!filename)
		return -1;
	FILE *fp;
	fp=fopen(filename,"r");
	if(fp<=0)
		return -1;
	fgets(taitle,MAXRECLEN,fp);  /**< Taitle of file */
	Tab2Space(taitle);
	reclen=strlen(taitle);
	Tab2Space(taitle);	
	while(!feof(fp))
	{
		memset(&streclist,0x00,sizeof(struct reclist));
		memset(rectmp,0x00,sizeof(rectmp));
		fgets(rectmp,MAXRECLEN,fp);
		Tab2Space(rectmp);				/**< Change table to space */
		streclist.rec=(char*)malloc(MAXRECLEN);
		if(!streclist.rec)
		{
			fclose(fp);
			return -1;
		}
		memcpy(streclist.rec,rectmp,strlen(rectmp));
		strlen(rectmp)>reclen ? reclen=strlen(rectmp) : reclen;
		reclist_add(prec_head,&streclist);
		num++;
	}
	fclose(fp);
	return num;
}

Tab2Space( char * str )
{
	int i=0,j=0;
	int n;
	char acTmpStr[MAXRECLEN + 1] ;
	memset(acTmpStr,0x00,sizeof(acTmpStr)) ;
	for	(i=0;str[i]!=0x00;i++ )
	{
		switch(str[i])
		{
			case '\t':
				for ( n = 0 ;n < 4 ; n++ )
					acTmpStr[j++] = ' ' ;
				break ;
			case '\n':
				acTmpStr[j++] = 0x00 ;
				break ;
			default:
				acTmpStr[j++] = str[i] ;
		}
	}
	acTmpStr[j] = 0x00 ;
	strcpy(str,acTmpStr) ;
	return 0;
}

void inputpanel(PANEL *mainpanel,char *str,char *lablename,int lablelen)
{
	WINDOW *mainwindow;
	WINDOW *window;
	PANEL *panel;
	int subline=INPUTPANEL_LINE;
	int subclos=INPUTPANEL_CLOS;
	int begx;
	int begy;
	int maxx;
	int maxy;
	int subx;
	int suby;
	mainwindow=	panel_window(mainpanel);
	getbegyx(mainwindow,begy,begx);
	getmaxyx(mainwindow,maxx,maxy);
	subx=maxx-begx;
	suby=maxy-begy;
	printf("%d,%d\n",subx,suby);

	window=derwin(mainwindow,subline,subclos,subx/2-subline/2,suby/2-subclos/2);
	panel=new_panel(window);
	wclear(window);
	top_panel(panel);
	mvwhline(window,subline/2+1,4,0,subclos-8);
	box(window,0,0);
	mvwaddstr(window,2,subclos/2-lablelen/2,lablename);
	update_panels();
	doupdate();

	echo();
	mvwgetstr(window,subline/2,5,str);
	noecho();
	del_panel(panel);
	delwin(window);
}

void docpanel(PANEL *mainpanel,char *docstr,int doclen)
{
	WINDOW *mainwindow;
	WINDOW *window;
	PANEL *panel;
	int subline=INPUTPANEL_LINE;
	int subclos=INPUTPANEL_CLOS;
	int begx;
	int begy;
	int maxx;
	int maxy;
	int subx;
	int suby;
	mainwindow=	panel_window(mainpanel);
	getbegyx(mainwindow,begy,begx);
	getmaxyx(mainwindow,maxx,maxy);
	subx=maxx-begx;
	suby=maxy-begy;

	window=derwin(mainwindow,subline,subclos,subx/2-subline/2,suby/2-subclos/2);
	panel=new_panel(window);
	wclear(window);
	top_panel(panel);
	box(window,0,0);
	mvwaddstr(window,subline/2-1,subclos/2-doclen/2,docstr);
	update_panels();
	doupdate();
	halfdelay(20);
	wgetch(window);
	cbreak();
	del_panel(panel);
	delwin(window);
}

int Match2List(struct list_head *oldhead,struct list_head *newhead, char *matchstr)
{
	int num=0;
	struct reclist *preclist;
	struct reclist matchrec;
	RECLIST_EACH(preclist,oldhead)
	{
		if(strstr(preclist->rec,matchstr))
		{
			memset(&matchrec,0x00,sizeof(struct reclist));
			memcpy(&matchrec,preclist,sizeof(struct reclist));
			matchrec.rec=preclist->rec;
			reclist_add(newhead,&matchrec);
			num++;
		}
	}
	return num;
}

void showmatchlist(PANEL *mainpanel,struct list_head *prechead,int recnum)
{
	int wclos;
	int wline;
	int loop;
	int commond;
	int currcols=0;
	int currline=0;
	int begx;
	int begy;
	char showline[WCLOS4];
	struct reclist *preclist;
	
	wclos=WCLOS4;
	wline=WLINE4;

	WINDOW *mainwindow=panel_window(mainpanel);
	getbegyx(mainwindow,begy,begx);
	WINDOW *window=newwin(wline,wclos,begy+3,begx+5);
	PANEL *panel=new_panel(window);
	

	keypad(window,TRUE); /**< Function Keys model */
	noecho();			 /**< No echo model */
	wclear(window);      /**< clear screen*/
	
	memset(atail,0x00,sizeof(atail));
	strcpy(atail,"    ");
	strcat(atail,htail);

	do
	{
		memset(showline,0,sizeof(showline));
		strncpy(showline,taitle+currcols,sizeof(showline));
		mvwaddstr(window,1,1,showline);
		loop=0;
		RECLIST_EACH(preclist,prechead)
		{
			if(loop>=currline)
			{
				memset(showline,0,sizeof(showline));
				memcpy(showline,(preclist->rec)+currcols,wclos-2);
				dealchinese(preclist->rec,showline,currcols);
				mvwaddstr(window,loop+3-currline,1,showline);
				if((loop+5-currline) > (wline-3))
				{
					break;
				}
			}
			loop++;
		}
		while((loop+5-currline) <= (wline-3))
		{
			memset(showline,' ',sizeof(showline));
			mvwaddstr(window,loop+3-currline,1,showline);
			loop++;
		}
		mvwhline(window,2,1,0,wclos);
		mvwhline(window,wline-3,1,0,wclos);
		mvwaddstr(window,wline-2,1,atail);
		box(window,0,0);
		top_panel(panel);
		update_panels();
		doupdate();

		if(getcommond(panel,prechead,&currcols,&currline,recnum,wline-5,wclos)<0)
			break;

	}while(1);
	del_panel(panel);
	delwin(window);
}

int getcommond(PANEL *panel,struct list_head *prechead,int *currcols,int *currline,int recnum,int maxline,int maxclos)
{
	int commond;
	int matchrecnum;
	char findbuf[64];
	WINDOW *window=panel_window(panel);
	commond=wgetch(window);
	switch(commond)
	{
		case 'K':
		case 'k':
		case KEY_UP:
			if((*currline)>0)
			{
				(*currline)--;
			}
			//memset(tail,0x00,sizeof(tail));
			strcpy(atail,"向上");
			strcat(atail,htail);
			break;
		case 'J':
		case 'j':
		case KEY_DOWN:
			if((*currline<recnum-maxline))
			{
				(*currline)++;
			}
			//memset(tail,0x00,sizeof(tail));
			strcpy(atail,"向下");
			strcat(atail,htail);
			break;
		case 'L':
		case 'l':
		case KEY_RIGHT:
			if((*currcols)<reclen-maxclos)
			{
				(*currcols)++;
			} 
			//memset(tail,0x00,sizeof(tail));
			strcpy(atail,"向右");
			strcat(atail,htail);
			break;
		case 'H':
		case 'h':
		case KEY_LEFT:
			if((*currcols)>0)
			{
				(*currcols)--;
			}
			//memset(tail,0x00,sizeof(tail));
			strcpy(atail,"向左");
			strcat(atail,htail);
			break;
		case 't':
		case 'T':
			(*currline)=0;
			//memset(tail,0x00,sizeof(tail));
			strcpy(atail,"向上");
			strcat(atail,htail);
			break;
		case 'b':
		case 'B':
			(*currline)=recnum-maxline;
			//memset(tail,0x00,sizeof(tail));
			strcpy(atail,"向下");
			strcat(atail,htail);
			break;
		case 'a':
		case 'A':
			(*currcols)=0;
			//memset(tail,0x00,sizeof(tail));
			strcpy(atail,"向左");
			strcat(atail,htail);
			break;
		case 'e':
		case 'E':
			(*currcols)=reclen-maxclos;
			//memset(tail,0x00,sizeof(tail));
			strcpy(atail,"向右");
			strcat(atail,htail);
			break;
		case 'q':
		case 'Q':
			return -1;
		case '/':
			
			inputpanel(panel,findbuf,"输入查询条件",12);
			if(!findbuf[0])
				break;
			LIST_HEAD(match_rec_list);
			if(!(matchrecnum=Match2List(prechead,&match_rec_list,findbuf)))
			{
				docpanel(panel,"找不到匹配记录",14);
				break;
			}
			else
			{
				showmatchlist(panel,&match_rec_list,matchrecnum);
				break;
			}
	}
	return 0;
}

int ischinese(unsigned ch)
{
	if (ch >= 0xa0) return 1;
	else return 0;
}

void dealchinese(char *wholeline,char *showline,int currcols)
{
	int i;
	int iChinese;
	iChinese = 0;
	if (ischinese((unsigned char) showline[0]) )
	{
		for (i=currcols;i>=0;i--)
		{
			if (!ischinese((unsigned char) wholeline[i])) break;
			else iChinese++;
		}
		if (iChinese%2==0) showline[0]=' ';	
	}
/*	if (ischinese((unsigned char) showline[sizeof(showline)]) )
	{
		for (i=MAXRECLEN;i>=currcols;i--)
		{
			if (!ischinese((unsigned char) wholeline[i])) break;
			else iChinese++;
		}
		if (iChinese%2==0) showline[sizeof(showline)]=' ';	
	}*/
}

void showlist(struct list_head *prechead,int recnum,PANEL *panel)
{
	int wclos;
	int wline;
	int loop;
	int commond;
	int currcols=0;
	int currline=0;
	char showline[WCLOS4];
	struct reclist *preclist;

	wclos=WCLOS4;
	wline=WLINE4;

	WINDOW *window=panel_window(panel);

	keypad(window,TRUE); /**< Function Keys model */
	noecho();			 /**< No echo model */
	wclear(window);      /**< clear screen*/
	
	strcpy(atail,"    ");
	strcat(atail,htail);

	do
	{
		memset(showline,0,sizeof(showline));
		strncpy(showline,taitle+currcols,sizeof(showline));
		mvwaddstr(window,1,1,showline);
		loop=0;
		RECLIST_EACH(preclist,prechead)
		{
			if(loop>=currline)
			{
				memset(showline,0,sizeof(showline));
				memcpy(showline,(preclist->rec)+currcols,sizeof(showline));
				dealchinese(preclist->rec,showline,currcols);
				mvwaddstr(window,loop+3-currline,1,showline);
				if((loop+5-currline) > (wline-3))
				{
					break;
				}
			}
			loop++;
		}
		while((loop+5-currline) <= (wline-3))
		{
			memset(showline,' ',sizeof(showline));
			mvwaddstr(window,loop+3-currline,1,showline);
			loop++;
		}
		mvwhline(window,2,1,0,wclos);
		mvwhline(window,wline-3,1,0,wclos);
		mvwaddstr(window,wline-2,1,atail);
		box(window,0,0);
		update_panels();
		doupdate();

		if(getcommond(panel,prechead,&currcols,&currline,recnum,wline-5,wclos)<0)
			break;

	}while(1);
}

int main(int argc, char const *argv[])
{
	if(argc!=2)
	{
		printf("Bad arguments\nUsage $1 <filename>\n");
		return -1;
	}
	int recnum;
	int wclos;
	int wline;
	WINDOW *mainwindow;
	PANEL *mainpanel;

	wclos=WCLOS4;
	wline=WLINE4;
	/** Create list head */
	LIST_HEAD(rec_head);

	/** Load file in list */
	recnum=LoadF2List((char *)argv[1],&rec_head);

	sprintf(htail,"    Q退出     T最上     B最下     A最左     E最右     /查找               %s",argv[1]);

	/** init screen */
	initscr();

	/** Create mainwindow */
	mainwindow=newwin(wline,wclos,0,0);
	mainpanel=new_panel(mainwindow);
	/** Show list in pad */
	showlist(&rec_head,recnum,mainpanel);

	del_panel(mainpanel);
	delwin(mainwindow);

	endwin();
	return 0;
}
