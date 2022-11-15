#ifndef __DMP_H_
#define __DMP_H_


#include <proto/icon.h>
//#include <proto/application.h>
//#include <proto/datatypes.h>
//#include <proto/graphics.h>
#ifdef _AEON_
	#include <proto/selectgadget.h>
	#include <proto/listviewer.h>
#else
	#include <proto/chooser.h>
	#include <proto/listbrowser.h>
#endif

#include <libraries/gadtools.h>
#include <workbench/icon.h>
#include <workbench/startup.h>
#include <classes/window.h>
#include <gadgets/layout.h>
#ifdef _AEON_
//	#include <gadgets/select.h>
//	#include <gadgets/listviewer.h>
#else
	#include <gadgets/chooser.h>
//	#include <gadgets/listbrowser.h>
#endif
#include <gadgets/button.h>
//#include <images/label.h>
//#include <images/bitmap.h>
#include <classes/requester.h>


#define OBJ(x)  Objects[x]
#define GAD(x)  (struct Gadget *)Objects[x]


enum {
 OID_MAIN = 0,
 OID_LISTPTRS,
 OID_CHOOSER,
 OID_CHOOSER_INFO,
 OID_SAVE,
 OID_USE,
 OID_QUIT,
 LAST_NUM
};

enum {
 COL_IMG = 0,
 COL_TXT,
 TOT_COL
};
//#define TOTALCOLUMNS  COL_LAST


//struct Library *DOSBase;
struct Library *IntuitionBase;
struct Library *UtilityBase = NULL;
struct Library *IconBase = NULL;
//struct Library *DataTypesBase = NULL;
struct Library *LocaleBase = NULL;
//struct Library *GfxBase = NULL;

//struct DOSIFace *IDOS;
struct IntuitionIFace *IIntuition;
struct UtilityIFace *IUtility = NULL;
struct IconIFace *IIcon = NULL;
//struct DataTypesIFace *IDataTypes = NULL;
struct LocaleIFace *ILocale = NULL;
//struct GraphicsIFace *IGraphics = NULL;

#ifdef _AEON_
struct Library *SelectGadgetBase = NULL, *ListViewerBase = NULL;
struct SelectGIFace *ISelectG = NULL;
#else
	struct Library *ChooserBase = NULL, *ListBrowserBase = NULL;
	struct ChooserIFace *IChooser = NULL;
	struct ListBrowserIFace *IListBrowser = NULL;
#endif
// the class library base
struct ClassLibrary *ButtonBase = NULL, *RequesterBase = NULL,
                    *LayoutBase = NULL,  *WindowBase = NULL;
// the class pointer
Class *ButtonClass, *RequesterClass, *LayoutClass, *WindowClass,
#ifdef _AEON_
      *SelectClass, *ListViewerClass;
#else
      *ChooserClass, *ListBrowserClass;
#endif


BOOL OpenLibs(uint32);
void CloseLibs(void);
void DMP_from_WB(void);
void DMPwindow(int32, struct List *, struct Node *);
int32 ListDMP_WB(STRPTR);
int32 make_chooser_list(struct List *);
void free_chooser_list(struct List *);
int32 DeletePointers(void);
int CopyPointers(CONST_STRPTR);
BOOL BufferedCopy(CONST_STRPTR, CONST_STRPTR);
APTR SleepWindow(struct Window *);
void WakeWindow(struct Window *, APTR);
uint32 DoMessage(char *, char, STRPTR);
struct Screen *FrontMostScr(void);
void sort_chooserlist(int32, struct List *, struct Hook *);
struct Node *getSavedThemeNode(struct List *, CONST_STRPTR);
BOOL usePointers(CONST_STRPTR, struct Window *);


#endif
