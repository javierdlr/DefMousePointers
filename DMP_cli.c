//#define _CDEBUG_
#ifdef _CDEBUG_
    #define DBUG(x,...) IExec->DebugPrintF("[%s|%s:%-4ld] "x, CLASSNAME,__FILE__,__LINE__, ##__VA_ARGS__)
#else
    #define DBUG(x,...)
#endif


#define CATCOMP_NUMBERS
//#define CATCOMP_BLOCK
//#define CATCOMP_CODE
extern struct LocaleInfo li;

#include "DMP_global.h"
#include <devices/input.h>
#include <devices/inputevent.h>

extern STRPTR textbuf, pointers_path;
extern BOOL BufferedCopy(CONST_STRPTR, CONST_STRPTR);

void DMP_from_CLI(void);
void ListDMP_CLI(STRPTR string, LONG moreinfo);
void CenterMousePointer(void);


/* DefMousePointer is in CLI mode */
void DMP_from_CLI(void)
{
 BPTR file = 0;
 struct RDArgs *rdargs;
 CONST_STRPTR argTemplate = "CM=CENTERMOUSE/S,USE=USETHEME/S,V=VERBOSE/S";
 STATIC struct{
               LONG CenterMouse;
               LONG UseTheme;
               LONG MoreInfo;
              } Args;

  rdargs = IDOS->ReadArgs(argTemplate, (LONG *)&Args, NULL);
  if(rdargs != NULL)
  {
   STRPTR string = IExec->AllocVecTags(MAX_STRING, TAG_DONE);

   if(Args.CenterMouse)
   {
    CenterMousePointer();
   }

   if(Args.UseTheme)
   {
    STRPTR theme = IExec->AllocVecTags(MAX_THEME, TAG_DONE);

    IUtility->Strlcpy(string, pointers_path, MAX_STRING);
    IDOS->AddPart(string, PREFS_FILENAME, MAX_STRING); // prefs file fullpath
    if( (file=IDOS->Open(string, MODE_OLDFILE)) )
    {
//DBUG("getting theme pointers...\n");
     IDOS->FGets(file, theme, MAX_THEME);
     IDOS->Close(file);

     IUtility->Strlcpy(string, pointers_path, MAX_STRING);
     IDOS->AddPart(string, theme, MAX_STRING); // theme drawer fullpath
//DBUG("copying theme pointers...\n");
     ListDMP_CLI(string, Args.MoreInfo);
    }
    else
    {
     IDOS->Printf(GetString(&li,MSG_WARN_NOPREFSFILE),PREFS_FILENAME);
    }

    IExec->FreeVec(theme);
   }
   else
   {
    IDOS->Printf("%s %s%s\n%s",VERS,DATE,GetString(&li,MSG_CLI_TRANSLATOR),GetString(&li,MSG_CLI_ABOUT));
   }

   IExec->FreeVec(string);
  }
  else
  {
   IDOS->PutErrStr( GetString(&li,MSG_CLI_WRONG_ARGS) );
  }

  IDOS->FreeArgs(rdargs);
}


/* Get pointer theme list to USE and copy them to 'ENV:Sys/' */
void ListDMP_CLI(STRPTR string, LONG moreinfo)
{
 STRPTR src_file = IExec->AllocVecTags(MAX_SRC, TAG_DONE),
        dst_file = IExec->AllocVecTags(MAX_DST, TAG_DONE),
        pattern_ms = IExec->AllocVecTags(MAX_PATTERN, TAG_DONE);

 IDOS->ParsePatternNoCase("def_#?pointer.info", pattern_ms, MAX_PATTERN);

 APTR context = IDOS->ObtainDirContextTags(EX_StringNameInput, string,
                                           EX_DataFields, (EXF_NAME|EXF_TYPE),
                                           EX_MatchString, pattern_ms,
                                          TAG_END);
 if(context)
 {
  struct ExamineData *dat;
  int32 pointers_tot = 0;
//DBUG("theme '%s'...\n",string);
  while( (dat=IDOS->ExamineDir(context)) )
  {
   if( EXD_IS_FILE(dat) )
   {
    IUtility->Strlcpy(src_file, string, MAX_SRC);
    IDOS->AddPart(src_file, dat->Name, MAX_SRC);

    IUtility->Strlcpy(dst_file, "ENV:Sys/", MAX_DST);
    IDOS->AddPart(dst_file, dat->Name, MAX_DST);
//DBUG("copying '%s'...\n",dat->Name);
    if(BufferedCopy(src_file, dst_file) == 0)
    {
     pointers_tot++;
    }
   }
  }

  if( ERROR_NO_MORE_ENTRIES != IDOS->IoErr() )
  {
   IDOS->PrintFault(IDOS->IoErr(), "DMP_from_CLI()");
  }

  if(moreinfo)
  {
   IDOS->Printf(GetString(&li,MSG_CLI_INFO),IDOS->FilePart(string),pointers_tot,"ENV:Sys/");
  }

 }
 else
 {
  IDOS->Fault(IDOS->IoErr(), NULL, textbuf, MAX_TXTB);
  IDOS->Printf(GetString(&li,MSG_ERROR_COPY_POINTERTHEME),IDOS->FilePart(string),textbuf);
 }

 IDOS->ReleaseDirContext(context);
 IExec->FreeVec(pattern_ms);
 IExec->FreeVec(src_file);
 IExec->FreeVec(dst_file);
}


/************************************************************/
/* This piece of code is by Thomas Rapp & Wolfgang Hosemann */
void CenterMousePointer(void)
{
 struct MsgPort *inputport = IExec->AllocSysObject(ASOT_PORT, NULL);

 if(inputport)
 {
  struct IOStdReq *inputreq = IExec->AllocSysObjectTags(ASOT_IOREQUEST,
                                      ASOIOR_Size, sizeof(struct IOStdReq),
                                      ASOIOR_ReplyPort, inputport,
                                     TAG_END);

  if(inputreq)
  {
   if( !(IExec->OpenDevice("input.device",0,(struct IORequest *)inputreq,0)) )
   {
    struct InputEvent *inputevent = IExec->AllocVecTags(sizeof(struct InputEvent)+sizeof(struct IEPointerPixel),
                                                        AVT_ClearWithValue,0, AVT_Type,MEMF_SHARED, TAG_DONE);
    if(inputevent)
    {
     struct IEPointerPixel *ptrpixel = (struct IEPointerPixel *)(inputevent + 1);
     struct Screen *scr = IIntuition->LockPubScreen(NULL);
     if(scr)
     {
      /* Set up IEPointerPixel fields */
      ptrpixel->iepp_Screen     = scr;
      ptrpixel->iepp_Position.X = scr->Width / 2;
      ptrpixel->iepp_Position.Y = scr->Height / 2;

      /* Set up InputEvent fields */
      inputevent->ie_EventAddress = ptrpixel;              /* IEPointerPixel       */
      inputevent->ie_Class        = IECLASS_NEWPOINTERPOS; /* new mouse pos        */
      inputevent->ie_SubClass     = IESUBCLASS_PIXEL;      /* on pixel             */
      inputevent->ie_Code         = IECODE_NOBUTTON;
      //inputevent->ie_NextEvent    = NULL;
      //inputevent->ie_Qualifier    = 0;                     /* absolute positioning */

      inputreq->io_Data    = inputevent;                /* InputEvent */
      inputreq->io_Length  = sizeof(struct InputEvent);
      inputreq->io_Command = IND_WRITEEVENT;
      IExec->DoIO( (struct IORequest *)inputreq );

      IIntuition->UnlockPubScreen(NULL, scr);
     }
     else
     {
      IDOS->PutErrStr( GetString(&li,MSG_CLI_FAILED_CENTERMOUSE) );
     }
     IExec->FreeVec(inputevent);
    }
    IExec->CloseDevice( (struct IORequest *)inputreq );
   }
   else
   {
    IDOS->Printf(GetString(&li,MSG_ERROR_OPEN),"input.device");
   }
   IExec->FreeSysObject(ASOT_IOREQUEST, inputreq);
  }
  IExec->FreeSysObject(ASOT_PORT, inputport);
 }
}
/* This piece of code is by Thomas Rapp & Wolfgang Hosemann */
/************************************************************/
