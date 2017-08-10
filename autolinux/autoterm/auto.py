#!/usr/bin/python
import os
import sys
import json
import multiprocessing

sys.path.append(os.path.join(os.environ.get('AUTOBASEDIR'),'lib'))
import sysconf
import logutil
from app import *

if __name__=='__main__':
	if len(sys.argv)<2:
		print('Useage $0 [caseid,...]')
		sys.exit()

	sysconf=sysconf.getsysconf()
	log=logutil.ULogger('System')

	#sysconf check
	if not sysconf.has_key('basedir'):
		log.error('System config not exist field [basedir]')
		sys.exit()
	elif  not sysconf.has_key('application_support'):
		log.error('System config not exist field [application_support]')
	else:
		pass

	#multi process
	if sysconf.has_key('multi_support') and sysconf['multi_support']:
		log.info('Run in concurrent mode!')
		processlist=[]

		#init process lock
		lock=multiprocessing.Lock()

		for caseid in sys.argv[1:len(sys.argv)]:
			casejsonfile=os.path.join(sysconf['basedir'],'case/'+caseid+'.json')
			try:
				with open(casejsonfile) as f:
					caseconf=json.load(f)
			except Exception as e:
				log.error('Paser case config file ['+casejsonfile+'] error')
			#Application support check
			supportflag=False
			for app_support in sysconf['application_support']:
				if not cmp(app_support['app_name'],caseconf['application']):
					supportflag=True
					break
				else:
					pass
			if not supportflag:
				log.error('System do not support application['+caseconf['application']+']')
				sys.exit()
			#Dynamic load module
			app_module='app.'+app_support['app_module']
			if app_module in sys.modules:
				pass
			else:
				__import__(app_module)

			#Dynamic load class
			app_bind=app_support['app_bind']
			module=sys.modules[app_module]
			app_class=getattr(module,app_bind)
			app_instance=app_class(caseid,**caseconf['arguments'])

			#init child process
			processdict={}
			processdict['caseid']=caseid
			processdict['process']=multiprocessing.Process(target=app_instance.run,args=(lock,))
			processlist.append(processdict)

		#process start
		for process in processlist:
			process['process'].start()
			log.info('case ['+process['caseid']+'] process start ['+str(process['process'].pid)+']')

		#process join
		for process in processlist:
			process['process'].join()

	#single process
	else:
		log.info('Run in singleton mode!')
		for caseid in sys.argv[1:len(sys.argv)]:
			casejsonfile=os.path.join(sysconf['basedir'],'case/'+caseid+'.json')
			try:
				with open(casejsonfile) as f:
					caseconf=json.load(f)
			except Exception as e:
				log.error('Paser case config file ['+casejsonfile+'] error')
			#Application support check
			supportflag=False
			for app_support in sysconf['application_support']:
				if not cmp(app_support['app_name'],caseconf['application']):
					supportflag=True
					break
				else:
					pass
			if not supportflag:
				log.error('System do not support application['+caseconf['application']+']')
				sys.exit()
			#Dynamic load module
			app_module='app.'+app_support['app_module']
			if app_module in sys.modules:
				pass
			else:
				__import__(app_module)

			#Dynamic load class
			app_bind=app_support['app_bind']
			module=sys.modules[app_module]
			app_class=getattr(module,app_bind)
			app_instance=app_class(caseid,**caseconf['arguments'])
			
			log.info('case ['+caseid+'] begin')
			app_instance.run()
			log.info('case ['+caseid+'] end')
	
	log.info('System end')
