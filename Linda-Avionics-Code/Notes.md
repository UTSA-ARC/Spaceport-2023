Rocket will be sideways at apogee most likely

Will need around 3-4 samples to check across to make sure of apogee
Each sample will be and average across 20 ( set as param ) checks of each sensor
Each sample should contain the average of ~1 second of time

Velocity will still carry, even when motor is at apogee
Theoretically, time - timeDelta && time + timeDelta should read same velocity values ( just signed differently ) for a short window of time

Unit tests eventually
Fuzzy tests eventually

"If you're testing the happy path, you're doing it wrong"
aH
