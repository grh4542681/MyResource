import os
import sys
import abc

class AppBase:
	__metaclass__=abc.ABCMeta

	@abc.abstractmethod
	def run(self,lock=None):pass
