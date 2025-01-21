#include "symLink_32.sl"
#include "symLink_30.sl"
#include "symLink_48.sl"

//=================================================================================================
// CODE
//=================================================================================================
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wvarargs"
// This function alloc's a new string and sets first char at index 0 to be end-of-string character
char* createStrHelper(...){
        char* returnStrCreateStr;
        int defaultStrInc = 128;

        va_list args;
        va_start(args, 1);
        int sizeIn = va_arg(args, int);

        if (sizeIn == 0){
                returnStrCreateStr = malloc(sizeof(char)*(defaultStrInc+1));
                returnStrCreateStr[0] = '\0';
        }
        else{
                returnStrCreateStr = malloc(sizeof(char)*(sizeIn+1));
                returnStrCreateStr[0] = '\0';
        }

        return returnStrCreateStr;
}
#pragma GCC diagnostic pop
//=================================================================================================


//=================================================================================================
// BACKUP OF DEFINES 
//=================================================================================================
/*
#define createStrHelperDefine(...) ({ \
                char* returnStr; \
                do { \
                        int numArgs = COUNT_ARGS(__VA_ARGS__); \
                        if (numArgs == 0){ returnStr = createStrHelper(0);} \
                        else{ returnStr = createStrHelper(__VA_ARGS__);} \
                } while(0); \
                returnStr; \
                }) \


#define createStr(...) createStrHelperDefine(__VA_ARGS__) \
*/
//=================================================================================================

