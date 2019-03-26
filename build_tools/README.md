install build tools on windows xp.

https://www.hpcalc.org/details/3809
progtool.zip for RPL programmers, along with the DOS4GW runtime. No documentation is included, but the file names are libcrc.com, makerom.exe, rplcomp.exe, sasm.exe, and sload.exe.

minstub39 rpl code use tools in progtool.zip to build. HPGCC000.000 is build output.
minstub39 hp2aplet.c.k use https://github.com/Arnie97/milky to convert. hp2aplet.c is convert output.
minstub39 hp2aplet.c use gcc for example codeblocks-17.12mingw to build. hp2aplet.exe is build output.
main part use cross build toolchain in HPGCC-1.1-p1_with_39G to build.

install Conn3x and HPCalculatorUSBSetup to transfer aplet to hp39gs.

It says `Undefined XLIB Name` when trying to open this aplet.
- Go to the `MEMORY MANAGER` and check whether the ARM ToolBox
(i.e. Library 275, or L275 for short) is installed in your libraries.
- If not, please install lib275.sys.
