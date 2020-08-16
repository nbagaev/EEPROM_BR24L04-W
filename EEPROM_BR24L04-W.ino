//Arduino test sketch for ROHM EEPROM BR24L04-W and Atmel EEPROM AT24C04 4K
#include <Wire.h>

//there is eeprom BR24L04 4Kbit 512x8
//A01 and A02 are LOW, A0 is not connected
//eeprom addresses 0x50 and 0x51, each 256 Bytes

#define EEPROM_ADDRESS 0x50

//read 1 byte from address 0 - 511
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
          result = 0;  //success  
        }
        else
          result = 11;  //no bytes to read
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

//sequential read of up to 32 bytes
//it is possible to read the entire memory, but the wire library only has a 32-byte buffer
uint8_t readDataBytesSequential(uint8_t eeprom_address, unsigned int byte_address, int number, uint8_t *data_bytes)
{
  uint8_t result;
  Wire.beginTransmission(eeprom_address);
  uint8_t size = Wire.write(byte_address);
  if (size == 0)
    result = 10;  //Failed to set address
  else
  {
    result = Wire.endTransmission(false);
    if (result == 0)
    {
      size = Wire.requestFrom(eeprom_address, number, true);  //address, bytes_to_read, stop_after_request
      if (size == 0)
        result = 12;  //no bytes returned from the slave device
      else
      { 
        if (Wire.available() == number)  //number of bytes available to read
        {
          for (int byte_count = 0; byte_count < number; byte_count++)
            data_bytes[byte_count] = Wire.read();
          result = 0;  //success  
        }
        else
          result = 11;  //no bytes to read
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

//page write up to 16 bytes data to eeprom address 0x50/0x51 and byte address 0 - 255 
uint8_t pageWriteDataBytes1(uint8_t eeprom_address, unsigned int byte_address, uint8_t number, uint8_t *data_bytes)
{
  uint8_t result;
  Wire.beginTransmission(eeprom_address);
  uint8_t size = Wire.write(byte_address);
  if (size == 0) 
    result = 10;   //Failed to set address
  else
  {
    size = Wire.write(data_bytes, number);
    if (size == 0)
    { 
      result = 11;     //Failed to write data
      Wire.endTransmission(true);
    }
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

//page write up to 16 bytes data to eeprom address 0x50/0x51 and byte address 0 - 255 
uint8_t pageWriteDataBytes(uint8_t eeprom_address, unsigned int byte_address, uint8_t number, uint8_t *data_bytes)
{
  uint8_t result;
  Wire.beginTransmission(eeprom_address);
  uint8_t size = Wire.write(byte_address);
  if (size == 0) 
    result = 10;   //Failed to set address
  else
  {
    for (int byte_count = 0; byte_count < number; byte_count++)
      size = Wire.write(data_bytes[byte_count]);
    if (size == 0)
    { 
      result = 11;     //Failed to write data
      Wire.endTransmission(true);
    }
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

//write 1 byte to address 0 - 511
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
    { 
      result = 11;     //Failed to write data
      Wire.endTransmission(true);
    }
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
  unsigned int byte_address = 0;
  uint8_t data_byte_w = 1;
  uint8_t data_byte_r = 255;
  uint8_t result=255;
  uint8_t data_bytes_r[32];
  uint8_t number_r = 32;
  uint8_t data_bytes_w[16];
  uint8_t number_w = 16;

  //initialize read array
  for (uint8_t byte_count = 0; byte_count < 32; byte_count++)
  {
    data_bytes_r[byte_count] = 255;
  }
  //initialize write array
  for (uint8_t byte_count = 0; byte_count < 16; byte_count++)
  {
    data_bytes_w[byte_count] = 1;
  }
  
  /*    
  result = readDataByteWholeEEPROM(EEPROM_ADDRESS, byte_address, data_byte_r);
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
  if(result == 0)
  {
    Serial.println("Write data successfully");
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
      case 11: Serial.println("failed to write data"); break;
      default: Serial.println("unknown error"); break;  
    }
  }
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
  */

  /*
  for (int byte_count = 0; byte_count < 256; byte_count++)
  {
    result=writeDataByteWholeEEPROM(EEPROM_ADDRESS, byte_count, byte_count);
    delay(5);   //wait until write is completed
  }
  */
  
  /*
  int i = 255;
  for (int byte_count = 255; byte_count > -1; byte_count--)
  {
    i++;
    writeDataByteWholeEEPROM(EEPROM_ADDRESS, i, byte_count);
    delay(5);   //wait until write is completed
  }
  */ 

  /*
  result = pageWriteDataBytes1(EEPROM_ADDRESS, byte_address, number_w, data_bytes_w);
  delay(5); //wait until write is completed
  */

  /*
  readDataBytesSequential(EEPROM_ADDRESS, byte_address, number_r, data_bytes_r);
  for (int byte_count = 0; byte_count < number_r; byte_count++)
  {
    Serial.println(data_bytes_r[byte_count]);
  }
  */

  /**/
  for (int byte_count = 0; byte_count < 512; byte_count++)
  {
    readDataByteWholeEEPROM(EEPROM_ADDRESS,byte_count, data_byte_r);
    Serial.println(data_byte_r);
  }
  

}
void loop()
{

}
