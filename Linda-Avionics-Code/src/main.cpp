/**
 *
 * @file main.cpp
 * @author UTSA ARC Avionics Team 2023
 * @brief The Avionics code for the Spaceport 2023 vehicle
 * @version 1.0.0
 * @date 2022-2023
 *
 */

#include "setup.h"
#include "functions.h"
#include "samples.h"

int apogee = 0;
int SurfaceAlt = 0;
bool landed = false;

void setup() {
    // Find hexadecimal representation of accelerometer range based on decimal global variable AccelRange defined above //
    // Find decimal representation of LSB Sensitivity based on decimal global variable AccelRange defined above //

    Serial.begin( 115200 ); //Changed to higher rate 4/21/22

    while ( !Serial ); //! DELETE WHEN PUTTING IN ROCKET

    // ----------------------------------------------------------------

    // Builtin SD Card Initialization
    Serial.print( "Initializing SD card..." );

    while ( !SD.begin( BUILTIN_SDCARD ) ) {

        Serial.println( "Card not readable" );
        delay( 2000 );

    }

    Serial.println( "SD card is ready!" );

    // ----------------------------------------------------------------

    Serial.println( "Initializing BMP3..." );
    while ( !bmp.begin_I2C() ) { // hardware I2C mode, can pass in address & alt Wire

        Serial.println( "Could not find a valid BMP3 sensor, check wiring!\n" );
        delay( 2000 );

    }

    Serial.println( "Found and initialized a valid BMP3 I2C sensor!" );

    // ----------------------------------------------------------------

    // Serial.println( "Initializing MPU6050..." );

    // Wire.beginTransmission( MPU );
    // while ( ( Wire.endTransmission() != 0 ) ) {

    //     Serial.println( "Could not find MPU\n" );
    //     delay( 2000 );

    // }

    // Serial.println( "Initialized MPU6050! ");

    // ----------------------------------------------------------------

    // Set up oversampling and filter initialization
    bmp.setTemperatureOversampling( BMP3_OVERSAMPLING_8X );
    bmp.setPressureOversampling( BMP3_OVERSAMPLING_4X );
    bmp.setIIRFilterCoeff( BMP3_IIR_FILTER_COEFF_3 );
    bmp.setOutputDataRate( BMP3_ODR_50_HZ );

    // ----------------------------------------------------------------

    // Set Ranges
    // while ( Set_Accel_Range( AccelRange ) != 0 ) {

    //     Serial.println( "Please Fix Accel Range" );
    //     delay( 2000 );

    //     }

    // while ( Set_Gyro_Range( GyroRange ) != 0 ) {

    //     Serial.println( "Please Fix Gyro Range" );
    //     delay( 2000 );

    // }

    // ----------------------------------------------------------------

    // Init_MPU();            // Initialize MPU

    // Configure_MPU( 0x1C ); // Config Register

    // Configure_Gyro( 0x1B ); // Config Register

    // ----------------------------------------------------------------

    Init_CSV(); // Initialize CSV

    // ----------------------------------------------------------------

    Data init_values = Get_All_Values(); // Set Initial Values
    delay( InitValueDelay * 1000 );      // Delay to compare data
    SurfaceAlt = init_values.altitude;   // Get surface altitude (Assuming Setup() will be called on surface ONLY)

    Data values = Get_All_Values(); // Get Current Values
    Result Check_Systems_Result = Check_Systems( values, init_values ); // Check Health of Systems
    while ( Check_Systems_Result.error != 0 ) { // While Systems Bad

        Serial.println( Check_Systems_Result.message ); // Print result

        init_values = values;      // Re-initialize values
        values = Get_All_Values(); // ' '

        Check_Systems_Result = Check_Systems( values, init_values ); // Re-Check result

    }

    // ----------------------------------------------------------------

    Serial.end(); // End Serial Transmission

    delay( ExitSetup * 1000 ); // Delay for N seconds before starting main loop

}

void loop() {

    if ( landed ) return; // Do nothing if landed

    SampleCollection Samples; // Get all data values

    Sample* sample_arr = Samples.Get_Sample_Array(); // Get Sample Array
    const int sample_size = Samples.Size(); // Get Sample Array Size

    int sample_movement[ sample_size - 1 ]; // Init Comparison array

    for ( int i = 1; i < sample_size; i++ ) { sample_movement[ i - 1 ] = Samples.Compare_Sample( ( i - 1 ), i ).error; } // Find movement of samples

    if ( !Paras_Armed[ 0 ] ) { // If Drogue is not armed

        for ( int i = 0; i < sample_size; i++ ) { // Iterate through all samples

            Result alt_result = Check_Altitude( sample_arr[ i ].Get_Avg_Data().altitude ); // Check if at Safe Arming Altitude
            sample_arr[ i ].Append_Message( ( alt_result.message + ',' ) ); // Record Result

            if ( alt_result.error == 0 ) { Arm_Parachute( 0 ); Arm_Parachute( 1 ); break; } // If safe, arm Paras

        }

    }

    if ( Paras_Armed[ 1 ] && apogee == 0 ) { // If Drogue Para is armed and apogee is not set

        int i = 0;
        while ( sample_movement[ i ] > 0 && i < ( sample_size - 2 ) ) i++; // Iterate through Comparison array until reading 0 or -1

        if ( sample_movement[ i + 1 ] < 0 ) { // If next index is -1

            Result launch_result = Launch_Parachute( 1 ); // Launch Drogue
            sample_arr[ i + 1 ].Append_Message( ( launch_result.message + ',' ) ); // Record result

        }

        apogee = sample_arr[ i + 1 ].Get_Avg_Data().altitude; // Set apogee

    }

    if ( apogee > 0 ) { // If apogee is set

        if ( Paras_Armed[ 0 ] ) { // If Main Para is armed

            for ( int i = 0; i < sample_size; i++ ) { // Iterate through Samples

                Result alt_result = Check_Main_Para( sample_arr[ i ].Get_Avg_Data().altitude ); // Check if at Main Para Altitude
                if ( alt_result.error == 1 ) { // If at Main Para Altitude

                    Launch_Parachute( 0 ); // Launch Main Para
                    sample_arr[ i ].Append_Message( ( alt_result.message + ',' ) ); // Record Result
                    break;

                }

            }

        }

        else { // If Main Para is not armed

            for ( int i = 0; i < sample_size; i++ ) { // Iterate through all Samples
                
                if ( sample_arr[ i ].Get_Avg_Data().altitude <= ( SurfaceAlt + SurfaceAltBias ) ) { // Check if sample is landed
                    
                    landed = true; // Set as true
                    sample_arr[ i ].Append_Message( "!!LANDED VEHICLE WOOOOOO!!" ); // Record Landing
                    break;

                }

            }

        }

    }

    for ( int i = 0; i < sample_size; i++ ) Record_Data( &sample_arr->Get_Avg_Data() ); // Print & Save All Values

    delay( ConsoleDelay * 1000 ); //! FOR JUST EASY READING

}

