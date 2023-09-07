#pragma once

#define PinInputVoltage A9
#define MINIMUM_INPUT_VOLTAGE 3.4f
#define MAXIMUM_INPUT_VOLTAGE 5.0f

#define PinVBAT 49

#define BMP_SCL A5
#define BMP_SDA A4
#define SEALEVELPRESSURE_HPA ( 1013.25f )

#define MPU 0x68 // MPU6050 I2C address
#define AccelRange 16
#define GyroRange 2000

#define PinDrogue 39
#define PinMain 33

#define InitValueDelay 2 // In seconds

#define ConsoleDelay 1 // In seconds

#define ExitSetup 20 // In seconds

#define SurfaceAltBias 10 //In meters
