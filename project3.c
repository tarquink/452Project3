#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#ifndef NAME_MAX
#define NAME_MAX 1024
#endif

static long runTheFiles(char *dirname)
{
  //sets up the directory
  DIR *dir = opendir(dirname);
  if (dir == 0)
    return 0;
  
  // stucts for dir and file stats
  struct dirent *dit;
  struct stat st;
  
  // size of a file
  long size = 0;
  // size of all runTheFiles
  long total_size = 0;
  // output of filepath
  char filePath[NAME_MAX];
  
  // 
  while ((dit = readdir(dir)) != NULL)
  {
    if ( (strcmp(dit->d_name, ".") == 0) || (strcmp(dit->d_name, "..") == 0) )
      continue;
    
    sprintf(filePath, "%s/%s", dirname, dit->d_name);
    if (lstat(filePath, &st) != 0)
      continue;
    size = st.st_size;
    
    //check if it is a directory
    if (S_ISDIR(st.st_mode))
    {
      long dir_size = runTheFiles(filePath) + size;
      printf("%s", filePath);
      printf(" (%ld bytes)\n", dir_size);
      total_size += dir_size;
    }
    else
    {
      //else it is a file
      total_size += size;  
      printf("%s", filePath);
      printf(" (%ld bytes)\n", size);
    }
  }
  return total_size;
}

int main(int argc, char *argv[])
{
  // If no command line args then just run on current dir
  if (argc < 2) {
    long theSize = runTheFiles(".");
    printf("Total size: %ld\n", theSize);
    return 0;
  }
  
  // else take the command line args
  long totalSize = runTheFiles(argv[1]);
  printf("Total size: %ld\n", totalSize);
  return 0;
}
