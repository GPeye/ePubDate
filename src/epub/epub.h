#ifndef EPUB_H
#define EPUB_H

#include <stdio.h>
#include "pd_api.h"
#include "unzip.h"

typedef struct
{
    PlaydateAPI* pd;
    ZIPFILE zpf;
    unzFile zHandle;
    char szComment[256], szName[256];
    char szTemp[256];
} SEpub;

// epub state data
extern SEpub Epub;

void InitEpub(PlaydateAPI* pd);
void readStuff();

#endif