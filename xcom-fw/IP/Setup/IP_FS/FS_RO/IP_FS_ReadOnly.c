#include <stdio.h>  // For NULL.
#include <string.h> // For memcpy(), strcmp().

#include "IP_FS.h"
#include "WEBS_Conf.h"

#include "assets/Siemens_logo.h" // For SiemensLogo_jpg[].
#include "assets/TPS_logo.h"     // For TPSLogo_jpg[].
#include "assets/background.h"   // For backgrnd_jpg[].
#include "assets/index.h"        // For index_htm[].
#include "assets/scheme.h"       // For scheme_png[].
#include "assets/script.h"       // For script_js[].
#include "assets/style.h"        // For style_css[].

typedef struct {
    const IP_FS_READ_ONLY_FILE_ENTRY *pEntry;
    unsigned DirLength;
    unsigned LastFolderLength;
    const IP_FS_READ_ONLY_FILE_ENTRY *pPrevious;
} IP_FS_READ_ONLY_CONTEXT;

static IP_FS_READ_ONLY_FILE_HOOK *_pFirstFileHook;

static const IP_FS_READ_ONLY_FILE_ENTRY _aFile[] = {
    //  path                               file data file size
    //  -------------------                -------------- -------------
    {"/index.htm", __Html_code_index_htm, __Html_code_index_htm_size},
    {"/background.jpg", __Html_code_background_jpg,
     __Html_code_background_jpg_size},
    {"/TPS_logo.svg", __Html_code_TPS_logo_svg, __Html_code_TPS_logo_svg_size},
    {"/Siemens_logo.svg", __Html_code_Siemens_logo_svg,
     __Html_code_Siemens_logo_svg_size},
    {"/scheme.png", __Html_code_scheme_png, __Html_code_scheme_png_size},
    {"/script.js", __Html_code_script_js, __Html_code_script_js_size},
    {"/style.css", __Html_code_style_css, __Html_code_style_css_size},
};

/*********************************************************************
 *
 *       _CompareDir()
 */
static int _CompareDir(const char *sDir, const char *sPath) {
    int i;
    char c0;
    char c1;

    for (i = 0;; i++) {
        c0 = *sDir++;
        if (c0 == 0) {
            break;
        }
        c1 = *sPath++;
        if (c0 != c1) {
            return 1; // No match, file not in this directory.
        }
    }
    return 0; // Match.
}

/*********************************************************************
 *
 *       _GetFolderLength()
 */
static int _GetFolderLength(const char *sPath) {
    const char *pEnd;
    unsigned r;

    pEnd = strstr(sPath, "/");
    if (pEnd == NULL) {
        r = 0u;
    } else {
        r = (unsigned)(pEnd - sPath);
    }
    return r;
}

/*********************************************************************
 *
 *       _FS_RO_FS_Open()
 */
static void *_FS_RO_FS_Open(const char *sPath) {
    const IP_FS_READ_ONLY_FILE_ENTRY *pEntry;
    IP_FS_READ_ONLY_FILE_HOOK *pHook;
    int i;

    //
    // Use dynamically added files list first.
    //
    for (pHook = _pFirstFileHook; pHook != NULL; pHook = pHook->pNext) {
        pEntry = &pHook->FileEntry;
        if (strcmp(sPath, pEntry->sPath) == 0) {
            return (void *)pEntry;
        }
    }
    //
    // Use fixed list.
    //
    for (i = 0;; i++) {
        pEntry = &_aFile[i];
        if (pEntry->sPath == NULL) {
            break;
        }
        if (strcmp(sPath, pEntry->sPath) == 0) {
            return (void *)pEntry;
        }
    }
    return NULL;
}

/*********************************************************************
 *
 *       _FS_RO_Close()
 */
static int _FS_RO_Close(void *hFile) {
    (void)hFile;

    return 0;
}

/*********************************************************************
 *
 *       _FS_RO_ReadAt()
 */
static int _FS_RO_ReadAt(void *hFile, void *pDest, U32 Pos, U32 NumBytes) {
    IP_FS_READ_ONLY_FILE_ENTRY *pEntry;

    pEntry = (IP_FS_READ_ONLY_FILE_ENTRY *)hFile;
    memcpy(pDest, pEntry->pData + Pos, NumBytes);
    return 0;
}

/*********************************************************************
 *
 *       _FS_RO_GetLen()
 */
static long _FS_RO_GetLen(void *hFile) {
    IP_FS_READ_ONLY_FILE_ENTRY *pEntry;

    pEntry = (IP_FS_READ_ONLY_FILE_ENTRY *)hFile;
    return pEntry->FileSize;
}

/*********************************************************************
 *
 *       _FS_RO_ForEachDirEntry()
 */
static void _FS_RO_ForEachDirEntry(void *pContext, const char *sDir,
                                   void (*pf)(void *pContext,
                                              void *pFileEntry)) {
    IP_FS_READ_ONLY_CONTEXT Context;
    IP_FS_READ_ONLY_FILE_ENTRY *pEntry;
    IP_FS_READ_ONLY_FILE_HOOK *pHook;
    int i;

    Context.DirLength = (unsigned)strlen(sDir);
    Context.LastFolderLength = 0u;
    Context.pPrevious = NULL;
    //
    // Use dynamically added files list first.
    //
    for (pHook = _pFirstFileHook; pHook != NULL; pHook = pHook->pNext) {
        pEntry = &pHook->FileEntry;
        if (_CompareDir(sDir, pEntry->sPath) == 0) {
            //
            // Check if this entry corresponds to the folder previously
            // reported.
            //
            if (Context.LastFolderLength != 0u) {
                if (memcmp(Context.pPrevious->sPath, pEntry->sPath,
                           Context.LastFolderLength) == 0) {
                    continue;
                }
            }
            Context.pEntry = pEntry;
            pf(pContext, (void *)&Context);
        }
    }
    //
    // Use fixed list.
    // Might report a filename for the second time as
    // we do not check if a filename has been overwritten
    // using the dynamic file list.
    //
    i = 0;
    while (1) {
        if (_aFile[i].sPath == NULL) {
            break;
        }
        if (_CompareDir(sDir, _aFile[i].sPath) == 0) {
            //
            // Check if this entry corresponds to the folder previously
            // reported.
            //
            if (Context.LastFolderLength != 0u) {
                if (memcmp(Context.pPrevious->sPath, _aFile[i].sPath,
                           Context.LastFolderLength) == 0) {
                    i++;
                    continue;
                }
            }
            Context.pEntry = &_aFile[i];
            pf(pContext, (void *)&Context);
        }
        i++;
    }
}

/*********************************************************************
 *
 *       _FS_RO_GetFileName()
 */
static void _FS_RO_GetFileName(void *pFileEntry, char *pBuffer,
                               U32 BufferSize) {
    IP_FS_READ_ONLY_CONTEXT *pContext;
    const IP_FS_READ_ONLY_FILE_ENTRY *pEntry;
    const char *pStart;
    unsigned FilenameLen;
    unsigned n;

    BufferSize--; // Reserve one byte for string termination.
    pContext = (IP_FS_READ_ONLY_CONTEXT *)pFileEntry;
    pEntry = pContext->pEntry;
    n = 0u;
    //
    // Check the presence of a folder.
    //
    pStart = pEntry->sPath + pContext->DirLength;
    if (*pStart == '/') {
        pStart++;
        n++;
    }
    FilenameLen = _GetFolderLength(pStart);
    if (FilenameLen == 0u) {
        FilenameLen = (unsigned)strlen(pStart);
    } else {
        pContext->LastFolderLength = pContext->DirLength + n + FilenameLen +
                                     1u; // Add 1 to add the '/' at the end.
        pContext->pPrevious = pEntry;
    }
    FilenameLen = SEGGER_MIN(FilenameLen, BufferSize);
    memcpy(pBuffer, pStart, FilenameLen);
    *(pBuffer + FilenameLen) = 0; // Terminate string.
}

/*********************************************************************
 *
 *       _FS_RO_GetFileSize()
 */
static U32 _FS_RO_GetFileSize(void *pFileEntry, U32 *pFileSizeHigh) {
    const IP_FS_READ_ONLY_CONTEXT *pContext;
    const IP_FS_READ_ONLY_FILE_ENTRY *pEntry;
    const char *pStart;
    unsigned DirLen;
    U32 r;

    (void)pFileSizeHigh;

    pContext = (IP_FS_READ_ONLY_CONTEXT *)pFileEntry;
    pEntry = pContext->pEntry;
    r = 0uL; // folder, indicate no length.
    //
    // Check the presence of a folder.
    //
    pStart = pEntry->sPath + pContext->DirLength;
    if (*pStart == '/') {
        pStart++;
    }
    DirLen = _GetFolderLength(pStart);
    if (DirLen == 0u) {
        r = (U32)pEntry->FileSize;
    }
    return r;
}

/*********************************************************************
 *
 *       _FS_RO_GetFileAttributes()
 */
static int _FS_RO_GetFileAttributes(void *pFileEntry) {
    const IP_FS_READ_ONLY_CONTEXT *pContext;
    const IP_FS_READ_ONLY_FILE_ENTRY *pEntry;
    const char *pStart;
    unsigned FilenameLen;
    int r;

    pContext = (IP_FS_READ_ONLY_CONTEXT *)pFileEntry;
    pEntry = pContext->pEntry;
    r = 0;
    //
    // Check the presence of a folder.
    //
    pStart = pEntry->sPath + pContext->DirLength;
    if (*pStart == '/') {
        pStart++;
    }
    FilenameLen = _GetFolderLength(pStart);
    if (FilenameLen != 0u) {
        r = 1; // folder.
    }
    return r;
}

/*********************************************************************
 *
 *       Global functions
 *
 **********************************************************************
 */

/*********************************************************************
 *
 *       IP_FS_READ_ONLY_ClrFileHooks()
 *
 *  Function description
 *    Clears all files that have been dynamically added.
 */
void IP_FS_READ_ONLY_ClrFileHooks(void) { _pFirstFileHook = NULL; }

/*********************************************************************
 *
 *       IP_FS_READ_ONLY_AddFileHook()
 *
 *  Function description
 *    Adds a file to the list of static read only files.
 *
 *  Parameters
 *    pHook   : Management element of type IP_FS_READ_ONLY_FILE_HOOK.
 *    sPath   : Path (filename) of the file to add.
 *    pData   : Content of the file.
 *    FileSize: Size of content.
 */
void IP_FS_READ_ONLY_AddFileHook(IP_FS_READ_ONLY_FILE_HOOK *pHook,
                                 const char *sPath, const unsigned char *pData,
                                 unsigned int FileSize) {
    IP_FS_READ_ONLY_FILE_ENTRY *pEntry;

    pHook->pNext = _pFirstFileHook;
    pEntry = &pHook->FileEntry;
    pEntry->sPath = sPath;
    pEntry->pData = pData;
    pEntry->FileSize = FileSize;
    _pFirstFileHook = pHook;
}

/*********************************************************************
 *
 *       Public API structures
 *
 **********************************************************************
 */

const IP_FS_API IP_FS_ReadOnly = {
    _FS_RO_FS_Open,           // pfOpenFile
    _FS_RO_Close,             // pfCloseFile
    _FS_RO_ReadAt,            // pfReadAt
    _FS_RO_GetLen,            // pfGetLen
    _FS_RO_ForEachDirEntry,   // pfForEachDirEntry
    _FS_RO_GetFileName,       // pfGetDirEntryFileName
    _FS_RO_GetFileSize,       // pfGetDirEntryFileSize
    NULL,                     // pfGetDirEntryFileTime
    _FS_RO_GetFileAttributes, // pfGetDirEntryAttributes
    NULL,                     // pfCreate
    NULL,                     // pfDeleteFile
    NULL,                     // pfRenameFile
    NULL,                     // pfWriteAt
    NULL,                     // pfMKDir
    NULL,                     // pfRMDir
    NULL,                     // pfIsFolder
    NULL                      // pfMove
};
#include <stdio.h>  // For NULL.
#include <string.h> // For memcpy(), strcmp().

#include "IP_FS.h"
#include "WEBS_Conf.h"

#include "assets/Siemens_logo.h" // For SiemensLogo_jpg[].
#include "assets/TPS_logo.h"     // For TPSLogo_jpg[].
#include "assets/background.h"   // For backgrnd_jpg[].
#include "assets/index.h"        // For index_htm[].
#include "assets/scheme.h"       // For scheme_png[].
#include "assets/script.h"       // For script_js[].
#include "assets/style.h"        // For style_css[].

typedef struct {
    const IP_FS_READ_ONLY_FILE_ENTRY *pEntry;
    unsigned DirLength;
    unsigned LastFolderLength;
    const IP_FS_READ_ONLY_FILE_ENTRY *pPrevious;
} IP_FS_READ_ONLY_CONTEXT;

static IP_FS_READ_ONLY_FILE_HOOK *_pFirstFileHook;

static const IP_FS_READ_ONLY_FILE_ENTRY _aFile[] = {
    //  path                               file data file size
    //  -------------------                -------------- -------------
    {"/index.htm", __Html_code_index_htm, __Html_code_index_htm_size},
    {"/background.jpg", __Html_code_background_jpg,
     __Html_code_background_jpg_size},
    {"/TPS_logo.svg", __Html_code_TPS_logo_svg, __Html_code_TPS_logo_svg_size},
    {"/Siemens_logo.svg", __Html_code_Siemens_logo_svg,
     __Html_code_Siemens_logo_svg_size},
    {"/scheme.png", __Html_code_scheme_png, __Html_code_scheme_png_size},
    {"/script.js", __Html_code_script_js, __Html_code_script_js_size},
    {"/style.css", __Html_code_style_css, __Html_code_style_css_size},
};

/*********************************************************************
 *
 *       _CompareDir()
 */
static int _CompareDir(const char *sDir, const char *sPath) {
    int i;
    char c0;
    char c1;

    for (i = 0;; i++) {
        c0 = *sDir++;
        if (c0 == 0) {
            break;
        }
        c1 = *sPath++;
        if (c0 != c1) {
            return 1; // No match, file not in this directory.
        }
    }
    return 0; // Match.
}

/*********************************************************************
 *
 *       _GetFolderLength()
 */
static int _GetFolderLength(const char *sPath) {
    const char *pEnd;
    unsigned r;

    pEnd = strstr(sPath, "/");
    if (pEnd == NULL) {
        r = 0u;
    } else {
        r = (unsigned)(pEnd - sPath);
    }
    return r;
}

/*********************************************************************
 *
 *       _FS_RO_FS_Open()
 */
static void *_FS_RO_FS_Open(const char *sPath) {
    const IP_FS_READ_ONLY_FILE_ENTRY *pEntry;
    IP_FS_READ_ONLY_FILE_HOOK *pHook;
    int i;

    //
    // Use dynamically added files list first.
    //
    for (pHook = _pFirstFileHook; pHook != NULL; pHook = pHook->pNext) {
        pEntry = &pHook->FileEntry;
        if (strcmp(sPath, pEntry->sPath) == 0) {
            return (void *)pEntry;
        }
    }
    //
    // Use fixed list.
    //
    for (i = 0;; i++) {
        pEntry = &_aFile[i];
        if (pEntry->sPath == NULL) {
            break;
        }
        if (strcmp(sPath, pEntry->sPath) == 0) {
            return (void *)pEntry;
        }
    }
    return NULL;
}

/*********************************************************************
 *
 *       _FS_RO_Close()
 */
static int _FS_RO_Close(void *hFile) {
    (void)hFile;

    return 0;
}

/*********************************************************************
 *
 *       _FS_RO_ReadAt()
 */
static int _FS_RO_ReadAt(void *hFile, void *pDest, U32 Pos, U32 NumBytes) {
    IP_FS_READ_ONLY_FILE_ENTRY *pEntry;

    pEntry = (IP_FS_READ_ONLY_FILE_ENTRY *)hFile;
    memcpy(pDest, pEntry->pData + Pos, NumBytes);
    return 0;
}

/*********************************************************************
 *
 *       _FS_RO_GetLen()
 */
static long _FS_RO_GetLen(void *hFile) {
    IP_FS_READ_ONLY_FILE_ENTRY *pEntry;

    pEntry = (IP_FS_READ_ONLY_FILE_ENTRY *)hFile;
    return pEntry->FileSize;
}

/*********************************************************************
 *
 *       _FS_RO_ForEachDirEntry()
 */
static void _FS_RO_ForEachDirEntry(void *pContext, const char *sDir,
                                   void (*pf)(void *pContext,
                                              void *pFileEntry)) {
    IP_FS_READ_ONLY_CONTEXT Context;
    IP_FS_READ_ONLY_FILE_ENTRY *pEntry;
    IP_FS_READ_ONLY_FILE_HOOK *pHook;
    int i;

    Context.DirLength = (unsigned)strlen(sDir);
    Context.LastFolderLength = 0u;
    Context.pPrevious = NULL;
    //
    // Use dynamically added files list first.
    //
    for (pHook = _pFirstFileHook; pHook != NULL; pHook = pHook->pNext) {
        pEntry = &pHook->FileEntry;
        if (_CompareDir(sDir, pEntry->sPath) == 0) {
            //
            // Check if this entry corresponds to the folder previously
            // reported.
            //
            if (Context.LastFolderLength != 0u) {
                if (memcmp(Context.pPrevious->sPath, pEntry->sPath,
                           Context.LastFolderLength) == 0) {
                    continue;
                }
            }
            Context.pEntry = pEntry;
            pf(pContext, (void *)&Context);
        }
    }
    //
    // Use fixed list.
    // Might report a filename for the second time as
    // we do not check if a filename has been overwritten
    // using the dynamic file list.
    //
    i = 0;
    while (1) {
        if (_aFile[i].sPath == NULL) {
            break;
        }
        if (_CompareDir(sDir, _aFile[i].sPath) == 0) {
            //
            // Check if this entry corresponds to the folder previously
            // reported.
            //
            if (Context.LastFolderLength != 0u) {
                if (memcmp(Context.pPrevious->sPath, _aFile[i].sPath,
                           Context.LastFolderLength) == 0) {
                    i++;
                    continue;
                }
            }
            Context.pEntry = &_aFile[i];
            pf(pContext, (void *)&Context);
        }
        i++;
    }
}

/*********************************************************************
 *
 *       _FS_RO_GetFileName()
 */
static void _FS_RO_GetFileName(void *pFileEntry, char *pBuffer,
                               U32 BufferSize) {
    IP_FS_READ_ONLY_CONTEXT *pContext;
    const IP_FS_READ_ONLY_FILE_ENTRY *pEntry;
    const char *pStart;
    unsigned FilenameLen;
    unsigned n;

    BufferSize--; // Reserve one byte for string termination.
    pContext = (IP_FS_READ_ONLY_CONTEXT *)pFileEntry;
    pEntry = pContext->pEntry;
    n = 0u;
    //
    // Check the presence of a folder.
    //
    pStart = pEntry->sPath + pContext->DirLength;
    if (*pStart == '/') {
        pStart++;
        n++;
    }
    FilenameLen = _GetFolderLength(pStart);
    if (FilenameLen == 0u) {
        FilenameLen = (unsigned)strlen(pStart);
    } else {
        pContext->LastFolderLength = pContext->DirLength + n + FilenameLen +
                                     1u; // Add 1 to add the '/' at the end.
        pContext->pPrevious = pEntry;
    }
    FilenameLen = SEGGER_MIN(FilenameLen, BufferSize);
    memcpy(pBuffer, pStart, FilenameLen);
    *(pBuffer + FilenameLen) = 0; // Terminate string.
}

/*********************************************************************
 *
 *       _FS_RO_GetFileSize()
 */
static U32 _FS_RO_GetFileSize(void *pFileEntry, U32 *pFileSizeHigh) {
    const IP_FS_READ_ONLY_CONTEXT *pContext;
    const IP_FS_READ_ONLY_FILE_ENTRY *pEntry;
    const char *pStart;
    unsigned DirLen;
    U32 r;

    (void)pFileSizeHigh;

    pContext = (IP_FS_READ_ONLY_CONTEXT *)pFileEntry;
    pEntry = pContext->pEntry;
    r = 0uL; // folder, indicate no length.
    //
    // Check the presence of a folder.
    //
    pStart = pEntry->sPath + pContext->DirLength;
    if (*pStart == '/') {
        pStart++;
    }
    DirLen = _GetFolderLength(pStart);
    if (DirLen == 0u) {
        r = (U32)pEntry->FileSize;
    }
    return r;
}

/*********************************************************************
 *
 *       _FS_RO_GetFileAttributes()
 */
static int _FS_RO_GetFileAttributes(void *pFileEntry) {
    const IP_FS_READ_ONLY_CONTEXT *pContext;
    const IP_FS_READ_ONLY_FILE_ENTRY *pEntry;
    const char *pStart;
    unsigned FilenameLen;
    int r;

    pContext = (IP_FS_READ_ONLY_CONTEXT *)pFileEntry;
    pEntry = pContext->pEntry;
    r = 0;
    //
    // Check the presence of a folder.
    //
    pStart = pEntry->sPath + pContext->DirLength;
    if (*pStart == '/') {
        pStart++;
    }
    FilenameLen = _GetFolderLength(pStart);
    if (FilenameLen != 0u) {
        r = 1; // folder.
    }
    return r;
}

/*********************************************************************
 *
 *       Global functions
 *
 **********************************************************************
 */

/*********************************************************************
 *
 *       IP_FS_READ_ONLY_ClrFileHooks()
 *
 *  Function description
 *    Clears all files that have been dynamically added.
 */
void IP_FS_READ_ONLY_ClrFileHooks(void) { _pFirstFileHook = NULL; }

/*********************************************************************
 *
 *       IP_FS_READ_ONLY_AddFileHook()
 *
 *  Function description
 *    Adds a file to the list of static read only files.
 *
 *  Parameters
 *    pHook   : Management element of type IP_FS_READ_ONLY_FILE_HOOK.
 *    sPath   : Path (filename) of the file to add.
 *    pData   : Content of the file.
 *    FileSize: Size of content.
 */
void IP_FS_READ_ONLY_AddFileHook(IP_FS_READ_ONLY_FILE_HOOK *pHook,
                                 const char *sPath, const unsigned char *pData,
                                 unsigned int FileSize) {
    IP_FS_READ_ONLY_FILE_ENTRY *pEntry;

    pHook->pNext = _pFirstFileHook;
    pEntry = &pHook->FileEntry;
    pEntry->sPath = sPath;
    pEntry->pData = pData;
    pEntry->FileSize = FileSize;
    _pFirstFileHook = pHook;
}

/*********************************************************************
 *
 *       Public API structures
 *
 **********************************************************************
 */

const IP_FS_API IP_FS_ReadOnly = {
    _FS_RO_FS_Open,           // pfOpenFile
    _FS_RO_Close,             // pfCloseFile
    _FS_RO_ReadAt,            // pfReadAt
    _FS_RO_GetLen,            // pfGetLen
    _FS_RO_ForEachDirEntry,   // pfForEachDirEntry
    _FS_RO_GetFileName,       // pfGetDirEntryFileName
    _FS_RO_GetFileSize,       // pfGetDirEntryFileSize
    NULL,                     // pfGetDirEntryFileTime
    _FS_RO_GetFileAttributes, // pfGetDirEntryAttributes
    NULL,                     // pfCreate
    NULL,                     // pfDeleteFile
    NULL,                     // pfRenameFile
    NULL,                     // pfWriteAt
    NULL,                     // pfMKDir
    NULL,                     // pfRMDir
    NULL,                     // pfIsFolder
    NULL                      // pfMove
};
