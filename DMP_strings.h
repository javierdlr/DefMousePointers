#ifndef DMP_STRINGS_H
#define DMP_STRINGS_H


/****************************************************************************/


/* This file was created automatically by CatComp.
 * Do NOT edit by hand!
 */


#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

#ifdef CATCOMP_CODE
#ifndef CATCOMP_BLOCK
#define CATCOMP_ARRAY
#endif
#endif

#ifdef CATCOMP_ARRAY
#ifndef CATCOMP_NUMBERS
#define CATCOMP_NUMBERS
#endif
#ifndef CATCOMP_STRINGS
#define CATCOMP_STRINGS
#endif
#endif

#ifdef CATCOMP_BLOCK
#ifndef CATCOMP_STRINGS
#define CATCOMP_STRINGS
#endif
#endif


/****************************************************************************/


#ifdef CATCOMP_NUMBERS

#define MSG_GUI_GROUP_CHOOSER 1
#define MSG_GUI_GROUP_LB 2
#define MSG_GUI_LB_NAME 3
#define MSG_GUI_LB_IMAGE 4
#define MSG_GUI_SAVE 5
#define MSG_GUI_USE 6
#define MSG_GUI_QUIT 7
#define MSG_GUI_CHOOSER_INFO 8
#define MSG_OK_GAD 9
#define MSG_CONT_CANCEL_GAD 10
#define MSG_ERROR_NOGUI 11
#define MSG_ERROR_BCOPY_MEM 12
#define MSG_ERROR_LIB 13
#define MSG_ERROR_OPEN 14
#define MSG_ERROR_COPY_POINTERS 15
#define MSG_ERROR_DEL_POINTERS 16
#define MSG_SAVE_WARNING 17
#define MSG_WARN_NO_POINTERTHEMES 18
#define MSG_WARN_NOPREFSFILE 19
#define MSG_ERROR_POINTERSDRAWER 20
#define MSG_WARN_NO_POINTERFILES 21
#define MSG_ERROR_COPY_POINTERTHEME 22
#define MSG_CLI_TRANSLATOR 23
#define MSG_CLI_ABOUT 24
#define MSG_CLI_WRONG_ARGS 25
#define MSG_CLI_INFO 26
#define MSG_CLI_FAILED_CENTERMOUSE 27

#endif /* CATCOMP_NUMBERS */


/****************************************************************************/


#ifdef CATCOMP_STRINGS

#define MSG_GUI_GROUP_CHOOSER_STR "Mouse Pointers Themes"
#define MSG_GUI_GROUP_LB_STR "Available Mouse Pointers"
#define MSG_GUI_LB_NAME_STR "Name"
#define MSG_GUI_LB_IMAGE_STR "Image"
#define MSG_GUI_SAVE_STR "_Save"
#define MSG_GUI_USE_STR "_Use"
#define MSG_GUI_QUIT_STR "_Quit"
#define MSG_GUI_CHOOSER_INFO_STR "Selected theme has %ld mouse pointers"
#define MSG_OK_GAD_STR "_OK"
#define MSG_CONT_CANCEL_GAD_STR "C_ontinue|_Cancel"
#define MSG_ERROR_NOGUI_STR "Failed to open ReAction classes."
#define MSG_ERROR_BCOPY_MEM_STR "Can't allocate memory for copying pointers.\n"
#define MSG_ERROR_LIB_STR "Can't open '%s' V%ld!"
#define MSG_ERROR_OPEN_STR "Can't open '%s'!"
#define MSG_ERROR_COPY_POINTERS_STR "Errors while copying new theme pointer."
#define MSG_ERROR_DEL_POINTERS_STR "\033cSome unused pointers couldn't be deleted.\n(non existing in 'ENV:Sys/')"
#define MSG_SAVE_WARNING_STR "\033cPointers not being part of\n\033b%s \033ntheme\nwill be deleted on your system!"
#define MSG_WARN_NO_POINTERTHEMES_STR "\033cCan't load pointer themes,\nclosing DefMousePointers."
#define MSG_WARN_NOPREFSFILE_STR "No prefs file '%s' found!\n"
#define MSG_ERROR_POINTERSDRAWER_STR "\033cFailed to load 'POINTERS' drawer:\n%s."
#define MSG_WARN_NO_POINTERFILES_STR "\033cAt least one pointer theme drawer seems\nempty, closing DefMousePointers."
#define MSG_ERROR_COPY_POINTERTHEME_STR "Failed to load pointer theme '%s': %s.\n"
#define MSG_CLI_TRANSLATOR_STR "\0"
#define MSG_CLI_ABOUT_STR "Copy pointer theme (previously saved with its GUI) to 'ENV:Sys/'.\nLaunch it from WB to get a nice ReAction GUI.\n"
#define MSG_CLI_WRONG_ARGS_STR "Wrongs args!\n"
#define MSG_CLI_INFO_STR "Copied '%s' pointer theme with %ld pointers to '%s'.\n"
#define MSG_CLI_FAILED_CENTERMOUSE_STR "Failed to center mouse pointer!"

#endif /* CATCOMP_STRINGS */


/****************************************************************************/


#ifdef CATCOMP_ARRAY

struct CatCompArrayType
{
    LONG         cca_ID;
    CONST_STRPTR cca_Str;
};

STATIC CONST struct CatCompArrayType CatCompArray[] =
{
    {MSG_GUI_GROUP_CHOOSER,(CONST_STRPTR)MSG_GUI_GROUP_CHOOSER_STR},
    {MSG_GUI_GROUP_LB,(CONST_STRPTR)MSG_GUI_GROUP_LB_STR},
    {MSG_GUI_LB_NAME,(CONST_STRPTR)MSG_GUI_LB_NAME_STR},
    {MSG_GUI_LB_IMAGE,(CONST_STRPTR)MSG_GUI_LB_IMAGE_STR},
    {MSG_GUI_SAVE,(CONST_STRPTR)MSG_GUI_SAVE_STR},
    {MSG_GUI_USE,(CONST_STRPTR)MSG_GUI_USE_STR},
    {MSG_GUI_QUIT,(CONST_STRPTR)MSG_GUI_QUIT_STR},
    {MSG_GUI_CHOOSER_INFO,(CONST_STRPTR)MSG_GUI_CHOOSER_INFO_STR},
    {MSG_OK_GAD,(CONST_STRPTR)MSG_OK_GAD_STR},
    {MSG_CONT_CANCEL_GAD,(CONST_STRPTR)MSG_CONT_CANCEL_GAD_STR},
    {MSG_ERROR_NOGUI,(CONST_STRPTR)MSG_ERROR_NOGUI_STR},
    {MSG_ERROR_BCOPY_MEM,(CONST_STRPTR)MSG_ERROR_BCOPY_MEM_STR},
    {MSG_ERROR_LIB,(CONST_STRPTR)MSG_ERROR_LIB_STR},
    {MSG_ERROR_OPEN,(CONST_STRPTR)MSG_ERROR_OPEN_STR},
    {MSG_ERROR_COPY_POINTERS,(CONST_STRPTR)MSG_ERROR_COPY_POINTERS_STR},
    {MSG_ERROR_DEL_POINTERS,(CONST_STRPTR)MSG_ERROR_DEL_POINTERS_STR},
    {MSG_SAVE_WARNING,(CONST_STRPTR)MSG_SAVE_WARNING_STR},
    {MSG_WARN_NO_POINTERTHEMES,(CONST_STRPTR)MSG_WARN_NO_POINTERTHEMES_STR},
    {MSG_WARN_NOPREFSFILE,(CONST_STRPTR)MSG_WARN_NOPREFSFILE_STR},
    {MSG_ERROR_POINTERSDRAWER,(CONST_STRPTR)MSG_ERROR_POINTERSDRAWER_STR},
    {MSG_WARN_NO_POINTERFILES,(CONST_STRPTR)MSG_WARN_NO_POINTERFILES_STR},
    {MSG_ERROR_COPY_POINTERTHEME,(CONST_STRPTR)MSG_ERROR_COPY_POINTERTHEME_STR},
    {MSG_CLI_TRANSLATOR,(CONST_STRPTR)MSG_CLI_TRANSLATOR_STR},
    {MSG_CLI_ABOUT,(CONST_STRPTR)MSG_CLI_ABOUT_STR},
    {MSG_CLI_WRONG_ARGS,(CONST_STRPTR)MSG_CLI_WRONG_ARGS_STR},
    {MSG_CLI_INFO,(CONST_STRPTR)MSG_CLI_INFO_STR},
    {MSG_CLI_FAILED_CENTERMOUSE,(CONST_STRPTR)MSG_CLI_FAILED_CENTERMOUSE_STR},
};

#endif /* CATCOMP_ARRAY */


/****************************************************************************/


#ifdef CATCOMP_BLOCK

STATIC CONST UBYTE CatCompBlock[] =
{
    "\x00\x00\x00\x01\x00\x16"
    MSG_GUI_GROUP_CHOOSER_STR "\x00"
    "\x00\x00\x00\x02\x00\x1A"
    MSG_GUI_GROUP_LB_STR "\x00\x00"
    "\x00\x00\x00\x03\x00\x06"
    MSG_GUI_LB_NAME_STR "\x00\x00"
    "\x00\x00\x00\x04\x00\x06"
    MSG_GUI_LB_IMAGE_STR "\x00"
    "\x00\x00\x00\x05\x00\x06"
    MSG_GUI_SAVE_STR "\x00"
    "\x00\x00\x00\x06\x00\x06"
    MSG_GUI_USE_STR "\x00\x00"
    "\x00\x00\x00\x07\x00\x06"
    MSG_GUI_QUIT_STR "\x00"
    "\x00\x00\x00\x08\x00\x26"
    MSG_GUI_CHOOSER_INFO_STR "\x00"
    "\x00\x00\x00\x09\x00\x04"
    MSG_OK_GAD_STR "\x00"
    "\x00\x00\x00\x0A\x00\x12"
    MSG_CONT_CANCEL_GAD_STR "\x00"
    "\x00\x00\x00\x0B\x00\x22"
    MSG_ERROR_NOGUI_STR "\x00\x00"
    "\x00\x00\x00\x0C\x00\x2E"
    MSG_ERROR_BCOPY_MEM_STR "\x00\x00"
    "\x00\x00\x00\x0D\x00\x16"
    MSG_ERROR_LIB_STR "\x00"
    "\x00\x00\x00\x0E\x00\x12"
    MSG_ERROR_OPEN_STR "\x00\x00"
    "\x00\x00\x00\x0F\x00\x28"
    MSG_ERROR_COPY_POINTERS_STR "\x00"
    "\x00\x00\x00\x10\x00\x4A"
    MSG_ERROR_DEL_POINTERS_STR "\x00\x00"
    "\x00\x00\x00\x11\x00\x4A"
    MSG_SAVE_WARNING_STR "\x00"
    "\x00\x00\x00\x12\x00\x38"
    MSG_WARN_NO_POINTERTHEMES_STR "\x00\x00"
    "\x00\x00\x00\x13\x00\x1C"
    MSG_WARN_NOPREFSFILE_STR "\x00\x00"
    "\x00\x00\x00\x14\x00\x28"
    MSG_ERROR_POINTERSDRAWER_STR "\x00"
    "\x00\x00\x00\x15\x00\x4C"
    MSG_WARN_NO_POINTERFILES_STR "\x00\x00"
    "\x00\x00\x00\x16\x00\x28"
    MSG_ERROR_COPY_POINTERTHEME_STR "\x00"
    "\x00\x00\x00\x17\x00\x02"
    MSG_CLI_TRANSLATOR_STR "\x00"
    "\x00\x00\x00\x18\x00\x72"
    MSG_CLI_ABOUT_STR "\x00\x00"
    "\x00\x00\x00\x19\x00\x0E"
    MSG_CLI_WRONG_ARGS_STR "\x00"
    "\x00\x00\x00\x1A\x00\x36"
    MSG_CLI_INFO_STR "\x00"
    "\x00\x00\x00\x1B\x00\x20"
    MSG_CLI_FAILED_CENTERMOUSE_STR "\x00"
};

#endif /* CATCOMP_BLOCK */


/****************************************************************************/


#ifndef PROTO_LOCALE_H
 #ifndef __NOLIBBASE__
  #define _NLB_DEFINED_
  #define __NOLIBBASE__
 #endif
 #ifndef __NOGLOBALIFACE__
  #define _NGI_DEFINED_
  #define __NOGLOBALIFACE__
 #endif
 #include <proto/locale.h>
 #ifdef _NLB_DEFINED_
  #undef __NOLIBBASE__
  #undef _NLB_DEFINED_
 #endif
 #ifdef _NGI_DEFINED_
  #undef __NOGLOBALIFACE__
  #undef _NGI_DEFINED_
 #endif
#endif

struct LocaleInfo
{
#ifndef __amigaos4__
    struct Library     *li_LocaleBase;
#else
    struct LocaleIFace *li_ILocale;
#endif
    struct Catalog     *li_Catalog;
};


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CONST_STRPTR GetString(struct LocaleInfo *li, LONG stringNum);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#ifdef CATCOMP_CODE

CONST_STRPTR GetString(struct LocaleInfo *li, LONG stringNum)
{
#ifndef __amigaos4__
    struct Library     *LocaleBase = li->li_LocaleBase;
#else
    struct LocaleIFace *ILocale    = li->li_ILocale;
#endif
    LONG         *l;
    UWORD        *w;
    CONST_STRPTR  builtIn = NULL;

    l = (LONG *)CatCompBlock;

    while (*l != stringNum && l < (LONG *)(&CatCompBlock[sizeof(CatCompBlock)]))
    {
        w = (UWORD *)((ULONG)l + 4);
        l = (LONG *)((ULONG)l + (ULONG)*w + 6);
    }
    if (*l == stringNum)
    {
        builtIn = (CONST_STRPTR)((ULONG)l + 6);
    }

#ifndef __amigaos4__
    if (LocaleBase)
    {
        return GetCatalogStr(li->li_Catalog, stringNum, builtIn);
    }
#else
    if (ILocale)
    {
#ifdef __USE_INLINE__
        return GetCatalogStr(li->li_Catalog, stringNum, builtIn);
#else
        return ILocale->GetCatalogStr(li->li_Catalog, stringNum, builtIn);
#endif
    }
#endif
    return builtIn;
}


#endif /* CATCOMP_CODE */


/****************************************************************************/


#endif /* DMP_STRINGS_H */
