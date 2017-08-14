/********************************************************************
    created:   2010/12/8
    filename:  codeConv.c
    author:    付强
    depend:    iconv.lib
    build:
    purpose:   提供从UTF-8到GB2312的内码转换，以及反向的转换

*********************************************************************/
#include <iconv.h>
#include <string.h>
#include "pub.h"
/*代码转换:从一种编码转为另一种编码*/
int code_convert(char* from_charset, char* to_charset, char* inbuf,
               int inlen, char* outbuf, int outlen)
{
    iconv_t cd;
    char** pin = &inbuf;
    char** pout = &outbuf;
    cd = iconv_open(to_charset,from_charset);
    if(cd == 0)
    {
      printf("iconv_open error\n");
       return -1;
    }
    memset(outbuf,0,outlen);
    if(iconv(cd,(char**)pin,(size_t*)&inlen,pout,(size_t*)&outlen)
       ==(size_t)-1)
    {
      printf("iconv error[%d]\n",errno);
       return -1;
    }

    iconv_close(cd);
    printf("dsdsd[%s][%s]\n,",inbuf,*pout);
    return 0;
}
/*UNICODE码转为GB2312码*/
/*成功则返回一个动态分配的char*变量，需要在使用完毕后手动free，失败返回NULL*/
/*
char* u2g(char *inbuf)
{
    int nOutLen = 2 * strlen(inbuf);
    char* szOut = (char*)malloc(nOutLen);
    if (-1 == code_convert("UTF-8","GB2312",inbuf,strlen(inbuf),szOut,nOutLen))
    {
      printf("code convert error\n");
       free(szOut);
       szOut = NULL;
    }
    return szOut;
}*/
/*GB2312码转为UNICODE码*/
/*成功则返回一个动态分配的char*变量，需要在使用完毕后手动free，失败返回NULL*/
/*
char* g2u(char *inbuf)
{
    int nOutLen = 2 * strlen(inbuf);
    char* szOut = (char*)malloc(nOutLen);
    if (-1 == code_convert("GB2312","UTF-8",inbuf,strlen(inbuf),szOut,nOutLen))
    {
      printf("code convert g2u error\n");
       free(szOut);
       szOut = NULL;
    }
    return szOut;
}
*/
/*UNICODE码转为GB2312码 */
/*int u2g(char *inbuf,int inlen,char *outbuf,int outlen)
{
    return code_convert("utf-8","gb2312",inbuf,inlen,outbuf,outlen);
}*/
/*GB2312码转为UNICODE码 */
int g2u(char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
    return code_convert("gb2312","utf-8",inbuf,inlen,outbuf,outlen);
}
int code2u(char *inbuf,int inlen,char** outbuf,char* code)
{
  printf("*****************\n");
    char *buf=(char*)malloc(2*inlen);
        printf("size[%d]\n",inlen);

    code_convert(code,"utf-8",inbuf,inlen,buf,2*inlen);
    printf("2u[%s]\n",buf);
    *outbuf=buf;
    return 0;
}

int u2code(char *inbuf,int inlen,char** outbuf,char* code)
{
  printf("*****************\n");
    char *buf=(char*)malloc(2*inlen);
        printf("size[%d]\n",inlen);

    code_convert("UTF-8",code,inbuf,inlen,buf,2*inlen);
    *outbuf=buf;
    return 0;
}

int u2g(char *inbuf,int inlen,char* outbuf,int outlen)
{
   printf("inbuf[%s]inlen[%d]outbuf[%s]outlen[%d]\n",inbuf,inlen,outbuf,outlen);
  iconv_t cd;
  cd = iconv_open("GB2312","UTF-8");
  if(cd == 0)
  {
    printf("iconv_open error\n");
       return -1;
  }
  if(iconv(cd,&inbuf,(size_t*)&inlen,&outbuf,(size_t*)&outlen)==(size_t)-1)
  {
    printf("iconv error[%d]\n",errno);
    return -1;
  }

  iconv_close(cd);
  printf("inbuf[%s]inlen[%d]outbuf[%s]outlen[%d]\n",inbuf,inlen,outbuf,outlen);
}
