//Mr.Fab Ardu-Remote V2.1
//created by Fabien SANTOS-CESSAC
//Latest Update: 2017-01-05
//Version 2.1


#include <math.h> //for PID stuff and compass tilt compensation
#include "MegaJoy.h"

int altitude = 0, heading = 0;

int remoteThrottle = 30, remotePitch = 90, remoteRoll = 90, remoteSettings = 0, remoteYaw = 90;
int pitchMod = 0, rollMod = 0, yawMod = 0;
int cameraAngle = 90;
int mpuHeading = 0, mpuPitch = 0, mpuRoll = 0;//IMU

int periphPitch = 90, periphRoll = 90, periphHeading = 0;

char settings = 0; //8 booleans in the form of a char

bool boop = 0;
//int battDisplay = 50;

////////////////////////////////////////////////////////////
void setup() //***
{
//  Serial.begin(19200);  //debugging, can't use with UnoJoy
  
  Serial1.begin(19200); //Xbee, 19200 for acceptable transmission and longer range
//  Serial2.begin(19200); //Periph
  
//  setupIMU();
  setupButtons();
  setupScreen();
  
//  melody();//Startup melody


 // setupMegaJoy();
  
  while(analogRead(A15) < 1023)//require throttle to be down before proceeding
  {
    buzz(200);//Make a tone
    sigLED(1,0,0); //RED
    auxLED(1,0,0); //RED
    delay(200);
    sigLED(0,1,0); //GREEN
    auxLED(0,1,0); //GREEN
    delay(200);
    sigLED(0,0,1); //BLUE
    auxLED(0,0,1); //BLUE
    delay(200);
  }
  
  sigLED(0,0,0);
  auxLED(0,0,0);
  
  buzzOff();//lol
}


void loop() //***
{
  checkButtons(); //check what buttons have been pressed
  updateAnalogControls();
  checkSignals();
  sendData();     //transmit data to robot
  
  printTxt(remoteThrottle, 50, 220);
  printTxt((remotePitch - 30)*(1023/120.0),50,240);
  printTxt(remoteRoll,     50, 260);
  printTxt(remoteYaw,      50, 280);
  printTxt(remoteSettings, 50, 300);

  int battDisplay = checkBatteryLvl();
  if(battDisplay != -1)
    printTxt(battDisplay, 120, 300);
  
  tftUpdate();//refresh screen
  
  if(Serial1.available() > 2)
    receiveData();
  
//  if(Serial2.available() > 4)
//    receivePeripheralData();

//  UnoJoy:
  //megaJoyControllerData_t controllerData = getControllerData();
  //setControllerData(controllerData);
  
  delay(49);
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

void checkSignals()
{
  if(robotConnected())//if no robot signal
  {
    if(boop == 0)
      sigLED(0,1,0); //GREEN
    boop = 1;
  }
  else //strong signal
  {//emergency stop?
    if(boop == 1)
    {  
      sigLED(1,0,0); //RED
      buzz(1000);
    }
    boop = 0;
  }
  
//  if(periphConnected())//if no periph signal
//  {
//    auxLED(0,1,0); //GREEN
//  }
//  else //strong signal
//  {
//    auxLED(0,0,0); //OFF
//  }
}

megaJoyControllerData_t getControllerData()
{
  megaJoyControllerData_t controllerData = getBlankDataForMegaController();
  controllerData.buttonArray[0] = remoteSettings;
  
  controllerData.analogAxisArray[2] = (remoteThrottle - 30)*(1023/120.0);
  controllerData.analogAxisArray[3] = (remotePitch    - 30)*(1023/120.0);
  controllerData.analogAxisArray[4] = (remoteRoll     - 30)*(1023/120.0);
  controllerData.analogAxisArray[5] = (remoteYaw      - 30)*(1023/120.0);
  
  return controllerData;
}

