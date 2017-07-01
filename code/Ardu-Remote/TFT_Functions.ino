#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library

// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

int prevThrottle = 0, prevPitch = 0, prevRoll = 0, prevHeading = 0, prevAltitude = 0;
int pitchChange = 0, rollChange = 0;//helps clear the top/bottom pitch bar when necessary

Adafruit_TFTLCD tft;//Display is 240x320
String tftHeading[8] = {
  "W NW N NE E",//### shift right by a few pixels
  "SW W NW N NE",
  "S SW W NW N",//    shift right by a few pixels
  "SE S SW W NW",
  "E SE S SW W",//    shift right by a few pixels
  "NE E SE S SW",
  "N NE E SE S",//    shift right by a few pixels
  "NW N NE E SE"
  };

void setupScreen()
{
  
  tft.reset();
  uint16_t identifier = tft.readID();
  tft.begin(identifier);
  
  tft.setRotation(2);   //rotate 180°
  tftClear();
  tftPrepScreen();
}

void tftClear()
{
  tft.fillScreen(BLACK);
}


void tftPrint(int xPos, int yPos, uint16_t color, String text, int textSize)
{
  tft.setCursor(xPos, yPos);
  tft.setTextColor(color);
  tft.setTextSize(textSize);
  tft.println(text);
}


void tftPrepScreen()
{
  tft.drawFastHLine(0, 15, 240, GREEN);//Orientation
  tft.drawFastHLine(0, 25, 240, GREEN);//Orientation
  
  tft.fillTriangle(
    120, 26, //point
    112, 33, //point
    128, 33, //point
    RED);
  
  tftPrint(0,29,RED,"Thr", 2);
  tft.drawRect(-1, 45, 30, 232, RED);//Throttle bar
  
  tftPrint(204, 29,RED,"Alt",2);
  tftPrint(195, 47,RED,"76", 1);
  tftPrint(195,103,RED,"57", 1);
  tftPrint(195,159,RED,"38", 1);
  tftPrint(195,215,RED,"19", 1);
  tftPrint(200,271,RED,"0",  1);
  tft.drawRect(210, 45, 240, 232, RED);//Altitude bar

  //Pitch/Roll cross
  tft.drawRect(117, 54,  7, 97, RED);//Pitch
  tft.drawRect( 72, 99, 97,  7, RED);//Roll
  tft.fillRect(118, 100, 5,  5, BLACK);//Center Square
  
}


void tftUpdate()
{
  int diff;//for all bars

  tft.fillRect(25, 0, 190,  14, BLACK);
  int val = heading;
  if(val > 239)
    val -= 256;
  if((heading < 16) ||(heading > 239))
    tftPrint(56 + val,0,YELLOW,tftHeading[0], 2);
  else if(heading < 48)
    tftPrint(50 + (val - 32),0,YELLOW,tftHeading[1], 2);
  else if(heading < 80)
    tftPrint(56 + (val - 64),0,YELLOW,tftHeading[2], 2);
  else if(heading < 112)
    tftPrint(50 + (val - 96),0,YELLOW,tftHeading[3], 2);
  else if(heading < 144)
    tftPrint(56 + (val - 128),0,YELLOW,tftHeading[4], 2);
  else if(heading < 176)
    tftPrint(50 + (val - 160),0,YELLOW,tftHeading[5], 2);
  else if(heading < 208)
    tftPrint(56 + (val - 192),0,YELLOW,tftHeading[6], 2);
  else
    tftPrint(50 + (val - 224),0,YELLOW,tftHeading[7], 2);

  
  diff = (230/((float)(PWM_OUT_MAX - PWM_OUT_MIN)))*(remoteThrottle - PWM_OUT_MIN) - prevThrottle;
  if(diff == 0)
    ;//do nothing
  else if(diff > 0)
  {
    tft.fillRect(0, 276 - (prevThrottle + diff), 28, diff, GREEN);
    prevThrottle += diff;
  }
  else
  {
    tft.fillRect(0, 276 - prevThrottle, 28, -diff, BLACK);
    prevThrottle += diff;
  }
  
  diff = (230/180.0)*(altitude) - prevAltitude;
  if(diff == 0)
    ;//do nothing
  else if(diff > 0)
  {
    tft.fillRect(211, 276 - (prevAltitude + diff), 28, diff, GREEN);
    prevAltitude += diff;
  }
  else
  {
    tft.fillRect(211, 276 - prevAltitude, 28, -diff, BLACK);
    prevAltitude += diff;
  }


  // Pitch/Roll cross
  if(remotePitch < 90)
  {
    if(pitchChange != 1)
    {
      tft.fillRect(118, 105, 5, 45, BLACK);
      prevPitch = 0;
    }
    pitchChange = 1;
    diff = (0.5)*(90 - remotePitch) - prevPitch;
    if(diff > 0)
    {
      tft.fillRect(118, 100 - (prevPitch + diff), 5, diff, GREEN);
      prevPitch += diff;
    }
    else if(diff < 0)
    {
      tft.fillRect(118, 100 - prevPitch, 5, -diff, BLACK);
      prevPitch += diff;
    }
  }
  else if(remotePitch > 90)
  {
    if(pitchChange != 2)
    {
      tft.fillRect(118, 55, 5, 45, BLACK);
      prevPitch = 0;
    }
    pitchChange = 2;
    diff = (0.5)*(remotePitch - 90) - prevPitch;
    if(diff > 0)
    {
      tft.fillRect(118, 105 + prevPitch, 5, diff, GREEN);
      prevPitch += diff;
    }
    else if(diff < 0)
    {
      tft.fillRect(118, 105 + (prevPitch + diff), 5, -diff, BLACK);
      prevPitch += diff;
    }
  }
  else// = 90
  {
    pitchChange = 0;
    tft.fillRect(118, 105, 5, 45, BLACK);
    tft.fillRect(118, 55, 5, 45, BLACK);
  }
  
  if(remoteRoll > 90)
  {
    if(rollChange != 1)
    {
      tft.fillRect(73, 100, 45, 5, BLACK);
      prevRoll = 0;
    }
    rollChange = 1;
    diff = (0.5)*(remoteRoll - 90) - prevRoll;
    if(diff > 0)
    {
      tft.fillRect(123 + prevRoll, 100, diff, 5, GREEN);
      prevRoll += diff;
    }
    else if(diff < 0)
    {
      tft.fillRect(123 + prevRoll + diff, 100, -diff, 5, BLACK);
      prevRoll += diff;
    }
  }
  else if(remoteRoll < 90)
  {
    if(rollChange != 2)
    {
      tft.fillRect(123, 100, 45, 5, BLACK);
      prevRoll = 0;
    }
    rollChange = 2;
    diff = (0.5)*(90 - remoteRoll) - prevRoll;
    if(diff > 0)
    {
      tft.fillRect(73 + 45 - prevRoll - diff, 100, diff, 5, GREEN);
      prevRoll += diff;
    }
    else if(diff < 0)
    {
      tft.fillRect(73 + 45 - prevRoll, 100, -diff, 5, BLACK);
      prevRoll += diff;
    }
  }
  else// = 90
  {
    rollChange = 0;
    tft.fillRect(73, 100, 45, 5, BLACK);
    tft.fillRect(123, 100, 45, 5, BLACK);
  }
}


void printTxt(int i, int a, int b)
{
  tft.fillRect(a, b, 80, 16, BLACK);
  tftPrint(a,b,BLUE,String(i), 2);
}

