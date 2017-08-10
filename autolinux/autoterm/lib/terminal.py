import os
import sys
import abc
import gettext

import logutil
import guiutil
import sysconf

from CaseException import *

#self.args :
#{
#'guiframe'=''
#'commond'=''
#'args'=''
#'language'=''
#'languagepackage'=''
#'packagepath'=''
#}
class Terminal:
	__metaclass__=abc.ABCMeta
	@abc.abstractmethod
	def open(self):pass
	@abc.abstractmethod
	def close(self):pass
	@abc.abstractmethod
	def execute(self,cmd):pass
	@abc.abstractmethod
	def clear(self):pass


class Gnome_Terminal(Terminal):
	def __init__(self,**kwargs):
		self.log=logutil.ULogger('Terminal')
		self.args={}
		if not kwargs.has_key('guiframe'):
			self.args['guiframecls']='BindLdtpGUI'
		else:
			self.args['guiframecls']=kwargs['guiframecls']
		if not kwargs.has_key('commond'):
			self.args['terminalcmd']='gnome-terminal'
		else:
			self.args['terminalcmd']=kwargs['terminalcmd']
		if kwargs.has_key('terminalargs'):
			self.args['terminalargs']=kwargs['terminalargs']
		try:
			self.args['language']=kwargs['language']
			self.args['languagepackage']=kwargs['languagepackage']
			self.args['languagepackagepath']=kwargs['languagepackagepath']
		except Exception as e:
			self.log.error(repr(e))
			raise TerminalException(repr(e))

		try:
			guiclass=getattr(guiutil,self.args['guiframecls'])
			if self.args.has_key('terminalargs'):
				self.gui=guiclass(self.args['terminalcmd'],self.args['terminalargs'])
			else:
				self.gui=guiclass(self.args['terminalcmd'])
		except Exception as e:
			self.log.error('Instance guiclass['+self.args['guiframecls']+'] error ['+repr(e)+']')
			raise TerminalException(repr(e))

		try:
			gettext.install(self.args['languagepackage'],self.args['languagepackagepath'],unicode=False)
			gettext.translation(self.args['languagepackage'],self.args['languagepackagepath'],languages=[self.args['language']]).install(True)
		except Exception as e:
			self.log.error('Language package init error ['+repr(e)+']')
			raise TerminalException(repr(e))
		self.log.info('Terminal with arguments['+repr(self.args)+'] init succ')

	def open(self):
		try:
			self.gui.appopen()
		except GuiFrmException as e:
			self.log.error('Open termianl error['+repr(e)+']')
			raise TerminalException(repr(e))

	def close(self):
		try:
			menuitem=_('_File')+':'+_('C_lose Terminal')
			self.gui.menuselect(menuitem)
		except Exception as e:
			self.log.error(repr(e))
			raise TerminalException(repr(e))
	
	def execute(self,cmd):
		try:
			self.gui.keytext(cmd)
			self.gui.keyclick(['enter'])
		except Exception as e:
			self.log.error(repr(e))
			raise TerminalException(repr(e))

	def clear(self):
		try:
			self.execute('clear')
		except:
			self.log.error(repr(e))
			raise


#class Xterm_Terminal(Terminal):pass


if __name__ == '__main__':
	a=Gnome_Terminal(language='en_AU',languagepackage='gnome-terminal',languagepackagepath='/usr/share/locale-langpack')
	a.open()
	a.execute('ls -la')
	a.clear()
	a.close()

