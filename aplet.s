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
	Link	_Prepare
	Link	_Display
	Link	_DispXYMini

* Put here your self-made label. Do not forget to DEFINE AND INT_xx them beneath.  

endLink
RPL
DEFINE Main		INT_00
DEFINE Entry	INT_01
DEFINE Exit		INT_02
DEFINE Action	INT_03
DEFINE Prepare	INT_04
DEFINE Display	INT_05
DEFINE DispXYMini INT_06

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

DEFINE MODE_D	INT_00	( Mode Dec )
DEFINE MODE_H	INT_01	( Mode Hex )
DEFINE MODE_B	INT_02	( Mode Bin )
DEFINE UNSIGNED	INT_00	( unsigned )
DEFINE SIGNED	INT_01	( signed )

DEFINE  AppVar! TopicVar1!
DEFINE  AppVar@ TopicVar1@
DEFINE  AppMode! TopicVar2!
DEFINE  AppMode@ TopicVar2@
DEFINE  AppSign! TopicVar3!
DEFINE  AppSign@ TopicVar3@

ASSEMBLE
=DISPXYmini	EQU	#3F8A0					( unsupported entry to use the miniFONT )
RPL
DEFINE CLEARSCREEN HARDBUFF ZEROZERO BINT_131d SIXTYFOUR GROB!ZERODRP NULL$ DISPROW1

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
	TRUE 
	{

		{ :: "BIN" MakeStdLabel ; :: MODE_B AppMode! Display ; }
		{ :: "BIN" MakeBoxLabel ; :: MODE_B AppMode! Display ; }


		{ :: "HEX" MakeStdLabel ; :: MODE_H AppMode! Display ; }
		{ :: "HEX" MakeBoxLabel ; :: MODE_H AppMode! Display ; }
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
	HARDBUFF ZEROZERO BINT_131d SIXTYFOUR GROB!ZERODRP

	$ "HexCalcP By Jejer" DISPROW1
	$ "Press Enter to start" DISPROW8
;



*************************************************
*			Begin of the program
*************************************************
NAMELESS _Action ::
	Prepare
;


**************************************************
*			Start Of Prepare
**************************************************
NAMELESS _Prepare ::
	HARDBUFF ZEROZERO BINT_131d SIXTYFOUR GROB!ZERODRP	( clearscreen )

	INT_00 AppVar!			( AppVar init to 0 )
	MODE_H AppMode!			( AppMode init to Hex )
	UNSIGNED AppSign!		( AppSign init to UNSIGNED )

	Display
	WaitForKey
;


**************************************************
*			Start Of Display
**************************************************
NAMELESS _Display ::
	CLEARSCREEN

	BINT0 BINT5 $ "BIN" DispXYMini 	( display "BIN" )
	BINT130 BINT0 PIXON
	BINT120 BINT0 PIXON
	BINT110 BINT0 PIXON
	BINT100 BINT0 PIXON
	BINT90  BINT0 PIXON
	BINT80  BINT0 PIXON
	BINT70  BINT0 PIXON
	BINT60  BINT0 PIXON

	BINT0 BINT25 $ "HEX" DispXYMini 	( display "HEX" )
	BINT0 BINT45 $ "DEC" DispXYMini 	( display "DEC" )

	DispMenu
;


**************************************************
*			Start Of Utilities
**************************************************
NAMELESS _DispXYMini ::
	HARDBUFF										( show the text on hardbuff ) 
	FOUR ROLL FOUR ROLL								( take X and Y coordinate )
	FOUR ROLL DUPLEN$ 								( take the string, duplicate and give length )
	FOUR #* 										( length * 4 )
	DISPXYmini										( Display it in miniFont *unsupported entry* )
	DROP											( drop ) 
;													( end of DispXYMini )


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
*			Note of the aplet
**************************************************
* if you want to put text in a note do it here
* beneath, between the " "
**************************************************

$ "Put your note here!"

ASSEMBLE
	Vfield DirNot,DirEnd
RPL
	{ }
