#define SampleAmount 3 // Amount of samples to analyze at a time
#define MeasurementAmount 20 // Amount of measurements in 1 sample
#define SampleTimeSpan 1 // In Seconds

#define SampleAltTolerance 0.05f // % in decimal form
#define SamplePressureTolerance 0.05f // % in decimal form
#define SampleAccelTolerance 0.05f // % in decimal form
#define SampleTiltTolerance 0.05f // % in decimal form
#define SampleTemperatureTolerance 0.05f // % in decimal form

#define AltEqualityWeight 2
#define PressureEqualityWeight 1
#define TemperatureEqualityWeight 1
#define RawAccelEqualityWeight 0
#define NormalizedAccelEqualityWeight 0
#define RawGyroEqualityWeight 0
#define NormalizedGyroEqualityWeight 0

#define EqualitySumMin -2 // What the weights should sum to, to be considered equal
#define EqualitySumMax 2 // What the weights should sum under, to be considered equal
