#ifndef CREATESTR
#define CREATESTR

#include "symLink_32.sl"
#include "symLink_30.sl"
#include "symLink_48.sl"


//=================================================================================================
// DEFINES
//=================================================================================================


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

//=================================================================================================

//=================================================================================================
// FUNCTION DECLARATIONS
//=================================================================================================

char* createStrHelper(...);
//=================================================================================================

#endif
