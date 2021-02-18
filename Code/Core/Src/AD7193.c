#include <AD7193.h>

unsigned long registerMap[4] = {
  0x00,
  0x080060,
  0x000117,
  0x000000
};

int registerSize[8] = 
{
  1,
  3,
  3,
  3,
  1,
  1,
  3,
  3
};


void Reset(void)  
{
  HAL_Delay(100);
	uint8_t reset = 0xFF;
  char i = 0;
  for(i = 0; i < 6; i++)
  {
    HAL_SPI_Transmit(ADC_SPI, &reset, 1, SPI_TIMEOUT);
  }    
  HAL_Delay(100);
}

void SetPGAGain(int gain)  
{

  unsigned long gainSetting;

  if(gain == 1)         {gainSetting = 0x0;}
  else if (gain == 8)   {gainSetting = 0x3;}
  else if (gain == 16)  {gainSetting = 0x4;}
  else if (gain == 32)  {gainSetting = 0x5;}
  else if (gain == 64)  {gainSetting = 0x6;}
  else if (gain == 128) {gainSetting = 0x7;}

  registerMap[2] &= 0xFFFFF8; //keep all bit values except gain bits
  registerMap[2] |= gainSetting;

  SetRegisterValue(2, registerMap[2], registerSize[2], 1);
}

void SetAveraging(int FS)  
{
  registerMap[1] &= 0xFFFC00; //keep all bit values except filter setting bits
  registerMap[1] |= FS;

  SetRegisterValue(1, registerMap[1], registerSize[1], 1);
}

void SetPsuedoDifferentialInputs(void)  
{
  registerMap[2] &= 0xFBFFFF;
  registerMap[2] |= 0x040000;

  SetRegisterValue(2, registerMap[2], registerSize[2], 1);
}

void AppendStatusValuetoData(void) 
{
  registerMap[1] &= 0xEFFFFF; //keep all bit values except DAT_STA bit
  registerMap[1] |= 0x100000;  // set DAT_STA to 1

  SetRegisterValue(1, registerMap[1], registerSize[1], 1);

  registerSize[3] = 4; // change register size to 4, b/c status register is now appended
}

void SetFilterSinc3(void)
{
    registerMap[1] &= 0xFF7FFF; //keep all bit values except SINC3
    registerMap[1] |= 0x008000;  // set SINC3 to 1

    SetRegisterValue(1, registerMap[1], registerSize[1], 1);
}

uint8_t WaitForADC(void)  
{
    int breakTime = 0;

    while(1)
		{
      if (HAL_GPIO_ReadPin(ADC_MISO_GPIO_Port, ADC_MISO_Pin) == 0)
			{  
				return 0;// Break if ready goes low
        break;
      }

      if (breakTime > 5000) {                       // Break after five seconds - avoids program hanging up
        return 1;//Serial.print("Data Ready never went low!");
        break;
      }

      if (HAL_GPIO_ReadPin(ADC_MISO_GPIO_Port, ADC_MISO_Pin)) 
			{
				//Serial.print(".");
			}
			
      HAL_Delay(1);
      breakTime = breakTime + 1;
		}
		return 0;
}

uint8_t Calibrate(void) 
{

  HAL_Delay(100);


  registerMap[1] &= 0x1FFFFF; //keep all bit values except Channel bits
  registerMap[1] |= 0x800000; // internal zero scale calibration

  SetRegisterValue(1, registerMap[1], 3, 0);  // overwriting previous MODE reg setting 

  if (WaitForADC() == 0)
	{
		registerMap[1] &= 0x1FFFFF; //keep all bit values except Channel bits
		registerMap[1] |= 0xA00000; // internal full scale calibration

		SetRegisterValue(1, registerMap[1], 3, 0);  // overwriting previous MODE reg setting 
		
		if(  WaitForADC() ==0)
		{
			 HAL_Delay(100); 
			return 0;
		}
		else
			return 1;
	}
	else
		return 1;
}

void IntitiateSingleConversion(void) 
{
  HAL_Delay(100);
  
  registerMap[1] &= 0x1FFFFF; //keep all bit values except Channel bits
  registerMap[1] |= 0x200000; // single conversion mode bits  

  SetRegisterValue(1, registerMap[1], 3, 0);  // overwriting previous MODE reg setting 
}

unsigned long ReadADCData(void)  
{
  
    unsigned char byteIndex = 0;
    unsigned long buffer = 0;
    unsigned char receiveBuffer = 0;
    unsigned char dataLength = registerSize[3];  // data length depends on if Status register is appended to Data read - see AppendStatusValuetoData()

		uint8_t start = 0x58;
		uint8_t dummy = 0x00;
	
    HAL_SPI_Transmit(ADC_SPI, &start, 1, SPI_TIMEOUT); // command to start read data
    
    while(byteIndex < dataLength)
    {
      HAL_SPI_TransmitReceive(ADC_SPI, &dummy, &receiveBuffer, 1, SPI_TIMEOUT);
      buffer = (buffer << 8) + receiveBuffer;
      byteIndex++;
    }

    return(buffer);
}


void SetChannel(int channel) {

    // generate Channel settings bits for Configuration write
    unsigned long shiftvalue = 0x00000100;
    unsigned long channelBits = shiftvalue << channel;

    // Write Channel bits to Config register, keeping other bits as is
    registerMap[2] &= 0xFC00FF; //keep all bit values except Channel bits
    registerMap[2] |= channelBits;

    // write channel selected to Configuration register
    SetRegisterValue(2, registerMap[2], registerSize[2], 1);
    HAL_Delay(10);
}

unsigned long ReadADCChannel(int channel)  {
     
    SetChannel(channel);

    // write command to initial conversion
    IntitiateSingleConversion();
    //delay(100); // hardcoded wait time for data to be ready
    // should scale the wait time by averaging

    WaitForADC();
    
    unsigned long ADCdata = ReadADCData();
    HAL_Delay(10);

    // end communication cycle, bringing CS pin High manually 
    HAL_Delay(10);

    return(ADCdata);
}



float DataToVoltage(long rawData)  {
  float voltage = 0;
  float mVref = 2.5;
  char mPolarity = 0;

  int PGASetting = registerMap[2] & 0x000007;  // keep only the PGA setting bits
  int PGAGain;

  if (PGASetting == 0) {
    PGAGain = 1;
  } else if (PGASetting == 3) {
    PGAGain = 8;
  } else if (PGASetting == 4) {
    PGAGain = 16;
  } else if (PGASetting == 5) {
    PGAGain = 32;
  } else if (PGASetting == 6) {
    PGAGain = 64;
  } else if (PGASetting == 7) {
    PGAGain = 128;
  } else {
    PGAGain = 1;
  }

  
  //Serial.print("PGA Gain = ");
  //Serial.println(PGAGain);

  
  if(mPolarity == 1)
  {
    voltage = ((double)rawData / 16777216 / (1 << PGAGain)) * mVref; 
  }
  if(mPolarity == 0)
  {
    voltage = (((float)rawData / (float)8388608) - (float)1) * (mVref / (float)PGAGain);
  }


  return(voltage);
}

/*! Reads the value of a register. */
unsigned long GetRegisterValue(unsigned char registerAddress, unsigned char bytesNumber, unsigned char modifyCS)  
{//getregistervalue
    
    unsigned char receiveBuffer = 0;
    unsigned char writeByte = 0;
    unsigned char byteIndex = 0;
    unsigned long buffer = 0;
	
    uint8_t dummy = 0x00;
	
    writeByte = AD7193_COMM_READ | AD7193_COMM_ADDR(registerAddress);

    HAL_SPI_Transmit(ADC_SPI, &writeByte, 1, SPI_TIMEOUT);
	
    while(byteIndex < bytesNumber)
    {
        HAL_SPI_TransmitReceive(ADC_SPI, &dummy, &receiveBuffer, 1, SPI_TIMEOUT);
        buffer = (buffer << 8) + receiveBuffer;
        byteIndex++;
    }

//    char str[32];
		//jakbym mial usart
//    sprintf(str, "%06x", buffer);

//    Serial.print("    Read Register Address: ");
//    Serial.print(registerAddress, HEX);
//    Serial.print(", command: ");
//    Serial.print(writeByte, HEX);
//    Serial.print(", recieved: ");
//    Serial.println(buffer, HEX);
    
    
    return(buffer);
}

/*! Writes data into a register. */
void SetRegisterValue(unsigned char registerAddress,  unsigned long registerValue,  unsigned char bytesNumber,  unsigned char modifyCS)  {//setregistervalue
    unsigned char commandByte = 0;
    unsigned char txBuffer[4] = {0, 0, 0 ,0};
    
    commandByte = AD7193_COMM_WRITE | AD7193_COMM_ADDR(registerAddress);
  
    txBuffer[0] = (registerValue >> 0)  & 0x000000FF;
    txBuffer[1] = (registerValue >> 8)  & 0x000000FF;
    txBuffer[2] = (registerValue >> 16) & 0x000000FF;
    txBuffer[3] = (registerValue >> 24) & 0x000000FF;

    HAL_SPI_Transmit(ADC_SPI, &commandByte, 1, SPI_TIMEOUT);
    while(bytesNumber > 0)
    {
        HAL_SPI_Transmit(ADC_SPI, &txBuffer[bytesNumber - 1], 1, SPI_TIMEOUT);
        bytesNumber--;
    }

    /*Serial.print("    Write Register Address: ");
    Serial.print(registerAddress, HEX);
    Serial.print(", command: ");
    Serial.print(commandByte, HEX);
    Serial.print(",     sent: ");
    Serial.println(registerValue, HEX);*/
}

void ReadRegisterMap(void)  
{
  GetRegisterValue(0, 1, 1);
  GetRegisterValue(1, 3, 1);
  GetRegisterValue(2, 3, 1);
  GetRegisterValue(3, registerSize[3], 1);
  GetRegisterValue(4, 1, 1);
  GetRegisterValue(6, 3, 1);
  GetRegisterValue(7, 3, 1);
  HAL_Delay(100);
}
