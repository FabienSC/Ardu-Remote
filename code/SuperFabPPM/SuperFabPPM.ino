//this programm will put out a PPM signal

//////////////////////CONFIGURATION///////////////////////////////
#define chanel_number 8  //set the number of chanels
#define default_servo_value 1111  //set the default servo value
#define PPM_Pause 10000  //set the PPM frame length in microseconds (1ms = 1000µs)
#define PPM_PulseLen 6//set the pulse length
#define SIG_PIN 9  //set PPM signal output pin on the arduino
#define LED_PIN 13  //set PPM signal output pin on the arduino
//////////////////////////////////////////////////////////////////

/*this array holds the servo values for the ppm signal
 change theese values in your code (usually servo values move between 1000 and 2000)*/
int ppm[chanel_number], ppmX[(chanel_number+1)*2];
unsigned long lastPing = 0, lastPPM = 0;
unsigned char settings = 0;
unsigned char pulseCounter = 17;


void setup(){
  Serial.begin(19200);
  //initiallize default ppm values
  for(int i=0; i<chanel_number; i++){
    ppm[i]= default_servo_value;
  }
  updatePPM();

  pinMode(SIG_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);//turn LEDs off

  timerSetup();
}

void loop(){
  //put main code here
  if(Serial.available() > 5)
  {
    if(Serial.read() == 'F')
    {
      Serial.write('F');
      ppm[2] = map(Serial.read(),30,150,1000,1500);//T
      ppm[3] = map(Serial.read(),30,150,1000,2000);//Y
      ppm[1] = map(Serial.read(),30,150,2000,1000);//P
      ppm[0] = map(Serial.read(),30,150,1000,2000);//R
      
      settings = Serial.read();
      if(bitRead(settings, 0)) //a1 - arming
        ppm[4] = 1750; //arm
      else
        ppm[4] = 1250; //disarm
        
      if(bitRead(settings, 1)) //a2 - flight mode
        ppm[5] = 1750; //angle
      else
        ppm[5] = 1250; //headfree
        
      if(bitRead(settings, 2)) //LED control
        digitalWrite(LED_PIN, HIGH);//turn LEDs on
      else
        digitalWrite(LED_PIN, LOW); //turn LEDs off
      
      ppm[6] = 1500;//A3
      ppm[7] = 1500;//A4
      
      lastPing = millis();
    }
    Serial.flush();
  }
  
  if((millis()-lastPing) > 1050)//if no signal for 1.05 secs
  {
      ppm[2] = 1000;//T
      ppm[3] = 1500;//Y
      ppm[1] = 1500;//P
      ppm[0] = 1500;//R
      ppm[4] = 1250;//A1 - disarm
      ppm[5] = 1750;//A2 - angle
      ppm[6] = 1500;//A3
      ppm[7] = 1500;//A4
//      ppm[0] = 1000;//R
//      ppm[1] = 1100;//P
//      ppm[2] = 1200;//T
//      ppm[3] = 1300;//Y
//      ppm[4] = 1400;//A1 - disarm
//      ppm[5] = 1500;//A2 - angle
//      ppm[6] = 1600;//A3
//      ppm[7] = 1700;//A4
      digitalWrite(LED_PIN, LOW);//turn LEDs off
  }
  
  
//  delay(5);
}

ISR(TIMER1_COMPA_vect){//timer1 interrupt 1Hz toggles pin 13 (LED)
//generates pulse wave of frequency 1Hz/2 = 0.5kHz (takes two cycles for full wave- toggle high then toggle low)
 
  OCR1A = ppmX[pulseCounter++];
  
  if(pulseCounter >= 18)
  {
    pulseCounter = 0;
    updatePPM();
  }
}

void updatePPM()
{
  ppmX[0]  = 2 * PPM_PulseLen;
  ppmX[1]  = 2 * ppm[0] - PPM_PulseLen;
  ppmX[2]  = 2 * PPM_PulseLen;
  ppmX[3]  = 2 * ppm[1] - PPM_PulseLen;
  ppmX[4]  = 2 * PPM_PulseLen;
  ppmX[5]  = 2 * ppm[2] - PPM_PulseLen;
  ppmX[6]  = 2 * PPM_PulseLen;
  ppmX[7]  = 2 * ppm[3] - PPM_PulseLen;
  ppmX[8]  = 2 * PPM_PulseLen;
  ppmX[9]  = 2 * ppm[4] - PPM_PulseLen;
  ppmX[10] = 2 * PPM_PulseLen;
  ppmX[11] = 2 * ppm[5] - PPM_PulseLen;
  ppmX[12] = 2 * PPM_PulseLen;
  ppmX[13] = 2 * ppm[6] - PPM_PulseLen;
  ppmX[14] = 2 * PPM_PulseLen;
  ppmX[15] = 2 * ppm[7] - PPM_PulseLen;
  ppmX[16] = 2 * PPM_PulseLen;
  ppmX[17] = 2 * PPM_Pause;
}


void timerSetup()
{
  cli();//stop interrupts

  //set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1A |= _BV(COM1A0);//toggle pin 9 on compare
  TIFR1 = (1<<OCF1A);//clear its interrupt flag
  
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 50000;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS11)|(1 << CS11);
  // enable timer compare interrupt
  TIMSK1 |= _BV(OCIE1A);

  sei();//allow interrupts
}

