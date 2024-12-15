@echo off

call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"

SET includes=
SET links=
SET defines=


cl /EHsc %includes% %defines% Source/main.cpp %links% 
