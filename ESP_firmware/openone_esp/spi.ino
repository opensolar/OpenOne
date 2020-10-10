
//  _____   _____  _______ __   _ _______  _____         _______  ______   _____  _____ 
// |     | |_____] |______ | \  | |______ |     | |      |_____| |_____/     |   |     |
// |_____| |       |______ |  \_| ______| |_____| |_____ |     | |    \_ . __|__ |_____|

// Author: Martin Lafleur (mlafleur@opensolar.io)
// Date: Sep 2020
// Project: OpenOne
//
// Copyright 2020 opensolar.io
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and 
// associated documentation files (the "Software"), to deal in the Software without restriction, 
// including without limitation the rights to use, copy, modify, merge, publish, distribute, 
// sublicense, and/or sell copies of the Software, and to permit persons to whom the Software 
// is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in 
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//    

typedef struct {
  MOTOR motor[2];
  CONTROL st_control;
} SPI_TX_FRAME;

typedef struct{
  uint16_t hall[NB_HALLS];
  uint16_t st_status;
  uint16_t spare;
} SPI_RX_FRAME;

const int SPI_BUFFER_SIZE = sizeof(SPI_TX_FRAME);


uint8_t txData[SPI_BUFFER_SIZE];
uint8_t rxData[SPI_BUFFER_SIZE];    
int rxLength;



// To initialize the SPI and CAN, must be called once at startup
void spiBegin(){
//    GPIO    Name
//   ===================================
//     15     SS
//     13     MOSI
//     12     MISO 
//     14     SCK
  
    

  if(sizeof(SPI_TX_FRAME) == sizeof(SPI_RX_FRAME)){
    SPI.begin();
    pinMode(GPIO_SS, OUTPUT);
    digitalWrite(GPIO_SS, HIGH);
  } else {
    Serial.println("ERROR: SPI RX & TX frames are not equal");
  }

  Serial.println("motor size:" + String(sizeof(MOTOR)));
  Serial.println("control size:" + String(sizeof(CONTROL)));
  Serial.println("tx size:" + String(sizeof(SPI_TX_FRAME)));
}

// Send data to the ST microcontroller via SPI
bool writeData(const uint8_t * tx, size_t txLength) {
  if(txLength > SPI_BUFFER_SIZE) {
    Serial.println("error SPI frame too large\n");
    return false;
  }
  Serial.println("tx size:" + String(txLength));

  memset(txData, 0, SPI_BUFFER_SIZE);
  memcpy(txData, tx, txLength);
 
  digitalWrite(GPIO_SS, LOW);
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  
  delayMicroseconds(50); // must be lower than the ST interrupt latency
  
  for(int i = 0; i < SPI_BUFFER_SIZE; i++){
    rxData[i] = SPI.transfer(txData[i]);  
  }


  digitalWrite(GPIO_SS, HIGH);
  SPI.endTransaction();
  return true;
}

void execute_spi_transaction(){
  MOTOR roll = { CW, 0, 32768};
  MOTOR pitch = { CCW, 0, 7};
  CONTROL control = { 1,2,0};
  SPI_TX_FRAME txFrame;
  memcpy(&txFrame.motor[0], &roll, sizeof(MOTOR));
  memcpy(&txFrame.motor[1], &pitch, sizeof(MOTOR));
  memcpy(&txFrame.st_control, &control, sizeof(CONTROL));
  //txFrame.motor[0] = roll;
  //txFrame.motor[1] = pitch;
  //txFrame.st_control = control;
  //Serial.println("Roll speed:" + String(txFrame.motor[0].speed));


  writeData((const uint8_t *)&txFrame, sizeof(SPI_TX_FRAME)); 

  SPI_RX_FRAME rxFrame;
  memcpy((void *)&rxFrame, (void *)rxData, sizeof(SPI_RX_FRAME));

  Serial.print("halls: ");
  for(int i = 0; i < NB_HALLS; i++){
      //uint32_t mv = 3000 * rxFrame.hall[i] / 4096;
      //Serial.print(String(mv)+ " ");
      Serial.print(String(rxFrame.hall[i])+ " ");
  }
  Serial.print("\n");
  Serial.println("ST status:" + String(rxFrame.st_status));
}
