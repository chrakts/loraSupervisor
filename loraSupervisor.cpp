#include "loraSupervisor.h"
#include "loraCommands.h"

#ifdef SENDER_BUILT
#pragma message ( "---------------------- Sender-Version will be build ----------------------" )
#endif // SENDER_BUILT

#ifdef RECEIVER_BUILT
#pragma message ( "---------------------- Receiver-Version will be build ----------------------" )
#endif // RECEIVER_BUILT



class LoRaClass;

uint32_t counter = 0;

void setup()
{
	init_clock(SYSCLK,PLL,true,CLOCK_CALIBRATION);

	PORTA_DIRSET = 0xf0; // 4x Eingang, dann 4x LEDs
	PORTB_DIRSET = 0x00;; // nichts angeschlossen
	PORTC_DIRSET = 0b00001011; // 4x LED, dann 4x Ausgang
	PORTD_DIRSET = 0b10111000; // nur RS232 an Pin6/7
	PORTE_DIRSET = 0x00; // nichts angeschlossen

	PORTA_OUTSET = 0xf0;

  uint8_t i,led;
	led = 0x10;
	for(i=0;i<=3;i++)
	{
		PORTA_OUT = ~led | 0x0f;
		_delay_ms(100);
		led = led<<1;
	}
	PORTA_OUT = 0xFF;
	//init_mytimer();

	PMIC_CTRL = PMIC_LOLVLEX_bm | PMIC_HILVLEN_bm | PMIC_MEDLVLEN_bm;
	sei();
	cmulti.open(Serial::BAUD_57600,F_CPU);



  #ifdef SENDER_BUILT
  cmulti.sendInfo("Hello from LORA-SENDER-Board","BR");
  #endif // SENDER_BUILT
  #ifdef RECEIVER_BUILT
  cmulti.sendInfo("Hello from LORA-RECEIVER-Board","BR");
  #endif // RECEIVER_BUILT

  if (!LoRa.begin(868E6))
  {
    while(1)
    {
      LED_ROT_TOGGLE;
      _delay_ms(100);
    }
  }

  RTC.PERL = 3;
  RTC.PERH = 0;
  CLK.RTCCTRL = CLK_RTCSRC_ULP_gc | CLK_RTCEN_bm; // 1kHz ULP
  RTC.CTRL = RTC_PRESCALER_DIV1024_gc;
  RTC.INTCTRL = RTC_OVFINTLVL_LO_gc;


#ifdef SENDER_BUILT
  //LoRa.onTxDone(onTxDone);
#endif // SENDER_BUILT
#ifdef RECEIVER_BUILT
  LoRa.onReceive(onReceive);
  LoRa_rxMode();
#endif // RECEIVER_BUILT
}

int main()
{
  setup();
  cmulti.sendInfo("Sending packet: ","BR");

#ifdef SENDER_BUILT
  while(1)
  {

  // send packet
    LED_BLAU_ON;
    //PORTC.OUTCLR = PIN0_bm;
    //PORTC.OUTSET = PIN1_bm;
    loraCmulti.clearChecksum();
    loraCmulti.sendStandardInt("BR",'S','0','C',counter);
    LoRa_sendMessage( &(loraCmulti.get()[1]) ); // header weglassen
    //while(txIsReady==false)
    //{}
    counter++;
    LED_BLAU_OFF;
    //deInitReadMonitor();
    LoRa.sleep();
    set_sleep_mode(SLEEP_MODE_PWR_SAVE);
    sleep_enable();
    sleep_cpu();
    sleep_disable();
    //initReadMonitor();

    cmultiRec.comStateMachine();
    cmultiRec.doJob();
    _delay_ms(500);
  }
#endif // SENDER_BUILT


#ifdef RECEIVER_BUILT
  while(1)
  {

    while(rxIsReady==false)
    {
      LED_GRUEN_OFF;
      set_sleep_mode(SLEEP_MODE_PWR_DOWN);
      sleep_enable();
      sleep_cpu();
      sleep_disable();
      LED_GRUEN_ON;
    }
    rxIsReady=false;
    evaluate();
    _delay_ms(5);


    cmultiRec.comStateMachine();
    cmultiRec.doJob();

  }
#endif // RECEIVER_BUILT

}

void evaluate(void)
{
char target[3],source[3],text[25];
char func,adr,job;
uint8_t l;

  l = strlen(rxMessage);
  cmulti.sendInfo(rxMessage,"BR");
  /*
  if( l>=10 ) // 15
  {
    strncpy(target,&(rxMessage.c_str()[1]),2);
    target[2] = '\0';
    strncpy(source,&(rxMessage.c_str()[3]),2);
    source[2] = '\0';
    func = rxMessage.c_str()[6];
    adr = rxMessage.c_str()[7];
    job = rxMessage.c_str()[8];
    l-=15;
    text[0] = 0;
    if(l>0)
    {
      strncpy(text,&(rxMessage.c_str()[10]),l);
      text[l] = 0;
    }
    //cmulti.setAlternativeNode(source);
    cmulti.sendStandard(text,target,func,adr,job,'T');
    cmulti.sendStandardInt(target,'R','S','0',(int32_t) rxRssi);
    //cmulti.resetNode();
  }
  */

}

void LoRa_rxMode(){
  //LoRa.enableInvertIQ();                // active invert I and Q signals
  LoRa.receive();                       // set receive mode
}

void LoRa_txMode(){
  LoRa.idle();                          // set standby mode
  LoRa.disableInvertIQ();               // normal mode
}

void LoRa_sendMessage(char *message)
{
  txIsReady = false;
  //LoRa_txMode();                        // set tx mode
  LoRa.beginPacket();                   // start packet
  LoRa.write((uint8_t*)message,strlen(message));                  // add payload
  LoRa.endPacket(false);                 // finish packet and send it !!!!!!!!!!!!! war true
}

void onReceive(int packetSize)
{
  uint8_t cointer=0;
  while (LoRa.available()) {
    rxMessage[cointer] = (char)LoRa.read();
    cointer++;
    if (cointer>=LORAMESSAGELENGTH-1)
      cointer--;
  }
  rxMessage[cointer]=0;
  rxRssi = LoRa.packetRssi();
  rxIsReady=true;
}

void onTxDone() {
  LED_BLAU_OFF;
  txIsReady = true;
}


ISR ( RTC_OVF_vect )
{

}

