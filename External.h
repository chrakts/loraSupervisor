#ifndef EXTERNAL_H_INCLUDED
#define EXTERNAL_H_INCLUDED

#include "Serial.h"
#include "myConstants.h"
#include "Communication.h"
#include "CRC_Calc.h"
#include "SPI.h"
#include "LoRa.h"
#include "Cmulti2Buffer.h"
#include "ComReceiver.h"
#include "loraSupervisor.h"

extern const char *Node;

extern uint8_t isBroadcast;

extern const char *fehler_text[];

extern char Compilation_Date[];
extern char Compilation_Time[];

extern volatile float fExternalTemperature;

extern char SecurityLevel;

extern CRC_Calc crcGlobal;


extern uint8_t outputPins[];
extern SPI mySPI;

extern Communication cmulti;
extern ComReceiver cmultiRec;
class LoRaClass;

extern LoRaClass LoRa;

extern volatile  uint8_t txIsReady;
extern volatile  uint8_t rxIsReady;

extern volatile int8_t rxRssi;
extern  char rxMessage[LORAMESSAGELENGTH];

extern uint8_t key[];

extern char LoraCmultiBuffer[];
extern Cmulti2Buffer loraCmulti;

#endif // EXTERNAL_H_INCLUDED


