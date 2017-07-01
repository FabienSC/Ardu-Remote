//battery should range from 4.2V down to ~3.5V
#define BATTERY_MULT 0.7 // ### test value
#define BATTERY_OFFSET 716 // 3.5V
#define BATT A14  //battery pin
#define ALPHA 0.99//filtering parameter

int battLvl = 100;
float batteryLvlfiltered = 50, batteryLvlPrecise = 50;


int checkBatteryLvl()
{
//  Serial.print("analog: ");
//  Serial.println(analogRead(BATT));
  batteryLvlfiltered = ALPHA * batteryLvlPrecise + (1-ALPHA) * (BATTERY_MULT * (analogRead(BATT) - BATTERY_OFFSET));
  return analogRead(BATT);
  
//  Serial.print("filtered: ");
//  Serial.println(batteryLvlfiltered);
  if(batteryLvlfiltered < 0)
    batteryLvlfiltered = 0;
  else if (batteryLvlfiltered > 100)
    batteryLvlfiltered = 100;
    
  //schmitt trigger style algorithm:
  if(((float)battLvl - batteryLvlfiltered) > 0.8)//Assume battery can only discharge
  {
    battLvl = batteryLvlfiltered;
    return battLvl;
  }
  else if(((float)battLvl - batteryLvlPrecise) < -0.8)//battery is charging?
  {
    battLvl = batteryLvlPrecise;
    return battLvl;
  }
  //else
  return -1;//indicate that the battery Lvl hasn't changed
}

