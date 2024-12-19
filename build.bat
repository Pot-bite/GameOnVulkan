@echo off

call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"

SET includes=/ISource /I%VULKAN_SDK%/Include 
SET links=/link /LIBPATH:%VULKAN_SDK%/Lib vulkan-1.lib User32.lib
SET defines=/D DEBUG


cl /EHsc %includes% %defines% Source/platform/Win32_platform.cpp %links% 
