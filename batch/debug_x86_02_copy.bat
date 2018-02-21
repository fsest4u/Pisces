echo off
rem -----------------------------------------------------
rem Copy Images, license, redistribute for Install
rem -----------------------------------------------------
mkdir ..\..\Pisces-Desktop-MSVC2013-32bit-Debug\bin\Debug

rem -----------------------------------------------------
rem Copy Utility for Execute
rem -----------------------------------------------------
xcopy ..\reader\* 					..\..\Pisces-Desktop-MSVC2013-32bit-Debug\bin\Debug\reader\* /e /h /k

