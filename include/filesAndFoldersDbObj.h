#ifndef FILESANDFOLDERSDBOBJ
#define FILESANDFOLDERSDBOBJ

struct filesAndFoldersDbObj{
        int id;
        char type;
        int inUse;
        char* fullPath;
        char* fileName;
        char* extension;
        char* pathInRepo;
        int movedOrChangedName;
        char* oldPath;
        int symLinkNum;
        int rmd; // rename (4), move (2), delete (1)
        int rwe; // same as system
};

#endif
