#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <sys/param.h>
#include "filesize.c"

//got the idea to implement this fuction from this website:
//https://codeforwin.org/2018/03/c-program-to-list-all-files-in-a-directory-recursively.html

char fileNames[MAXPATHLEN][MAXPATHLEN];
char pathArr[MAXPATHLEN][MAXPATHLEN];

int i = 0, totalFiles = 0;
int totalFilesSize = 0;

void files_recursively(char *path,bool value);

//GIVES ALL FILE NAMES AND PATHS FOR GIVEN DIRECTORY WHICH MAY CONTAIN SUB DIRECTORY. 
//HERE char *path is directory name. 
void files_recursively(char *basePath,bool value)
{
    char path[100000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);
    //Cannot open the directory.
    if (!dir){
        return;
    }
    
    while ((dp = readdir(dir)) != NULL)
    { 
        if(value==1)
        {
            if(!(strcmp(dp->d_name,".")==0 || strcmp(dp->d_name,"..")==0)){

            //gets new path from our base path
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);
            strcpy(fileNames[i],dp->d_name); //copies the directory name into fileNames 
            
            //we don't need paths for directories and only need total files counts.
            DIR *d = opendir(path);
            if(d == NULL){      //if it is not a directory 
                strcpy(pathArr[i],path);
                totalFiles++;
                totalFilesSize += findSize(path);
            }
            i++;
            files_recursively(path, value); //performs recursion for the subdirectories.
            }
        }

         
        else if (value==0 ) {
            if (dp->d_name[0] == 46) {
                continue;
            }
            else 
            {
                //gets new path from our base path
                strcpy(path, basePath);
                strcat(path, "/");
                strcat(path, dp->d_name);
                strcpy(fileNames[i],dp->d_name);
                
                DIR *d = opendir(path);
                if(d == NULL)
                {
                    strcpy(pathArr[i],path);
                    totalFiles++;
                    totalFilesSize += findSize(path);
                }
                i++;
                files_recursively(path, value);
            }
        }
    }
    closedir(dir);
}

