import traceback


class CaseException(Exception):pass


class LogException(CaseException):
	def __init__(self,Msg=''):
		self.traceback=traceback.format_exc()
		self.msg=Msg

	def __str__(self):
		return self.traceback+'Message: '+self.msg


class GuiFrmException(CaseException):
	def __init__(self,Msg=''):
		self.traceback=traceback.format_exc()
		self.msg=Msg

	def __str__(self):
		return self.traceback+'Message: '+self.msg

class AppException(CaseException):
	def __init__(self,Msg=''):
		self.traceback=traceback.format_exc()
		self.msg=Msg

	def __str__(self):
		return self.traceback+'Message: '+self.msg

class TerminalException(CaseException):
	def __init__(self,Msg=''):
		self.traceback=traceback.format_exc()
		self.msg=Msg

	def __str__(self):
		return self.traceback+'Message: '+self.msg
