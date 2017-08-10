import json
import os
def getsysconf():
	if os.path.exists('../etc/system.json'):
		with open('../etc/system.json') as f:
			sysconf=json.load(f)
	elif os.path.exists('./etc/system.json'):
		with open('./etc/system.json') as f:
			sysconf=json.load(f)
	else:
		pass
	return sysconf
