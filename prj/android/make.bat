@echo off

set NDK_BUILD=%ANDROID_NDK_HOME%\ndk-build
set MY_BASH=%CYGWIN_BIN%\bash
set MY_BACKUP=%CD%
set CYGWIN=nodosfilewarning

if "%1"=="clean" (
	%MY_BASH% -i %NDK_BUILD% clean
) else (
	%MY_BASH% -i %NDK_BUILD%	
)
