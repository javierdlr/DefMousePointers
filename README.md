# DefMousePointers
DefMousePointer is a tool to change image pointers on the fly. It has a ReAction GUI and a CLI/Shell support too.

   *********************************************************************
       MAKE A BACKUP OF 'ENVARC:Sys/def_#?pointer.info' AS THIS TOOL    
             NEED SUCH FILES _NOT_ TO RESIDE IN 'ENVARC:Sys/'.          
   *********************************************************************

ReAction GUI:
-Launch from Workbench with WBSTARTUP tooltype disabled and you get a nice
 GUI with a pointer theme chooser and a listbrowser showing pointer theme
 images (from 'POINTERS' drawer).
-Buttons:
 [SAVE]: creates a preferences file (inside 'POINTERS' drawer) with
         choosed pointer theme name (see CLI/Shell for more info).

 [USE] : copies pointer theme images/icons (def_#?pointe.info) to
         'ENV:Sys/' so you can use such pointers.

 [QUIT]: exits program.

-Tooltypes (only usefull from WBStartup Prefs.):
 WBSTARTUP: copies saved pointer theme (using DMP's GUI) to 'ENV:Sys/' and
            centers mouse pointer on screen.


CLI/Shell:
-Usage: DefMousePointers [CM=CENTERMOUSE] [USE=USETHEME] [V=VERBOSE]
 CM=CENTERMOUSE: centers mouse pointer on screen.
 USE=USETHEME  : copies saved pointer theme (using DMP's GUI) to 'ENV:Sys/'.
 V=VERBOSE     : prints result of USE argument.


INSTALLATION:
a)Use the install script provided.
b)Or follow this steps:
  -Copy DMP drawer wherever you want.
  -Move 'ENVARC:Sys/def_#?pointer.info' to '<DMP_path>/POINTERS/custom' drawer.
  -If you want to use saved pointer theme on WB launch, add it to you U-S:
   'Run >NIL: *>NIL: <path>DefMousePointers USE'
   or use WBStartup Prefs. and DMP tooltype WBSTARTUP.


NOTES:
1)Why to (re)move such icons from 'ENVARC:Sys/'?
If I have in 'ENVARC:Sys/' only def_pointer.info and the pointer theme I
choose doesn't have one def_pointer.info, when I click on Use button, such
image will be used, 'cos env-handler (kickstart module) clones 'ENVARC:'
(theme doesn't have 'def_pointer.info', but alas it exists in 'ENVARC:'..).
2)When using from U-S, pointers sometimes aren't loaded, known problem [WIP].


SPECIAL THANKS:
Martin Merz for their awesome pointer icons.
Tuomas Hokka for 'BufferCopy()' code.
Thomas Rapp & Wolfgang Hosemann for 'CenterMousePointer()' code.
Everyone at os4coding forum!
Testers/Translators: Guillaume 'zzd10h' Boesel, Samir 'samo79' Hawamdeh.


AUTHOR
Javier de las Rivas <jabierdlr@gmail.com>

