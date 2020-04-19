#define EXTERNALS_H_

#include "loraSupervisor.h"
#include "../Secrets/secrets.h"

#ifdef SENDER_BUILT
const char *Node = "tx";
#endif // SENDER_BUILT
#ifdef RECEIVER_BUILT
const char *Node = "rx";
#endif // RECEIVER_BUILT

char Compilation_Date[] = __DATE__;
char Compilation_Time[] = __TIME__;


const char *fehler_text[]={"memory errors","parameter error","unknown job","no transmission","command not allowed","CRC error","no active sensor"};

uint8_t isBroadcast = false;

volatile float fExternalTemperature = 5.0;

char SecurityLevel = 0;


SPI mySPI(0);
LoRaClass LoRa(&mySPI);

Communication cmulti(0,Node,5,true);
ComReceiver cmultiRec(&cmulti,Node,cnetCommands,NUM_COMMANDS,cnetInformation,NUM_INFORMATION);

volatile uint8_t txIsReady = false;
volatile uint8_t rxIsReady = false;

volatile int8_t rxRssi = 0;
String rxMessage;

uint8_t key[16] = {AES256_KEY};

char LoraCmultiBuffer[LORACMULTIBUFFERR_LENGTH];
Cmulti2Buffer loraCmulti(LoraCmultiBuffer,LORACMULTIBUFFERR_LENGTH,Node);
