#ifndef _uz_H
#define _uz_H

#include <stdint.h>

#ifndef UNZ_BUFSIZE
#define UNZ_BUFSIZE (1024)
//#define UNZ_BUFSIZE (16384)
#endif

typedef void *voidp;
typedef voidp unzFile;

typedef int32_t (ZIP_READ_CALLBACK)(void *pFile, uint8_t *pBuf, int32_t iLen);
typedef int32_t (ZIP_SEEK_CALLBACK)(void *pFile, int32_t iPosition, int iType);
typedef void * (ZIP_OPEN_CALLBACK)(const char *szFilename, int32_t *pFileSize);
typedef void (ZIP_CLOSE_CALLBACK)(void *pFile);

typedef struct zipFile
{
    int32_t iPos; // current file position
    int32_t iSize; // file size
    int iLastError;
    uint8_t *pData; // memory file pointer
    void * fHandle; // class pointer to File/SdFat or whatever you want
    void * zHandle; // pointer to unzFile
    ZIP_CLOSE_CALLBACK *pfnClose;
    ZIP_READ_CALLBACK *pfnRead;
    ZIP_SEEK_CALLBACK *pfnSeek;
    //uint8_t ucBuf[UNZ_BUFSIZE]; // local read buffer for compressed data
    uint8_t ucReadInfo[256]; // local info structure
    uint8_t ucInfo[256]; // unzFile structure
    uint8_t ucFlate[9000]; // inflate buffers
} ZIPFILE;

unzFile unzOpen (const char *path, ZIPFILE *pzf, ZIP_OPEN_CALLBACK *pfnOpen, ZIP_READ_CALLBACK *pfnRead, ZIP_SEEK_CALLBACK *pfnSeek, ZIP_CLOSE_CALLBACK *pfnClose);

#endif