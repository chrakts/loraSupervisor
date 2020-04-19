#ifndef LORACOMMANDS_H_INCLUDED
#define LORACOMMANDS_H_INCLUDED

#include "ComReceiver.h"

#define NUM_COMMANDS 14

#define NUM_INFORMATION 1

extern COMMAND cnetCommands[];
extern INFORMATION cnetInformation[];

void jobTestByteArray(ComReceiver *comRec, char function,char address,char job, void * pMem);
void jobTestDecrypt(ComReceiver *comRec, char function,char address,char job, void * pMem);
void jobTestFloat(ComReceiver *comRec, char function,char address,char job, void * pMem);
void jobGotCRCError(ComReceiver *comRec, char function,char address,char job, void * pMem);
void jobSetSecurityKey(ComReceiver *comRec, char function,char address,char job, void * pMem);
void jobGetSecurityKey(ComReceiver *comRec, char function,char address,char job, void * pMem);
void jobGetCompilationDate(ComReceiver *comRec, char function,char address,char job, void * pMem);
void jobGetCompilationTime(ComReceiver *comRec, char function,char address,char job, void * pMem);
void jobGetFreeMemory(ComReceiver *comRec, char function,char address,char job, void * pMem);



#endif // LORACOMMANDS_H_INCLUDED
