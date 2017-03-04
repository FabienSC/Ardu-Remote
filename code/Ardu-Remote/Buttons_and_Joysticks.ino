//INPUT PINS

#define BTN_A 23
#define BTN_B 25
#define BTN_C 43
#define BTN_D 45
#define BTN_E 47
#define BTN_PITCH_U 51
#define BTN_PITCH_D 49
#define BTN_ROLL_L 46
#define BTN_ROLL_R 50
#define BTN_YAW_L 22 //22 or 24
#define BTN_YAW_R 24 //22 or 24
#define BTN_BTN_JOYSTICK 53

int buttonPins[] = {BTN_A,BTN_B,BTN_C,BTN_D,BTN_E,BTN_PITCH_U,BTN_PITCH_D,BTN_ROLL_L,BTN_ROLL_R,BTN_YAW_L,BTN_YAW_R,BTN_BTN_JOYSTICK};

#define THROTTLE A15
#define PITCH A13
#define ROLL A12
#define PITCHMIDHIGH 545.0
#define PITCHMIDLOW 503.0
//#define PITCHMIDLOW 485.0
#define PITCHMIN 129

//OUTPUT PINS
int outputPins[] = {27,29,31,35,37,39,48};
#define LED_SIG_R 27
#define LED_SIG_G 29
#define LED_SIG_B 31
#define LED_AUX_R 39
#define LED_AUX_G 37
#define LED_AUX_B 35
#define BUZZER 48

unsigned long debounceTimers[] = {0,0,0,0,0,0,0,0,0,0,0,0};
#define DEBOUNCE_TIME 500

#define PWM_IN_MIN 0
#define PWM_IN_MAX 180
#define PWM_OUT_MIN 30
#define PWM_OUT_MAX 150

void setupButtons()
{
  pinMode(52, OUTPUT);   //solves pull-up problem on pin 50
  digitalWrite(52, HIGH);//solves pull-up problem on pin 50
  
  for(int i = 0; i < 12; i++)//set up buttons
    pinMode(buttonPins[i], INPUT_PULLUP);//set as input and pull-up
    
  for(int i = 0; i < 7; i++)//set up outputs
  {
    pinMode(outputPins[i], OUTPUT);//set as output
    digitalWrite(outputPins[i], LOW);//turn off
  }
}

void checkButtons()
{
  unsigned long t = millis();//number of milliseconds since start of program
  for(int i = 0; i < 12; i++)
  {
    if((!digitalRead(buttonPins[i])) && (debounceTimers[i] == 0))//if button is pressed and was previously off
    {
      debounceTimers[i] = t;
    }
    else if(digitalRead(buttonPins[i]) && ((t - debounceTimers[i]) > 20))//button not pressed, avoid bouncing
    {
      debounceTimers[i] = 0;
    }
  }

  if(debounceTimers[0] == t)//BTN_A pressed
  {
//    Serial.println("btn A");
    //do stuff
    if(yawMod < 30)
      yawMod++;
    else
      buzz(200);//error
  }
  if(debounceTimers[1] == t)//BTN_B pressed
  {
//    Serial.println("btn B");
    //do stuff
    if(yawMod > -30)
      yawMod--;
    else
      buzz(200);//error
  }
  if(debounceTimers[2] == t)//BTN_C pressed
  {
//    Serial.println("btn C");
    //do stuff
    remoteSettings ^= 1 << 0;//toggle bit 0
  }
  if(debounceTimers[3] == t)//BTN_D pressed
  {
//    Serial.println("btn D");
    //do stuff
    remoteSettings ^= 1 << 1;//toggle bit 1
  }
  if(debounceTimers[4] == t)//BTN_E pressed
  {
//    Serial.println("btn E");
    //do stuff
    remoteSettings ^= 1 << 2;//toggle bit 2
  }
  if(debounceTimers[5] == t)//BTN_PITCH_U pressed
  {
//    Serial.println("btn PU");
    //do stuff
    if(pitchMod < 30)
      pitchMod++;
    else
      buzz(200);//error
  }
  if(debounceTimers[6] == t)//BTN_PITCH_D pressed
  {
//    Serial.println("btn PD");
    //do stuff
    if(pitchMod > -30)
      pitchMod--;
    else
      buzz(200);//error
  }
  if(debounceTimers[7] == t)//BTN_ROLL_L pressed
  {
//    Serial.println("btn RL");
    //do stuff
    if(rollMod > -30)
      rollMod--;
    else
      buzz(200);//error
  }
  if(debounceTimers[8] == t)//BTN_ROLL_R pressed
  {
//    Serial.println("btn RR");
    //do stuff
    if(rollMod < 30)
      rollMod++;
    else
      buzz(200);//error
  }
  
  if(!digitalRead(BTN_YAW_L) && digitalRead(BTN_YAW_R))//  if(debounceTimers[9] == t)//BTN_YAW_L pressed
  {
//    Serial.println("btn YL");
    //do stuff
    remoteYaw = 90 - 10 + yawMod;
  }
  else if(digitalRead(BTN_YAW_L) && !digitalRead(BTN_YAW_R))//  if(debounceTimers[10] == t)//BTN_YAW_R pressed
  {
//    Serial.println("btn YR");
    //do stuff
    remoteYaw = 90 + 10 + yawMod;
  }
  else
    remoteYaw = 90 + yawMod;
  
  if(debounceTimers[11] == t)//BTN_BTN_JOYSTICK pressed
  {
//    Serial.println("btn joy");
    //do stuff
  }
  
}

void updateAnalogControls()
{
  remoteThrottle = (PWM_IN_MAX/1023.0) * (1023 - analogRead(THROTTLE));//map to 0~180
  
  remotePitch    = analogRead(PITCH);
  if(remotePitch > 542)
    remotePitch = 90 + (remotePitch - PITCHMIDHIGH) * (90/(1023 - PITCHMIDHIGH));
  else if(remotePitch < 504)
    remotePitch = (remotePitch - PITCHMIN) * (90/(PITCHMIDLOW - PITCHMIN));
  else
    remotePitch = 90;
  remotePitch = 180 - remotePitch;//reverse
  
  remoteRoll     = 1 + (PWM_IN_MAX/1023.0) * (analogRead(ROLL));

  remotePitch    = map(remotePitch,    PWM_IN_MIN, PWM_IN_MAX, PWM_OUT_MIN, PWM_OUT_MAX);//soften controls
  remoteRoll     = map(remoteRoll,     PWM_IN_MIN, PWM_IN_MAX, PWM_OUT_MIN, PWM_OUT_MAX);//soften controls

  remoteThrottle = map(remoteThrottle, PWM_IN_MIN, PWM_IN_MAX, PWM_OUT_MIN, PWM_OUT_MAX);//soften controls

  remotePitch += pitchMod;
  remoteRoll += rollMod;
  
  //limit checks
  if(remoteThrottle > PWM_OUT_MAX)
    remoteThrottle = PWM_OUT_MAX;
  else if(remoteThrottle < PWM_OUT_MIN)
    remoteThrottle = PWM_OUT_MIN;
    
  if(remotePitch > PWM_OUT_MAX)
    remotePitch = PWM_OUT_MAX;
  else if(remotePitch < PWM_OUT_MIN)
    remotePitch = PWM_OUT_MIN;
    
  if(remoteRoll > PWM_OUT_MAX)
    remoteRoll = PWM_OUT_MAX;
  else if(remoteRoll < PWM_OUT_MIN)
    remoteRoll = PWM_OUT_MIN;
}

