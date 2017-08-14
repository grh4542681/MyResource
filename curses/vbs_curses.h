/**********************************************************************
    程序名:vost.h		
	NOTE: This file should be 615 lines.
**********************************************************************/

#ifndef _VOST_H_
#define _VOST_H_

#include <curses.h>

/************************* 宏定义 *************************/

#define  SQLCODE 	sqlca.sqlcode

#ifdef	 VOST_DB2
#define	 SQLNAME 				sqlname.data
typedef struct sqlvar			SQLVAR ;
#else
#define	 SQLNAME 				sqlname
typedef struct sqlvar_struct 	SQLVAR ;
#endif

#define SFIEDNUM 100
#define SELENUM 21
#define MAXSE 16

#define NOEDIT  1
#define NOCREA  2
#define RCHECK  4
#define RPG     8
#define NOBOX   16
#define TBSELE  32
#define ENTAB   64
#define RNUMSH  128

#define  DBTYPE_CHAR		0
#define  DBTYPE_SMALL		1
#define  DBTYPE_INT			2
#define  DBTYPE_FLOAT		3
#define  DBTYPE_DECIMAL		4
#define  DBTYPE_DATE		5

extern 	int		puvalnum;			/* 输入值对应公用数据变量下标 */
extern 	char	*sval_ptr; 			/* 输入值缓冲区指针 		  */

extern int SELELINE;
extern int MAXSLNUM;

/* 键盘按键定义 */
#define KBD_EXT_BASE	0x0100 

/* IBM键盘按键定义 */
#define KBD_IBM_BASE	KBD_EXT_BASE		/* ESC序列扩展按键基值 */

#define KBD_UP			(KBD_IBM_BASE+'A')
#define KBD_DOWN		(KBD_IBM_BASE+'B')
#define KBD_RIGHT		(KBD_IBM_BASE+'C')
#define KBD_LEFT		(KBD_IBM_BASE+'D')

#define KBD_PAD5		(KBD_IBM_BASE+'E')

#define KBD_END			(KBD_IBM_BASE+'F')
#define KBD_PGDN		(KBD_IBM_BASE+'G')
#define KBD_HOME		(KBD_IBM_BASE+'H')
#define KBD_PGUP		(KBD_IBM_BASE+'I')

#define KBD_INS			(KBD_IBM_BASE+'L')

#define KBD_F1			(KBD_IBM_BASE+'M')
#define KBD_F2			(KBD_IBM_BASE+'N')
#define KBD_F3			(KBD_IBM_BASE+'O')
#define KBD_F4			(KBD_IBM_BASE+'P')
#define KBD_F5			(KBD_IBM_BASE+'Q')
#define KBD_F6			(KBD_IBM_BASE+'R')
#define KBD_F7			(KBD_IBM_BASE+'S')
#define KBD_F8			(KBD_IBM_BASE+'T')
#define KBD_F9			(KBD_IBM_BASE+'U')
#define KBD_F10			(KBD_IBM_BASE+'V')
#define KBD_F11			(KBD_IBM_BASE+'W')
#define KBD_F12			(KBD_IBM_BASE+'X')

#define KBD_SHT_F1		(KBD_IBM_BASE+'Y')
#define KBD_SHT_F2		(KBD_IBM_BASE+'Z')
#define KBD_SHT_F3		(KBD_IBM_BASE+'a')
#define KBD_SHT_F4		(KBD_IBM_BASE+'b')
#define KBD_SHT_F5		(KBD_IBM_BASE+'c')
#define KBD_SHT_F6		(KBD_IBM_BASE+'d')
#define KBD_SHT_F7		(KBD_IBM_BASE+'e')
#define KBD_SHT_F8		(KBD_IBM_BASE+'f')
#define KBD_SHT_F9		(KBD_IBM_BASE+'g')
#define KBD_SHT_F10		(KBD_IBM_BASE+'h')
#define KBD_SHT_F11		(KBD_IBM_BASE+'i')
#define KBD_SHT_F12		(KBD_IBM_BASE+'j')

#define KBD_CTL_A		1
#define KBD_CTL_B		2
#define KBD_CTL_C		3
#define KBD_CTL_D		4
#define KBD_CTL_E		5
#define KBD_CTL_F		6
#define KBD_CTL_G		7
#define KBD_CTL_H		8
#define KBD_CTL_I		9
#define KBD_CTL_J		10
#define KBD_CTL_K		11
#define KBD_CTL_L		12
#define KBD_CTL_M		13
#define KBD_CTL_N		14
#define KBD_CTL_O		15
#define KBD_CTL_P		16
#define KBD_CTL_Q		17
#define KBD_CTL_R		18
#define KBD_CTL_S		19
#define KBD_CTL_T		20
#define KBD_CTL_U		21
#define KBD_CTL_V		22
#define KBD_CTL_W		23
#define KBD_CTL_X		24
#define KBD_CTL_Y		25
#define KBD_CTL_Z		26

/* DEC键盘按键定义 */
#define KBD_DEC_BASE		0x0200 /* DEC 序列按键基值 */

#define KBD_PF1			(KBD_DEC_BASE+'P')
#define KBD_PF2			(KBD_DEC_BASE+'Q')
#define KBD_PF3			(KBD_DEC_BASE+'R')
#define KBD_PF4			(KBD_DEC_BASE+'S')
#define KBD_PF5			(KBD_DEC_BASE+'t')
#define KBD_PF6			(KBD_DEC_BASE+'u')
#define KBD_PF7			(KBD_DEC_BASE+'v')
#define KBD_PF8			(KBD_DEC_BASE+'l')
#define KBD_PF9			(KBD_DEC_BASE+'w')
#define KBD_PF10		(KBD_DEC_BASE+'x')
#define KBD_PF11		(KBD_DEC_BASE+'y')
#define KBD_PF12		(KBD_DEC_BASE+'z')

/* 特殊按键定义 */
#define KBD_ESC			0x1B
#define KBD_DEL			0x7F
#define KBD_BS			0x08
#define KBD_TAB			0x09
#define KBD_CR			0x0D
#define KBD_LF			0x0A
#define KBD_ENTER		KBD_LF
#define KBD_RETURN		KBD_LF
#define KBD_SPACE		0x20
#define KBD_PAUSE		0x13

/* 颜色定义 */
#define SCR_BRIGHT_BASE		0x1000

#define SCR_BLACK		COLOR_BLACK	
#define SCR_RED			COLOR_RED
#define SCR_YELLOW		COLOR_YELLOW
#define SCR_GREEN		COLOR_GREEN
#define SCR_BLUE		COLOR_BLUE
#define SCR_MAGENTA		COLOR_MAGENTA
#define SCR_CYAN		COLOR_CYAN
#define SCR_WHITE		COLOR_WHITE
#define SCR_BRIGHT_BLACK	(COLOR_BLACK  +SCR_BRIGHT_BASE)	
#define SCR_BRIGHT_RED		(COLOR_RED    +SCR_BRIGHT_BASE)
#define SCR_BRIGHT_YELLOW	(COLOR_YELLOW +SCR_BRIGHT_BASE)
#define SCR_BRIGHT_GREEN	(COLOR_GREEN  +SCR_BRIGHT_BASE)
#define SCR_BRIGHT_BLUE		(COLOR_BLUE   +SCR_BRIGHT_BASE)
#define SCR_BRIGHT_MAGENTA	(COLOR_MAGENTA+SCR_BRIGHT_BASE)
#define SCR_BRIGHT_CYAN		(COLOR_CYAN   +SCR_BRIGHT_BASE)
#define SCR_BRIGHT_WHITE	(COLOR_WHITE  +SCR_BRIGHT_BASE)

#define scr_kbhit()						scr_wkbhit( stdscr )
#define scr_getch()						scr_wgetch( stdscr )
#define scr_gets(string,maxlen)			scr_wgets ( stdscr, string, maxlen )
#define scr_fetchchar()					scr_wfetchchar ( stdscr )
#define scr_fetchstr(str,num)			scr_wfetchstr  ( stdscr, str, num )
#define scr_box(row1,col1,row2,col2) 	scr_wbox(stdscr, row1, col1, row2, col2)
#define scr_line(row1,col1,row2,col2) 	scr_wline ( stdscr,row1,col1,row2, col2)
#define scr_clearstr(row,col,num)		scr_wclearstr (stdscr, row, col, num)
#define scr_clearblock(row1,col1,row2,col2) scr_wclearblock (stdscr,row1,col1,row2,col2);
#define scr_prompt(row,col,prompt)		scr_wprompt( stdscr, row, col, prompt )

/* 函数声明 */
int   scr_open 	 	  (void);
int   scr_close 	  (void);
int   scr_wkbhit	  (WINDOW *win);
int   scr_wgetch 	  (WINDOW *win);
int   scr_wgets 	  (WINDOW *win, char *string, int maxlen);
int   scr_wfetchchar  (WINDOW *win);
char *scr_wfetchstr   (WINDOW *win, char *str, int num);
int   scr_wbox 	 	  (WINDOW *win, int row1, int col1, int row2, int col2);
int   scr_wline	 	  (WINDOW *win, int row1, int col1, int row2, int col2);
int   scr_wclearstr   (WINDOW *win, int row, int col, int num);
int   scr_wclearblock (WINDOW *win, int row, int col, int rows, int cols);
int   scr_wprompt 	  (WINDOW *win, int row, int col, char * prompt );
void *scr_sethook();
int   SELELINE;



extern char *strpack( char * ) ;
void vost_show_helpbar ( char *prompt );
extern char *double_to_money ( char *result, double dbl );
extern int vost_test_date ( long date );

extern int vost_input_str  (char *value,int width,WINDOW *win,int row,int col);
extern int vost_input_int  (char *value,int width,WINDOW *win,int row,int col);
extern int vost_input_float(char *value,int width,int scale,WINDOW *win,
									int row,int col);
extern int vost_input_date (char *value,int width,WINDOW *win,int row,int col);
extern int vost_input_money(char *value,int width,WINDOW *win,int row,int col);
extern int vost_input_digit(char *value,int width,int interrupt,
									WINDOW *win, int row, int col);
extern int vost_input_pass (char *value,int width,WINDOW *win,int row,int col);


/* the following macros are for back-compatable, do not use them from now */

#define pack_funt 		run_transaction
#define get_select		select_bar


#define min(a,b)		((a)>(b) ? (b) : (a))
#define max(a,b)		((a)<(b) ? (b) : (a))


#define CONFIRM_KEY		'/'

struct tb_meg 
{
	char	tabname[21];	/*表名*/
	char	tabshna[21];	/*表显示名称*/
	char	filter[251];	/*记录筛选条件*/
	char	findex[41];		/*记录排序条件*/
	int		begfied;		/*起始显示字段序号*/
	int		topfied;		/*固定显示字段序号*/
	struct sqlda *fdata;	/*返回的当前记录值结构*/
	char *	fdatabuf;		/*记录值缓冲区指针*/
};

typedef int(*PF)(void*);

struct sh_tb 
{
    char 	fdna[21];
    char 	shfdna[21];
	PF 		infunc;
	PF 		outfunc;
	int		xgbz;
	char	help[71];
};

#endif

