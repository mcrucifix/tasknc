#define _GNU_SOURCE
#define _XOPEN_SOURCE
#
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int get_project_completion (const char* cmdstr, char* completion)
{
 char* to_complete;
 char* buffer;
 FILE* cmd;
 int ret;
 int linelen = 1024;
 int sum;
 buffer = malloc(20);

 char* pattern;
 char* line;
 line    = malloc(1024);
 pattern = malloc(256);

 sum = 0;
 cmd = popen("task _project", "r"); 

 sscanf(cmdstr,"%s project:%s", buffer, pattern);
 printf("cmdstr is %s \n", cmdstr);
 strcat(pattern," %s \n");
 free(buffer);

 while ( fgets(line, linelen-1, cmd) != NULL ) 
 {    
   ret =  sscanf(line , pattern,  completion ) ;
   printf("returned code is %d sum %d  with completion %s \n", ret, sum, completion);
   sum += ret;
 }
 pclose(cmd);
 

 /*sscanf(":[A-Za-z0-9- ]project:%[A-Za-z0-9-.]", cmdstr,to_complete); */
 return sum;

}

int main()
{
 char* completion;
 const char* cmdstr = ":modify project:2";
 int ret;
 int sum;

 completion = malloc(200);
 sum = get_project_completion (cmdstr, completion);

 if (sum > 0)
 {
 printf("the completion is %s  out of %d possibilies \n", completion, sum);
 }
 
 return 0 ;

}

  


