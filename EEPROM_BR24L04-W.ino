#include <Wire.h>

//there is eeprom BR24L04 4Kbit 512x8
//A01 and A02 are LOW, A0 is not connected
//eeprom addresses 0x50 and 0x51, each 256 Bytes

#define EEPROM_ADDRESS 0x50

//read data from address 0 - 511
uint8_t readDataByteWholeEEPROM(uint8_t eeprom_address, unsigned int byte_address, uint8_t &data_byte)
{
  uint8_t result;
  if (byte_address > 255)
  {
    eeprom_address = eeprom_address | 0x1;  //set high address area
    byte_address = byte_address - 256;      //convert address from 16Bit to 8Bit
  } 

  Wire.beginTransmission(eeprom_address);
  uint8_t size = Wire.write(byte_address);

  if (size == 0)
    result = 10;  //Failed to set address
  else
  {
    result = Wire.endTransmission(false);
    if (result == 0)
    {
      size = Wire.requestFrom(eeprom_address, 1, true);//address, bytes_to_read, stop_after_request
      if (size == 0)
        result = 12;  //no bytes returned from the slave device
      else
      { 
        if (Wire.available() > 0)  //number of bytes available to read
        {
          data_byte = Wire.read();
          result =  0;  //success  
        }
        else
          result =  11;  //no bytes to read
      }
    }
  }
  return result;

 /* 
  *  return values
  *  0 = success
  *  1 = data too long to fit in buffer
  *  2 = receive NAK when transmiting address
  *  3 = receive NAK when transmiting data
  *  4 = other error
  *  10 = failed to set address
  *  11 = no bytes to read
  *  12 = no bytes returned from the slave device
  */
}

//write data to address 0 - 511
uint8_t writeDataByteWholeEEPROM(uint8_t eeprom_address, unsigned int byte_address, uint8_t data_byte)
{
  uint8_t result;
  if (byte_address > 255)
  {
    eeprom_address=eeprom_address | 0x1; //set high address area
    byte_address = byte_address - 256;//convert address from 16Bit to 8Bit
  } 
  Wire.beginTransmission(eeprom_address);
  uint8_t size = Wire.write(byte_address);
  if (size == 0) 
    result = 10;   //Failed to set address
  else
  {
    size = Wire.write(data_byte);
    if (size == 0) 
      result = 11;     //Failed to write data
    else
      result = Wire.endTransmission(true);
  }
  return result;
  
 /* 
  *  return values
  *  0 = success
  *  1 = data too long to fit in buffer
  *  2 = receive NAK when transmiting address
  *  3 = receive NAK when transmiting data
  *  4 = other error
  *  10 = failed to set address
  *  11 = failed to write data
  */
}

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  unsigned int byte_address=512;
  uint8_t data_byte_w = 5;
  uint8_t data_byte_r;
  uint8_t result;
  result = readDataByteWholeEEPROM(EEPROM_ADDRESS,byte_address, data_byte_r);
  if(result == 0)
  {
    Serial.print("Read old data: ");
    Serial.println(data_byte_r);
  }
  else
  {
    Serial.print("Error: ");
    switch(result)
    {
      case 1: Serial.println("data too long to fit in buffer"); break;
      case 2: Serial.println("receive NAK when transmiting address"); break;
      case 3: Serial.println("receive NAK when transmiting data"); break;
      case 4: Serial.println("other error"); break;
      case 10: Serial.println("failed to set address"); break;
      case 11: Serial.println("no bytes to read"); break;
      case 12: Serial.println("no bytes returned from the slave device"); break;
      default: Serial.println("unknown error"); break;  
    }
  }
  
  writeDataByteWholeEEPROM(EEPROM_ADDRESS, byte_address, data_byte_w);
  delay(5);   //wait until write is completed
  
  result = readDataByteWholeEEPROM(EEPROM_ADDRESS,byte_address, data_byte_r);
  if(result == 0)
  {
    Serial.print("Read new data: ");
    Serial.println(data_byte_r);
  }
  else
  {
    Serial.print("Error: ");
    switch(result)
    {
      case 1: Serial.println("data too long to fit in buffer"); break;
      case 2: Serial.println("receive NAK when transmiting address"); break;
      case 3: Serial.println("receive NAK when transmiting data"); break;
      case 4: Serial.println("other error"); break;
      case 10: Serial.println("failed to set address"); break;
      case 11: Serial.println("no bytes to read"); break;
      case 12: Serial.println("no bytes returned from the slave device"); break;
      default: Serial.println("unknown error"); break;  
    }
  }  
}

void loop()
{

}
