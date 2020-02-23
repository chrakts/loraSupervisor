#include "loraSupervisor.h"
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

  initReadMonitor();
  initBusyCounter();


	PMIC_CTRL = PMIC_LOLVLEX_bm | PMIC_HILVLEN_bm | PMIC_MEDLVLEN_bm;
	sei();
	cmulti.open(Serial::BAUD_57600,F_CPU);

  cmulti.sendInfo("Hello from LORA-Board","BR");

  if (!LoRa.begin(868E6))
  {
    //Serial.println("Starting LoRa failed!");
    LED_ROT_ON;
    while (1);
  }

  RTC.PERL = 3;
  RTC.PERH = 0;
  CLK.RTCCTRL = CLK_RTCSRC_ULP_gc | CLK_RTCEN_bm; // 1kHz ULP
  RTC.CTRL = RTC_PRESCALER_DIV1024_gc;
  RTC.INTCTRL = RTC_OVFINTLVL_LO_gc;


#ifdef SENDER_BUILT
  LoRa.onTxDone(onTxDone);
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
    PORTC.OUTCLR = PIN0_bm;
    PORTC.OUTSET = PIN1_bm;
    loraCmulti.sendStandardInt("BR",'S','0','C',counter);
    LoRa_sendMessage(loraCmulti.get());
    /*cmulti.sendStandardInt("BR",'S','0','C',counter);
    _delay_ms(5);*/
    while(txIsReady==false)
    {}
    counter++;
    deInitReadMonitor();
    LoRa.sleep();
    set_sleep_mode(SLEEP_MODE_PWR_SAVE);
    sleep_enable();
    sleep_cpu();
    sleep_disable();
    initReadMonitor();
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

  }
#endif // RECEIVER_BUILT

}

void evaluate(void)
{
char target[3],source[3],text[25];
char func,adr,job;
uint8_t l;

  strncpy(target,&(rxMessage.c_str()[1]),2);
  target[2] = '\0';
  strncpy(source,&(rxMessage.c_str()[3]),2);
  source[2] = '\0';
  func = rxMessage.c_str()[6];
  adr = rxMessage.c_str()[7];
  job = rxMessage.c_str()[8];
  l = strlen(rxMessage.c_str())-15;
  strncpy(text,&(rxMessage.c_str()[10]),l);
  text[l] = 0;
  cmulti.setAlternativeNode(source);
  cmulti.sendStandard(text,target,func,adr,job,'T');
  cmulti.sendStandardInt(target,'R','S','0',(int32_t) rxRssi);
  cmulti.resetNode();
}

void LoRa_rxMode(){
  //LoRa.enableInvertIQ();                // active invert I and Q signals
  LoRa.receive();                       // set receive mode
}

void LoRa_txMode(){
  LoRa.idle();                          // set standby mode
  LoRa.disableInvertIQ();               // normal mode
}

void LoRa_sendMessage(String message) {
  txIsReady = false;
  LoRa_txMode();                        // set tx mode
  LoRa.beginPacket();                   // start packet
  LoRa.print(message);                  // add payload
  LoRa.endPacket(true);                 // finish packet and send it
}

void onReceive(int packetSize) {
  LED_GELB_ON;
  rxMessage="";
  while (LoRa.available()) {
    rxMessage += (char)LoRa.read();
  }
  rxRssi = LoRa.packetRssi();
  rxIsReady=true;
  LED_GELB_OFF;
}

void onTxDone() {
  LED_BLAU_OFF;
  txIsReady = true;
}


ISR ( RTC_OVF_vect )
{

}

