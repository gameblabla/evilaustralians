PATH=C:\3DODev\bin;%PATH% 
del swing.iso
make clean
make
pause
start /d "OperaFS" OperaFS.exe
