#include <stdlib.h>
#include <stdio.h>

#include "epub.h"
#include "unzip.h"

SEpub Epub;

void *myOpen(const char *filename, int32_t *size)
{
    Epub.pd->system->logToConsole("Attempting to open");
    size_t filesize;
    SDFile *myfile = Epub.pd->file->open(filename, 1);
    if (myfile)
    {
        Epub.pd->file->seek(myfile, 0, SEEK_END);
        filesize = Epub.pd->file->tell(myfile);
        Epub.pd->file->seek(myfile, 0, SEEK_SET);
        *size = (int32_t)filesize;
    }
    Epub.pd->system->logToConsole("myfile handle: %d", myfile);
    return (void *)myfile;
}

void myClose(void *p)
{
    ZIPFILE *pzf = (ZIPFILE *)p;
    SDFile *f = (SDFile *)pzf->fHandle;
    Epub.pd->file->close(f);
    Epub.pd->system->logToConsole("File closed on handle: %d", f);
}

int32_t myRead(void *p, uint8_t *buffer, int32_t length)
{
    ZIPFILE *pzf = (ZIPFILE *)p;
    if (!pzf)
        return 0;
    SDFile *f = (SDFile *)pzf->fHandle;
    // pd->system->logToConsole("File read on handle: %d", f);
    //    return f->read(buffer, length);
    return (int32_t)Epub.pd->file->read(f, buffer, length);
}

int32_t mySeek(void *p, int32_t position, int iType)
{
    ZIPFILE *pzf = (ZIPFILE *)p;
    if (!pzf)
        return 0;
    SDFile *f = (SDFile *)pzf->fHandle;
    // pd->system->logToConsole("file seek on %d", f);
    return Epub.pd->file->seek(f, position, iType);
}

void readStuff()
{
    // listFiles(pd);
    Epub.pd->system->logToConsole("reading stuff");
    int rc = 0;
    ZIPFILE zpf;
    Epub.pd->system->logToConsole("about to open");
    unzFile zHandle = unzOpen("pg2701.epub", NULL, 0, &zpf, myOpen, myRead, mySeek, myClose);
    //unzFile zHandle = unzOpen("testfile.zip", NULL, 0, &zpf, myOpen, myRead, mySeek, myClose);

    if (zHandle == NULL)
    {
        Epub.pd->system->logToConsole("Failed to open");
    }
    else
    {
        Epub.pd->system->logToConsole("We got a handle: %d", zHandle);
    }
    char szComment[256], szName[256];
    rc = unzGetGlobalComment(zHandle, szComment, sizeof(szComment));
    if (rc == UNZ_OK)
    {
        Epub.pd->system->logToConsole("comment: %s", &szComment);
    }
    else
    {
        Epub.pd->system->logToConsole("bad comment %d", rc);
    }

    // // rc = unzLocateFile(zHandle, "META-INF/container.xml", 2);
    // // rc = unzLocateFile(zHandle, "OEBPS/content.opf", 2);
    rc = unzLocateFile(zHandle, "OEBPS/3484760691463238453_2701-h-1.htm.html", 2);
    //rc = unzLocateFile(zHandle, "testfile", 2);

    if (rc != UNZ_OK) /* Report the file not found */
    {
        Epub.pd->system->logToConsole("file %s not found within archive", "META-INF/container.xml");
        unzClose(zHandle);
        //return -1;
    }
    else
    {
        Epub.pd->system->logToConsole("testfile found");
        rc = unzOpenCurrentFile(zHandle); /* Try to open the file we want */
        if (rc != UNZ_OK)
        {
            Epub.pd->system->logToConsole("Error opening file = %d\n", rc);
            unzClose(zHandle);
            //return -1;
        }
        Epub.pd->system->logToConsole("File located within archive.\n");
        rc = 1;
        int i = 0;

        char szTemp[256];
        while (rc > 0)
        {
            rc = unzReadCurrentFile(zHandle, szTemp, sizeof(szTemp));
            if (rc >= 0)
            {
                i += rc;
                if (rc > 0)
                {
                    Epub.pd->system->logToConsole("%s", szTemp);
                }
            }
            else
            {
                Epub.pd->system->logToConsole("Error reading from file\n");
                break;
            }
        }
        Epub.pd->system->logToConsole("Total bytes read = %d (reading 256 bytes at a time)\n", i);
        rc = unzCloseCurrentFile(zHandle);
        unzClose(zHandle);
    }
}

void InitEpub(PlaydateAPI *pd)
{
    Epub.pd = pd;
    pd->system->logToConsole("initing");
};