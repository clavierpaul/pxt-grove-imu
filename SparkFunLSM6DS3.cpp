#include "SparkFunLSM6DS3.h"
#include "stdint.h"

#include "MicroBit.h"
int16_t ax=0,ay=0,az=0;int16_t gx=0,gy=0,gz=0;
#define Gyr_Gain 0.00763358
float dt=0.02;float AccelX;float AccelY;float GyroY;float mixY;LSM6DS3Core::LSM6DS3Core(uint8_t inputArg): i2c(I2C_SDA0,I2C_SCL0){I2CAddress=inputArg<<1;}status_t LSM6DS3Core::beginCore(void){status_t returnError=IMU_SUCCESS;volatile uint8_t temp=0;for(uint16_t i=0;i<10000;i++){temp++;}uint8_t readCheck;readRegister(&readCheck,LSM6DS3_ACC_GYRO_WHO_AM_I_REG);if(readCheck!=0x69){returnError=IMU_HW_ERROR;}return returnError;}status_t LSM6DS3Core::readRegisterRegion(char*outputPointer,uint8_t offset,uint8_t length){status_t returnError=IMU_SUCCESS;i2c.start();i2c.write(I2CAddress,(char*)&offset,1);i2c.read(I2CAddress,outputPointer,length);return returnError;}status_t LSM6DS3Core::readRegister(uint8_t*outputPointer,uint8_t offset){uint8_t result;status_t returnError=IMU_SUCCESS;i2c.start();result=i2c.readRegister(I2CAddress,offset);i2c.stop();*outputPointer=result;return returnError;}status_t LSM6DS3Core::readRegisterInt16(int16_t*outputPointer,uint8_t offset){char myBuffer[2];status_t returnError=readRegisterRegion(myBuffer,offset,2);int16_t output=(int16_t)myBuffer[0]|int16_t(myBuffer[1]<<8);*outputPointer=output;return returnError;}status_t LSM6DS3Core::writeRegister(uint8_t offset,uint8_t dataToWrite){status_t returnError=IMU_SUCCESS;i2c.start();i2c.writeRegister(I2CAddress,offset,dataToWrite);i2c.stop();return returnError;}status_t LSM6DS3Core::embeddedPage(void){status_t returnError=writeRegister(LSM6DS3_ACC_GYRO_RAM_ACCESS,0x80);return returnError;}status_t LSM6DS3Core::basePage(void){status_t returnError=writeRegister(LSM6DS3_ACC_GYRO_RAM_ACCESS,0x00);return returnError;}LSM6DS3::LSM6DS3(uint8_t inputArg): LSM6DS3Core(inputArg){settings.gyroEnabled=1;settings.gyroRange=2000;settings.gyroSampleRate=416;settings.gyroBandWidth=400;settings.gyroFifoEnabled=1;settings.gyroFifoDecimation=1;settings.accelEnabled=1;settings.accelODROff=1;settings.accelRange=16;settings.accelSampleRate=416;settings.accelBandWidth=100;settings.accelFifoEnabled=1;settings.accelFifoDecimation=1;settings.tempEnabled=1;settings.commMode=1;settings.fifoThreshold=3000;settings.fifoSampleRate=10;settings.fifoModeWord=0;allOnesCounter=0;nonSuccessCounter=0;}status_t LSM6DS3::begin(){uint8_t dataToWrite=0;status_t returnError=beginCore();dataToWrite=0;if(settings.accelEnabled==1){switch(settings.accelBandWidth){case 50:dataToWrite|=LSM6DS3_ACC_GYRO_BW_XL_50Hz;break;case 100:dataToWrite|=LSM6DS3_ACC_GYRO_BW_XL_100Hz;break;case 200:dataToWrite|=LSM6DS3_ACC_GYRO_BW_XL_200Hz;break;default:case 400:dataToWrite|=LSM6DS3_ACC_GYRO_BW_XL_400Hz;break;}switch(settings.accelRange){case 2:dataToWrite|=LSM6DS3_ACC_GYRO_FS_XL_2g;break;case 4:dataToWrite|=LSM6DS3_ACC_GYRO_FS_XL_4g;break;case 8:dataToWrite|=LSM6DS3_ACC_GYRO_FS_XL_8g;break;default:case 16:dataToWrite|=LSM6DS3_ACC_GYRO_FS_XL_16g;break;}switch(settings.accelSampleRate){case 13:dataToWrite|=LSM6DS3_ACC_GYRO_ODR_XL_13Hz;break;case 26:dataToWrite|=LSM6DS3_ACC_GYRO_ODR_XL_26Hz;break;case 52:dataToWrite|=LSM6DS3_ACC_GYRO_ODR_XL_52Hz;break;default:case 104:dataToWrite|=LSM6DS3_ACC_GYRO_ODR_XL_104Hz;break;case 208:dataToWrite|=LSM6DS3_ACC_GYRO_ODR_XL_208Hz;break;case 416:dataToWrite|=LSM6DS3_ACC_GYRO_ODR_XL_416Hz;break;case 833:dataToWrite|=LSM6DS3_ACC_GYRO_ODR_XL_833Hz;break;case 1660:dataToWrite|=LSM6DS3_ACC_GYRO_ODR_XL_1660Hz;break;case 3330:dataToWrite|=LSM6DS3_ACC_GYRO_ODR_XL_3330Hz;break;case 6660:dataToWrite|=LSM6DS3_ACC_GYRO_ODR_XL_6660Hz;break;case 13330:dataToWrite|=LSM6DS3_ACC_GYRO_ODR_XL_13330Hz;break;}}else{}writeRegister(LSM6DS3_ACC_GYRO_CTRL1_XL,dataToWrite);readRegister(&dataToWrite,LSM6DS3_ACC_GYRO_CTRL4_C);dataToWrite&=~((uint8_t)LSM6DS3_ACC_GYRO_BW_SCAL_ODR_ENABLED);if(settings.accelODROff==1){dataToWrite|=LSM6DS3_ACC_GYRO_BW_SCAL_ODR_ENABLED;}writeRegister(LSM6DS3_ACC_GYRO_CTRL4_C,dataToWrite);dataToWrite=0;if(settings.gyroEnabled==1){switch(settings.gyroRange){case 125:dataToWrite|=LSM6DS3_ACC_GYRO_FS_125_ENABLED;break;case 245:dataToWrite|=LSM6DS3_ACC_GYRO_FS_G_245dps;break;case 500:dataToWrite|=LSM6DS3_ACC_GYRO_FS_G_500dps;break;case 1000:dataToWrite|=LSM6DS3_ACC_GYRO_FS_G_1000dps;break;default:case 2000:dataToWrite|=LSM6DS3_ACC_GYRO_FS_G_2000dps;break;}switch(settings.gyroSampleRate){case 13:dataToWrite|=LSM6DS3_ACC_GYRO_ODR_G_13Hz;break;case 26:dataToWrite|=LSM6DS3_ACC_GYRO_ODR_G_26Hz;break;case 52:dataToWrite|=LSM6DS3_ACC_GYRO_ODR_G_52Hz;break;default:case 104:dataToWrite|=LSM6DS3_ACC_GYRO_ODR_G_104Hz;break;case 208:dataToWrite|=LSM6DS3_ACC_GYRO_ODR_G_208Hz;break;case 416:dataToWrite|=LSM6DS3_ACC_GYRO_ODR_G_416Hz;break;case 833:dataToWrite|=LSM6DS3_ACC_GYRO_ODR_G_833Hz;break;case 1660:dataToWrite|=LSM6DS3_ACC_GYRO_ODR_G_1660Hz;break;}}else{}writeRegister(LSM6DS3_ACC_GYRO_CTRL2_G,dataToWrite);if(settings.tempEnabled==1){}uint8_t result;readRegister(&result,LSM6DS3_ACC_GYRO_WHO_AM_I_REG);return returnError;}int16_t LSM6DS3::readRawAccelX(void){int16_t output;status_t errorLevel=readRegisterInt16(&output,LSM6DS3_ACC_GYRO_OUTX_L_XL);if(errorLevel!=IMU_SUCCESS){if(errorLevel==IMU_ALL_ONES_WARNING){allOnesCounter++;}else{nonSuccessCounter++;}}return output;}float LSM6DS3::readFloatAccelX(void){float output=calcAccel(readRawAccelX());return output;}int16_t LSM6DS3::readRawAccelY(void){int16_t output;status_t errorLevel=readRegisterInt16(&output,LSM6DS3_ACC_GYRO_OUTY_L_XL);if(errorLevel!=IMU_SUCCESS){if(errorLevel==IMU_ALL_ONES_WARNING){allOnesCounter++;}else{nonSuccessCounter++;}}return output;}float LSM6DS3::readFloatAccelY(void){float output=calcAccel(readRawAccelY());return output;}int16_t LSM6DS3::readRawAccelZ(void){int16_t output;status_t errorLevel=readRegisterInt16(&output,LSM6DS3_ACC_GYRO_OUTZ_L_XL);if(errorLevel!=IMU_SUCCESS){if(errorLevel==IMU_ALL_ONES_WARNING){allOnesCounter++;}else{nonSuccessCounter++;}}return output;}float LSM6DS3::readFloatAccelZ(void){float output=calcAccel(readRawAccelZ());return output;}float LSM6DS3::calcAccel(int16_t input){float output=(float)input*0.061*(settings.accelRange>>1)/1000;return output;}int16_t LSM6DS3::readRawGyroX(void){int16_t output;status_t errorLevel=readRegisterInt16(&output,LSM6DS3_ACC_GYRO_OUTX_L_G);if(errorLevel!=IMU_SUCCESS){if(errorLevel==IMU_ALL_ONES_WARNING){allOnesCounter++;}else{nonSuccessCounter++;}}return output;}float LSM6DS3::readFloatGyroX(void){float output=calcGyro(readRawGyroX());return output;}int16_t LSM6DS3::readRawGyroY(void){int16_t output;status_t errorLevel=readRegisterInt16(&output,LSM6DS3_ACC_GYRO_OUTY_L_G);if(errorLevel!=IMU_SUCCESS){if(errorLevel==IMU_ALL_ONES_WARNING){allOnesCounter++;}else{nonSuccessCounter++;}}return output;}float LSM6DS3::readFloatGyroY(void){float output=calcGyro(readRawGyroY());return output;}int16_t LSM6DS3::readRawGyroZ(void){int16_t output;status_t errorLevel=readRegisterInt16(&output,LSM6DS3_ACC_GYRO_OUTZ_L_G);if(errorLevel!=IMU_SUCCESS){if(errorLevel==IMU_ALL_ONES_WARNING){allOnesCounter++;}else{nonSuccessCounter++;}}return output;}float LSM6DS3::readFloatGyroZ(void){float output=calcGyro(readRawGyroZ());return output;}int16_t LSM6DS3::computeY(void){gy=readRawGyroY();ax=readRawAccelX();ay=readRawAccelY();az=readRawAccelZ();GyroY=Gyr_Gain*(gy)*-1;AccelY=(atan2(ay,az)*180/PI);AccelX=(atan2(ax,az)*180/PI);float K=0.98;float A=K/(K+dt);mixY=A*(mixY+GyroY*dt)+(1-A)*AccelX;fifoClear();return mixY;}float LSM6DS3::calcGyro(int16_t input){uint8_t gyroRangeDivisor=settings.gyroRange/125;if(settings.gyroRange==245){gyroRangeDivisor=2;}float output=(float)input*4.375*(gyroRangeDivisor)/1000;return output;}int16_t LSM6DS3::readRawTemp(void){int16_t output;readRegisterInt16(&output,LSM6DS3_ACC_GYRO_OUT_TEMP_L);return output;}float LSM6DS3::readTempC(void){float output=(float)readRawTemp()/16;output+=25;return output;}float LSM6DS3::readTempF(void){float output=(float)readRawTemp()/16;output+=25;output=(output*9)/5+32;return output;}void LSM6DS3::fifoBegin(void){uint8_t thresholdLByte=settings.fifoThreshold&0x00FF;uint8_t thresholdHByte=(settings.fifoThreshold&0x0F00)>>8;uint8_t tempFIFO_CTRL3=0;if(settings.gyroFifoEnabled==1){tempFIFO_CTRL3|=(settings.gyroFifoDecimation&0x07)<<3;}if(settings.accelFifoEnabled==1){tempFIFO_CTRL3|=(settings.accelFifoDecimation&0x07);}uint8_t tempFIFO_CTRL4=0;uint8_t tempFIFO_CTRL5=0;switch(settings.fifoSampleRate){default:case 10:tempFIFO_CTRL5|=LSM6DS3_ACC_GYRO_ODR_FIFO_10Hz;break;case 25:tempFIFO_CTRL5|=LSM6DS3_ACC_GYRO_ODR_FIFO_25Hz;break;case 50:tempFIFO_CTRL5|=LSM6DS3_ACC_GYRO_ODR_FIFO_50Hz;break;case 100:tempFIFO_CTRL5|=LSM6DS3_ACC_GYRO_ODR_FIFO_100Hz;break;case 200:tempFIFO_CTRL5|=LSM6DS3_ACC_GYRO_ODR_FIFO_200Hz;break;case 400:tempFIFO_CTRL5|=LSM6DS3_ACC_GYRO_ODR_FIFO_400Hz;break;case 800:tempFIFO_CTRL5|=LSM6DS3_ACC_GYRO_ODR_FIFO_800Hz;break;case 1600:tempFIFO_CTRL5|=LSM6DS3_ACC_GYRO_ODR_FIFO_1600Hz;break;case 3300:tempFIFO_CTRL5|=LSM6DS3_ACC_GYRO_ODR_FIFO_3300Hz;break;case 6600:tempFIFO_CTRL5|=LSM6DS3_ACC_GYRO_ODR_FIFO_6600Hz;break;}tempFIFO_CTRL5|=settings.fifoModeWord=6;writeRegister(LSM6DS3_ACC_GYRO_FIFO_CTRL1,thresholdLByte);writeRegister(LSM6DS3_ACC_GYRO_FIFO_CTRL2,thresholdHByte);writeRegister(LSM6DS3_ACC_GYRO_FIFO_CTRL3,tempFIFO_CTRL3);writeRegister(LSM6DS3_ACC_GYRO_FIFO_CTRL4,tempFIFO_CTRL4);writeRegister(LSM6DS3_ACC_GYRO_FIFO_CTRL5,tempFIFO_CTRL5);}void LSM6DS3::fifoClear(void){while((fifoGetStatus()&0x1000)==0){fifoRead();}}int16_t LSM6DS3::fifoRead(void){uint8_t tempReadByte=0;uint16_t tempAccumulator=0;readRegister(&tempReadByte,LSM6DS3_ACC_GYRO_FIFO_DATA_OUT_L);tempAccumulator=tempReadByte;readRegister(&tempReadByte,LSM6DS3_ACC_GYRO_FIFO_DATA_OUT_H);tempAccumulator|=((uint16_t)tempReadByte<<8);return tempAccumulator;}uint16_t LSM6DS3::fifoGetStatus(void){uint8_t tempReadByte=0;uint16_t tempAccumulator=0;readRegister(&tempReadByte,LSM6DS3_ACC_GYRO_FIFO_STATUS1);tempAccumulator=tempReadByte;readRegister(&tempReadByte,LSM6DS3_ACC_GYRO_FIFO_STATUS2);tempAccumulator|=(tempReadByte<<8);return tempAccumulator;}void LSM6DS3::fifoEnd(void){writeRegister(LSM6DS3_ACC_GYRO_FIFO_STATUS1,0x00);}