
#include "mpu9250.h"

#define ID_MPU	0x11

enum Mscale {
  MFS_14BITS = 0, // 0.6 mG per LSB
  MFS_16BITS      // 0.15 mG per LSB
};

uint8_t Mscale = MFS_16BITS; // MFS_14BITS or MFS_16BITS, 14-bit or 16-bit magnetometer resolution
uint8_t Mmode = 0x06;        // Either 8 Hz 0x02) or 100 Hz (0x06) magnetometer data ODR
float aRes, gRes, mRes;      // scale resolutions per LSB for the sensors

float gyroBias[3] = {0, 0, 0};
float accelBias[3] = {0, 0, 0}; // Bias corrections for gyro and accelerometer
float magCalibration[3] = {0, 0, 0};
float magbias[3] = {0, 0, 0};  // Factory mag calibration and mag bias

static int32_t	accel_bias_int[3] = {0, 0, 0};
static int32_t  gyro_bias_int[3] = {0, 0, 0};

//Angles d'Euler (Roll, Pitch and Yaw)
 float phi=0.0f, theta=0.0f, psi=0.0f;


//================================================================
//			INIT MPU9250
//================================================================
void mpu9250_InitMPU9250()
{
	mpu9250_ResetMPU9250();
	HAL_Delay(100);
	// wake up device
	i2c1_WriteRegByte_IT(MPU9250_ADDRESS, PWR_MGMT_1, 	0x00); // Clear sleep mode bit (6), enable all sensors
	HAL_Delay(100); // Delay 100 ms for PLL to get established on x-axis gyro; should check for PLL ready interrupt
	i2c1_WriteRegByte_IT(MPU9250_ADDRESS, PWR_MGMT_1, 	0x01);  // Set clock source to be PLL with x-axis gyroscope reference, bits 2:0 = 001
	i2c1_WriteRegByte_IT(MPU9250_ADDRESS, PWR_MGMT_2, 	0x00);
	i2c1_WriteRegByte_IT(MPU9250_ADDRESS, CONFIG, 		BANDWITH_GYRO_250_HZ); //41
	i2c1_WriteRegByte_IT(MPU9250_ADDRESS, SMPLRT_DIV, 	SAMPLE_RATE_DIVIDER_0);
	i2c1_WriteRegByte_IT(MPU9250_ADDRESS, GYRO_CONFIG, 	GYRO_FULL_SCALE_1000_DPS);
	i2c1_WriteRegByte_IT(MPU9250_ADDRESS, ACCEL_CONFIG, 	ACC_FULL_SCALE_4_G);
	i2c1_WriteRegByte_IT(MPU9250_ADDRESS, ACCEL_CONFIG2, BANDWITH_ACCEL_41_Hz);
	i2c1_WriteRegByte_IT(MPU9250_ADDRESS, INT_PIN_CFG, 	BYPASS_EN); // 0x37 0x02
}
//================================================================
//			INIT MAGNETOMETER AK8963
//================================================================
void mpu9250_InitAK8963(float * destination)
{
// First extract the factory calibration for each magnetometer axis
uint8_t rawData[3];  // x/y/z gyro calibration data stored here
i2c1_WriteRegByte_IT( AK8963_ADDRESS, AK8963_CNTL, 0x00); // Power down magnetometer
HAL_Delay(10);
i2c1_WriteRegByte_IT( AK8963_ADDRESS, AK8963_CNTL, 0x0F); // Enter Fuse ROM access mode
HAL_Delay(10);
i2c1_ReadRegBuffer_IT( AK8963_ADDRESS, AK8963_ASAX,&rawData[0],3);  // Read the x-, y-, and z-axis calibration values
destination[0] =  (float)(rawData[0] - 128)/256.0f + 1.0f;   // Return x-axis sensitivity adjustment values, etc.
destination[1] =  (float)(rawData[1] - 128)/256.0f + 1.0f;
destination[2] =  (float)(rawData[2] - 128)/256.0f + 1.0f;
i2c1_WriteRegByte_IT(AK8963_ADDRESS, AK8963_CNTL, 0x00); // Power down magnetometer
HAL_Delay(10);
// Configure the magnetometer for continuous read and highest resolution
// set Mscale bit 4 to 1 (0) to enable 16 (14) bit resolution in CNTL register,
// and enable continuous mode data acquisition Mmode (bits [3:0]), 0010 for 8 Hz and 0110 for 100 Hz sample rates
i2c1_WriteRegByte_IT(AK8963_ADDRESS, AK8963_CNTL, Mscale << 4 | Mmode); // Set magnetometer data resolution and sample ODR  // 0x0A 0x16
HAL_Delay(10);
}
//================================================================
//	GET MPU9250 DEVICE ID (Default Value : 0x71)
//================================================================
uint8_t mpu9250_WhoAmI(void)
{
	uint8_t response=0;
	i2c1_ReadRegBuffer_IT( MPU9250_ADDRESS, WHO_AM_I_MPU9250 ,&response,1);
	return response;
}
//================================================================
// 		GET MAGNETOMETER AK8963 DEVICE ID
//================================================================
uint8_t mpu9250_MagWhoAmI(void)
{
	uint8_t response=0;
	i2c1_ReadRegBuffer_IT( AK8963_ADDRESS, WHO_AM_I_AK8963 ,&response,1);
	return response;
}
//================================================================
//				STEP
//================================================================
void mpu9250_Step(void)
{
		uint8_t Buf[14];


		// ____________________________________
		// :::  accelerometer and gyroscope :::

		// Lecture de 14 Registres à partir de ACCEL_XOUT_H
		// ACCEL_XOUT_H, ACCEL_XOUT_L, ACCEL_YOUT_H, ACCEL_YOUT_L, ACCEL_ZOUT_H, ACCEL_ZOUT_L
		// TEMP_OUT_H, TEMP_OUT_L, GYRO_XOUT_H, GYRO_XOUT_L, GYRO_YOUT_H, GYRO_YOUT_L, GYRO_ZOUT_H, GYRO_ZOUT_L
		i2c1_ReadRegBuffer( MPU9250_ADDRESS,ACCEL_XOUT_H ,Buf,14);

		int16_t ax=-(Buf[0]<<8 | Buf[1]);
		int16_t ay=-(Buf[2]<<8 | Buf[3]);
		int16_t az=Buf[4]<<8 | Buf[5];

		ax=ax+accel_bias_int[0];
		ay=ay+accel_bias_int[1];
		az=az-accel_bias_int[2];

		float axc = ax*ACC_MOD_CONST; //ACC_MOD_CONST = (4/32768)
		float ayc = ay*ACC_MOD_CONST; //ACC_MOD_CONST = (4/32768)
		float azc = az*ACC_MOD_CONST; //ACC_MOD_CONST = (4/32768)

		int16_t gx=-(Buf[8]<<8 | Buf[9]);
		int16_t gy=-(Buf[10]<<8 | Buf[11]);
		int16_t gz=Buf[12]<<8 | Buf[13];

		gx = gx + gyro_bias_int[0];
		gy = gy + gyro_bias_int[1];
		gz = gz - gyro_bias_int[2];

		float gxc = gx*GYRO_MOD_CONST; //GYRO_MOD_CONST  = (1000.0/32768.0)*(PI/180.0)
		float gyc = gy*GYRO_MOD_CONST; //GYRO_MOD_CONST  = (1000.0/32768.0)*(PI/180.0)
		float gzc = gz*GYRO_MOD_CONST; //GYRO_MOD_CONST  = (1000.0/32768.0)*(PI/180.0)

		MadgwickAHRSupdateIMU(gxc, gyc, gzc, axc, ayc, azc);

		calc_matRot(q0,q1,q2,q3);

		int16_t phi_int, theta_int, psi_int;

		phi=(180.0/PI)*phi;
		psi=(180.0/PI)*psi;
		theta=(180.0/PI)*theta;

		phi_int = (int16_t)phi;
		psi_int = (int16_t)psi;
		theta_int = (int16_t)theta;

		// Split short into two char

		char lowerByte1 = gx & 0xFF; // Lower byte gx
		char upperByte1 = gx >> 8; // Upper byte gx
		char lowerByte2 = phi_int & 0xFF; // Lower byte phi
		char upperByte2 = phi_int >> 8; // Upper byte phi
		char lowerByte3 = theta_int & 0xFF; // Lower byte teta
		char upperByte3 = theta_int >> 8; // Upper byte teta
		char lowerByte4 = psi_int & 0xFF; // Lower byte psi
		char upperByte4 = psi_int >> 8; // Upper byte psi

		//Test Union

		CAN_Message      txMsg;
		txMsg.id = ID_MPU; //Card ID

		//Send gx
		txMsg.data[0] = lowerByte1;//measure from sensor
		txMsg.data[1] = upperByte1;

		//Send angle for current instant through CAN
		txMsg.data[2] = lowerByte2;
		txMsg.data[3] = upperByte2;

		txMsg.data[4] = lowerByte3;
		txMsg.data[5] = upperByte3;

		txMsg.data[6] = lowerByte4;
		txMsg.data[7] = upperByte4;

		txMsg.len = 8;
		txMsg.format = CANStandard;
		txMsg.type = CANData;

		can_Write(txMsg);


		//term_printf("Phi(Roll): %d Theta (Pitch): %d Psi (Yaw): %d \n\r",phi_int , theta_int, psi_int);

		/*Read valeurs de Acc et Gyr ReadData?
		* executer function MadgwickAHRSupdateIMU pour trouver quaternions
		* calculer matrix de rotation avec quaternions
		* trouver angles d'euler
		* importer angles pour faire le mouvement avec openGl
		*/


#if USE_MAGNETOMETER
	   	//_____________________
	    // :::  Magnetometer :::
	    // Read register Status 1 and wait for the DRDY: Data Ready
	    uint8_t magStatus1=0;
	    do
	    {
	   	 i2c1_ReadRegBuffer_IT( AK8963_ADDRESS,AK8963_ST1 ,&magStatus1,1);
	    }
	    while (!(magStatus1&0x01));
	    // Read magnetometer data
	    uint8_t Mag[7];
	   	i2c1_ReadRegBuffer_IT( AK8963_ADDRESS,AK8963_XOUT_L,Mag,7);
    	int16_t mx=-(Mag[3]<<8 | Mag[2]);
    	int16_t my=-(Mag[1]<<8 | Mag[0]);
    	int16_t mz=-(Mag[5]<<8 | Mag[4]);

#endif

  }
//================================================================
//			READ ACCELERATION
//================================================================
void mpu9250_ReadAccelData(int16_t * destination)
{
  uint8_t rawData[6];  // x/y/z accel register data stored here
  i2c1_ReadRegBuffer_IT( MPU9250_ADDRESS, ACCEL_XOUT_H,&rawData[0],6);  // Read the six raw data registers into data array
  destination[0] = (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]) ;  // Turn the MSB and LSB into a signed 16-bit value
  destination[1] = (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]) ;
  destination[2] = (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]) ;
}
//================================================================
//			READ GYRO
//================================================================
void mpu9250_ReadGyroData(int16_t * destination)
{
  uint8_t rawData[6];  // x/y/z gyro register data stored here
  i2c1_ReadRegBuffer_IT( MPU9250_ADDRESS, GYRO_XOUT_H, &rawData[0],6);  // Read the six raw data registers sequentially into data array
  destination[0] = (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]) ;  // Turn the MSB and LSB into a signed 16-bit value
  destination[1] = (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]) ;
  destination[2] = (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]) ;
}
//================================================================
//			READ MAGNETOMETER
//================================================================
void mpu9250_ReadMagData(int16_t * destination)
{
  uint8_t rawData[7];  // x/y/z gyro register data, ST2 register stored here, must read ST2 at end of data acquisition
  uint8_t magStatus1=0;
  do
  {
 	 i2c1_ReadRegBuffer_IT( AK8963_ADDRESS,AK8963_ST1 , &magStatus1,1);
  }
  while (!(magStatus1&0x01)); // wait for magnetometer data ready bit to be set

  i2c1_ReadRegBuffer_IT(AK8963_ADDRESS, AK8963_XOUT_L, &rawData[0],7);  // Read the six raw data and ST2 registers sequentially into data array
  uint8_t c = rawData[6]; // End data read by reading ST2 register
    if(!(c & 0x08)) { // Check if magnetic sensor overflow set, if not then report data
    destination[0] = (int16_t)(((int16_t)rawData[1] << 8) | rawData[0]);  // Turn the MSB and LSB into a signed 16-bit value
    destination[1] = (int16_t)(((int16_t)rawData[3] << 8) | rawData[2]) ;  // Data stored as little Endian
    destination[2] = (int16_t)(((int16_t)rawData[5] << 8) | rawData[4]) ;
   }
}
//================================================================
//			READ TEMPERATURE
//================================================================
int16_t mpu9250_ReadTempData()
{
  uint8_t rawData[2];  // x/y/z gyro register data stored here
  i2c1_ReadRegBuffer_IT( MPU9250_ADDRESS, TEMP_OUT_H, &rawData[0],2);  // Read the two raw data registers sequentially into data array
  return (int16_t)(((int16_t)rawData[0]) << 8 | rawData[1]) ;  // Turn the MSB and LSB into a 16-bit value
}
//================================================================
//			RESET MPU9250
//================================================================
void mpu9250_ResetMPU9250()
{
	i2c1_WriteRegByte_IT( MPU9250_ADDRESS, PWR_MGMT_1, 0x80); // Write a one to bit 7 reset bit; toggle reset device
	  HAL_Delay(100);
}
//================================================================
//			CALIBRATE
//================================================================
void mpu9250_CalibrateMPU9250()
{
	int nb_mes=0;
	uint8_t Buf[14];
	int16_t ax=0, ay=0, az=0;
	int16_t gx=0, gy=0, gz=0;
	int32_t ax_acc=0, ay_acc=0, az_acc=0;
	int32_t	gx_acc=0, gy_acc=0, gz_acc=0;
	//uint8_t data[12]; // data array to hold accelerometer and gyro x, y, z, data

	for(nb_mes=0 ; nb_mes < 100 ; nb_mes++)
	{
	i2c1_ReadRegBuffer_IT( MPU9250_ADDRESS,ACCEL_XOUT_H ,  Buf,14);
	ax=(int16_t)((int16_t)Buf[0]<<8 	| Buf[1]);
	ay=(int16_t)((int16_t)Buf[2]<<8 	| Buf[3]);
	az=(int16_t)((int16_t)Buf[4]<<8 	| Buf[5]);
	gx=(int16_t)((int16_t)Buf[8]<<8 	| Buf[9]);
	gy=(int16_t)((int16_t)Buf[10]<<8 	| Buf[11]);
	gz=(int16_t)((int16_t)Buf[12]<<8 	| Buf[13]);

	ax_acc=ax_acc+(int32_t)ax;
	ay_acc=ay_acc+(int32_t)ay;
	az_acc=az_acc+(int32_t)az;
	gx_acc=gx_acc+(int32_t)gx;
	gy_acc=gy_acc+(int32_t)gy;
	gz_acc=gz_acc+(int32_t)gz;

	HAL_Delay(20);
	}

	accel_bias_int[0]=ax_acc/100;
	accel_bias_int[1]=ay_acc/100;
	accel_bias_int[2]=az_acc/100;
	gyro_bias_int[0]=gx_acc/100;
	gyro_bias_int[1]=gy_acc/100;
	gyro_bias_int[2]=gz_acc/100;

	// NORMALIZATION AND PUSH INTO BIAS HARDWARE REGISTER --> DOESN'T WORK
	// Construct the gyro biases for push to the hardware gyro bias registers, which are reset to zero upon device startup
	/*data[0] = (-gyro_bias_int[0]/GYRO_RATIO  >> 8) 	& 0xFF; // Divide by 4 to get 32.9 LSB per deg/s to conform to expected bias input format
	data[1] = (-gyro_bias_int[0]/GYRO_RATIO )       & 0xFF; // Biases are additive, so change sign on calculated average gyro biases
	data[2] = (-gyro_bias_int[1]/GYRO_RATIO   >> 8) & 0xFF;
	data[3] = (-gyro_bias_int[1]/GYRO_RATIO )       & 0xFF;
	data[4] = (-gyro_bias_int[2]/GYRO_RATIO   >> 8) & 0xFF;
	data[5] = (-gyro_bias_int[2]/GYRO_RATIO )       & 0xFF;*/

	// Push gyro biases to hardware registers ('works' randomly)
/*	i2c1_WriteRegByte_IT(MPU9250_ADDRESS, XG_OFFSET_H, data[0]);
	i2c1_WriteRegByte_IT(MPU9250_ADDRESS, XG_OFFSET_L, data[1]);
	i2c1_WriteRegByte_IT(MPU9250_ADDRESS, YG_OFFSET_H, data[2]);
	i2c1_WriteRegByte_IT(MPU9250_ADDRESS, YG_OFFSET_L, data[3]);
	i2c1_WriteRegByte_IT(MPU9250_ADDRESS, ZG_OFFSET_H, data[4]);
	i2c1_WriteRegByte_IT(MPU9250_ADDRESS, ZG_OFFSET_L, data[5]);*/
}

void calc_matRot(float q0, float q1, float q2, float q3){
	float R11, R12, R13, R21, R22, R23, R31, R32, R33;

	//Calcul de la Matrice de Rotation à Partir du Quaternion
	R11 = pow(q0,2) + pow(q1,2) - pow(q2,2) - pow(q3,2);
	R12 = 2*(q1*q2 - q0*q3);
	R13 = 2*(q1*q3 + q0*q2);
	R21 = 2*(q0*q3 + q1*q2);
	R22 = pow(q0,2) - pow(q1,2) + pow(q2,2) - pow(q3,2);
	R23 = 2*(q2*q3 - q0*q1);
	R31 = 2*(q1*q3 - q0*q2);
	R32 = 2*(q2*q3 + q0*q1);
	R33 = pow(q0,2) - pow(q1,2) -pow(q2,2) + pow(q3,2);

	//Calcul des Angles d’Euler à Partir de la matrice de rotation
	phi = atan2(-R31, R33);
	theta = asin(R32);
	psi = atan2(-R12, R22);

}

//=================================================================

void sendToCAN(char *data, int dataSize, uint16_t id){

	CAN_Message msg;

	for(int i=0; i<dataSize; i++){
		msg.data[i] = data[i];
	}

	msg.id = 0x55;
	msg.len = dataSize;
	msg.format = CANStandard;
	msg.type = CANData;

	can_Write(msg);
}

void intToChar(int toConvert, char *converted){

	converted[0] = (char)((toConvert << 24) && 0xFF);
	converted[1] = (char)((toConvert << 16) && 0XFF);
	converted[2] = (char)((toConvert << 8) && 0XFF);
	converted[3] = (char)((toConvert) && 0XFF);
}
