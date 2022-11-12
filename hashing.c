#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <getopt.h>
#include "strSHA2.c"
#include "listfiles.c"


#define CHECK_ALLOC(p) if(p == NULL) { perror(__func__); exit(EXIT_FAILURE); }
#define	HASHTABLE_SIZE		997

int duplicatesCount = 0; 
int duplicatesSize = 0;

// Struct for saving duplicates of a given hash
typedef struct duplicates {
    char *fileName;
    char *filePath;
    int fileSize;
    struct duplicates *next; 
} DUPES;

// Struct for saving files and their hashes
typedef struct _list {
    char           *hash;
    char           *fileName;
    char           *filePath;
    int            fileSize;
    struct duplicates *first;
    struct _list   *next;
} LIST;

// Create new duplicate item
DUPES *dupe_new_item (char *fileName, char *filePath, int fileSize) 
{
    DUPES *new = malloc( sizeof(DUPES) );
    CHECK_ALLOC(new);

    new->fileName = strdup(fileName);
    CHECK_ALLOC(new->fileName);

    new->filePath = strdup(filePath);
    CHECK_ALLOC(new->filePath);

    new->fileSize = fileSize;
    new->next = NULL;

    return new;
}

// Add duplicate to target file node
void add_duplicate(LIST *list, char *fileName, char *filePath, int fileSize)
{
    if (list->first == NULL) 
    {
        DUPES *new = dupe_new_item(fileName, filePath, fileSize);
        list->first = new; 
    }
    else {
        DUPES *first = list->first;

        bool foundLast = false;

        while(!foundLast) 
        {
            if (first->next == NULL)
            {
                DUPES *new = dupe_new_item(fileName, filePath, fileSize);
                first->next = new; 
                foundLast = true;
            }
            first = first->next;
        }
    }
}

// Find a list item with the given hash
LIST  *list_find(LIST *list, char *wanted)
{
    while(list != NULL) {
        if(strcmp(list->hash, wanted) == 0) {
            return list;
        }
        list	= list->next;
    }
    return NULL;
}

// Create new list item node
LIST *list_new_item(char *newHash, char *fileName, char *filePath, int fileSize)
{
    LIST *new       = malloc( sizeof(LIST) );
    CHECK_ALLOC(new);

    new->hash     =  strdup(newHash);
    CHECK_ALLOC(new->hash);

    new->fileName = strdup(fileName);
    CHECK_ALLOC(new->fileName);

    new->filePath = strdup(filePath);
    CHECK_ALLOC(new->filePath);

    new->fileSize = fileSize;
    new->first = NULL;
    new->next = NULL;

    return new;
}

// Add New List item to Hashtable
LIST *list_add(LIST *list, char *hash, char *fileName, char *filePath, int fileSize)
{
    if(list_find(list, hash)) {            // only add each item once
        return list;
    }
    else {                                      // add new item to head of list
        LIST *new   = list_new_item(hash, fileName, filePath, fileSize);
        new->next   = list;
        return new;
    }
}

typedef	LIST * HASHTABLE; 

//CONVERTS HASH WHICH IS TYPE CHAR INTO uint32_t
uint32_t hash_string(char *string)  //this fuction takes hash returned by strSHA2(char *filename) function.
{
    uint32_t hash = 0;

    while(*string != '\0') {
        hash = hash*33 + *string;
        ++string;
    }
    return hash % HASHTABLE_SIZE; //all the hashes are in the length of HASHTABLE.
}

// Creates new hashtable
HASHTABLE *hashtable_new(void)
{
    HASHTABLE   *new = malloc(HASHTABLE_SIZE * sizeof(LIST *));

    CHECK_ALLOC(new);
    return new;
}

// Add new file node to hashtable
void hashtable_add(HASHTABLE *hashtable, char *hash, char *fileName, char *filePath, int fileSize)
{
    uint32_t h   = hash_string(hash) % HASHTABLE_SIZE;    

    hashtable[h] = list_add(hashtable[h], hash, fileName, filePath,fileSize);
}

// Find an existing file node in hashtable
LIST *hashtable_find(HASHTABLE *hashtable, char *hash)
{
    uint32_t h	= hash_string(hash) % HASHTABLE_SIZE;     

    return list_find(hashtable[h], hash);
}


// Main Hashtable Creation function
int hash_file_directory(HASHTABLE *hashtable, char *path, bool checkAll)
{
    int id = open(path,O_RDONLY);

    // Populate hashtable from directory
    if(id >= 0)
    {
        files_recursively(path,checkAll);

        for (int a=0; a<totalFiles; a++)
        {
            char *currentHash = strSHA2(pathArr[a]);   
            if (currentHash != NULL)
            {                     
                int currentSize = findSize(pathArr[a]);

                LIST *targetNode = hashtable_find(hashtable, currentHash);
                if (targetNode == NULL)
                {
                    // Add current file to hashtable
                    hashtable_add(hashtable, currentHash, fileNames[a], pathArr[a], currentSize); 
                }
                else
                {
                    // Add current file to the duplicates list of target hash
                    add_duplicate(targetNode, fileNames[a], pathArr[a], currentSize);
                    duplicatesCount = duplicatesCount + 1;
                    duplicatesSize += findSize(pathArr[a]);
                }
            }	
        }
    } 
    return 0; 
}      