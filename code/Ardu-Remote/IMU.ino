//IMU stuff
#include <Wire.h>             //SDA SCL for MPU
#include <HMC5883L.h>         //compass
#include <MPU6050.h>          //accelerometer and gyro


// Accel + gyro //
MPU6050 mpu;
int gyroX, gyroY, gyroZ;          //current gyroscope inputs roll, pitch and yaw speeds
int accelX, accelY, accelZ;       //current Accelerometer inputs
int accelXCal = 0, accelYCal = 0, accelZCal = 0;//calibration for accelerometer

// compass //
HMC5883L compass;

void setupIMU()
{
  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
    delay(50);
  
  // Enable bypass mode
  mpu.setI2CMasterModeEnabled(false);
  mpu.setI2CBypassEnabled(true) ;
  mpu.setSleepEnabled(false);
  
  while (!compass.begin())// Initialize Initialize HMC5883L
    delay(50);
  
  compass.setRange(HMC5883L_RANGE_1_3GA);// Set measurement range
  compass.setMeasurementMode(HMC5883L_CONTINOUS);// Set measurement mode
  compass.setDataRate(HMC5883L_DATARATE_30HZ);// Set data rate
  compass.setSamples(HMC5883L_SAMPLES_8);// Set number of samples averaged
}

void updateMPU()//re-do equations
{
 /*  OLD CODE
  *   
  *   //Read vectors
  Vector mag = compass.readNormalize();
  Vector normAccel = mpu.readScaledAccel();  

  mpuRoll = asin(normAccel.YAxis);
  mpuPitch = asin(-normAccel.XAxis);

  if(mpuRoll > 1.5)
    mpuRoll = 1.5;
  else if (mpuRoll < -1.5)
    mpuRoll = -1.5;

  if(mpuPitch > 1.5)
    mpuPitch = 1.5;
  else if (mpuPitch < -1.5)
    mpuPitch = -1.5;

  if(isnan(mpuRoll))
    mpuRoll = oldMpuRoll;
  else
    oldMpuRoll = mpuRoll;

  if(isnan(mpuPitch))
    mpuPitch = oldMpuPitch;
  else
    oldMpuPitch = mpuPitch;

  mpuRollSmooth  += alphaR * (mpuRoll  - mpuRollSmooth);
  mpuPitchSmooth += alphaP * (mpuPitch - mpuPitchSmooth);
  
  roll  = (mpuRollSmooth  + 1.5) * 200/3;
  pitch = (mpuPitchSmooth + 1.5) * 200/3;

  if (mpuRoll > 0.78 || mpuRoll < -0.78 || mpuPitch > 0.78 || mpuPitch < -0.78)//info isn't reliable anymore
    compassEnable = 0;
  else
   compassEnable = 1;
  
  // Some of these are used twice, so rather than computing them twice in the algorithem we precompute them before hand.
  float cosRoll = cos(mpuRoll);
  float sinRoll = sin(mpuRoll);  
  float cosPitch = cos(mpuPitch);
  float sinPitch = sin(mpuPitch);
  
  // Tilt compensation
  float Xh = mag.XAxis * cosPitch + mag.ZAxis * sinPitch;
  float Yh = mag.XAxis * sinRoll * sinPitch + mag.YAxis * cosRoll - mag.ZAxis * sinRoll * cosPitch;
 
  compensatedHeading = atan2(Yh, Xh);
  
  if (compensatedHeading < 0)
    compensatedHeading += 2 * PI;
  if (compensatedHeading > 2 * PI)
    compensatedHeading -= 2 * PI;

  if(compassEnable)                  //if data is reliable
    oldHeading = compensatedHeading;//store heading
  else                              //if data is unreliable
    compensatedHeading = oldHeading;//keep previous reliable heading
    
  heading = ((compensatedHeading) * 256) /(2*PI);*/
}


