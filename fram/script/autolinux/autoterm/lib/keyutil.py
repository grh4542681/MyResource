import json

def singleton(cls, *args, **kw):  
    instances = {}  
    def _singleton():  
        if cls not in instances:  
            instances[cls] = cls(*args, **kw)  
        return instances[cls]  
    return _singleton  

@singleton
class UKeyBoardLdtp:
	def __init__(self):
		self._pressshift='!@#$%^&*()_+QWERTYUIOP{}|ASDFGHJKL:"ZXCVBNM<>?'
		self.keylist=[]
		for i in range(128):
			char=chr(i)
			if char in self._pressshift:
				if char=='<':
					char=','
				if char=='>':
					char='.'
				if char=='\'' or char=='\\':
					char='\\'+char						
				self.keylist.append('<shift>'+char)
			else:
				self.keylist.append(char)

	def getlist(self):
		return self.keylist

if __name__ == '__main__':
	from ldtp import *

	a=UKeyBoardLdtp()
	dirt=UKeyBoardLdtp().getlist()
	generatekeyevent(dirt[65]+dirt[66])

