USE master;
GO

IF NOT EXISTS (select * from sys.databases where name = 'SampleDB')
        CREATE DATABASE SampleDB
GO
