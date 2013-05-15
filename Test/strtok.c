#include <stdio.h>
#include <string.h>

int main(void) 
{ 
   char input[16] = "abc:dfd:yu"; 
   char *p;
   char *tmp = NULL;

   /* strtok places a NULL terminator 
   in front of the token, if found */ 
   p = strtok(input, ":"); 
   if (p)   printf("1、%s\n", p); 

   /* A second call to strtok using a NULL 
   as the first parameter returns a pointer 
   to the character following the token  */ 
   p = strtok(NULL, ":"); 
   if (p)   printf("2.%s\n", p); 
   
   p = strtok(NULL, ":"); 
   if (p)   printf("3.%s\n", p);
   
   return 0; 
} 
