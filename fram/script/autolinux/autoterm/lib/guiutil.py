import os
import sys
import abc
import ldtp
import ldtputils

import logutil
import keyutil
import sysconf
from CaseException import *

class AutoGuiBase():
	__metaclass__=abc.ABCMeta

	@abc.abstractmethod
	def appopen(self):pass

	@abc.abstractmethod
	def appclose(self):pass	




	@abc.abstractmethod
	def keytext(self,string):pass
	
	@abc.abstractmethod
	def keyclick(self,keys=[]):pass

	@abc.abstractmethod
	def keypress(self,keys=[]):pass

	@abc.abstractmethod
	def keyrelease(self,keys=[]):pass


	
	@abc.abstractmethod
	def mouseclickright(self,objectname,windowname=None):pass
		
	@abc.abstractmethod
	def mouseclickleft(self,objectname,windowname=None):pass

	@abc.abstractmethod
	def mousemove(self,objectname,windowname=None):pass


	@abc.abstractmethod
	def menuselect(self,menuname,windowname=None):pass



	@abc.abstractmethod
	def imagecapture(self,windowname,x=0,y=0,width=None,height=None):pass

	


#self.window is dirt :
#{
#	'name'=''
#	'x'=''
#	'y'=''
#	'width'=''
#	'height'=''
#	'child'=[{
#				'name'=''
#				'x'=''
#				'y'=''
#				'width'=''
#				'height'=''
#				'child'=[{}]
#			},
#			{
#				'name'=''
#				'x'=''
#				'y'=''
#				'width'=''
#				'height'=''
#				'child'=[{}]
#			}
#			]
#}
class BindLdtpGUI(AutoGuiBase):
	def __init__(self,cmd,argument=[]):
		print(sysconf.getsysconf())
		self.log=logutil.ULogger('Ldtp')
		self.cmd=cmd
		self.argument=argument
		self.apppid=None
		self.window={}

	def appopen(self):
		WindowList=ldtp.getwindowlist()
		WindowNum=len(WindowList)
		try:
			ret=ldtp.launchapp(self.cmd,self.argument)
		except ldtp.LdtpExecutionError as e:
			self.log.error(repr(e))
			raise GuiFrmException(repr(e))
		ldtp.wait(1)
		WindowList1=ldtp.getwindowlist()
		WindowNum1=len(WindowList1)
		if WindowNum1 != (WindowNum+1):
			self.log.error('Open application window number error before['+str(WindowNum)+'] after ['+str(WindowNum1)+']')
			raise GuiFrmException('Open application window number error before['+str(WindowNum)+'] after ['+str(WindowNum1)+']')
		self.window['name']=WindowList1[WindowNum1-1]
		windowposition=ldtp.getwindowsize(self.window['name'])
		self.window['x']=windowposition[0]
		self.window['y']=windowposition[1]
		self.window['width']=windowposition[2]
		self.window['height']=windowposition[3]
		self.log.info('Open application ['+self.cmd+' '+' '.join(self.argument)+'],Main window name ['+self.window['name']+']')
		self.apppid=ret

	def appclose(self):
		ret=ldtp.closewindow(self.window['name'])
		self.log.info('Window['+self.window['name']+'] closed')
		return ret

	def keytext(self,string):
		keylist=keyutil.UKeyBoardLdtp().getlist()
		newstr=''
		for char in string:
			if ord(char)>128:
				self.log.error('Unknow character ['+char+']')
				raise GuiFrmException('Unknow character ['+char+']')
			newstr+=keylist[ord(char)]
		ldtp.generatekeyevent(string)

	def keyclick(self,keylist=[]):
		try:
			for key in keylist:
				ldtp.keypress('<'+key.lower()+'>')
				ldtp.keyrelease('<'+key.lower()+'>')
		except Exception as e:
				self.log.error(repr(e))
				raise GuiFrmException(repr(e))
		
	def keypress(self,key):
		try:
			ldtp.keypress('<'+key.lower()+'>')
		except Exception as e:
			self.log.error(repr(e))
			raise GuiFrmException(repr(e))

	def keyrelease(self,key):
		try:
			ldtp.keyrelease('<'+key.lower()+'>')
		except Exception as e:
			self.log.error(repr(e))
			raise GuiFrmException(repr(e))

	def mouseclickright(self,objectname,windowname=None):
		if not windowname:
			windowname=self.window['name']
		try:
			ldtp.mouserightclick(windowname,objectname)
		except LdtpExecutionError as e:
			self.log.error(repr(e))
			raise GuiFrmException(repr(e))

	def mouseclickleft(self,objectname,windowname=None):
		if not windowname:
			windowname=self.window['name']
		try:
			ldtp.mouseleftclick(windowname,objectname)
		except LdtpExecutionError as e:
			self.log.error(repr(e))
			raise GuiFrmException(repr(e))

	def mousemove(self,objectname,windowname=None):
		if not windowname:
			windowname=self.window['name']
		try:
			ldtp.mousemove(windowname,objectname)
		except LdtpExecutionError as e:
			self.log.error(repr(e))
			raise GuiFrmException(repr(e))
	
	def menuselect(self,menuname,windowname=None):
		if not windowname:
			windowname=self.window['name']
		menulist=menuname.split(':')
		newmenulist=''
		for menuitem in menulist:
			newmenulist=newmenulist+'mnu'+menuitem+';'
		newmenulist=newmenulist[0:len(newmenulist)-1]
		try:
			ldtp.selectmenuitem(windowname,newmenulist)
		except Exception as e:
			self.log.error(repr(e))
			raise GuiFrmException(repr(e))

	def imagecapture(self,outfile,windowname=None,x=0,y=0,width=None,height=None):
		if not windowname:
			windowname=self.window['name']
		try:
			ldtp.activatewindow(windowname)
			ldtp.imagecapture(windowname,outfile,x,y,width,height,)
		except LdtpExecutionError as e:
			self.log.error(repr(e))
			raise GuiFrmException(repr(e))


#class BindDogtailGUI(AutoGuiBase):pass


if __name__=='__main__':
	import guiutil
	c=sys.modules['guiutil']
	b=getattr(c,'BindLdtpGUI')
	a=b('gnome-terminal',['--geometry','74X24'])
	#a=BindLdtpGUI('gnome-terminal',['--geometry','74X24'])
	#a=BindLdtpGUI('gedit')
	#a=BindLdtpGUI('agedit')
	a.appopen()
	a.keytext('AS!@K{asd2810')
	print(a.window)
	a.imagecapture('~/grh.png')
	#a.appclose()

	 


	
