#include "params/sample_params.h"
#include "globals.h"
#include "functions.h"

class Sample { // An Average of Measurements

    private:

        Data measurements[ MeasurementAmount ]; // An array of measurements
        SampleData avg_data; // Avg of measurements

        void Set_Measurements( const Data* new_measurements ) { *measurements = *new_measurements; } // Set measurements

        void Find_Avg() { // Find the average

            avg_data.message = "";
            avg_data.time = measurements[0].time;
            avg_data.timeEnd = measurements[MeasurementAmount -  1].time;

            for ( int i = 0; i < MeasurementAmount; i++ ) {

                avg_data.raw_accel[0] += ( measurements[i].raw_accel[0] * ( 1.0f / MeasurementAmount ) );
                avg_data.raw_accel[1] += ( measurements[i].raw_accel[1] * ( 1.0f / MeasurementAmount ) );
                avg_data.raw_accel[2] += ( measurements[i].raw_accel[2] * ( 1.0f / MeasurementAmount ) );

                avg_data.raw_gyro[0] += ( measurements[i].raw_gyro[0] * ( 1.0f / MeasurementAmount ) );
                avg_data.raw_gyro[1] += ( measurements[i].raw_gyro[1] * ( 1.0f / MeasurementAmount ) );
                avg_data.raw_gyro[2] += ( measurements[i].raw_gyro[2] * ( 1.0f / MeasurementAmount ) );

                avg_data.normalized_accel[0] += ( measurements[i].normalized_accel[0] * ( 1.0f / MeasurementAmount ) );
                avg_data.normalized_accel[1] += ( measurements[i].normalized_accel[1] * ( 1.0f / MeasurementAmount ) );
                avg_data.normalized_accel[2] += ( measurements[i].normalized_accel[2] * ( 1.0f / MeasurementAmount ) );

                avg_data.normalized_gyro[0] += ( measurements[i].normalized_gyro[0] * ( 1.0f / MeasurementAmount ) );
                avg_data.normalized_gyro[1] += ( measurements[i].normalized_gyro[1] * ( 1.0f / MeasurementAmount ) );
                avg_data.normalized_gyro[2] += ( measurements[i].normalized_gyro[2] * ( 1.0f / MeasurementAmount ) );

                avg_data.altitude += ( measurements[i].altitude * ( 1.0f / MeasurementAmount ) );

                avg_data.pressure += ( measurements[i].pressure * ( 1.0f / MeasurementAmount ) );

                avg_data.temperature += ( measurements[i].temperature * ( 1.0f / MeasurementAmount ) );

                avg_data.message += measurements[i].message;

            }

        }

    public:

        Sample() { // Initialize Sample object

            for ( int i = 0; i < MeasurementAmount; i++ ) {

                measurements[i] = Get_All_Values();
                delay( ( SampleTimeSpan * 1000 ) / MeasurementAmount );

            }

            Find_Avg();

        }

        Sample( Data* new_measurements ) { // Initialize Sample object with provided measurements

            Set_Measurements( new_measurements );

            Find_Avg();

        }

        Data* Get_Measurements() { return measurements; } // Get measurements array

        SampleData Get_Avg_Data() { return avg_data; } // Get Avg_data

        void Append_Message( const String message ) { avg_data.message += message;  } // Append to message string of Sample object

};

class SampleCollection { // A collection of sample objects

    private:
        Sample* Samples; // Array of Samples
        int size; // Size of array

        Result Compare_Altitude( const float altitude_a, const float altitude_b ) { // Compare Altitudes of 2 Samples

            const float H = altitude_a * ( 1 + SampleAltTolerance );
            const float L = altitude_a * ( 1 - SampleAltTolerance );

            if ( altitude_b > H ) return { 1, "Altitude A Is Less Than Altitude B" };

            if ( altitude_b < L ) return { -1, "Altitude A Is Greater Than Altitude B" };

            return { 0, "Altitude A Is Equal to Altitude B" };

        }

        Result Compare_Pressure( const float pressure_a, const float pressure_b ) { // Compare Pressures of 2 Samples

            const float H = pressure_a * ( 1 + SamplePressureTolerance );
            const float L = pressure_a * ( 1 - SamplePressureTolerance );

            if ( pressure_b > H ) return { -1, "Pressure A is Less than Pressure B" };

            if ( pressure_b < L ) return { 1, "Pressure A is Greater than Pressure B" };

            return { 0, "Pressure A is Equal to Pressure B" };

        }

        Result Compare_Temperature( const float temperature_a, const float temperature_b ) { // Compare Temperatures of 2 Samples

            const float H = temperature_a * ( 1 + SampleTemperatureTolerance );
            const float L = temperature_b * ( 1 - SampleTemperatureTolerance );

            if ( temperature_b > H ) return { -1, "Temperature A is Less than Temperature B" };

            if ( temperature_b < L ) return { 1, "Temperature A is Greater than Temperature B" };

            return { 0, "Temperature A is Equal to Temperature B" };

        }

        Result Compare_Raw_Accel( const int* raw_accel_a, const int* raw_accel_b ) { // Compare the Raw Acceleration of 2 Samples

            const float H[ 3 ] = {

                abs( raw_accel_a[0] * ( 1 + SampleAccelTolerance ) ),
                abs( raw_accel_a[1] * ( 1 + SampleAccelTolerance ) ),
                abs( raw_accel_a[2] * ( 1 + SampleAccelTolerance ) )

            };

            const float L[ 3 ] = {

                abs( raw_accel_a[0] * ( 1 - SampleAccelTolerance ) ),
                abs( raw_accel_a[1] * ( 1 - SampleAccelTolerance ) ),
                abs( raw_accel_a[2] * ( 1 - SampleAccelTolerance ) )

            };

            bool X, Y, Z;
            X = Y = Z = false;

            if ( abs( raw_accel_b[0] ) > L[0] && abs( raw_accel_b[0] ) < H[0] ) X = true;

            if ( abs( raw_accel_b[1] ) > L[1] && abs( raw_accel_b[1] ) < H[1] ) Y = true;

            if ( abs( raw_accel_b[2] ) > L[2] && abs( raw_accel_b[2] ) < H[2] ) Z = true;

            if ( !X && Y && Z ) return { 1, "Y and Z raw Accel Axis are Equal" };
            if ( X && !Y && Z ) return { 2, "X and Z raw Accel Axis are Equal" };
            if ( X && Y && !Z ) return { 3, "X and Y raw Accel Axis are Equal" };
            if ( X && !Y && !Z ) return { 4, "Only X raw Accel Axis is Equal" };
            if ( !X && Y && !Z ) return { 5, "Only Y raw Accel Axis is Equal" };
            if ( !X && !Y && Z ) return { 6, "Only Z raw Accel Axis is Equal" };
            if ( !X && !Y && !Z ) return { -1, "No raw Accel Axis are Equal" };

            return { 0, "All raw Accel Axis are Equal " };

        }

        Result Compare_Normalized_Accel( const float* accel_a, const float* accel_b ) { // Compare the Normalized Acceleration of 2 Samples

            const float H[ 3 ] = {

                abs( accel_a[0] * ( 1 + SampleAccelTolerance ) ),
                abs( accel_a[1] * ( 1 + SampleAccelTolerance ) ),
                abs( accel_a[2] * ( 1 + SampleAccelTolerance ) )

            };

            const float L[ 3 ] = {

                abs( accel_a[0] * ( 1 - SampleAccelTolerance ) ),
                abs( accel_a[1] * ( 1 - SampleAccelTolerance ) ),
                abs( accel_a[2] * ( 1 - SampleAccelTolerance ) )

            };

            bool X, Y, Z;
            X = Y = Z = false;

            if ( abs( accel_b[0] ) > L[0] && abs( accel_b[0] ) < H[0] ) X = true;

            if ( abs( accel_b[1] ) > L[1] && abs( accel_b[1] ) < H[1] ) Y = true;

            if ( abs( accel_b[2] ) > L[2] && abs( accel_b[2] ) < H[2] ) Z = true;

            if ( !X && Y && Z ) return { 1, "Y and Z Accel Axis are Equal" };
            if ( X && !Y && Z ) return { 2, "X and Z Accel Axis are Equal" };
            if ( X && Y && !Z ) return { 3, "X and Y Accel Axis are Equal" };
            if ( X && !Y && !Z ) return { 4, "Only X Accel Axis is Equal" };
            if ( !X && Y && !Z ) return { 5, "Only Y Accel Axis is Equal" };
            if ( !X && !Y && Z ) return { 6, "Only Z Accel Axis is Equal" };
            if ( !X && !Y && !Z ) return { -1, "No Accel Axis are Equal" };

            return { 0, "All Accel Axis are Equal " };

        }

        Result Compare_Raw_Tilt( const int* raw_gyro_a, const int* raw_gyro_b ) { // Compare the Raw Tilt of 2 Samples

            const float H[ 3 ] = {

                ( raw_gyro_a[0] * ( 1 + SampleTiltTolerance ) ),
                ( raw_gyro_a[1] * ( 1 + SampleTiltTolerance ) ),
                ( raw_gyro_a[2] * ( 1 + SampleTiltTolerance ) )

            };

            const float L[ 3 ] = {

                ( raw_gyro_a[0] * ( 1 - SampleTiltTolerance ) ),
                ( raw_gyro_a[1] * ( 1 - SampleTiltTolerance ) ),
                ( raw_gyro_a[2] * ( 1 - SampleTiltTolerance ) )

            };

            bool X, Y, Z;
            X = Y = Z = false;

            if ( raw_gyro_b[0] > L[0] && raw_gyro_b[0] < H[0] ) X = true;

            if ( raw_gyro_b[1] > L[1] && raw_gyro_b[1] < H[1] ) Y = true;

            if ( raw_gyro_b[2] > L[2] && raw_gyro_b[2] < H[2] ) Z = true;

            if ( !X && Y && Z ) return { 1, "Y and Z Raw Gyro Axis are Equal" };
            if ( X && !Y && Z ) return { 2, "X and Z Raw Gyro Axis are Equal" };
            if ( X && Y && !Z ) return { 3, "X and Y Raw Gyro Axis are Equal" };
            if ( X && !Y && !Z ) return { 4, "Only X Raw Gyro Axis is Equal" };
            if ( !X && Y && !Z ) return { 5, "Only Y Raw Gyro Axis is Equal" };
            if ( !X && !Y && Z ) return { 6, "Only Z Raw Gyro Axis is Equal" };
            if ( !X && !Y && !Z ) return { -1, "No Raw Gyro Axis are Equal" };

            return { 0, "All Raw Gyro Axis are Equal " };

        }

        Result Compare_Normalized_Tilt( const float* gyro_a, const float* gyro_b ) { // Compare the Normalized Tilt of 2 Samples

            const float H[ 3 ] = {

                abs( gyro_a[0] * ( 1 + SampleTiltTolerance ) ),
                abs( gyro_a[1] * ( 1 + SampleTiltTolerance ) ),
                abs( gyro_a[2] * ( 1 + SampleTiltTolerance ) )

            };

            const float L[ 3 ] = {

                abs( gyro_a[0] * ( 1 - SampleTiltTolerance ) ),
                abs( gyro_a[1] * ( 1 - SampleTiltTolerance ) ),
                abs( gyro_a[2] * ( 1 - SampleTiltTolerance ) )

            };

            bool X, Y, Z;
            X = Y = Z = false;

            if ( abs( gyro_b[0] ) > L[0] && abs( gyro_b[0] ) < H[0] ) X = true;

            if ( abs( gyro_b[1] ) > L[1] && abs( gyro_b[1] ) < H[1] ) Y = true;

            if ( abs( gyro_b[2] ) > L[2] && abs( gyro_b[2] ) < H[2] ) Z = true;

            if ( !X && Y && Z ) return { 1, "Y and Z Gyro Axis are Equal" };
            if ( X && !Y && Z ) return { 2, "X and Z Gyro Axis are Equal" };
            if ( X && Y && !Z ) return { 3, "X and Y Gyro Axis are Equal" };
            if ( X && !Y && !Z ) return { 4, "Only X Gyro Axis is Equal" };
            if ( !X && Y && !Z ) return { 5, "Only Y Gyro Axis is Equal" };
            if ( !X && !Y && Z ) return { 6, "Only Z Gyro Axis is Equal" };
            if ( !X && !Y && !Z ) return { -1, "No Gyro Axis are Equal" };

            return { 0, "All Gyro Axis are Equal " };

        }

    public:

        SampleCollection() { // Initialize the Collection object

            Samples = new Sample[ SampleAmount ];
            size = SampleAmount;

        }

        SampleCollection( const int sample_amount ) { // Initialize the Collection object with a given amount

            Samples = new Sample[ sample_amount ];
            size = sample_amount;

        }

        SampleCollection( const SampleCollection &sc ) { // Copy Constructor

            Samples = new Sample[ SampleAmount ];
            *Samples = *sc.Samples;
            size = sc.size;

        }

        Sample* Get_Sample_Array() { return Samples; } // Get Sample Array

        int Size() { return size; } // Get the size of the Sample Array

        Sample Get_Sample( const int index ) { return Samples[ index ]; } // Get a specified Sample

        Result Record_Samples() { // Initialize Samples

            for ( int i = 0; i < size; i++ ) Samples[i] = Sample();

            return { 0, "Recorded Samples" };

        }

        Result Compare_Sample( const int sample_a_index, const int sample_b_index ) { // Compare 2 samples //* Return 0 if equal, 1 if greater, -1 if lesser

            int eq = 0;

            Result C_Alt = Compare_Altitude( Samples[sample_a_index].Get_Avg_Data().altitude, Samples[sample_b_index].Get_Avg_Data().altitude );

            Result C_Pres = Compare_Pressure( Samples[sample_a_index].Get_Avg_Data().pressure, Samples[sample_b_index].Get_Avg_Data().pressure );

            Result C_Temp = Compare_Temperature( Samples[sample_a_index].Get_Avg_Data().temperature, Samples[sample_b_index].Get_Avg_Data().temperature );

            Result C_R_Accel = Compare_Raw_Accel( Samples[sample_a_index].Get_Avg_Data().raw_accel, Samples[sample_b_index].Get_Avg_Data().raw_accel );

            Result C_N_Accel = Compare_Normalized_Accel( Samples[sample_a_index].Get_Avg_Data().normalized_accel, Samples[sample_b_index].Get_Avg_Data().normalized_accel );

            Result C_R_Tilt = Compare_Raw_Tilt( Samples[sample_a_index].Get_Avg_Data().raw_gyro, Samples[sample_b_index].Get_Avg_Data().raw_gyro );

            Result C_N_Tilt = Compare_Normalized_Tilt( Samples[sample_a_index].Get_Avg_Data().normalized_gyro, Samples[sample_b_index].Get_Avg_Data().normalized_gyro );

            String msg = C_Alt.message + C_Pres.message + C_Temp.message + C_R_Accel.message + C_N_Accel.message + C_R_Tilt.message + C_N_Tilt.message;


            if ( C_Alt.error > 0 ) eq += AltEqualityWeight;
            else if ( C_Alt.error < 0 ) eq -= AltEqualityWeight;

            if ( C_Pres.error > 0 ) eq += PressureEqualityWeight;
            else if ( C_Pres.error < 0 ) eq -= PressureEqualityWeight;

            if ( C_Temp.error > 0 ) eq += TemperatureEqualityWeight;
            else if ( C_Temp.error < 0 ) eq -= TemperatureEqualityWeight;

            if ( C_R_Accel.error > 0 ) eq +=  RawAccelEqualityWeight;
            else if ( C_R_Accel.error < 0 ) eq -= RawAccelEqualityWeight;

            if ( C_N_Accel.error > 0 ) eq += NormalizedAccelEqualityWeight;
            else if ( C_N_Accel.error < 0 ) eq -= NormalizedAccelEqualityWeight;

            if ( C_R_Tilt.error > 0 ) eq += RawGyroEqualityWeight;
            else if ( C_R_Tilt.error < 0 ) eq -= RawGyroEqualityWeight;

            if ( C_N_Tilt.error > 0 ) eq += NormalizedGyroEqualityWeight;
            else if ( C_N_Tilt.error < 0 ) eq -= NormalizedGyroEqualityWeight;

            if ( eq > EqualitySumMax ) return { 1, msg };
            if ( eq < EqualitySumMin ) return { -1, msg };

            return { 0, msg };

        }

};
