//#define _WDEBUG_
#ifdef _WDEBUG_
    #define DBUG(x,...) IExec->DebugPrintF("[%s|%s:%-4ld] "x, CLASSNAME,__FILE__,__LINE__, ##__VA_ARGS__)
#else
    #define DBUG(x,...)
#endif


#define CATCOMP_NUMBERS
#define CATCOMP_BLOCK
#define CATCOMP_CODE
struct LocaleInfo li;


#define _AEON_
#include "stdarg.h"

#include "DMP_global.h"
#include "DMP.h"
#include "xml.h" // list of pointers

const char *version = VERSTAG;
STRPTR textbuf,       // for localized messages
       dmp_name,      // DMP executable name
       pointers_path; // DMP fullpath (name NOT included) + 'POINTERS' drawer
//struct List *listviewer_list;
struct WBStartup *WBenchMsg = NULL;
struct Window *pwindow = NULL;
Object *Objects[LAST_NUM];

extern void ListDMP_CLI(STRPTR, LONG); // DMP_cli.c
extern void DMP_from_CLI(void);        // DMP_cli.c


int main(int argc, char **argv)
{
    textbuf = IExec->AllocVecTags(MAX_TXTB, TAG_DONE);

    if(argc == 0)
    {
        WBenchMsg = (struct WBStartup *)argv;
    }

    if( OpenLibs((int32)WBenchMsg) )
    {
        pointers_path = IExec->AllocVecTags(MAX_PREF, TAG_DONE);
        dmp_name = IExec->AllocVecTags(MAX_FNAME, TAG_DONE);

       if(WBenchMsg)
       {// Running from WORKBENCH: open ReAction GUI
           IDOS->NameFromLock(WBenchMsg->sm_ArgList->wa_Lock, pointers_path, MAX_PREF);
           IDOS->AddPart(pointers_path, "POINTERS", MAX_PREF);
           IUtility->Strlcpy(dmp_name, WBenchMsg->sm_ArgList->wa_Name, MAX_FNAME);
DBUG("'%s' started from WB (aeon)\n",dmp_name);
           DMP_from_WB();
       }
       else
       {// Running from SHELL/CLI: parse arguments
           IDOS->NameFromLock(IDOS->GetProgramDir(), pointers_path, MAX_PREF);
           IDOS->AddPart(pointers_path, "POINTERS", MAX_PREF);
DBUG("'%s' started from CLI\n",argv[0]);
           DMP_from_CLI();
       }

      IExec->FreeVec(pointers_path);
      IExec->FreeVec(dmp_name);
    }

    CloseLibs();

    IExec->FreeVec(textbuf);
    return(RETURN_OK);
}


/* Open required (mode WB/CLI) libs/classes */
BOOL OpenLibs(int32 gui_mode)
{
    li.li_Catalog = NULL;
    if( (LocaleBase = IExec->OpenLibrary("locale.library", 52)) &&
       (ILocale = (struct LocaleIFace *)IExec->GetInterface(LocaleBase, "main", 1, NULL)) )
    {
        li.li_ILocale = ILocale;
        li.li_Catalog = ILocale->OpenCatalog(NULL, "defmousepointers.catalog",
                                             OC_BuiltInLanguage, "english",
                                             OC_PreferExternal, TRUE,
                                            TAG_END);
    }
    else
    {
        IDOS->PutErrStr("Failed to use catalog system. Using built-in strings.\n");
    }

    UtilityBase = IExec->OpenLibrary("utility.library", 52);
    if(!UtilityBase)
    {
        IDOS->Printf(GetString(&li,MSG_ERROR_LIB),"utility.library",52);
        return FALSE;
    }
    IUtility = (struct UtilityIFace *)IExec->GetInterface(UtilityBase, "main", 1, NULL);
    if(!IUtility)
    {
        IDOS->Printf(GetString(&li,MSG_ERROR_OPEN),"UtilityIFace");
        return FALSE;
    }

    IntuitionBase = IExec->OpenLibrary("intuition.library", 52);
    if(!IntuitionBase)
    {
        IDOS->Printf( GetString(&li,MSG_ERROR_LIB),"intuition.library",52 );
        return FALSE;
    }
    IIntuition = (struct IntuitionIFace *)IExec->GetInterface(IntuitionBase, "main", 1, NULL);
    if(!IIntuition)
    {
        IDOS->Printf(GetString(&li,MSG_ERROR_OPEN),"IntuitionIFace");
        return FALSE;
    }

    /*GfxBase = IExec->OpenLibrary("graphics.library", 52);
    if(!GfxBase)
    {
        IDOS->Printf( GetString(&li,MSG_ERROR_LIB),"graphics.library",52 );
        return FALSE;
    }
    IGraphics = (struct GraphicsIFace *)IExec->GetInterface(GfxBase, "main", 1, NULL);
    if(!IGraphics)
    {
        IDOS->Printf(GetString(&li,MSG_ERROR_OPEN),"GraphicsIFace");
        return FALSE;
    }*/

    if(gui_mode)
    {
        IconBase = IExec->OpenLibrary("icon.library", 52);
        if(!IconBase)
        {
            IUtility->SNPrintf(textbuf, MAX_TXTB, GetString(&li,MSG_ERROR_LIB),"icon.library",52);
            DoMessage(textbuf, REQIMAGE_ERROR, NULL);
            return FALSE;
        }
        IIcon = (struct IconIFace *)IExec->GetInterface(IconBase, "main", 1, NULL);
        if(!IIcon)
        {
            IUtility->SNPrintf(textbuf, MAX_TXTB, GetString(&li,MSG_ERROR_OPEN),"IconIFace");
            DoMessage(textbuf, REQIMAGE_ERROR, NULL);
            return FALSE;
        }

        ButtonBase = IIntuition->OpenClass("gadgets/button.gadget", 52, &ButtonClass);
        //BitMapBase = IIntuition->OpenClass("images/bitmap.image", 52, &BitMapClass);
        //LabelBase = IIntuition->OpenClass("images/label.image", 52, &LabelClass);
        LayoutBase = IIntuition->OpenClass("gadgets/layout.gadget", 52, &LayoutClass);
        WindowBase = IIntuition->OpenClass("window.class", 52, &WindowClass);
        RequesterBase = IIntuition->OpenClass("requester.class", 52, &RequesterClass);
        if(!ButtonBase||!LayoutBase||!WindowBase||!RequesterBase)
        {
            DoMessage( (STRPTR)GetString(&li, MSG_ERROR_NOGUI), REQIMAGE_ERROR, NULL );
            return FALSE;
        }

        ListViewerBase = (APTR)IIntuition->OpenClass("gadgets/listviewer.gadget", 53, &ListViewerClass);
        if(!ListViewerBase)
        {
            IUtility->SNPrintf(textbuf, MAX_TXTB, GetString(&li,MSG_ERROR_LIB),"listviewer.gadget",53);
            DoMessage(textbuf, REQIMAGE_ERROR, NULL);
            return FALSE;
        }

        SelectGadgetBase = (APTR)IIntuition->OpenClass("gadgets/select.gadget", 53, &SelectClass);
        if(!SelectGadgetBase)
        {
            IUtility->SNPrintf(textbuf, MAX_TXTB, GetString(&li,MSG_ERROR_LIB),"select.gadget",53);
            DoMessage(textbuf, REQIMAGE_ERROR, NULL);
            return FALSE;
        }
        ISelectG = (struct SelectGIFace *)IExec->GetInterface(SelectGadgetBase, "main", 1, NULL);
        if(!ISelectG)
        {
            IUtility->SNPrintf(textbuf, MAX_TXTB, GetString(&li,MSG_ERROR_OPEN),"SelectGIFace");
            DoMessage(textbuf, REQIMAGE_ERROR, NULL);
           return FALSE;
        }
    }

    return TRUE;
}


void CloseLibs(void)
{
    //IIntuition->CloseClass(LabelBase);
    //IIntuition->CloseClass(BitMapBase);
    IIntuition->CloseClass(RequesterBase);
    IIntuition->CloseClass(ButtonBase);
    IIntuition->CloseClass(LayoutBase);
    IIntuition->CloseClass(WindowBase);

    IExec->DropInterface( (struct Interface *)ISelectG );
    IIntuition->CloseClass( (APTR)SelectGadgetBase );

    IIntuition->CloseClass( (APTR)ListViewerBase );

    if(ILocale)
    {
        ILocale->CloseCatalog(li.li_Catalog);
        IExec->DropInterface( (struct Interface *)ILocale );
    }
    IExec->CloseLibrary( (struct Library *)LocaleBase );

    IExec->DropInterface( (struct Interface *)IIcon );
    IExec->CloseLibrary(IconBase);
    IExec->DropInterface( (struct Interface *)IUtility );
    IExec->CloseLibrary(UtilityBase);
    IExec->DropInterface( (struct Interface *)IIntuition );
    IExec->CloseLibrary(IntuitionBase);
    //IExec->DropInterface( (struct Interface *)IGraphics );
    //IExec->CloseLibrary(GfxBase);
}


//#include <proto/graphics.h>
/* DefMousePointer is in WB mode */
void DMP_from_WB(void)
{
    STRPTR theme, columninfo[TOT_COL];
    int32 pointers_tot;
    struct Node *node = NULL;
    struct List *chooser_list = NULL;

    //listviewer_list = IExec->AllocSysObject(ASOT_LIST, NULL);

/*
#define RGB8to32(RGB) ( (uint32)(RGB) * 0x01010101UL )
     struct Screen *MainScreen = FrontMostScr();
     struct ColorMap *cm = MainScreen->ViewPort.ColorMap;
     uint32 PenArray = IGraphics->ObtainBestPenA(cm, //0x2F, 0x66, 0xAF,
                                  RGB8to32( (0x2F66AF >> 16) & 255 ), // Red
                                  RGB8to32( (0x2F66AF >> 8)  & 255 ), // Green
                                  RGB8to32(  0x2F66AF        & 255 ), // Blue
                                 TAG_DONE);
*/

    columninfo[0] = (STRPTR)GetString(&li,MSG_GUI_LB_IMAGE);
    columninfo[1] = (STRPTR)GetString(&li,MSG_GUI_LB_NAME);
//DBUG("columninfo[0]='%s'\n",columninfo[0]);
//DBUG("columninfo[1]='%s'\n",columninfo[1]);
    // Create list pointers object
    OBJ(OID_LISTPTRS) = IIntuition->NewObject(ListViewerClass, NULL, //"listviewer.gadget",
                         GA_ID,       OID_LISTPTRS,
                         //GA_RelVerify, FALSE,
                         GA_ReadOnly, TRUE,
                         LISTVIEWER_Columns,    TOT_COL,
                         LISTVIEWER_ShowTitle,  TRUE,
                         LISTVIEWER_TitleNames, columninfo,
                         LISTVIEWER_Focus,      TRUE,
                         LISTVIEWER_ShowActive, FALSE,
                         //LISTVIEWER_MinVisible, 5,
//                         LISTVIEWER_Background1_Color, 0xFFAAAAAA,
//                         LISTVIEWER_Background2_Color, 0xFFAAAAAA,
                         LISTVIEWER_Background1_DrPen, BACKGROUNDPEN,
                         LISTVIEWER_Background2_DrPen, BACKGROUNDPEN,
                         LISTVIEWER_HorizScroller_Visibility, LISTVIEWERV_HorizScroller_Visibility_Off,
                        TAG_DONE);
//DBUG("OBJ(OID_LISTPTRS)=0x%08lx\n",OBJ(OID_LISTPTRS));
    chooser_list = IExec->AllocSysObject(ASOT_LIST, NULL);

    // Get themes/drawers list and show 1st in chooser_list
    if( make_chooser_list(chooser_list) )
    {
        node = IExec->GetHead(chooser_list);
        ISelectG->GetSelectNodeAttrs(node, SGNA_Text,&theme, TAG_DONE);
//DBUG("0x%08lx '%s' (1st node)\n",node,theme);
        pointers_tot = ListDMP_WB(theme); // get theme's/drawer's mouse pointers list
        DMPwindow(pointers_tot, chooser_list); // show GUI and process events

        free_chooser_list(chooser_list);
        chooser_list = NULL;
    }
    else
    {
        DoMessage( (STRPTR)GetString(&li, MSG_WARN_NO_POINTERTHEMES), REQIMAGE_ERROR, NULL );
    }

    if( OBJ(OID_LISTPTRS) )
    {
        IIntuition->DisposeObject( OBJ(OID_LISTPTRS) );
    }
    //IExec->FreeSysObject(ASOT_LIST, listviewer_list);
    //listviewer_list = NULL;


/*
    IGraphics->ReleasePen(MainScreen->ViewPort.ColorMap, PenArray);
    PenArray = 0;
*/
}


/*****************************************************************/
/* This part of code is from René W. Olsens' listviewer examples */
static uint32 VARARGS68K AddNode(Object *List, APTR Node, ...)
{
    struct TagItem *TagList;
    va_list ap;
    uint32 pos;

    va_startlinear(ap, Node);
    TagList = va_getlinearva(ap, struct TagItem *);

    pos = IIntuition->DoGadgetMethod( (APTR)List, pwindow, NULL,
                                     LISTVIEWERM_Insert, NULL, Node,-1,
                                    TagList );

    va_end(ap);

    return(pos);
}
/* This part of code is from René W. Olsens' listviewer examples */
/*****************************************************************/


//#define IMG_W 42
//#define IMG_H 42
int32 ListDMP_WB(STRPTR theme_name)
{
 STRPTR theme_drawer_name = IExec->AllocVecTags(MAX_TDRW, TAG_DONE),
        pointer_fullname = IExec->AllocVecTags(MAX_SRC, TAG_DONE), // fullpath + pointer name
        pattern_ms = IExec->AllocVecTags(MAX_PATTERN, TAG_DONE);
 int32 success = 0;
 struct DiskObject *icon = NULL;

 IUtility->Strlcpy(theme_drawer_name, pointers_path, MAX_TDRW);
 IDOS->AddPart(theme_drawer_name, theme_name, MAX_TDRW);

 IDOS->ParsePatternNoCase("def_#?pointer.info", pattern_ms, MAX_PATTERN);

 APTR context = IDOS->ObtainDirContextTags(EX_StringNameInput, theme_drawer_name,
                                           EX_DataFields,  (EXF_NAME|EXF_TYPE),
                                           EX_MatchString, pattern_ms,
                                          TAG_END);
//DBUG("context=0x%08lx\n",context);
 if(context)
 {
  struct ExamineData *dat;
  // Re-read theme drawer, but first "clean" listviewer_list
  IIntuition->DoGadgetMethod(GAD(OID_LISTPTRS), pwindow, NULL, LISTVIEWERM_Clear);

  while( (dat=IDOS->ExamineDir(context)) )
  {
   if( EXD_IS_FILE(dat) )
   {
    IUtility->Strlcpy(pointer_fullname, theme_drawer_name, MAX_SRC);
    IDOS->AddPart(pointer_fullname, dat->Name, MAX_SRC);
    pointer_fullname[IUtility->Strlen(pointer_fullname) - 5] = '\0'; // remove '.info'

    icon = IIcon->GetIconTags(pointer_fullname,
                              ICONGETA_FailIfUnavailable, TRUE,
                              ICONGETA_UseFriendBitMap, TRUE,
                              //ICONGETA_Width,IMG_W,
                              //ICONGETA_Height,IMG_H, 
                              //ICONGETA_ForceScaling, TRUE,
                              //ICONGETA_AllowUpscaling, TRUE,
                             TAG_END);

    success = AddNode(OBJ(OID_LISTPTRS), NULL,
                      LISTVIEWERN_ColumnNr,COL_IMG-1,
                          LISTVIEWERN_Type,  LISTVIEWERV_Type_Image,
                          LISTVIEWERN_Image, icon->do_Gadget.GadgetRender,
                      LISTVIEWERN_ColumnNr,COL_TXT-1,
                          LISTVIEWERN_Type,     LISTVIEWERV_Type_Text,
                          LISTVIEWERN_CopyText, dat->Name,
                          //LISTVIEWERN_TextColor, 0xFFFF0000,
                     TAG_END) + 1;
DBUG("%2ld)MOUSEPOINTER='%s'\n",success,dat->Name);
   }

  }
  if( ERROR_NO_MORE_ENTRIES != IDOS->IoErr() )
  {
   IDOS->Fault(IDOS->IoErr(), "[DMP]ListDMP_WB()", textbuf, MAX_TXTB);
   DoMessage(textbuf, REQIMAGE_ERROR, NULL);
  }

 }
 else
 {
  IDOS->Fault(IDOS->IoErr(), "[DMP]ListDMP_WB()", textbuf, MAX_TXTB);
  DoMessage(textbuf, REQIMAGE_ERROR, NULL);
 }

 IDOS->ReleaseDirContext(context);
 IExec->FreeVec(pattern_ms);
 IExec->FreeVec(theme_drawer_name);
 IExec->FreeVec(pointer_fullname);

 return(success);
}


void DMPwindow(int32 pointers_tot, struct List *list)
{
 Object *win_obj;//, *Objects[LAST_NUM];
 //struct Window *pwindow;
 struct DiskObject *iconify = NULL;
 struct MsgPort *wAppPort = NULL;
 // Used in OBJ(OID_CHOOSER_INFO): MSG_GUI_CHOOSER_INFO has '%ld' as argument
 struct { uint32 value; }va;

 va.value = pointers_tot;
DBUG("pointers_tot=%ld ('%s')\n",pointers_tot,dmp_name);
 // reset icon X/Y positions so it iconifies properly on Workbench
 iconify = IIcon->GetIconTags(dmp_name, ICONGETA_FailIfUnavailable,FALSE, TAG_END);
 iconify->do_CurrentX = NO_ICON_POSITION;
 iconify->do_CurrentY = NO_ICON_POSITION;

 wAppPort = IExec->AllocSysObjectTags(ASOT_PORT, TAG_END);

 win_obj = IIntuition->NewObject(WindowClass, NULL, //"window.class",
         WA_ScreenTitle,  VERS" "DATE,
         WA_Title,        "DefMousePointers",
         WA_DragBar,      TRUE,
         WA_CloseGadget,  TRUE,
         WA_SizeGadget,   TRUE,
         WA_DepthGadget,  TRUE,
         WA_Activate,     TRUE,
         WA_InnerWidth,   300,
         //WA_InnerHeight,  460,
         WINDOW_Position, WPOS_CENTERSCREEN,
         WINDOW_IconifyGadget, TRUE,     // iconify support
         WINDOW_AppPort,       wAppPort, // iconify support
         WINDOW_Icon,          iconify,  // iconify support
         WINDOW_Layout, IIntuition->NewObject(LayoutClass, NULL, //"layout.gadget",
          LAYOUT_Orientation, LAYOUT_ORIENT_VERT,
          LAYOUT_SpaceOuter,  TRUE,

          LAYOUT_AddChild, IIntuition->NewObject(LayoutClass, NULL, //"layout.gadget",
           LAYOUT_Orientation,    LAYOUT_ORIENT_VERT,
           LAYOUT_HorizAlignment, LALIGN_CENTER,
           LAYOUT_BevelStyle,     BVS_GROUP,
           LAYOUT_Label,          GetString(&li, MSG_GUI_GROUP_CHOOSER),
           LAYOUT_AddChild, OBJ(OID_CHOOSER) = IIntuition->NewObject(SelectClass, NULL,//ChooserClass, NULL, //"chooser.gadget",
            GA_ID,         OID_CHOOSER,
            GA_RelVerify,  TRUE,
            GA_Underscore, ~0,
            SELECT_LabelList, list, // themes/drawers list
            SELECT_Active,    0,
           TAG_DONE),
           CHILD_WeightedWidth, 0,
           LAYOUT_AddChild, OBJ(OID_CHOOSER_INFO) = IIntuition->NewObject(ButtonClass, NULL,
             GA_ReadOnly, TRUE,
             GA_Text,     GetString(&li,MSG_GUI_CHOOSER_INFO),
             BUTTON_VarArgs, &va,
           TAG_DONE),
          TAG_DONE),
          CHILD_WeightedHeight, 0,

          LAYOUT_AddChild, IIntuition->NewObject(LayoutClass, NULL, //"layout.gadget",
           LAYOUT_Orientation, LAYOUT_ORIENT_VERT,
           LAYOUT_SpaceOuter,  TRUE,
           LAYOUT_BevelStyle,  BVS_GROUP,
           LAYOUT_Label,       GetString(&li, MSG_GUI_GROUP_LB),
           LAYOUT_AddChild, OBJ(OID_LISTPTRS),
           CHILD_MinHeight, 300,
          TAG_DONE),

          LAYOUT_AddChild, IIntuition->NewObject(LayoutClass, NULL, //"layout.gadget",
           LAYOUT_Orientation, LAYOUT_ORIENT_HORIZ,
           LAYOUT_SpaceOuter,  TRUE,
           LAYOUT_AddChild, IIntuition->NewObject(ButtonClass, NULL, //"button.gadget",
            GA_Text,      GetString(&li, MSG_GUI_SAVE),
            GA_ID,        OID_SAVE,
            GA_RelVerify, TRUE,
           TAG_DONE),
           LAYOUT_AddChild, IIntuition->NewObject(ButtonClass, NULL, //"button.gadget",
            GA_Text,      GetString(&li, MSG_GUI_USE),
            GA_ID,        OID_USE,
            GA_RelVerify, TRUE,
           TAG_DONE),
           LAYOUT_AddChild, IIntuition->NewObject(ButtonClass, NULL, //"button.gadget",
            GA_Text,      GetString(&li, MSG_GUI_QUIT),
            GA_ID,        OID_QUIT,
            GA_RelVerify, TRUE,
           TAG_DONE),
          TAG_DONE),
          CHILD_WeightedHeight, 0,

         TAG_DONE),
        TAG_END);

 if( (pwindow=(struct Window *)IIntuition->IDoMethod(win_obj, WM_OPEN, NULL)) )
 {
  BOOL done = TRUE;
  char *res_string = NULL;
  uint16 code = 0;
  uint32 siggot = 0, result = WMHI_LASTMSG, res_node, res_value,
         wsigmask = 0, prev_chooser = 0;

  while(done)
  {
   IIntuition->GetAttr(WINDOW_SigMask, win_obj, &wsigmask);
   siggot = IExec->Wait(wsigmask|SIGBREAKF_CTRL_C);

   if(siggot & SIGBREAKF_CTRL_C)
   {
    done = FALSE;
    break;
   }

   while( (result=IIntuition->IDoMethod(win_obj, WM_HANDLEINPUT, &code)) != WMHI_LASTMSG )
   {
    switch(result & WMHI_CLASSMASK) {
     case WMHI_CLOSEWINDOW:
      done = FALSE;
     break;

     case WMHI_ICONIFY:
     {
DBUG("WMHI_ICONIFY\n");
      if( IIntuition->IDoMethod(win_obj, WM_ICONIFY) )
      {
       pwindow = NULL;
      }
     break;
     }

     case WMHI_UNICONIFY:
DBUG("WMHI_UNICONIFY\n");
      pwindow = (struct Window *)IIntuition->IDoMethod(win_obj, WM_OPEN, NULL);
     break;

     case WMHI_GADGETUP:
     {
      IIntuition->GetAttr(SELECT_ActiveNode, OBJ(OID_CHOOSER), (uint32 *)&res_node);
      ISelectG->GetSelectNodeAttrs( (struct Node *)res_node, SGNA_Text,&res_string, TAG_DONE );

      switch(result & WMHI_GADGETMASK)
      {
       case OID_SAVE:
       {
DBUG("OID_SAVE\n");
        BPTR file = 0;
        STRPTR prefs_file = NULL;//IExec->AllocVecTags(MAX_PREF, TAG_DONE);

        done = FALSE;

        IUtility->SNPrintf(textbuf, MAX_TXTB, GetString(&li,MSG_SAVE_WARNING),res_string);
        APTR lock = SleepWindow(pwindow);
        res_value = DoMessage(textbuf, REQIMAGE_WARNING, (STRPTR)GetString(&li,MSG_CONT_CANCEL_GAD));
        WakeWindow(pwindow, lock);
        if(res_value == 0)
        {
         done = TRUE;
         break;
        }

        prefs_file = IExec->AllocVecTags(MAX_PREF, TAG_DONE);
        IUtility->Strlcpy(prefs_file, pointers_path, MAX_PREF);
        IDOS->AddPart(prefs_file, PREFS_FILENAME, MAX_PREF);
        if( (file=IDOS->Open(prefs_file, MODE_NEWFILE)) )
        {
         IDOS->FPrintf(file, "%s",res_string);
         IDOS->Close(file);
        }
        IExec->FreeVec(prefs_file);

        if(DeletePointers() == 0)
        {
         DoMessage( (STRPTR)GetString(&li,MSG_ERROR_DEL_POINTERS), REQIMAGE_WARNING, NULL );
         done = TRUE;
        }

        if(CopyPointers(res_string) != 0)
        {
         APTR lock = SleepWindow(pwindow);
         DoMessage( (STRPTR)GetString(&li,MSG_ERROR_COPY_POINTERS), REQIMAGE_WARNING, NULL );
         WakeWindow(pwindow, lock);
         done = TRUE;
        }
       break;
       }

       case OID_USE:
DBUG("OID_USE\n");
DBUG("DELETING pointers not from theme '%s'\n",res_string);
        if(DeletePointers() == 0)
        {
         DoMessage( (STRPTR)GetString(&li,MSG_ERROR_DEL_POINTERS), REQIMAGE_INFO, NULL );
        }

        if(CopyPointers(res_string) != 0)
        {
         APTR lock = SleepWindow(pwindow);
         DoMessage( (STRPTR)GetString(&li,MSG_ERROR_COPY_POINTERS), REQIMAGE_WARNING, NULL );
         WakeWindow(pwindow, lock);
        }
       break;

       case OID_QUIT:
DBUG("OID_QUIT\n");
        done = FALSE;
       break;

       case OID_CHOOSER:
       {
DBUG("OID_CHOOSER: %ld (was=%ld)\n",code,prev_chooser);
        if(prev_chooser == code) break;// selector/chooser not changed! -> do nothing!
DBUG("\t0x%08lx '%s'\n[DETACHING LB]\n",res_node,res_string);
        // Detach the listbrowser list first
        IIntuition->SetAttrs(OBJ(OID_LISTPTRS), LISTVIEWER_Quiet,TRUE, TAG_END);

        va.value = ListDMP_WB(res_string); // get new theme's/drawer's mouse pointers list
        // Re-attach the listbrowser
DBUG("\t[RE-ATTACHING LB] %ld\n",&va.value);
        // Sort and refresh new listbrowser and chooser info
//        IIntuition->DoGadgetMethod(GAD(OID_LISTPTRS), pwindow, NULL,
//                                   LBM_SORT, NULL, COL_TXT, LBMSORT_FORWARD, NULL);
        IIntuition->RefreshSetGadgetAttrs(GAD(OID_LISTPTRS), pwindow, NULL,
                                          LISTVIEWER_Quiet,FALSE, TAG_END);

        IIntuition->RefreshSetGadgetAttrs(GAD(OID_CHOOSER_INFO), pwindow, NULL,
                                          BUTTON_VarArgs,&va, TAG_DONE);
        prev_chooser = code;
       break;
       }

/*       case OID_LISTPTRS:
       {
        IIntuition->GetAttr(LISTBROWSER_RelEvent, OBJ(OID_LISTPTRS), &res_value);
        if(res_value == LBRE_DOUBLECLICK) {
DBUG("OID_LISTPTRS: %ld\n",code);
         IIntuition->GetAttr(LISTBROWSER_SelectedNode, OBJ(OID_LISTPTRS), (uint32 *)&res_node);
         IListBrowser->GetListBrowserNodeAttrs( (struct Node *)res_node, LBNA_Column,COL_TXT,
                                                LBNCA_Text,&res_string, TAG_DONE );
DBUG("\t0x%08lx '%s'\n",res_node,res_string);
        }
       break;
       }*/
      } // END switch(result & WMHI_GADGETMASK)
     break;
     } // END case WMHI_GADGETUP:
    } // END switch(result & WMHI_CLASSMASK)
   } // END while( (result..
  } // END while(done)
 } // END if( (pwindow..

 IExec->FreeSysObject(ASOT_PORT, wAppPort);
 IIntuition->DisposeObject(win_obj);
 win_obj = NULL;
 pwindow = NULL;
 OBJ(OID_LISTPTRS) = NULL;
}


/* sort_chooslist() comparation hook */
int CompareNameNodes(struct Hook *hook, struct Node *node1, struct Node *node2)
{
 STRPTR name1, name2;

 ISelectG->GetSelectNodeAttrs(node1, SGNA_Text,&name1, TAG_DONE);
 ISelectG->GetSelectNodeAttrs(node2, SGNA_Text,&name2, TAG_DONE);
DBUG("[0x%08lx]'%s' <-?-> [0x%08lx]'%s'\n",node1,name1,node2,name2);
 return( IUtility->Stricmp(name1,name2) );
}

/* sort_chooslist() by Fredrik 'salas00' Wikstrom */
void sort_chooserlist(int32 n, struct List *list, struct Hook *cmphook)
{
 struct Node *curr, *next;
 int32 i, j;
DBUG("SORTING chooser_list:\n");
 for(i=0; i<n; i++)
 {
  curr = IExec->GetHead(list);
  for(j=0; j<n-1-i; j++)
  {
   next = IExec->GetSucc(curr);
   if( (int)IUtility->CallHookPkt(cmphook,curr,next) > 0 )
   //if(CompareNameNodes(curr,next) > 0)
   {
    IExec->Remove(curr);
    IExec->Insert(list, curr, next);
   }
   else
   {
    curr = next;
   }
  }
 }
}

/* sort_chooslist() by Thomas 'thomas' Rapp */
/*void sort_list(struct List *list, int (*compar)(const void*, const void*))
{
 struct Node **array, *node,*next;
 long n, i;

 n = 0;
 for(node=list->lh_Head; (next=node->ln_Succ); node=next)
  n++;

 if( (array=malloc(n * sizeof(*array))) ) {
  for(i=0; i<n; i++)
   array[i] = RemHead (list);

  qsort(array, n, sizeof(*array), compar);

  for(i=0; i<n; i++)
   AddTail(list, array[i]);

  free(array);
 }
}
int compare_nodes(const struct Node **n1, const struct Node **n2)
{
 return( Stricmp((*n1)->ln_Name,(*n2)->ln_Name) );
}*/

/* sort_chooslist() by 'OldFart' */
/*void sort_chooserlist(int32 n, struct Hook *cmphook) {
 struct Node *Curr = IExec->GetHead(&chooser_list);

 if(Curr != NULL) {
  struct Node *Next = IExec->GetSucc(Curr),
              *Prev;

  while(Next != NULL) {
   if( (int)IUtility->CallHookPkt(cmphook,Curr,Next) > 0 ) {
    IExec->Remove(Curr);
    IExec->Insert(&chooser_list, Curr, Next);
    Prev = IExec->GetPred(Next);

    while( (Prev != NULL) &&
           ((int)IUtility->CallHookPkt(cmphook,Prev,Next) > 0) ) {
     IExec->Remove(Prev);
     IExec->Insert(&chooser_list, Prev, Next);
     Prev = IExec->GetPred(Next);
    }
   }
   Next = IExec->GetSucc(Curr);
  }
 }
}*/

/* sort_chooslist() */
/*void sort_chooserlist(int32 n) {
 struct Node *n1, *n2, *n0 = IExec->GetHead(&chooser_list);
 STRPTR name1, name2;
 int32 i, j;

 for(i=n; i>0; i--) { // traverse all the chooser_list
#ifdef _DEBUG_
IChooser->GetChooserNodeAttrs(n0, CNA_Text,&name1, TAG_DONE);
IDOS->Printf("i%ld:[0x%08lx]'%s'\n",i,n0,name1);
#endif
  n2 = IExec->GetHead(&chooser_list);
  for(j=1; j<i; j++) {
   n2=IExec->GetSucc(n2);
   IChooser->GetChooserNodeAttrs(n2, CNA_Text,&name2, TAG_DONE);
   n1 = IExec->GetPred(n2);
   IChooser->GetChooserNodeAttrs(n1, CNA_Text,&name1, TAG_DONE);
#ifdef _DEBUG_
IDOS->Printf(" == j%ld:[0x%08lx]'%s'\n",j,n2,name2);
#endif
   if(IUtility->Stricmp(name1, name2) > 0) // name-compare of node1 and node2 
   {
#ifdef _DEBUG_
IDOS->Printf("SWAP [0x%08lx]'%s' <-> [0x%08lx]'%s'\n",n1,name1,n2,name2);
#endif
    IExec->Remove(n1);
    IExec->Insert(&chooser_list, n1, n2);
   }
  }
  n0 = IExec->GetSucc(n0);
 }
}
*/


/* Create chooser list only with drawers contaning 'def_#?pointer.info' */
int32 make_chooser_list(struct List *list)
{
 struct Node *node;
 char text_err[100];
 int32 themes_tot = 0;

 APTR context = IDOS->ObtainDirContextTags(EX_StringNameInput, pointers_path,
                                           EX_DataFields, (EXF_NAME|EXF_TYPE),
                                          TAG_END);
 if(context)
 {
  struct ExamineData *dat;
//DBUG("list=0x%08lx (0x%08lx)\n",list,chooser_list);
DBUG("list=0x%08lx\n",list);
  while( (dat=IDOS->ExamineDir(context)) )
  {
   if( EXD_IS_DIRECTORY(dat) )
   {
DBUG("DRAWERNAME[%ld]='%s'\n",themes_tot,dat->Name);
    if( ListDMP_WB(dat->Name) ) // lets see if it has pointer images/icons
    {
     if( (node=ISelectG->AllocSelectNode(SGNA_CopyText,TRUE,
                                         SGNA_Text,dat->Name, TAG_DONE)) )
     {
      IExec->AddTail(list, node);
      themes_tot++;
DBUG("DRAWERNAME[%ld]='%s' added to chooser [0x%08lx]\n",themes_tot,dat->Name,node);
     }
    }
   }
  }
  if( ERROR_NO_MORE_ENTRIES != IDOS->IoErr() )
  {
   IDOS->Fault(IDOS->IoErr(), "[DMP]make_chooser_list()", textbuf, MAX_TXTB);
   DoMessage(textbuf, REQIMAGE_WARNING, NULL);
  }
  if(themes_tot != 0)
  {
   struct Hook *prhook = (struct Hook *)IExec->AllocSysObjectTags(ASOT_HOOK, ASOHOOK_Entry,CompareNameNodes, TAG_DONE);
   sort_chooserlist(themes_tot, list, prhook); // sort chooser_list by name
   IExec->FreeSysObject(ASOT_HOOK, prhook);
  }
 }
 else
 {
  IDOS->Fault(IDOS->IoErr(), NULL, text_err, 100);
  IUtility->SNPrintf(textbuf, MAX_TXTB, GetString(&li,MSG_ERROR_POINTERSDRAWER),text_err);
  DoMessage(textbuf, REQIMAGE_WARNING, NULL);
 }

 IDOS->ReleaseDirContext(context);

 return(themes_tot);
}


void free_chooser_list(struct List *list)
{
    struct Node *node, *nextnode;

    node = list->lh_Head;
    while( (nextnode=node->ln_Succ) )
    {
        ISelectG->FreeSelectNode(node);
        node = nextnode;
    }
//DBUG("list=0x%08lx\n",list);
    IExec->FreeSysObject(ASOT_LIST, list);
    list = NULL;
}


/*****************************************************************/
/* This part of code is from René W. Olsens' listviewer examples */
static void VARARGS68K GetNodeData(struct lvNode *Node, uint32 Column, ...)
{
    struct TagItem *TagList;
    va_list ap;

    va_startlinear(ap, Column);

    TagList = va_getlinearva(ap, struct TagItem *);

    IIntuition->DoGadgetMethod(GAD(OID_LISTPTRS), pwindow, NULL,
                               LISTVIEWERM_GetColumnNode, NULL, Node, Column, TagList);

    va_end(ap);
}
/* This part of code is from René W. Olsens' listviewer examples */
/*****************************************************************/


/* Delete pointers not part of the new selected theme */
int32 DeletePointers(void)
{
    STRPTR res_string,
           pfile = IExec->AllocVecTags(MAX_FILE, TAG_DONE);
    int32 i, success = 1;
    //struct Node *n = NULL;

    //n = IExec->GetHead(listviewer_list);

    for(i=0; xml_pointers[i]; i++)
    {// List of all available pointers
        GetNodeData(LISTVIEWERV_GetColumnNode_Title, COL_TXT, LISTVIEWERN_Text,&res_string, TAG_END);
DBUG("%ld) '%s' == '%s';\n",i+1,xml_pointers[i],res_string);
        if(IUtility->Strnicmp(xml_pointers[i], res_string, IUtility->Strlen(res_string)) != 0)
        //{
        //    n = IExec->GetSucc(n); // avoid "re-compare" matched node (pointer name)
        //}
        //else
        {
            IUtility->Strlcpy(pfile, "ENV:Sys/", MAX_FILE);
            IDOS->AddPart(pfile, xml_pointers[i], MAX_FILE);
            success &= IDOS->Delete(pfile);
DBUG("%ld)IDOS->Delete(%s);\n",i+1,pfile);
        }
    }

    IExec->FreeVec(pfile);
    return success;
}


/* Get list of pointers and copy them to 'ENV:Sys/' */
int CopyPointers(STRPTR theme_drawer)
{
    char *res_string = NULL;
    STRPTR src_file = IExec->AllocVecTags(MAX_SRC, TAG_DONE),
           dst_file = IExec->AllocVecTags(MAX_DST, TAG_DONE);
    int errors = 0;
    uint32 tot = 0;

    IIntuition->GetAttr(LISTVIEWER_Entries, OBJ(OID_LISTPTRS), &tot);
    //struct Node *node = IExec->GetHead(listviewer_list);
DBUG("COPYING pointer theme '%s' to 'ENV:Sys/'\n",theme_drawer);
    // Get SOURCE and DESTINATION files to be copied
    //for(; node!=NULL; node=IExec->GetSucc(node) )
    while(tot--)
    {
        GetNodeData(LISTVIEWERV_GetColumnNode_Title, COL_TXT, LISTVIEWERN_Text,&res_string, TAG_END);
DBUG("\t'%s'\n",res_string);
        IUtility->Strlcpy(src_file, pointers_path, MAX_SRC);
        IDOS->AddPart(src_file, theme_drawer, MAX_SRC);
        IDOS->AddPart(src_file, res_string, MAX_SRC);

        IUtility->Strlcpy(dst_file, "ENV:Sys/", MAX_DST);
        IDOS->AddPart(dst_file, res_string, MAX_SRC);

        errors |= BufferedCopy(src_file, dst_file);
    }

    IExec->FreeVec(src_file);
    IExec->FreeVec(dst_file);

    return errors;
}


/*****************************************/
/* This piece of code is by Tuomas Hokka */
#define BUFF_SIZE 32768
BOOL BufferedCopy(CONST_STRPTR source, CONST_STRPTR target)
{
    BPTR src, tgt;
    BOOL error = TRUE;
    STRPTR buff;
    uint32 readsize;
DBUG("SRC:'%s'\n",source);
DBUG("TO:'%s'\n",target);
    tgt = IDOS->FOpen(target, MODE_NEWFILE, 0);
    if(tgt != ZERO)
    {
        src = IDOS->FOpen(source, MODE_OLDFILE, 0);
        if(src != ZERO)
        {
            buff = IExec->AllocVecTags(BUFF_SIZE, AVT_Type,MEMF_SHARED, TAG_DONE);
            if(buff != 0)
            {
                readsize = IDOS->FRead(src, buff, 1, BUFF_SIZE);
                while(readsize == BUFF_SIZE)
                {
                    IDOS->FWrite(tgt, buff, 1, readsize);
                    readsize = IDOS->FRead(src, buff, 1, BUFF_SIZE);
                }
                IDOS->FWrite(tgt, buff, 1, readsize);
                IExec->FreeVec(buff);
                error = FALSE;
            }
            else
            {
                if(WBenchMsg)
                {// WB or CLI/SHell
                    DoMessage( (STRPTR)GetString(&li,MSG_ERROR_BCOPY_MEM), REQIMAGE_ERROR, NULL );
                }
                else
                {
                    //IDOS->Printf( "%s\n",GetString(&li,MSG_ERROR_BCOPY_MEM) );
                    IDOS->PutErrStr( GetString(&li,MSG_ERROR_BCOPY_MEM) );
                }
            }

            IDOS->FClose(src);
        }
        else
        {
            IDOS->PrintFault(IDOS->IoErr(),"[DMP]BufferedCopy():src");
        }

        IDOS->FClose(tgt);
    }
    else
    {
        IDOS->PrintFault(IDOS->IoErr(),"[DMP]BufferedCopy():tgt");
    }

    return error;
}
/* This piece of code is by Tuomas Hokka */
/*****************************************/


/* Don't let user act on window if a requester (DoMessage) is shown */
APTR SleepWindow(struct Window *win)
{
    struct Requester *lock = NULL;

    if(win)
    {
        if( (lock=IExec->AllocVecTags(sizeof(struct Requester), AVT_ClearWithValue,0, TAG_DONE)) )
        {
            IIntuition->InitRequester(lock);
            IIntuition->Request(lock, win);
            if(win->FirstRequest == lock)
            {
                IIntuition->SetWindowPointer(win, WA_BusyPointer,TRUE, TAG_END);
            }
            else
            {
                IExec->FreeVec(lock);
                lock = NULL;
            }
        }
    }

    return(lock);
}


/* Re-activate Sleep'ed Window */
void WakeWindow(struct Window *win, APTR lock)
{
    if(lock)
    {
        IIntuition->EndRequest(lock, win);
        IExec->FreeVec(lock);
        IIntuition->SetWindowPointer(win, TAG_END);
    }
}


/* Show Errors/Warnings/..., used only in WB mode */
uint32 DoMessage(char *message, char reqtype, STRPTR buttons)
{
    uint32 button;
    Object *requester = IIntuition->NewObject(RequesterClass, NULL, //"requester.class",
                                     REQ_Image,      reqtype,
                                     REQ_TitleText,  "DefMousePointers",
                                     REQ_BodyText,   message,
                                     REQ_GadgetText, buttons? buttons : GetString(&li,MSG_OK_GAD),
                                     //REQ_StayOnTop, TRUE,
                                    TAG_DONE);

    button = IIntuition->IDoMethod( requester, RM_OPENREQ, NULL, NULL, FrontMostScr() );
    IIntuition->DisposeObject(requester);

    return button;
}


/*uint32 DoMessage(char *message, char reqtype, STRPTR buttons)
{
 int32 button;
 struct TagItem es_ti[] = { {ESA_Position, REQPOS_CENTERWINDOW}, {TAG_DONE, 0} };
 struct EasyStruct es;

 IUtility->ClearMem( &es, sizeof(es) );
 es.es_StructSize = sizeof(struct EasyStruct);
 es.es_Flags = ESF_TAGGED;
 //es.es_Title = "DefMousePointers";
 es.es_TagList = es_ti;

 button = IDOS->TimedDosRequesterTags(TDR_TitleString,  "DefMousePointers",
                                      TDR_GadgetString, buttons? buttons : GetString(&li,MSG_OK_GAD),
                                      TDR_FormatString, message,
                                      TDR_ImageType,    TDRIMAGE_ERROR,
                                      TDR_Window,       pwindow,
                                      TDR_EasyStruct,   &es,
                                      //TDR_NonBlocking, TRUE,
                                     TAG_DONE);

 return button;
}*/


/* Get screen at front, used only in DoMessage() */
struct Screen *FrontMostScr(void)
{
 struct Screen *front_screen_address, *public_screen_address = NULL;
 ULONG intuition_lock;
 struct List *public_screen_list;
 struct PubScreenNode *public_screen_node;

 intuition_lock = IIntuition->LockIBase(0L);

 front_screen_address = ((struct IntuitionBase *)IntuitionBase)->FirstScreen;
 if( (front_screen_address->Flags & PUBLICSCREEN) || (front_screen_address->Flags & WBENCHSCREEN) )
 {
  IIntuition->UnlockIBase(intuition_lock);

  public_screen_list = IIntuition->LockPubScreenList();
  public_screen_node = (struct PubScreenNode *)public_screen_list->lh_Head;
  while(public_screen_node)
  {
   if(public_screen_node->psn_Screen == front_screen_address)
   {
    public_screen_address = public_screen_node->psn_Screen;
    break;
   }

   public_screen_node = (struct PubScreenNode *)public_screen_node->psn_Node.ln_Succ;
  }

  IIntuition->UnlockPubScreenList();
 }
 else
 {
  IIntuition->UnlockIBase(intuition_lock);
 }

 if(!public_screen_address)
 {
  public_screen_address = IIntuition->LockPubScreen(NULL);
  IIntuition->UnlockPubScreen(NULL, public_screen_address);
 }

//DBUG("%lx\n", (int)public_screen_address);
 return(public_screen_address);
}
