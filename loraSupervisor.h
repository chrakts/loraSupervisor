#ifndef HX711ADAPTER_H_INCLUDED
#define HX711ADAPTER_H_INCLUDED


#include <avr/io.h>
#include <stdbool.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/sleep.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "myConstants.h"

#include "Serial.h"
#include "External.h"
#include "../Secrets/secrets.h"
#include "Communication.h"
#include "ComReceiver.h"
#include "loraCommands.h"
#include "../xmegaClocks/xmegaClocks.h"
#include "LoRa.h"
#include "Cmulti2Buffer.h"

void setup();
void onTxDone();
void onReceive(int packetSize);
void LoRa_sendMessage(String message);
void LoRa_txMode();
void LoRa_rxMode();
void evaluate(void);

#endif // HX711ADAPTER_H_INCLUDED
