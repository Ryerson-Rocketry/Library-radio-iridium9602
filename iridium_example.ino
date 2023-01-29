#include <IridiumSBD.h> // Click here to get the library: http://librarymanager/All#IridiumSBDI2C
#include <time.h>
/*
 * Time
 *
 * This sketch demonstrates how to retrieve the Iridium system time
 * from the modem using the getSystemTime method. This uses
 * the Iridium command AT-MSSTM to acquire the time.  The method will
 * fail if the Iridium network has not yet been acquired.
 *
 * Assumptions
 *
 * The sketch assumes an Arduino Mega or other Arduino-like device with
 * multiple HardwareSerial ports. It assumes the satellite modem is
 * connected to Serial1. Change this as needed. SoftwareSerial on an Uno
 * works fine as well.
 */

#define IridiumSerial Serial
#define DIAGNOSTICS true // Change this to see diagnostics

// Declare the IridiumSBD object
IridiumSBD modem(IridiumSerial);

void setup()
{
  int err;
  int signalQuality = -1;

  // Start the console serial port
  Serial.begin(115200);
  while (!Serial);

  // Start the serial port connected to the satellite modem
  IridiumSerial.begin(19200);

  // If we're powering the device by USB, tell the library to
  // relax timing constraints waiting for the supercap to recharge.
  modem.setPowerProfile(IridiumSBD::USB_POWER_PROFILE);

  // Begin satellite modem operation
  Serial.println(F("Starting modem..."));
  
  //signal quality check 
  err = modem.getSignalQuality(signalQuality);
  if (err != ISBD_SUCCESS)
  {
    Serial.print("SignalQuality failed: error ");
    Serial.println(err);
  }

  Serial.print("On a scale of 0 to 5, signal quality is currently ");
  Serial.print(signalQuality);
  Serial.println(".");

  /*err = modem.begin();
  if (err != ISBD_SUCCESS)
  {
    Serial.print(F("Begin failed: error "));
    Serial.println(err);
    if (err == ISBD_NO_MODEM_DETECTED )
      Serial.println(F("No modem detected: check wiring."));
    return;
  }*/
}

void loop()
{
   Serial.println("starts");
   struct tm t;
   int err = modem.getSystemTime(t);
   /*if (err == ISBD_SUCCESS)*/
   {
      char buf[32];
      sprintf(buf, "%d-%02d-%02d %02d:%02d:%02d",
         t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
      Serial.print(F("Iridium time/date is "));
      Serial.println(buf);
   }

   if (err == ISBD_NO_NETWORK) // Did it fail because the transceiver has not yet seen the network?
   {
      Serial.println(F("No network detected.  Waiting 10 seconds."));
   }

   else
   {
      Serial.print(F("Unexpected error "));
      Serial.println(err);
      return;
   }
   delay(10 * 1000UL);
   
}

#if DIAGNOSTICS
void ISBDConsoleCallback(IridiumSBD *device, char c)
{
  Serial.write(c);
}

void ISBDDiagsCallback(IridiumSBD *device, char c)
{
  Serial.write(c);
}
#endif
