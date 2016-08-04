#define _GNU_SOURCE
#define _XOPEN_SOURCE
#
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

int project_completion (const wchar_t* completestr, wchar_t* completion)
{
 FILE* cmd;
 int ret;
 int linelen = 1024;
 int sum;

 wchar_t* buffer = calloc(256, sizeof(wchar_t));
 wchar_t* pattern = calloc(256, sizeof(wchar_t));
 wchar_t* line = calloc(256, sizeof(wchar_t));
 char* command = calloc(256, sizeof(wchar_t));
 char* cmdstr = calloc(256, sizeof(char));
 wchar_t* tmp;
 sum = 0;
 int istop = 0;

 if   (swscanf(completestr,L"%[^ :]:%ls", command, pattern) != 2 )
   while( swscanf(completestr,L"%l[^ ] %[^ :]:%ls", buffer, command, pattern) <3) {
      tmp = wcschr(completestr, ' ');
      if(tmp != NULL) {completestr = tmp + 1;} else {istop = 1 ; break; };
   }


 // wprintf(L"pattern %ls \n",pattern);

 if (istop == 0) {
  // wprintf(L"completestr is %ls \n", completestr); 
  // wprintf(L"pattern is buffer %ls pattern %ls \n", buffer, pattern); 
 sprintf(cmdstr, "task _%s", command);
 cmd = popen(cmdstr , "r"); 
 wcscat(pattern,L" %ls \n");


 while ( fgetws(line, linelen-1, cmd) != NULL ) 
 {    
   ret =  swscanf(line , pattern,  buffer ) ;
   // wprintf(L"returned code is %d sum %d  with pattern:: %ls and completion:: %ls \n", ret, sum, pattern, completion); //
   if (ret > 0)
   {
     if ( sum == 0)
     {  wcscpy(completion, buffer); }
     else
     {
       istop = 0;
       while (buffer[istop] == completion[istop]) {istop++;}
       completion[istop] = 0;
     }

   sum += ret;
   }
 }
 pclose(cmd);
 }

 free(pattern);
 free(line);
 free(cmdstr);
 return sum;

}

