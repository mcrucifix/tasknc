#define _GNU_SOURCE
#define _XOPEN_SOURCE
#include <config.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include <locale.h>


int project_completion (const wchar_t* completestr, wchar_t* completion)
{
 FILE* cmd;
 int ret;
 int sum;

 wchar_t* buffer = calloc(256, sizeof(wchar_t));
 wchar_t* pattern = calloc(256, sizeof(wchar_t));
 char line[TOTALLENGTH/2];

 /*wchar_t line[linelen]; */
 wchar_t wline[1024];
 char* command = calloc(256, sizeof(wchar_t));
 char* cmdstr = calloc(256, sizeof(char));
 wchar_t* tmp;
 sum = 0;
 int istop = 0;

 setlocale(LC_ALL, "en.UTF-8");

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
 // cmd = fopen("myfile.txt","r"); 
 wcscat(pattern,L" %ls \n");

 while ( fgets(line, sizeof(line) -1, cmd) != NULL ) 
 {    
   mbstowcs( wline, line, TOTALLENGTH );
   ret =  swscanf(wline , pattern,  buffer ) ;
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
 /*free(line); */
 free(buffer);
 free(cmdstr);
 return sum;

}

