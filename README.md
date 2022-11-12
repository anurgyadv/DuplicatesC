**Background**

In recent years, computer disk sizes and densities have increased dramatically, with costs dropping to 4c/gigabyte (HDD) and 14c/gigabyte (SSD). As a consequence, we store files on our computers' disks in very different ways and, because we typically have a huge amount of free space available, we end up having multiple copies of many files on our disks. This presents few problems, until we eventually run out of disk space, or need to transfer files to more expensive cloud-storage, perhaps over a network with limited bandwidth. At that time we'd like to locate all duplicate files, and only make one copy of them to our backup destination.

The  **goal**  of this project is to design and develop a command-line utility program, named _duplicates_, to locate and report duplicate files in, and below, a named directory.

 Successful completion of the project will develop and enhance your understanding of advanced features of the C11 programming language, and core Linux operating system system-calls and POSIX function calls.

There are two parts to the project:

1. The _basic_ version requires you to develop a working program with a restricted set of features. It is possible to receive full marks for the project by only completing the _basic_ version.
2. 🌶If you would like a greater challenge, you may like to attempt the _advanced_ version of this project. Undertaking and completing significant parts of the advanced tasks provides the opportunity to _recover_ any marks not awarded for the _basic_ version of the project.

**Program invocation**

Your implementation of _duplicates_ will be invoked with zero or more valid command-line options, and one directory name. With no command-line options (i.e. only a directory name is provided) _duplicates_ will simply list 4 things (with just one integer per line):

1. the total number of files found,
2. the total size (in bytes) of all files found,
3. the total number of unique files (i.e. any duplicate is only counted once), and
4. the possible minimum total size of all files found (i.e. the sizes of duplicated files are only counted once).

Files and directories (other than the "starting" directory indicated on the command-line) which cannot be read should be silently ignored (no error messages should be printed).
 For the _basic_ project, the "starting" directory will only contain regular files and sub-directories. In particular, there will be no hard- or symbolic-links. Your project is required to support the following command-line options and, if attempting the _advanced_ version of the project, command-line options marked with a chili 🌶.

| -a | By default, hidden and configuration files (conventionally those beginning with a '.' character) are not considered by _duplicates_. Providing the _-a_ option requests that _all_ files be considered. This is similar to the standard Linux utility _ls_. |
| --- | --- |
| -A | This option indicates if the program attempts the _advanced_ version of the project.
_duplicates -A_ produces no output at all, simply terminating with _EXIT\_SUCCESS_ (for advanced) or with _EXIT\_FAILURE_ (for basic). |
| -f filename | find and list, one per line, the relative pathnames of all files whose SHA2 hash matches that of the indicated file. The name of the indicated file is _not_ listed.
_duplicates -f_ terminates with _EXIT\_SUCCESS_ if any matching files are found, or with _EXIT\_FAILURE_ otherwise. |
| -h hash | find and list, one per line, the relative pathnames of all files with the indicated SHA2 hash.
_duplicates -h_ terminates with _EXIT\_SUCCESS_ if any matching files are found, or with _EXIT\_FAILURE_ otherwise. |
| -l | _duplicates_ lists all duplicate files found. Each line of output consists of the relative pathnames of two or more files that are duplicates of each other. The pathnames of duplicate files (on the same line line) must be separated by the _TAB_ character. |
| -m | 🌶 _duplicates_ minimizes the total number of bytes required to store all files' data by modifying the directory structure being considered. |
| -q | _duplicates_ executes quietly, simply testing if the named directory contains any duplicate files.
_duplicates -q_ produces no output at all, simply terminating with _EXIT\_SUCCESS_ if there are no duplicates (i.e. storage is already minimized), or with _EXIT\_FAILURE_ otherwise. |

**Detecting duplicate files**

- Two or more files are _defined to be duplicates_ iff their _contents_ are identical. Duplicate files will thus have the same size but, when determining if two files are duplicates of each other, their filenames and modification times are not considered.
- A file is _defined to be unique_ iff no other file has the same contents.

To detect duplicate files we'll employ a _cryptographic hash function_ named _SHA2_ (pronounced 'shar-2') [[Wikipedia]](https://en.wikipedia.org/wiki/SHA-2) and [[Youtube]](https://www.youtube.com/watch?v=JPTrv3zy6Cw). SHA2 examines the contents of a file and produces a fixed-length summary of its contents. Cryptographic hash functions are designed by mathematicians and those developing encryption and security software.

Here is an implementation of the function [strSHA2.c](https://teaching.csse.uwa.edu.au/units/CITS2002/past-projects/p2021-2/strSHA2.c) which you may (should) use without attribution.

Two or more files are considered identical if their cryptographic hashes are identical. For this project, we'll use a C11 string to store this representation, and two files will be considered identical if their SHA2 string representations are identical. The function _strSHA2()_, with the following prototype:

**char** \*strSHA2( **char** \*filename);

is provided for this project (it is not a standard C11 function). If _strSHA2_ can read the indicated file, it will return a dynamically allocated string holding the SHA2 string representation of the file's contents. If the indicated file cannot be read, _strSHA2_ will return _NULL_.
