import os
import sys
import json
import logging
import logging.handlers

import sysconf
from CaseException import *


def singleton(cls, *args, **kw):  
    instances = {}  
    def _singleton():  
        if cls not in instances:  
            instances[cls] = cls(*args, **kw)  
        return instances[cls]  
    return _singleton  

@singleton
class ULogBase:
	def _basicconfig(self,CfgFileName):
		with open(CfgFileName) as fd:
			logdirt=json.load(fd)
		if not logdirt.has_key('basic'):
			raise LogException('Basic item not exist!')
		kwargs={}
		#log basic config
		if not logdirt['basic'].has_key('type'):
			raise LogException('Basic item attr type not exist!')
		if not cmp(logdirt['basic']['type'],'file'):
			if not logdirt['basic'].has_key('path'):
				raise LogException('Basic item attr type is file but path not exist!')
			else:
				kwargs['filename']=os.path.join(sysconf.getsysconf()['basedir'],'log/'+logdirt['basic']['path'])
		elif not cmp(logdirt['basic']['type'],'stream'):
			if not logdirt['basic'].has_key('object'):
				raise LogException('Basic item attr type is stream but object not exist!')
			else:
				if not cmp(logdirt['basic']['object'],'STDERR'):
					kwargs['stream']=logging.StreamHandler([sys.stderr])
				elif not cmp(logdirt['basic']['object'],'STDOUT'):
					kwargs['stream']=logging.StreamHandler([sys.stdout])
				elif not cmp(logdirt['basic']['object'],'STDIN'):
					kwargs['stream']=logging.StreamHandler([sys.stdin])
				else:
					raise LogException('Unknown stream object!')
		else:
			raise LogException('Unknown handler type ['+logdirt['basic']['type']+']')
		if logdirt['basic'].has_key('filemode'):
			kwargs['filemode']=logdirt['basic']['filemode']
		if logdirt['basic'].has_key('format'):
			kwargs['format']=logdirt['basic']['format']
		if logdirt['basic'].has_key('datefmt'):
			kwargs['datefmt']=logdirt['basic']['datefmt']
		if logdirt['basic'].has_key('level'):
			if not cmp(logdirt['basic']['level'],'DEBUG'):
				kwargs['level']=logging.DEBUG
			elif not cmp(logdirt['basic']['level'],'INFO'):
				kwargs['level']=logging.INFO
			elif not cmp(logdirt['basic']['level'],'WARNING'):
				kwargs['level']=logging.WARNING
			elif not cmp(logdirt['basic']['level'],'ERROR'):
				kwargs['level']=logging.ERROR
			elif not cmp(logdirt['basic']['level'],'CRITICAL'):
				kwargs['level']=logging.CRITICAL
			else:
				raise LogException('Unknown level ['+logdirt['basic']['level']+']')
		logging.basicConfig(**kwargs)

	def _loggerconfig(self,CfgFileName):
		with open(CfgFileName) as fd:
			logdirt=json.load(fd)
		if not logdirt.has_key('logger'):
			return
		else:
			for logger in logdirt['logger']:
				if not logger.has_key('name') or not logger.has_key('handler'):
					raise LogException('Logger attr name or handler not exists!')
				else:
					self._logger.append(logger['name'])
					userlogger=logging.getLogger(logger['name'])
					if logger.has_key('level'):
						if not cmp(logger['level'],'DEBUG'):
							userlogger.setLevel(logging.DEBUG)
						elif not cmp(logger['level'],'INFO'):
							userlogger.setLevel(logging.INFO)
						elif not cmp(logger['level'],'WARNING'):
							userlogger.setLevel(logging.WARNING)
						elif not cmp(logger['level'],'ERROR'):
							userlogger.setLevel(logging.ERROR)
						elif not cmp(logger['level'],'CRITICAL'):
							userlogger.setLevel(logging.GRITICAL)
						else:
							raise LogException('Unknown level ['+logger['level']+']')
					for handler in logger['handler']:
						if not handler.has_key('type'):
							raise LogException('Logger ['+logger['name']+'] handler item type attr not exists!')
						if not cmp(handler['type'],'file'):
							if not handler.has_key('path'):
								raise LogException('Logger ['+logger['name']+'] handler item type is file but path not exist!')
							else:
								if handler.has_key('filesize'):
									filesize=handler['filesize']
								else:
									filesize=1024*1000*10
								if handler.has_key('filenum'):
									filenum=handler['filenum']
								else:
									filenum=5
								filename=os.path.join(sysconf.getsysconf()['basedir'],'log/'+handler['path'])
								filehandler=logging.handlers.RotatingFileHandler(filename,maxBytes=filesize,backupCount=filenum)
						elif not cmp(handler['type'],'stream'):
							if not handler.has_key('object'):
								raise LogException('Logger ['+logger['name']+'] handler item attr type is stream but object not exist!')
							else:
								if not cmp(handler['object'],'STDERR'):
									filehandler=logging.StreamHandler(sys.stderr)
								elif not cmp(handler['object'],'STDOUT'):
									filehandler=logging.StreamHandler(sys.stdout)
								elif not cmp(handler['object'],'STDIN'):
									filehandler=logging.StreamHandler(sys.stdin)
								else:
									raise LogException('Unknown stream object!')
						else:
							raise LogException('Unknown stream object!')
						if handler.has_key('format') and handler.has_key('datefmt'):
							formatter=logging.Formatter(fmt=handler['format'],datefmt=handler['datefmt'])
						elif handler.has_key('format'):
							formatter=logging.Formatter(fmt=handler['format'])
						elif handler.has_key('datefmt'):
							formatter=logging.Formatter(datefmt=handler['datefmt'])
						else:
							formatter=logging.Formatter()
						filehandler.setFormatter(formatter)
						formatter=None
						if handler.has_key('filter'):
							for ufilter in handler['filter']:
								userfilter=logging.Filter(ufilter)
								filehandler.addFilter(userfilter)
								userfilter=None
						userlogger.addHandler(filehandler)
						filehandler=None
					userlogger=None

	def __init__(self,CfgFileName='./etc/log.json'):
		if not os.path.exists(CfgFileName):
			raise LogException('Config File ['+CfgFileName+'] not exist!')
		self._logger=[]
		try:
			self._basicconfig(CfgFileName)
			self._loggerconfig(CfgFileName)
		except LogException:
			raise
		self.rootlogger=logging.getLogger()

	def haslogger(self,name):
		if name in self._logger:
			return 1
		else:
			return 0

	def debug(self,msg):
		self.rootlogger.debug(msg)

	def info(self,msg):
		self.rootlogger.info(msg)

	def warning(self,msg):
		self.rootlogger.warning(msg)

	def error(self,msg):
		self.rootlogger.error(msg)

	def critical(self,msg):
		self.rootlogger.critical(msg)



class ULogger:
	def __init__(self,name=None):
		try:
			logbase=ULogBase()
		except LogException:
			raise
		if not name:
			self.logger=logging.getLogger()
		else:
			if logbase.haslogger(name):
				self.logger=logging.getLogger(name)
			else:
				raise LogException('Logger ['+name+'] not exists!')

	def debug(self,msg):
		filename=sys._getframe().f_back.f_code.co_filename
		lineno=sys._getframe().f_back.f_lineno
		self.logger.debug('File['+filename+'] Line['+str(lineno)+'] '+msg)

	def info(self,msg):
		filename=sys._getframe().f_back.f_code.co_filename
		lineno=sys._getframe().f_back.f_lineno
		self.logger.info('File['+filename+'] Line['+str(lineno)+'] '+msg)
	
	def warning(self,msg):
		filename=sys._getframe().f_back.f_code.co_filename
		lineno=sys._getframe().f_back.f_lineno
		self.logger.warning('File['+filename+'] Line['+str(lineno)+'] '+msg)

	def error(self,msg):
		filename=sys._getframe().f_back.f_code.co_filename
		lineno=sys._getframe().f_back.f_lineno
		self.logger.error('File['+filename+'] Line['+str(lineno)+'] '+msg)

	def critical(self,msg):
		filename=sys._getframe().f_back.f_code.co_filename
		lineno=sys._getframe().f_back.f_lineno
		self.logger.critical('File['+filename+'] Line['+str(lineno)+'] '+msg)



