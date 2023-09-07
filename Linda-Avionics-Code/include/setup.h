#include <SD.h>
#include <SPI.h>

#include "params/setup_params.h"
#include "globals.h"

bool Paras_Armed[2];
byte AFS_SEL, GFS_SEL;
unsigned int ALSB_Sensitivity, GLSB_Sensitivity;

int Set_Accel_Range( byte range ) { // Range and sensitivity of accelerometer

    switch ( AccelRange ) {

        case 2:                                     // 2Gs of range
            AFS_SEL = 0x00;
            ALSB_Sensitivity = 16384;
        break;

        case 4:                                    // 4Gs of range
            AFS_SEL = 0x08;
            ALSB_Sensitivity = 8192;
        break;

        case 8:                                   // 8Gs of range
            AFS_SEL = 0x10;
            ALSB_Sensitivity = 4096;
        break;

        case 16:                                  // 16Gs of range
            AFS_SEL = 0x18;
            ALSB_Sensitivity = 2048;
        break;

        default:
            Serial.println( "Must input 2, 4, 8, or 16Gs for MPU6050 accelerometer range! Exiting..." );
            return 1;
    }

    return 0;

}

int Set_Gyro_Range( unsigned int range ) { // Range and sensitivity of gyroscope

    switch ( GyroRange ) {

    case 250:                                    // 250 deg/s of range
            GFS_SEL = 0x00;
            GLSB_Sensitivity = 131;
            break;

        case 500:                                // 500 deg/s of range
            GFS_SEL = 0x08;
            GLSB_Sensitivity = 65.5;
            break;

        case 1000:                               // 1000 deg/s of range
            GFS_SEL = 0x10;
            GLSB_Sensitivity = 32.8;
            break;

        case 2000:                               // 2000 deg/s of range
            GFS_SEL = 0x18;
            GLSB_Sensitivity = 16.4;
            break;

        default:
            Serial.println( "Must input 250, 500, 1000, or 2000 deg/s for MPU6050 gyroscope range! Exiting..." );
            return 1;
    }

    return 0;

}

Result Check_Input_Voltage( int input_voltage ) { // Check if Input voltage is valid

    float real_input_voltage = input_voltage / 1023.0;

    if ( real_input_voltage < MINIMUM_INPUT_VOLTAGE ) return { -1, "!!INPUT VOLTAGE LESS THAN MINIMUM!!" };
    if ( real_input_voltage > MAXIMUM_INPUT_VOLTAGE ) return { -2, "!!INPUT VOLTAGE MORE THAN MAXIMUM!!" };

    return { 0, "Safe Input Voltage" };

}

Result Check_VBAT_Connection() { // Check if VBAT is connected

    if ( digitalRead( PinVBAT ) ) return { 0, "VBAT Connected"};

    return { -1, "!!VBAT NOT CONNECTED!!"};

}

void Init_Paras() { // Initialize Parachutes

    pinMode( PinMain, OUTPUT );
    pinMode( PinDrogue, OUTPUT );

}

void Init_MPU() { // Initialize MPU

    Wire.begin();                              // Initialize communication
    Wire.beginTransmission( MPU );             // Start communication with MPU6050 // MPU=0x68
    Wire.write( 0x6B );                        // Talk to the register 6B
    Wire.write( 0x00 );                        // Make reset - place a 0 into the 6B register
    Wire.endTransmission( true );              // End the transmission

}

void Init_CSV() { // Initialize CSV format

    String file_string = String( month() + '-' + day() + '-' + year() ) + ".csv";
    File myFile = SD.open( file_string.c_str(), FILE_WRITE | O_TRUNC );
    myFile.println(

        "Time ( RTC ),TimeEnd ( RTC ),Raw Ax ( g ),Raw Ay ( g ),Raw Az ( g ),Ax ( g ),Ay ( g ),Az ( g ),Raw Gx ( deg/s ),Raw Gy ( deg/s ),Raw Gz ( deg/s ),Gx ( deg/s ),Gy ( deg/s ),Gz ( deg/s ),Temperature ( *C ),Pressure ( kpA ),Altitude ( m ),Message"

    );

    myFile.close();

}

void Configure_MPU( int ACCEL_CONFIG ) { // Configure Accelerometer Sensitivity - Full Scale Range ( default +/- 2g )

    Wire.beginTransmission( MPU );
    Wire.write( ACCEL_CONFIG );                //Talk to the ACCEL_CONFIG register ( 1C hex )
    Wire.write( AFS_SEL );                     //Set the register bits as 00010000 ( +/- 8g full scale range )
    Wire.endTransmission( true );

}

void Configure_Gyro( int GYRO_CONFIG ) { // Configure Gyro Sensitivity - Full Scale Range ( default +/- 250deg/s )

    Wire.beginTransmission( MPU );
    Wire.write( GYRO_CONFIG );                 // Talk to the GYRO_CONFIG register ( 1B hex )
    Wire.write( GFS_SEL );                     // Set the register bits as 00010000 ( 1000deg/s full scale )
    Wire.endTransmission( true );

}