#define EXTERNALS_H_

#include "loraSupervisor.h"

#ifdef SENDER_BUILT
const char *Node = "tx";
#endif // SENDER_BUILT
#ifdef RECEIVER_BUILT
const char *Node = "rx";
#endif // RECEIVER_BUILT



const char *fehler_text[]={"memory errors","parameter error","unknown job","no transmission","command not allowed","CRC error","no active sensor"};

uint8_t isBroadcast = false;


char Compilation_Date[] = __DATE__;
char Compilation_Time[] = __TIME__;

char SecurityLevel = 0;


SPI mySPI(0);
LoRaClass LoRa(&mySPI);

Communication cmulti(0,Node,5);

volatile uint8_t txIsReady = false;
volatile uint8_t rxIsReady = false;

volatile int8_t rxRssi = 0;
String rxMessage;


char LoraCmultiBuffer[LORACMULTIBUFFERR_LENGTH];
Cmulti2Buffer loraCmulti(LoraCmultiBuffer,LORACMULTIBUFFERR_LENGTH,Node);
