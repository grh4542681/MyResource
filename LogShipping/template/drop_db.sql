USE master;
GO

IF EXISTS (select * from sys.databases where name = 'SampleDB')
	DROP DATABASE SampleDB

GO
