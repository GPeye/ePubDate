#ifndef _unz_H
#define _unz_H

#include <stdint.h>

#ifndef _ZLIB_H
#include "zlib.h"
#endif

#ifndef UNZ_BUFSIZE
#define UNZ_BUFSIZE (1024)
//#define UNZ_BUFSIZE (16384)
#endif

#ifndef UNZ_MAXFILENAMEINZIP
#define UNZ_MAXFILENAMEINZIP (256)
#endif

//
// Callback functions for file I/O
//
typedef int32_t (ZIP_READ_CALLBACK)(void *pFile, uint8_t *pBuf, int32_t iLen);
typedef int32_t (ZIP_SEEK_CALLBACK)(void *pFile, int32_t iPosition, int iType);
typedef void * (ZIP_OPEN_CALLBACK)(const char *szFilename, int32_t *pFileSize);
typedef void (ZIP_CLOSE_CALLBACK)(void *pFile);
//
// The zipfile structure to hold our file/memory state
//
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
    uint8_t ucBuf[UNZ_BUFSIZE]; // local read buffer for compressed data
    uint8_t ucReadInfo[256]; // local info structure
    uint8_t ucInfo[256]; // unzFile structure
    uint8_t ucFlate[32768+7168]; // inflate buffers
} ZIPFILE;

typedef voidp unzFile;

#define UNZ_OK                                  (0)
#define UNZ_END_OF_LIST_OF_FILE (-100)
#define UNZ_ERRNO               (Z_ERRNO)
#define UNZ_EOF                 (0)
#define UNZ_PARAMERROR                  (-102)
#define UNZ_BADZIPFILE                  (-103)
#define UNZ_INTERNALERROR               (-104)
#define UNZ_CRCERROR                    (-105)

/* tm_unz contain date/time info */
typedef struct tm_unz_s 
{
	uInt tm_sec;            /* seconds after the minute - [0,59] */
	uInt tm_min;            /* minutes after the hour - [0,59] */
	uInt tm_hour;           /* hours since midnight - [0,23] */
	uInt tm_mday;           /* day of the month - [1,31] */
	uInt tm_mon;            /* months since January - [0,11] */
	uInt tm_year;           /* years - [1980..2044] */
} tm_unz;

/* unz_global_info structure contain global data about the ZIPfile
   These data comes from the end of central dir */
typedef struct unz_global_info_s
{
	uLong number_entry;         /* total number of entries in
				       the central dir on this disk */
	uLong size_comment;         /* size of the global comment of the zipfile */
} unz_global_info;


/* unz_file_info contain information about a file in the zipfile */
typedef struct unz_file_info_s
{
    uLong version;              /* version made by                 2 bytes */
    uLong version_needed;       /* version needed to extract       2 bytes */
    uLong flag;                 /* general purpose bit flag        2 bytes */
    uLong compression_method;   /* compression method              2 bytes */
    uLong dosDate;              /* last mod file date in Dos fmt   4 bytes */
    uLong crc;                  /* crc-32                          4 bytes */
    uLong compressed_size;      /* compressed size                 4 bytes */ 
    uLong uncompressed_size;    /* uncompressed size               4 bytes */ 
    uLong size_filename;        /* filename length                 2 bytes */
    uLong size_file_extra;      /* extra field length              2 bytes */
    uLong size_file_comment;    /* file comment length             2 bytes */

    uLong disk_num_start;       /* disk number start               2 bytes */
    uLong internal_fa;          /* internal file attributes        2 bytes */
    uLong external_fa;          /* external file attributes        4 bytes */

    tm_unz tmu_date;
} unz_file_info;

extern int ZEXPORT unzStringFileNameCompare OF ((const char* fileName1,
												 const char* fileName2,
												 int iCaseSensitivity));
/*
   Compare two filename (fileName1,fileName2).
   If iCaseSenisivity = 1, comparision is case sensitivity (like strcmp)
   If iCaseSenisivity = 2, comparision is not case sensitivity (like strcmpi
								or strcasecmp)
   If iCaseSenisivity = 0, case sensitivity is defaut of your operating system
	(like 1 on Unix, 2 on Windows)
*/


extern unzFile ZEXPORT unzOpen OF((const char *path, uint8_t *pData, uint32_t u32DataSize, ZIPFILE *pzf, ZIP_OPEN_CALLBACK *pfnOpen, ZIP_READ_CALLBACK *pfnRead, ZIP_SEEK_CALLBACK *pfnSeek, ZIP_CLOSE_CALLBACK *pfnClose));
/*
  Open a Zip file. path contain the full pathname (by example,
     on a Windows NT computer "c:\\zlib\\zlib111.zip" or on an Unix computer
	 "zlib/zlib111.zip".
	 If the zipfile cannot be opened (file don't exist or in not valid), the
	   return value is NULL.
     Else, the return value is a unzFile Handle, usable with other function
	   of this unzip package.
*/

extern int ZEXPORT unzClose OF((unzFile file));
/*
  Close a ZipFile opened with unzipOpen.
  If there is files inside the .Zip opened with unzOpenCurrentFile (see later),
    these files MUST be closed with unzipCloseCurrentFile before call unzipClose.
  return UNZ_OK if there is no problem. */

extern int ZEXPORT unzGetGlobalInfo OF((unzFile file,
					unz_global_info *pglobal_info));
/*
  Write info about the ZipFile in the *pglobal_info structure.
  No preparation of the structure is needed
  return UNZ_OK if there is no problem. */


extern int ZEXPORT unzGetGlobalComment OF((unzFile file,
										   char *szComment,
					   uLong uSizeBuf));
/*
  Get the global comment string of the ZipFile, in the szComment buffer.
  uSizeBuf is the size of the szComment buffer.
  return the number of byte copied or an error code <0
*/


/***************************************************************************/
/* Unzip package allow you browse the directory of the zipfile */

extern int ZEXPORT unzGoToFirstFile OF((unzFile file));
/*
  Set the current file of the zipfile to the first file.
  return UNZ_OK if there is no problem
*/

extern int ZEXPORT unzGoToNextFile OF((unzFile file));
/*
  Set the current file of the zipfile to the next file.
  return UNZ_OK if there is no problem
  return UNZ_END_OF_LIST_OF_FILE if the actual file was the latest.
*/

extern int ZEXPORT unzLocateFile OF((unzFile file, 
				     const char *szFileName,
				     int iCaseSensitivity));
/*
  Try locate the file szFileName in the zipfile.
  For the iCaseSensitivity signification, see unzStringFileNameCompare

  return value :
  UNZ_OK if the file is found. It becomes the current file.
  UNZ_END_OF_LIST_OF_FILE if the file is not found
*/


extern int ZEXPORT unzGetCurrentFileInfo OF((unzFile file,
					     unz_file_info *pfile_info,
					     char *szFileName,
					     uLong fileNameBufferSize,
					     void *extraField,
					     uLong extraFieldBufferSize,
					     char *szComment,
					     uLong commentBufferSize));
/*
  Get Info about the current file
  if pfile_info!=NULL, the *pfile_info structure will contain somes info about
	    the current file
  if szFileName!=NULL, the filemane string will be copied in szFileName
			(fileNameBufferSize is the size of the buffer)
  if extraField!=NULL, the extra field information will be copied in extraField
			(extraFieldBufferSize is the size of the buffer).
			This is the Central-header version of the extra field
  if szComment!=NULL, the comment string of the file will be copied in szComment
			(commentBufferSize is the size of the buffer)
*/

/***************************************************************************/
/* for reading the content of the current zipfile, you can open it, read data
   from it, and close it (you can close it before reading all the file)
   */

extern int ZEXPORT unzOpenCurrentFile OF((unzFile file));
/*
  Open for reading data the current file in the zipfile.
  If there is no error, the return value is UNZ_OK.
*/

extern int ZEXPORT unzCloseCurrentFile OF((unzFile file));
/*
  Close the file in zip opened with unzOpenCurrentFile
  Return UNZ_CRCERROR if all the file was read but the CRC is not good
*/

												
extern int ZEXPORT unzReadCurrentFile OF((unzFile file, 
					  voidp buf,
					  uLong len));
/*
  Read bytes from the current file (opened by unzOpenCurrentFile)
  buf contain buffer where data must be copied
  len the size of buf.

  return the number of byte copied if somes bytes are copied
  return 0 if the end of file was reached
  return <0 with error code if there is an error
    (UNZ_ERRNO for IO error, or zLib error for uncompress error)
*/

extern z_off_t ZEXPORT unztell OF((unzFile file));
/*
  Give the current position in uncompressed data
*/

extern int ZEXPORT unzeof OF((unzFile file));
/*
  return 1 if the end of file was reached, 0 elsewhere 
*/

extern int ZEXPORT unzGetLocalExtrafield OF((unzFile file,
											 voidp buf,
											 unsigned len));
/*
  Read extra field from the current file (opened by unzOpenCurrentFile)
  This is the local-header version of the extra field (sometimes, there is
    more info in the local-header version than in the central-header)

  if buf==NULL, it return the size of the local extra field

  if buf!=NULL, len is the size of the buffer, the extra header is copied in
	buf.
  the return value is the number of bytes copied in buf, or (if <0) 
	the error code
*/


#endif /* _unz_H */