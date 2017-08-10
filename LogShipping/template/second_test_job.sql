USE msdb ;  
GO  

EXEC dbo.sp_start_job N'LSCopy_SampleDB' ;  
GO  
EXEC dbo.sp_start_job N'LSRestore_SampleDB' ;  
GO  
