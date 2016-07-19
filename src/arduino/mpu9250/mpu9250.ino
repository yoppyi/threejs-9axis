#include <Wire.h>

//Magnetometer Registers
#define AK8963_ADDRESS 0x0C<<1
#define WHO_AM_I_AK8963 0x00 // should return 0x48
#define INFO 0x01
#define AK8963_ST1 0x02 // data ready status bit 0
#define AK8963_XOUT_L 0x03 // data
#define AK8963_XOUT_H 0x04
#define AK8963_YOUT_L 0x05
#define AK8963_YOUT_H 0x06
#define AK8963_ZOUT_L 0x07
#define AK8963_ZOUT_H 0x08
#define AK8963_ST2 0x09 // Data overflow bit 3 and data read error status bit 2
#define AK8963_CNTL 0x0A // Power down (0000), single-measurement (0001), self-test (1000) and Fuse ROM (1111) modes on bits 3:0
#define AK8963_ASTC 0x0C // Self test control
#define AK8963_I2CDIS 0x0F // I2C disable
#define AK8963_ASAX 0x10 // Fuse ROM x-axis sensitivity adjustment value
#define AK8963_ASAY 0x11 // Fuse ROM y-axis sensitivity adjustment value
#define AK8963_ASAZ 0x12 // Fuse ROM z-axis sensitivity adjustment value

#define SELF_TEST_X_GYRO 0x00
#define SELF_TEST_Y_GYRO 0x01
#define SELF_TEST_Z_GYRO 0x02

/*#define X_FINE_GAIN 0x03 // [7:0] fine gain
 #define Y_FINE_GAIN 0x04
 #define Z_FINE_GAIN 0x05
 #define XA_OFFSET_H 0x06 // User-defined trim values for accelerometer
 #define XA_OFFSET_L_TC 0x07
 #define YA_OFFSET_H 0x08
 #define YA_OFFSET_L_TC 0x09
 #define ZA_OFFSET_H 0x0A
 #define ZA_OFFSET_L_TC 0x0B */

#define SELF_TEST_X_ACCEL 0x0D
#define SELF_TEST_Y_ACCEL 0x0E
#define SELF_TEST_Z_ACCEL 0x0F

#define SELF_TEST_A 0x10

#define XG_OFFSET_H 0x13 // User-defined trim values for gyroscope
#define XG_OFFSET_L 0x14
#define YG_OFFSET_H 0x15
#define YG_OFFSET_L 0x16
#define ZG_OFFSET_H 0x17
#define ZG_OFFSET_L 0x18
#define SMPLRT_DIV 0x19
#define CONFIG 0x1A
#define GYRO_CONFIG 0x1B
#define ACCEL_CONFIG 0x1C
#define ACCEL_CONFIG2 0x1D
#define LP_ACCEL_ODR 0x1E
#define WOM_THR 0x1F

#define MOT_DUR 0x20 // Duration counter threshold for motion interrupt generation, 1 kHz rate, LSB = 1 ms
#define ZMOT_THR 0x21 // Zero-motion detection threshold bits [7:0]
#define ZRMOT_DUR 0x22 // Duration counter threshold for zero motion interrupt generation, 16 Hz rate, LSB = 64 ms

#define FIFO_EN 0x23
#define I2C_MST_CTRL 0x24
#define I2C_SLV0_ADDR 0x25
#define I2C_SLV0_REG 0x26
#define I2C_SLV0_CTRL 0x27
#define I2C_SLV1_ADDR 0x28
#define I2C_SLV1_REG 0x29
#define I2C_SLV1_CTRL 0x2A
#define I2C_SLV2_ADDR 0x2B
#define I2C_SLV2_REG 0x2C
#define I2C_SLV2_CTRL 0x2D
#define I2C_SLV3_ADDR 0x2E
#define I2C_SLV3_REG 0x2F
#define I2C_SLV3_CTRL 0x30
#define I2C_SLV4_ADDR 0x31
#define I2C_SLV4_REG 0x32
#define I2C_SLV4_DO 0x33
#define I2C_SLV4_CTRL 0x34
#define I2C_SLV4_DI 0x35
#define I2C_MST_STATUS 0x36
#define INT_PIN_CFG 0x37
#define INT_ENABLE 0x38
#define DMP_INT_STATUS 0x39 // Check DMP interrupt
#define INT_STATUS 0x3A
#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40
#define TEMP_OUT_H 0x41
#define TEMP_OUT_L 0x42
#define GYRO_XOUT_H 0x43
#define GYRO_XOUT_L 0x44
#define GYRO_YOUT_H 0x45
#define GYRO_YOUT_L 0x46
#define GYRO_ZOUT_H 0x47
#define GYRO_ZOUT_L 0x48
#define EXT_SENS_DATA_00 0x49
#define EXT_SENS_DATA_01 0x4A
#define EXT_SENS_DATA_02 0x4B
#define EXT_SENS_DATA_03 0x4C
#define EXT_SENS_DATA_04 0x4D
#define EXT_SENS_DATA_05 0x4E
#define EXT_SENS_DATA_06 0x4F
#define EXT_SENS_DATA_07 0x50
#define EXT_SENS_DATA_08 0x51
#define EXT_SENS_DATA_09 0x52
#define EXT_SENS_DATA_10 0x53
#define EXT_SENS_DATA_11 0x54
#define EXT_SENS_DATA_12 0x55
#define EXT_SENS_DATA_13 0x56
#define EXT_SENS_DATA_14 0x57
#define EXT_SENS_DATA_15 0x58
#define EXT_SENS_DATA_16 0x59
#define EXT_SENS_DATA_17 0x5A
#define EXT_SENS_DATA_18 0x5B
#define EXT_SENS_DATA_19 0x5C
#define EXT_SENS_DATA_20 0x5D
#define EXT_SENS_DATA_21 0x5E
#define EXT_SENS_DATA_22 0x5F
#define EXT_SENS_DATA_23 0x60
#define MOT_DETECT_STATUS 0x61
#define I2C_SLV0_DO 0x63
#define I2C_SLV1_DO 0x64
#define I2C_SLV2_DO 0x65
#define I2C_SLV3_DO 0x66
#define I2C_MST_DELAY_CTRL 0x67
#define SIGNAL_PATH_RESET 0x68
#define MOT_DETECT_CTRL 0x69
#define USER_CTRL 0x6A // Bit 7 enable DMP, bit 3 reset DMP
#define PWR_MGMT_1 0x6B // Device defaults to the SLEEP mode
#define PWR_MGMT_2 0x6C
#define DMP_BANK 0x6D // Activates a specific bank in the DMP
#define DMP_RW_PNT 0x6E // Set read/write pointer to a specific start address in specified DMP bank
#define DMP_REG 0x6F // Register in DMP from which to read or to which to write
#define DMP_REG_1 0x70
#define DMP_REG_2 0x71
#define FIFO_COUNTH 0x72
#define FIFO_COUNTL 0x73
#define FIFO_R_W 0x74
#define WHO_AM_I_MPU9250 0x75 // Should return 0x71
#define XA_OFFSET_H 0x77
#define XA_OFFSET_L 0x78
#define YA_OFFSET_H 0x7A
#define YA_OFFSET_L 0x7B
#define ZA_OFFSET_H 0x7D
#define ZA_OFFSET_L 0x7E

// Using the MSENSR-9250 breakout board, ADO is set to 0
// Seven-bit device address is 110100 for ADO = 0 and 110101 for ADO = 1
//mbed uses the eight-bit device address, so shift seven-bit addresses left by one!
#define ADO 0
#if ADO
#define MPU9250_ADDRESS 0x69 // Device address when ADO = 1
#else
#define MPU9250_ADDRESS 0x68// Device address when ADO = 0
#endif

#define GYRO_FULL_SCALE_250_DPS 0x00
#define GYRO_FULL_SCALE_500_DPS 0x08
#define GYRO_FULL_SCALE_1000_DPS 0x10
#define GYRO_FULL_SCALE_2000_DPS 0x18

#define ACC_FULL_SCALE_2_G 0x00
#define ACC_FULL_SCALE_4_G 0x08
#define ACC_FULL_SCALE_8_G 0x10
#define ACC_FULL_SCALE_16_G 0x18

static const int SERIAL_BUFFER = 9600;

//uint8_t Ascale = AFS_2G;     // AFS_2G, AFS_4G, AFS_8G, AFS_16G
//uint8_t Gscale = GFS_250DPS; // GFS_250DPS, GFS_500DPS, GFS_1000DPS, GFS_2000DPS
//uint8_t Mscale = MFS_16BITS; // MFS_14BITS or MFS_16BITS, 14-bit or 16-bit magnetometer resolution
//uint8_t Mmode = 0x06; // Either 8 Hz 0x02) or 100 Hz (0x06) magnetometer data ODR
//float aRes, gRes, mRes;      // scale resolutions per LSB for the sensors

void setup() {

	// Arduino initializations
	Wire.begin();
	Serial.begin(SERIAL_BUFFER);

	write(MPU9250_ADDRESS, PWR_MGMT_1, 0x00);
	write(MPU9250_ADDRESS, INT_PIN_CFG, 0x02);

	write(MPU9250_ADDRESS, CONFIG, 0x00);
	// Set sample rate = gyroscope output rate/(1 + SMPLRT_DIV)
	write(MPU9250_ADDRESS, SMPLRT_DIV, 0x00);

	write(MPU9250_ADDRESS, GYRO_CONFIG, GYRO_FULL_SCALE_2000_DPS);
	write(MPU9250_ADDRESS, ACCEL_CONFIG, ACC_FULL_SCALE_16_G);
	write(MPU9250_ADDRESS, INT_PIN_CFG, 0x22);

	write(AK8963_ADDRESS, AK8963_CNTL, 0x01);

	delay(100);

}

void loop() {

	int16_t accelData[3];
	readAccelData(accelData);
	int16_t gyroData[3];
	readGyroData(gyroData);
	int16_t magData[3];
	readMagData(magData);
	int16_t temp = readTempData();



	Serial.print("{\"accel\" : {\"x\" : ");
	Serial.print(accelData[0], DEC);
	Serial.print(", \"y\" : ");
	Serial.print(accelData[1], DEC);
	Serial.print(", \"z\" : ");
	Serial.print(accelData[2], DEC);
	Serial.print("}, \"gyro\" : {\"x\" : ");
	Serial.print(gyroData[0], DEC);
	Serial.print(", \"y\" : ");
	Serial.print(gyroData[1], DEC);
	Serial.print(", \"z\" : ");
	Serial.print(gyroData[2], DEC);
	Serial.print("}, \"mag\" : {\"x\" : ");
	Serial.print(magData[0], DEC);
	Serial.print(", \"y\" : ");
	Serial.print(magData[1], DEC);
	Serial.print(", \"z\" : ");
	Serial.print(magData[2], DEC);
	Serial.print("}, \"temp\": ");
	Serial.print(temp, DEC);
	Serial.println("}");

	//delay(100);
}

void write(uint8_t address, uint8_t registerAddress, uint8_t data) {
	Wire.beginTransmission(address);
	Wire.write(registerAddress);
	Wire.write(data);
	Wire.endTransmission();
}

uint8_t read(uint8_t address, uint8_t registerAddress) {
	uint8_t rowData[1];
	read(address, registerAddress, 1, &rowData[0]);
	return rowData[0];
}

void read(uint8_t address, uint8_t registerAddress, uint8_t count,
		uint8_t *destination) {

	Wire.beginTransmission(address);
	int hoge = 0;
	Wire.write(registerAddress);

	Wire.endTransmission();

	Wire.requestFrom(address, count);
	uint8_t index = 0;
	while (Wire.available()) {
		destination[index++] = Wire.read();
	}
}


void readAccelData(int16_t * destination) {
	uint8_t rawData[6];
	read(MPU9250_ADDRESS, ACCEL_XOUT_H, 6, &rawData[0]);

	destination[0] = (int16_t)(((int16_t) rawData[0] << 8) | rawData[1]);
	destination[1] = (int16_t)(((int16_t) rawData[2] << 8) | rawData[3]);
	destination[2] = (int16_t)(((int16_t) rawData[4] << 8) | rawData[5]);
}
//
void readGyroData(int16_t * destination) {
	uint8_t rawData[6];
	read(MPU9250_ADDRESS, GYRO_XOUT_H, 6, &rawData[0]);
	destination[0] = (int16_t)(((int16_t) rawData[0] << 8) | rawData[1]);
	destination[1] = (int16_t)(((int16_t) rawData[2] << 8) | rawData[3]);
	destination[2] = (int16_t)(((int16_t) rawData[4] << 8) | rawData[5]);
}

void readMagData(int16_t * destination) {
	if ((read(AK8963_ADDRESS, AK8963_ST1) & 0x01)) {
		uint8_t rawData[7];
		read(AK8963_ADDRESS, AK8963_XOUT_L, 7, &rawData[0]);
		uint8_t c = rawData[6];
		if (!(c & 0x08)) {
			destination[0] = (int16_t)(((int16_t) rawData[1] << 8) | rawData[0]);
			destination[1] = (int16_t)(((int16_t) rawData[3] << 8) | rawData[2]);
			destination[2] = (int16_t)(((int16_t) rawData[5] << 8) | rawData[4]);
		}
	}
}

int16_t readTempData() {
	uint8_t rawData[2];
	read(MPU9250_ADDRESS, TEMP_OUT_H, 2, &rawData[0]);

	return (((int16_t)(((int16_t) rawData[0]) << 8 | rawData[1]) - 0) / 333.87)
			+ 21;
}
