// CITS2002 Project 2
// Names: Anurag Yadav, Dhwani Rahul Joshi
// Student Numbers: ,

#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <dirent.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <getopt.h>
#include "hashing.c"

#define OPTLIST  "aAfhlq"

//Prints usage instructions if input option is not defined.
void usage(char *progname) {
    fprintf(stderr, "Usage: %s, [options] dirname\n", progname);
    fprintf(stderr, "-a\t\tcheck all files including the files beginning with '.'\n");
    fprintf(stderr, "-l\t\tlist all duplicates\n");
    fprintf(stderr, "-q\t\tquietly test if any duplicates exist\n");
    fprintf(stderr, "-h hash\t\tfinds files with hash value equal to the one provided\n");
    fprintf(stderr, "-f filename\t\tfinds files with the same hash as the file provided\n");
    exit (EXIT_FAILURE);
}
char *File_Name;
char *hash_strSHA2;
char *directory;

void print_duplicates(LIST *targetNode, char *fileName);
void print_all_duplicates(HASHTABLE *hashtable);

int main(int argc, char *argv[])
{                                                
    char *progname = (progname = strrchr(argv[0], '/')) ? progname+1 : argv[0];

    bool checkAll = false; 
    bool checkFilename = false; 
    bool checkHash = false; 
    bool listDuplicates = false; 
    bool executeQuietly = false;

    int opt;

    //Parse Command Line Options
    while( (opt = getopt(argc, argv, OPTLIST)) != -1){ 
        switch (opt) {
            case 'a':   checkAll = true;
                        break;
            case 'A': exit(EXIT_FAILURE);
                        break;
            case 'f': checkFilename = true;
                        break;
            case 'h': checkHash = true;
                        break;
            case 'l': listDuplicates = true;
                        break;
            case 'q': executeQuietly = true; 
                        break;
            default: printf("%s : not an option \n -%c\n", progname, optopt);
                        argc = -1;
                        break;
        }
    }

    // Check if usage is proper
    if (argc<2)
    {
        usage(progname);
        exit(EXIT_FAILURE);
    }
    
    char *path = argv[argc-1];
    // Create new hashtable 
    HASHTABLE *hashtable = hashtable_new(); 
    hash_file_directory(hashtable, path, checkAll);

    // -f or -h Option execution
    if (checkFilename || checkHash)
    {
        char *fileName = argv[2];   
        int fileIndex=0;
        for(int a=0; a<totalFiles; a++)
        {
            if(!strcmp(fileNames[a],fileName))
            {                   
                fileIndex = a;
            }
        }
        
        char *givenHash; 
        if(checkHash)
        {
            givenHash = argv[2];
        } 
        else if (checkFilename)
        {
            givenHash = strSHA2(pathArr[fileIndex]);
        }
        
        LIST *targetNode = hashtable_find(hashtable, givenHash);
        
        if (targetNode != NULL)
        {
            print_duplicates(targetNode, fileName);
            exit(EXIT_SUCCESS);
        }
        exit(EXIT_FAILURE);
    }

    // -l Option execution
    if (listDuplicates)
    {
        print_all_duplicates(hashtable);
        exit(EXIT_SUCCESS);
    }

    // -q Option execution
    if(executeQuietly){
        exit(duplicatesCount!=0 ? EXIT_FAILURE : EXIT_SUCCESS);
    }

    // Basic Execution 
    else {
        printf("the total number of files found: %i\n",totalFiles);
        printf("the total size (in bytes) of all files found: %i\n",totalFilesSize);
        printf("the total number of unique files (i.e. any duplicate is only counted once):%i\n",totalFiles-duplicatesCount);
        printf("the possible minimum total size of all files found (i.e. the sizes of duplicated files are only counted once):%i \n", totalFilesSize - duplicatesSize);
        return 0;
    }
}

void print_duplicates (LIST *targetNode, char *fileName)
{
    DUPES *dupe = targetNode->first;
    int dupeCount = 0;
    if (strcmp(fileName, targetNode->fileName) != 0)
    {
        printf("./%s\n",targetNode->fileName);
    }

    while (dupe != NULL)
    {
        if (strcmp(fileName, dupe->fileName) != 0)
        {
            printf("./%s\n",dupe->fileName);
        }
        dupeCount++;
        dupe = dupe->next;
    }

    exit(dupeCount==0 ? EXIT_FAILURE : EXIT_SUCCESS);
}

void print_all_duplicates (HASHTABLE *hashtable)
{
    for (int i=0; i<HASHTABLE_SIZE; i++)
    {
        if ((hashtable[i] != NULL ) && (hashtable[i]->first != NULL))
        {
            printf("./%s\t", hashtable[i]->fileName);

            DUPES *dupe = hashtable[i]->first;

            while (dupe != NULL)
            {
                printf("./%s\t",dupe->fileName);
                dupe = dupe->next;
            }
            printf("\n");
        }
    }

}