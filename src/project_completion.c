#define _GNU_SOURCE
#define _XOPEN_SOURCE
#
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

int project_completion (const wchar_t* cmdstr, wchar_t* completion)
{
 wchar_t* buffer;
 FILE* cmd;
 int ret;
 int linelen = 1024;
 int sum;
 buffer = malloc(20);

 wchar_t* pattern = calloc(256, sizeof(wchar_t));
 wchar_t* line = calloc(256, sizeof(wchar_t));

 // printf("IICCII"); //
 sum = 0;
 cmd = popen("task _project", "r"); 

 swscanf(cmdstr,L"%ls project:%ls", buffer, pattern);
  // wprintf(L"cmdstr is %ls \n", cmdstr); 
  // wprintf(L"pattern is buffer %ls pattern %ls \n", buffer, pattern); 
 wcscat(pattern,L" %ls \n");
 free(buffer);

 while ( fgetws(line, linelen-1, cmd) != NULL ) 
 {    
   ret =  swscanf(line , pattern,  completion ) ;
   // wprintf(L"returned code is %d sum %d  with pattern:: %ls and completion:: %ls \n", ret, sum, pattern, completion); //
   sum += ret;
 }
 pclose(cmd);
 

 return sum;

}

