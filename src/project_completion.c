#define _GNU_SOURCE
#define _XOPEN_SOURCE
#
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

int project_completion (const wchar_t* cmdstr, wchar_t* completion)
{
 FILE* cmd;
 int ret;
 int linelen = 1024;
 int sum;

 wchar_t* buffer = calloc(256, sizeof(wchar_t));
 wchar_t* pattern = calloc(256, sizeof(wchar_t));
 wchar_t* line = calloc(256, sizeof(wchar_t));

 // printf("IICCII"); //
 sum = 0;
 cmd = popen("task _project", "r"); 

 if (swscanf(cmdstr,L"%ls project:%ls", buffer, pattern) == 2 || 
    (swscanf(cmdstr,L"project:%ls",  pattern) == 1 ))
 {
  // wprintf(L"cmdstr is %ls \n", cmdstr); 
  // wprintf(L"pattern is buffer %ls pattern %ls \n", buffer, pattern); 
 wcscat(pattern,L" %ls \n");

 while ( fgetws(line, linelen-1, cmd) != NULL ) 
 {    
   ret =  swscanf(line , pattern,  completion ) ;
   // wprintf(L"returned code is %d sum %d  with pattern:: %ls and completion:: %ls \n", ret, sum, pattern, completion); //
   sum += ret;
 }
 pclose(cmd);
 }

 free(buffer);
 free(pattern);
 free(line);
 return sum;

}

