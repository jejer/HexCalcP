	TITLE HexCalcP
*******************************************************
* HexCalcP for HP39gs
* by Jejer (jejer.net zjejer@gmail.com)
* Run build.bat to compile.
*******************************************************

INCLUDE =Entry39.h
INCLUDE =Common.h
INCLUDE =Command.h


*************************************************
*			Name of the Aplet
*************************************************
ASSEMBLE

	Dir_Head 8,HexCalcP,601
*            ^1    ^2   ^3
* ^1=Lenght of the name of the aplet.
* ^2=Name of the aplet.
* ^3=Number of the library. This is necessary even if the library is empty. 

RPL
	StndtType			( This aplet skeleton will only provide access to the "C" type of aplet )
INCLUDE =Sharedvar.h
ASSEMBLE 
	Vfield  L34,IntTable
	CON(5)	=DOHSTR
	REL(5)  endLink
	Link	_Main	
	Link	_Entry
	Link	_Exit
	Link	_Action		( Include all the variables needed by the aplet structure )

* Put here your self-made label. Do not forget to DEFINE AND INT_xx them beneath.  

endLink
RPL
DEFINE Main		INT_00
DEFINE Entry	INT_01
DEFINE Exit		INT_02
DEFINE Action	INT_03

ASSEMBLE
	Vfield  IntTable,DirVar
RPL
ASSEMBLE
	CON(5)	= DOHSTR
	REL(5)  endCode
RPL

* The HP39/40G provides a list of 91 TopicVariables that can contain any kind of object
* Instead of using local variables as on the HP48/49, it is a good idea to use these
* variables instead. The access is really fast and easy. The contain of these variables
* will be preserved while the aplet is running. If you are exiting the view or
* switch to another aplet, the content will be deleted.
* To ease the access to your variables, use alias like:


* Make your own variable here. 

DEFINE	Save@	LastBut3
DEFINE	Save!	LastBut3 REPLACE DROP

*************************************************
*			Self-made variables
*************************************************
* ici

NAMELESS _Main ::
	StndCheck
	' Entry
	' Exit
	' ::
		EnsureMenuOff
	;
	' :: 
		{
			{
				kcSymbView :: TakeOver Action ;	( Enter or Symb to continue the program )
				kcEnter :: TakeOver Action ;
			}
			NULL{}
			NULL{}
			NULL{}
			NULL{}
			NULL{}
		}
		ONE KeyFace
	;
    TRUE {
		{ ::
		; }
	}
    ONE
	' ::
		TURNMENUON RECLAIMDISP
		ERRJMP
    ;
;

*************************************************
*			Begin of the presentation
*************************************************
NAMELESS _Entry ::
	TURNMENUOFF
	HARDBUFF ZEROZERO BINT_131d SIXTYFOUR GROB!ZERODRP

	$ "HexCalcP By Jejer" DISPROW1
	$ "Press Enter to start" DISPROW8
;
**************************************************
*			End of the presentation
**************************************************


*************************************************
*			Begin of the program
*************************************************
NAMELESS _Action ::
	$ "Hello World!" Ck&DoMsgBox
;
**************************************************
*			End of the program
**************************************************


NAMELESS _Exit ::
	LeaveGraphView
;
ASSEMBLE
	endCode
RPL
ASSEMBLE
	Vfield DirVar,DirCode

* This variable will contain all the saved parameters of the aplet
* You can add your own variable in the list

RPL
	{
		ONE		( Add your default parameters here for the variable 'save' )
		TWO
	}
ASSEMBLE
	Vfield DirCode,DirAlt
RPL
	{ }
ASSEMBLE
	Vfield DirAlt,DirNot
RPL

**************************************************
*		     Note of the aplet			 *
**************************************************
* if you want to put text in a note do it here   *
* beneath, between the " "				 *
**************************************************

$ "Put your note here!"

ASSEMBLE
	Vfield DirNot,DirEnd
RPL
	{ }
