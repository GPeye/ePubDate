#include "myfiles.h"
#include "zlib.h"

static PlaydateAPI* pd = NULL;

void listFilesCallback(const char* path, void* userdata)
{
	static char buffer[1024];
	buffer[0] = '\0';
	int len = strlen(path);
	for (int i=0; i < len; i++)
	{
		char cBuf[100];
		unsigned char c = (unsigned char)path[i];
		sprintf(cBuf, "{%x}", c);
		strcat(buffer, cBuf);
	}
    pd->system->logToConsole(path);
    pd->system->logToConsole("version %s", zlibVersion());
}

void listFiles(PlaydateAPI* playdate)
{
    pd = playdate;
    void* userdata;
	int good = pd->file->listfiles(".",listFilesCallback,userdata, 1);
    
}