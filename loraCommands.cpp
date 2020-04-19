#include "loraCommands.h"
#include "External.h"
#include "../Secrets/secrets.h"


COMMAND cnetCommands[NUM_COMMANDS] =
{
  {'-','-',CUSTOMER,NOPARAMETER,0,jobGotCRCError}, // Achtung, muss immer der erste sein
  {'S','K',CUSTOMER,STRING,16,jobSetSecurityKey},
  {'S','k',CUSTOMER,NOPARAMETER,0,jobGetSecurityKey},
  {'S','C',DEVELOPMENT,NOPARAMETER,0,jobGetCompilationDate},
  {'S','T',DEVELOPMENT,NOPARAMETER,0,jobGetCompilationTime},
  {'S','m',PRODUCTION,NOPARAMETER,0,jobGetFreeMemory},
  {'T','d',CUSTOMER,STRING,34,jobTestDecrypt},    // JOB_RESET_ACT		'r'
  {'T','f',CUSTOMER,FLOAT,2,jobTestFloat},    // JOB_RESET			'R'
  {'T','B',CUSTOMER,BYTEARRAY,16,jobTestByteArray},    // JOB_DC_ATTENTION	'D'			// Direkten Channel zum rueckwaerigen Geraet
  {'M','C',CUSTOMER,NOPARAMETER,0,NULL},    // JOB_DC_START		'C'
  {'M','M',CUSTOMER,NOPARAMETER,0,NULL},    // JOB_BE_MASTER		'M'
  {'M','m',CUSTOMER,NOPARAMETER,0,NULL},    // JOB_RELEASE_MASTER	'm'
  {'M','T',CUSTOMER,NOPARAMETER,0,NULL},    // JOB_BE_TUNNEL		'T'
  {'M','t',CUSTOMER,NOPARAMETER,0,NULL},    // JOB_RELEASE_TUNNEL	't'

};

INFORMATION cnetInformation[NUM_INFORMATION]=
{
  {"tx",'C','1','T',FLOAT,1,(void*)&fExternalTemperature,NULL}
};

void jobTestByteArray(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
  cmulti.sendByteArray(((uint8_t *)pMem),16,"->",'S','B','B','B');
}

void jobTestFloat(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
  cmulti.sendAnswerDouble("!!", 'F','F','F',((float *)pMem)[0],true);
  cmulti.sendAnswerDouble("!!", 'G','G','G',((float *)pMem)[1],true);
}

void jobTestDecrypt(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
	comRec->sendAnswer((char *)pMem,function,address,job,false);
}

void jobGotCRCError(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
	comRec->sendAnswer("CRC-Fehler",function,address,job,false);
}

void jobSetSecurityKey(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
uint8_t ret = true;
	if (strcmp((char *)pMem,SECURITY_LEVEL_PRODUCTION_KEY)==0)
	{
    comRec->SetSecurityLevel(PRODUCTION);
	}
	else if(strcmp((char *)pMem,SECURITY_LEVEL_DEVELOPMENT_KEY)==0)
	{
    comRec->SetSecurityLevel(DEVELOPMENT);
	}
	else
	{
    comRec->SetSecurityLevel(CUSTOMER);
		ret = false;
	}
	comRec->sendAnswerInt(function,address,job,comRec->GetSecurityLevel(),ret);
}

void jobGetSecurityKey(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
	comRec->sendAnswerInt(function,address,job,comRec->GetSecurityLevel(),true);
}


void jobGetCompilationDate(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
char temp[20];
	strcpy(temp,Compilation_Date);
	comRec->sendAnswer(temp,function,address,job,true);
}

void jobGetCompilationTime(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
char temp[20];
	strcpy(temp,Compilation_Time);
	comRec->sendAnswer(temp,function,address,job,true);
}

void jobGetFreeMemory(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
extern int __heap_start, *__brkval;
int v;

	uint16_t mem = (uint16_t) &v - (__brkval == 0 ? (uint16_t) &__heap_start : (uint16_t) __brkval);
	comRec->sendAnswerInt(function,address,job,mem,true);
}
