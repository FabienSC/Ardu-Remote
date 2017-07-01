
#define TIMEOUT_ROBOT  300  //in milliseconds
#define TIMEOUT_PERIPH 200  //

unsigned long robotSignal = 0; //When the last signal from the robot was received
unsigned long periphSignal = 0;//When the last periph signal was received


void receiveData()//***
{
  char c = Serial1.read();
  
  if(c == 'F')
    {
      robotSignal = millis();
      
      unsigned char charH = Serial1.read();
      unsigned char charL = Serial1.read();
      altitude = readInt(charH, charL);
    }
  Serial1.flush();
  
}


void sendData()//***
{
  Serial1.write('F');//send sync Char 'F' to mark start of sequence
  Serial1.write(remoteThrottle);//send Throttle
  Serial1.write((remoteYaw + yawMod));//send Yaw
  Serial1.write((remotePitch + pitchMod));//send Pitch
  Serial1.write((remoteRoll + rollMod));//send Roll
  Serial1.write(remoteSettings);//send Settings
  
//  if(periphConnected)
//    Serial1.write(map(periphPitch,0,200,180,50));//fpv goggles control
//  else
//    Serial1.write(cameraAngle);//knob control?
}


void receivePeripheralData()//***
{
  char c = Serial2.read();
  if(c == 'F')// = 70
  {
    periphSignal  = millis();
    periphPitch   = Serial2.read();//pitch
    periphRoll    = Serial2.read();//roll
    periphHeading = Serial2.read();//heading
    periphSignal = millis();
  }
  
  Serial2.flush();
}


bool periphConnected()//returns true if a signal was recieved within the past timeout duration
{
  return ((millis() - periphSignal) < TIMEOUT_PERIPH);
}

bool robotConnected()//returns true if a signal was recieved within the past timeout duration
{
  printTxt((millis() - robotSignal), 140, 280);
  return ((millis() - robotSignal) < TIMEOUT_ROBOT);
}


int readInt(unsigned char charH, unsigned char charL)//converts 2 chars into 1 int
{
  return ((charH << 8) | charL);//reconstruct int
}


