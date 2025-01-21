#!/bin/bash

# GLOBAL VARS--------------------------------------------------------------------------------------
# File Locations
sourcedShellScriptsLoc="$sharedScripts"
csvDir="$(findPath -f 76)/" #fileAndFolderInfo dir
filesAndFoldersLoc="$(findPath -f 79)" #filesAndFolders.csv
incLoc="$(findPath -f 120)/" #include
tableColTypeListLoc="$(findPath -f 357)" #tableColTypeList.csv
autoExecLoc="$(findPath -f 112)" #autoExec.sh
structsHeaderLoc="$(findPath -f 190)" #structs.h"
accDataTypesLoc="$(findPath -f 356)" #acceptedDbDataTypes.csv

# Table Col Numbers
dataTypesColTypeListColNum="4" #HARDCODED_COL_NUM
colNameColTypeListColNum="3" #HARDCODED_COL_NUM
defaultValColTypeListColNum="5" #HARDCODED_COL_NUM
tableNameColTypeListColNum="2" #HARDCODED_COL_NUM
tableIdColTypeListColNum="1" #HARDCODED_COL_NUM
colIdColTypeListColNum="0" #HARDCODED_COL_NUM

# Database Files To Add Code To
addRowLoc="$(findPath -f 229)" #addRow.c
creatStructLoc="$(findPath -f 230)" #createStruct.c
deepCpyLoc="$(findPath -f 232)" #deepCpy.c
getRowsWhereLoc="$(findPath -f 223)" #getRowsWhere.c
getStructValLoc="$(findPath -f 231)" #getStructMemberVal.c
remRowLoc="$(findPath -f 226)" #remRow.c
remRowOnValLoc="$(findPath -f 225)" #remRowOnVal.c
setStructValLoc="$(findPath -f 228)" #setStructMemberVal.c

# genericC and genericCPreReqs Files To Add Code To
myFreeLoc="$(findPath -f 35)" #myFree.c
#myPrintLoc="$(findPath -f 152)" #printTable.c #FIXME uncomment when printTable is finished
getArrSizeLoc="$(findPath -f 161)" #getArrSize.c
convertLoc="$(findPath -f 31)" #convert.c

#FIXME add myPrintLoc here
listOfFilesToAddCodeTo=("$addRowLoc" "$deepCpyLoc" "$getRowsWhereLoc" "$getStructValLoc" "$remRowLoc" "$remRowOnValLoc" "$setStructValLoc" "$creatStructLoc" "$myFreeLoc" "$getArrSizeLoc" "$convertLoc")
listOfExistingFilesToAddCodeTo=("$myFreeLoc" "$getArrSizeLoc" "$convertLoc") #FIXME add myPrintLoc here
listOfDbFilesToAddCodeTo=("$addRowLoc" "$deepCpyLoc" "$getRowsWhereLoc" "$getStructValLoc" "$remRowLoc" "$remRowOnValLoc" "$setStructValLoc" "$creatStructLoc")

# Error Msg Var Name For Database Files To Add Code To
F_addRow="F_ADDROW"
F_creatStruct="F_CREATSTRUCT"
F_deepCpy="F_DEEPCPY"
F_getRowsWhere="F_GETROWSWHERE"
F_getStructVal="F_GETSTRUCTVAL"
F_remRow="F_REMROW"
F_remRowVal="F_REMROWVAL"
F_setStructVal="F_SETSTRUCTVAL"

# Error Msg Var Name For Database Files To Add Code To
F_myFree="F_MYFREE" #isn't used, but keep just in case
F_myPrint="F_PRNTTABLE"
F_getArrSize="F_GETARRSIZE" #isn't used, but keep just in case
F_convert="F_CONVERT"

# Other Vars
varNames=""
dbObjNum=""
structName=""
dataTypes=""
defaultVals=""
csvLoc=""

# File locs for files in dbCreateAndManage
dbCMDir="$(findPath -f 387)/" #dbCreateAndManage dir
dbACDir="$(findPath -f 390)/" #addCode dir
dbGCDir="$(findPath -f 396)/" #genCodeDb dir
dbRCDir="$(findPath -f 410)/" #remCode dir

dbAddAndRemLoc="$(findPath -f 409)" #addAndRemFromDbFiles.sh

acConvertShLoc="$(findPath -f 392)" #acConvert.sh
acCreatStructShLoc="$(findPath -f 394)" #acCreateStruct.sh
acGenericShLoc="$(findPath -f 395)" #acGeneric.sh
acGetArrSizeShLoc="$(findPath -f 393)" #acGetArrSize.sh
acMyFreeShLoc="$(findPath -f 391)" #acMyFree.sh

gcAddRowShLoc="$(findPath -f 404)" #gcAddRow.sh"
gcConvertShLoc="$(findPath -f 398)" #gcConvert.sh
gcCreateHeaderShLoc="$(findPath -f 403)" #gcCreateHeaderFile.sh
gcCreateStructShLoc="$(findPath -f 406)" #gcCreateStruct.sh
gcDeepCpyShLoc="$(findPath -f 402)" #gcDeepCpy.sh
gcFindRowsWhereShLoc="$(findPath -f 408)" #gcFindRowsWhere.sh
gcGetArrSizeShLoc="$(findPath -f 397)" #gcGetArrSize.sh
gcGetStructMemValShLoc="$(findPath -f 405)" #gcGetStructMemberVal.sh
gcMyFreeShLoc="$(findPath -f 399)" #gcMyFree.sh
gcRemRowIdShLoc="$(findPath -f 400)" #gcRemoveRowById.sh
gcRemRowValShLoc="$(findPath -f 407)" #gcRemoveRowByVal.sh
gcSetStructMemValShLoc="$(findPath -f 401)" #gcSetStructMemberVal.sh

rcConvertShLoc="$(findPath -f 414)" #rcConvert.sh
rcGenericShLoc="$(findPath -f 412)" #rcGeneric.sh
rcGetArrSizeShLoc="$(findPath -f 411)" #rcGetArrSize.sh
rcmyFreeShLoc="$(findPath -f 413)" #rcMyFree.sh

# Error Msg Constant Vars
F_DBSH="${BASH_SOURCE[0]}"
#--------------------------------------------------------------------------------------------------

# SOURCING-----------------------------------------------------------------------------------------
source "$sharedScripts"
checkFileLocs $F_DBSH

# Outside scripts
source "$sourcedShellScriptsLoc"
# In dbCreateAndManage
source "$dbAddAndRemLoc"
# In Add Code
source "$acConvertShLoc"
source "$acCreatStructShLoc"
source "$acGenericShLoc"
source "$acGetArrSizeShLoc"
source "$acMyFreeShLoc"
# In Generate Code
source "$gcAddRowShLoc"
source "$gcConvertShLoc"
source "$gcCreateHeaderShLoc"
source "$gcCreateStructShLoc"
source "$gcDeepCpyShLoc"
source "$gcFindRowsWhereShLoc"
source "$gcGetArrSizeShLoc"
source "$gcGetStructMemValShLoc"
source "$gcMyFreeShLoc"
source "$gcRemRowIdShLoc"
source "$gcRemRowValShLoc"
source "$gcSetStructMemValShLoc"
# In Remove Code
source "$rcConvertShLoc"
source "$rcGenericShLoc"
source "$rcGetArrSizeShLoc"
source "$rcmyFreeShLoc"
#--------------------------------------------------------------------------------------------------
