#include <stdio.h>
#include "backtrace.h"
#include <stdlib.h>
#include <string.h>
// #include "../../main.h"
void add_fn(char * name, unsigned int start, unsigned int end){
  fn_mem_loc * fn = &fn_mem_map[fn_num++];
  strcpy(fn->fn_name,name);
  fn->start = start;
  fn->end = end;
}


void load_fn_mem_map(){
  FILE * fp = fopen("/usr/share/vigil/memmap.txt","r");
  if(fp == NULL){
    printf("FATAL ERROR: memmap.txt not found."
          "Please re run gen_mem_map.py and copy memmap.txt to /usr/share/vigil\n");
    exit(-1);
  }
  char * line = (char *)malloc(sizeof(char) * 21);
  size_t pos, len = 0;
  while((pos = getline(&line,&len,fp)) != -1){
    line[strcspn(line,"\n")] = 0;
    // printf("%s\n",line);
    char name[32];
    unsigned int start;
    unsigned int end;


    int num_parsed = 0;
    char * token = strtok(line, "\x20");
    while(token != NULL){
      switch(num_parsed){
        case 0:

          strcpy(name,token);
          break;
        case 1:
          // printf("Start: %s\n",token);
          start = (unsigned int)strtol(token,NULL,16);
          break;
        case 2:
          end = (unsigned int) strtol(token,NULL,16);
          break;
        default:
          break;
      }
      token = strtok(NULL,"\x20");
      num_parsed++;
    }

    add_fn(name,start,end);
  }
  free(line);
  fclose(fp);
}


