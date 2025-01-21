#include "symLink_46.sl"
#include "symLink_30.sl"
#include "symLink_48.sl"

// THIS FILE IS NOT COMPLETE


// GLOBAL VARS-------------------------------------------------------------------------------------
// Error Msg Constant Vars
char* F_PRNTTABLE = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------

//=================================================================================================
// CODE
//=================================================================================================
void centerTableElement(char* val, int colWidth){
        int spacesEachSide = (colWidth-strlen(val))/2;
        for (int i = 0; i < spacesEachSide; i++){ printf(" ");}
        printf(val);
        for (int i = 0; i < spacesEachSide; i++){
                printf(" ");
        }
        if (colWidth-(spacesEachSide*2)-strlen(val) != 0){ printf(" ");}
        printf("|");

}


void printHeaderRowFilesAndFolders(){
          printf(" id |type|inUse|                              full path                             |           name            |ext|    path in repo    |       name in repo    |moved| old path        |");
        printf("\n----|----|-----|---------------------------------------------------------------------|--------------------------|------------------------------------------------------------------------\n");
}

void printFilesDbObj(struct filesAndFoldersDbObj* objIn){
	
        char idAsStr[128]; snprintf(idAsStr, sizeof(idAsStr), "%d", objIn->id);
        centerTableElement((char*) idAsStr, 4);
        char typeAsStr[2]; typeAsStr[0] = objIn->type; typeAsStr[1] = '\0';
        centerTableElement(typeAsStr, 4);
        char inUseAsStr[2]; snprintf(inUseAsStr, sizeof(inUseAsStr), "%d", objIn->inUse);
        centerTableElement(inUseAsStr, 5);
        centerTableElement(objIn->fullPath, 70);
        centerTableElement(objIn->fileName, 27);
        centerTableElement(objIn->extension, 3);
        centerTableElement(objIn->pathInRepo, 20);
        char movedAsStr[2]; snprintf(movedAsStr, sizeof(movedAsStr), "%d", objIn->movedOrChangedName);
        centerTableElement((char*) movedAsStr, 5);
        centerTableElement(objIn->oldPath, 20);
        
        char symLinkAsStr[32]; snprintf(symLinkAsStr, sizeof(symLinkAsStr), "%d", objIn->symLinkNum);
	centerTableElement(symLinkAsStr, 5);

	char rmdAsStr[32]; snprintf(rmdAsStr, sizeof(rmdAsStr), "%d", objIn->rmd);
	centerTableElement(rmdAsStr, 5);

	char rweAsStr[32]; snprintf(rweAsStr, sizeof(rweAsStr), "%d", objIn->rwe);
	centerTableElement(rweAsStr, 5);


        puts("");
}
//=================================================================================================

void printTable(void* obj){ puts("filler function. haven't built it out yet");}
