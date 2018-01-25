import sys
import re
import socket
import json
import time
import string
import pexpect
from AppBase import *

sys.path.append(os.path.join(os.environ.get('AUTOBASEDIR'),'lib'))
import terminal
import sysconf
import logutil
from CaseException import *
def _sockacceptblock(sockfd,overtime=None):
	sockfd.setblocking(1)
	sockfd.settimeout(overtime)
	return sockfd.accept()

def _sockconnectblock(sockfd,sockfile,overtime=None):
	sockfd.setblocking(1)
	sockfd.settimeout(overtime)
	return sockfd.connect(sockfile)

def _socksendblock(acptfd,messagedict,overtime=None):
	acptfd.setblocking(1)
	acptfd.settimeout(overtime)
	messagebody=json.dumps(messagedict)
	messagelen=len(messagebody)
	messagehead="%08d" % messagelen
	message=messagehead+messagebody
	acptfd.sendall(message)

def _sockrecvblock(acptfd,overtime=None):
	acptfd.setblocking(1)
	acptfd.settimeout(overtime)
	recvlen=8
	messagehead=''
	while recvlen>0:
		recvstr=acptfd.recv(recvlen)
		messagehead+=recvstr
		recvlen=recvlen-len(recvstr)
	recvlen=string.atoi(messagehead,10)
	messagebody=''
	while recvlen>0:
		recvstr=acptfd.recv(recvlen)
		messagebody+=recvstr
		recvlen=recvlen-len(recvstr)
	return json.loads(messagebody)

class AppTerminal(AppBase):
	def __init__(self,caseid,**kwargs):
		self.log=logutil.ULogger('Terminal')
		self.basedir=sysconf.getsysconf()['basedir']
		self.tmpdir=os.path.join(self.basedir,'tmp')
		self.caseid=caseid
		self.capdir=os.path.join(self.basedir,'picture/'+self.caseid)

		if not os.path.exists(self.capdir):
			os.makedirs(self.capdir)
			self.log.info('Create picture dir ['+self.capdir+']')

		try:
			self.terminalcls=kwargs['app']['terminalcls']
		except Exception as e:
			self.log.error(repr(e))
			raise AppException(repr(e))
		
		if not kwargs['app'].has_key('failrerun'):
			self.rerun=True
		else:
			self.rerun=kwargs['app']['failrerun']

		self.terminalargs={}
		if not kwargs['app'].has_key('guiframecls'):
			self.terminalargs['guiframecls']='BindLdtpGUI'
		else:
			self.terminalargs['guiframecls']=kwargs['app']['guiframecls']
		if kwargs['app'].has_key('terminalargs'):
			self.terminalargs['terminalargs']=kwargs['app']['terminalargs']

		envlang=os.environ.get('LANG')
		envlanguage=(re.split('_|\.',envlang))[0]
		envarea=(re.split('_|\.',envlang))[1]
		if envlanguage.lower()==envarea.lower():
			self.terminalargs['language']=envlanguage
		elif envlanguage.lower()=='en' and envarea.lower()=='us':
			self.terminalargs['language']='en_GB'
		else:
			self.terminalargs['language']=envlanguage+'_'+envarea
		try:
			self.terminalargs['terminalcmd']=kwargs['app']['terminalcmd']
			self.terminalargs['languagepackage']=kwargs['app']['language']['package']
			self.terminalargs['languagepackagepath']=kwargs['app']['language']['path']
		except Exception as e:
			self.log.error(repr(e))
			raise AppException(repr(e))

		self.steps=[]
		try:
			self.steps=kwargs['steps']
		except Exception as e:
			self.log.error(repr(e))
			raise AppException(repr(e))
		self.sockfilename=os.path.join(self.tmpdir,self.caseid+'.sock')
		self.badfilename=os.path.join(self.tmpdir,self.caseid+'.bad')
		self.sock=None
		self.overtime=None
		self.stepindex=0
		self.log.info('AppTerminal with args['+repr(self.terminalargs)+'] init succ')

	def _socketinit(self):
		self.sock=socket.socket(socket.AF_UNIX,socket.SOCK_STREAM)
		if os.path.exists(self.sockfilename):
			os.unlink(self.sockfilename)
		self.sock.bind(self.sockfilename)
		self.sock.listen(1)

	def _sendmsg(self,acptfd,commond,msg={}):
		senddict={}
		senddict['messagehead']={}
		senddict['messagehead']['commond']=commond
		senddict['messagehead']['caseid']=self.caseid
		senddict['messagehead']['stepindex']=self.stepindex
		senddict['messagebody']=msg
		if not self.overtime:
			self.overtime=5
		_socksendblock(acptfd,senddict,self.overtime)

	def _steperrrecord(index):
		with open(self.badfilename) as f:
			f.write(str(index))
		
	def _runsteps(self,terminal):

		stepnum=0
		if os.path.exists(self.badfilename) and not self.rerun:
			with open(self.badfilename) as f:
				stepnumstr=f.readline()
				if not stepnumstr:
					stepnum=0
				else:
					stepnum=string.atoi(stepnumstr,10)
		else:
			stepnum=0
	
		self.log.info('Step['+str(stepnum)+'] will be first step')

		clientcmd='python '+os.path.join(os.path.join(self.basedir,'app'),'AppTerminal.py')+' '+self.caseid
		terminal.execute(clientcmd)
		self.log.info('case['+self.caseid+'] AppTerminal Client run!')
		
		try:
			acptfd,address=_sockacceptblock(self.sock,5)
		except Exception as e:
			raise AppException(repr(e))
#again

		while True:
			senddict={}
			try:
				sockdict=_sockrecvblock(acptfd,self.overtime)
			except Exception as e:
				acptfd.close()
				self.log.error('case['+self.caseid+'] AppTerminal server error['+repr(e)+']')
				raise AppException(repr(e))
			
			self.log.info('case['+self.caseid+'] AppTerminal Server got a connect')
			if not cmp(sockdict['messagehead']['commond'],'verify'):
				if cmp(sockdict['messagehead']['caseid'],self.caseid):
					acptfd.close()
					raise AppException('Unknow case['+sockdict['messagehead']['caseid']+'] accept!')
				senddict={}
				senddict['stepargs']=self.steps
				self._sendmsg(acptfd,'complete',senddict)
			elif not cmp(sockdict['messagehead']['commond'],'capture'):
				try:
					capindex=sockdict['messagebody']['capindex']
				except Exception as e:
					acptfd.close()
					self._steperrrecord(sockdict['messagehead']['stepindex'])
					raise AppException(repr(e))
				capfile=os.path.join(self.capdir,("%s%02d%s" % (self.caseid,capindex,'.png')))
				time.sleep(1)
				terminal.gui.imagecapture(capfile)
				self._sendmsg(acptfd,'complete')
			elif not cmp(sockdict['messagehead']['commond'],'clear'):
				terminal.clear()
				self._sendmsg(acptfd,'complete')
			elif not cmp(sockdict['messagehead']['commond'],'execbuiltin'):
				builtinstep=sockdict['messagebody']['builtinstep']
				terminal.execute(builtinstep['commond'])
				time.sleep(2)
				self._sendmsg(acptfd,'complete')
			elif not cmp(sockdict['messagehead']['commond'],'complete'):
				acptfd.close()
				break
			elif not cmp(sockdict['messagehead']['commond'],'error'):
				self._steperrrecord(sockdict['messagehead']['stepindex'])
				raise AppException('case['+self.caseid+'] step['+repr(sockdict['messagehead']['stepindex'])+'] error msg['+sockdict['messagebody']['errormsg']+']')
			else:
				acptfd.close()
				raise AppException('Recv unknow commond['+sockdict['messagehead']['commond']+']')
		

	def run(self,lock=None):
		self.log.info('case['+self.caseid+'] AppTerminal run!')
		m=sys.modules['terminal']
		terminalcls=getattr(m,self.terminalcls)
		try:
			self._socketinit()
		except Exception as e:
			self.log.error('Socket Init error['+repr(e)+']')
			raise AppException(repr(e))
		try:
			terminal=terminalcls(**self.terminalargs)
			terminal.open()
		except Exception as e:
			self.log.error('Terminal open error['+repr(e)+']')
			raise AppException(repr(e))

		self._runsteps(terminal)

		time.sleep(2)
		terminal.close()
		self.log.info('Terminal closed')
		if os.path.exists(self.sockfilename):
			os.unlink(self.sockfilename)
		if os.path.exists(self.badfilename):
			os.unlink(self.badfilename)
		self.log.info('case['+self.caseid+'] AppTerminal Done')
		
		
class AppTerminalClient:
	def __init__(self,caseid):
		self.tmpdir=os.environ.get('AUTOTMPDIR')
		if not self.tmpdir:
			raise AppException('Environment variable AUTOTMPDIR not exist!')
		self.steps=None
		self.stepindex=0
		self.caseid=caseid
		self.sockfilename=os.path.join(self.tmpdir,caseid+'.sock')
		self.overtime=None

	def _sockinit(self):
		self.sock=socket.socket(socket.AF_UNIX,socket.SOCK_STREAM)
		_sockconnectblock(self.sock,self.sockfilename,5)

	def _sendmsg(self,commond,msg={}):
		senddict={}
		senddict['messagehead']={}
		senddict['messagehead']['commond']=commond
		senddict['messagehead']['caseid']=self.caseid
		senddict['messagehead']['stepindex']=self.stepindex
		senddict['messagebody']=msg
		if not self.overtime:
			self.overtime=5
		_socksendblock(self.sock,senddict,self.overtime)

	def _runstep(self,stepindex,step):
		if step.has_key('timeout'):
			self.overtime=step['timeout']
		else:
			self.overtime=10

		#if step.has_key('clear') and step['clear']:
		#	self._sendmsg('clear')

		stdout=sys.stdout
		stdoutfile=os.path.join(self.tmpdir,self.caseid+'.std')
		stdoutfd=open(stdoutfile,'w')
#		sys.stdout=stdoutfd
		if step.has_key('clear') and step['clear']:
			child=pexpect.spawn('bash',['-c','clear'])
			child.logfile_read=stdout
			child.expect(pexpect.EOF)
			child.logile_read=None
	
		child=pexpect.spawn('bash',['-c',step['commond']])
		child.logfile_read=stdout

		if step.has_key('interaction'):
			for interaction in step['interaction']:
				if interaction.has_key('timeout'):
					expect_timeout=interaction['timeout']
				else:
					expect_timeout=5
				if interaction.has_key('matchmode'):
					expect_matchmode=interaction['matchmode']
				else:
					expect_matchmode='regular'
				if interaction.has_key('matchsize'):
					expect_matchsize=interaction['matchsize']
				else:
					expect_matchsize=None

				patternlist=[]
				for _interaction in interaction['expect']:
					patternlist.append(_interaction['output'])
				patternlist.append(pexpect.TIMEOUT)
				patternlist.append(pexpect.EOF)
				if not cmp(expect_matchmode,'regular'):
					expect_index=child.expect(patternlist,expect_timeout,expect_matchsize)
				elif not cmp(expect_matchmode,'exact'):
					expect_index=child.expect_exact(patternlist,expect_timeout,expect_matchsize)
				else:
					raise AppException('Unknow expect match mode['+expect_matchmode+']')

				if not cmp(patternlist[expect_index],pexpect.TIMEOUT) or not cmp(patternlist[expect_index],pexpect.EOF):
					if interaction.has_key('ignorable') and interaction['ignorable']:
						continue
					else:
						if not cmp(patternlist[expect_index],pexpect.TIMEOUT):
							raise AppException('TIMEOUT when expect ['+repr(patternlist[0:len(patternlist)-2])+']')
						else:
							raise AppException('EOF when expect ['+repr(patternlist[0:len(patternlist)-2])+']')
				
				if interaction['expect'][expect_index].has_key('outcapture') and interaction['expect'][expect_index]['outcapture']:
					senddict={}
					senddict['capindex']=interaction['expect'][expect_index]['outpictureindex']
					self._sendmsg('capture',senddict)
					recvdict={}
					recvdict=_sockrecvblock(self.sock,self.overtime)
					if not cmp(recvdict['messagehead']['commond'],'complete'):
						pass
					elif not cmp(recvdict['messagehead']['commond'],'error'):
						raise AppException('Recv error from server commond when outcapture!')
					else:
						raise AppException('Recv unknow commond from server commond when outcapture!')

				child.send(interaction['expect'][expect_index]['input'])
				#child.send('\r')
				
				if interaction['expect'][expect_index].has_key('incapture') and interaction['expect'][expect_index]['incapture']:
					#time.sleep(len(interaction['expect'][expect_index]['input']))
					#time.sleep(10)
					senddict={}
					senddict['capindex']=interaction['expect'][expect_index]['inpictureindex']
					self._sendmsg('capture',senddict)
					recvdict={}
					recvdict=_sockrecvblock(self.sock,self.overtime)
					if not cmp(recvdict['messagehead']['commond'],'complete'):
						pass
					elif not cmp(recvdict['messagehead']['commond'],'error'):
						raise AppException('Recv error from server commond when incapture!')
					else:
						raise AppException('Recv unknow commond from server commond when incapture!')

				child.send('\r')
					
		child.expect(pexpect.EOF)
		if step.has_key('capture') and step['capture']:
			senddict={}
			senddict['capindex']=step['pictureindex']
			self._sendmsg('capture',senddict)
			recvdict={}
			recvdict=_sockrecvblock(self.sock,self.overtime)
			if not cmp(recvdict['messagehead']['commond'],'complete'):
				pass
			elif not cmp(recvdict['messagehead']['commond'],'error'):
				raise AppException('Recv error from server commond when capture!')
			else:
				raise AppException('Recv unknow commond from server commond when capture!')
		sys.stdout=stdout
	
	def run(self):
		self._sockinit()

		self._sendmsg('verify')
		recvdict={}
		recvdict=_sockrecvblock(self.sock,5)
		if not cmp(recvdict['messagehead']['commond'],'complete'):
			self.steps=recvdict['messagebody']['stepargs']
			self.stepindex=recvdict['messagehead']['stepindex']
		elif not cmp(recvdict['messagehead']['commond'],'error'):
			senddict={}
			senddict['errormsg']='Recv error from server commond when verify!'
			self._sendmsg('error',senddict)
			raise AppException('Recv error from server commond when verify!')
		else:
			senddict={}
			senddict['errormsg']='Recv unknow commond from server commond when verify!'
			self._sendmsg('error',senddict)
			raise AppException('Recv unknow commond from server commond when verify!')

		loop=0
		for step in self.steps:
			if loop<self.stepindex:
				continue
			else:
				self.stepindex=loop
			if not cmp(step['type'],'builtin'):
				"""
				senddict={}
				senddict['builtinstep']=step
				self._sendmsg('execbuiltin',senddict)
				recvdict={}
				recvdict=_sockrecvblock(self.sock,5)
				if not cmp(recvdict['messagehead']['commond'],'complete'):
					continue
				elif not cmp(recvdict['messagehead']['commond'],'error'):
					senddict={}
					senddict['errormsg']='Recv error from server commond when exec built-in ['+step['commond']+'!'
					self._sendmsg('error',senddict)
					raise AppException('Recv error from server commond when exec built-in ['+step['commond']+'!')
				else:
					senddict={}
					senddict['errormsg']='Recv unknow commond from server commond when exec built-in ['+step['commond']+'!'
					self._sendmsg('error',senddict)
					raise AppException('Recv unknow commond from server commond when exec built-in ['+step['commond']+'!')
				"""
				if not cmp(step['commond'].split()[0],'cd'):
					os.chdir(step['commond'].split()[1])
				else:
					raise AppException('Unknow build in commond')
			else:
				try:
					self._runstep(loop,step)
				except AppException as e:
					senddict={}
					senddict['errormsg']=repr(e)
					self._sendmsg('error',senddict)
					raise
			loop+=1
		self._sendmsg('complete')


if __name__ == '__main__':
	if len(sys.argv)!=2:
		print('Usage: $0 [caseid]')
		sys.exit()
	try:
		client=AppTerminalClient(sys.argv[1])
		client.run()
	except Exception as e:
		raise 
		sys.exit()
