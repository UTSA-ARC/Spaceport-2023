from typing import Union
from os import system
from readline import set_pre_input_hook

states: dict[str, bool] = { # For pin states

    'IgnitionTrigger': False,
    'GOXValve': False,
    'ArmingTrigger': False,
    'VentValve': False,
    'BottleValve': False,
    'TankValve': False,

    }

def Get_State( key: str ) -> Union[ bool, None ]: return states[key] # Get pin state

def Default_Pins( pi, pins ) -> None: # Defaults all pins

    print("Defaulting pins...\n")

    stop_ignition( pi, pins )
    close_gox( pi, pins )
    disarm_ignition( pi, pins )
    open_vent( pi, pins )
    close_bottle_valve( pi, pins )
    close_tank_valve( pi, pins )

    print("...Defaulted Pins\n")

def clear( pi=None, pins=None ) -> None: # Clear Console
    system('clear')

def list_commands( commands ) -> None: # List out all commands
    print('--> Here is a list of all the registered commands: \n')
    for key in commands:
        print( f'{key[0]} - {key[1]}\n' )

def open_bottle_valve( pi, pins ) -> None: # Trigger to Open Bottle Relay
    print('--> Opening Bottle Valve...\n')
    pi.write( pins['BottleValvePin'], True )
    states['BottleValve'] = True
    print('--> Bottle Valve Opened\n')

def close_bottle_valve( pi, pins ) -> None: # Trigger to Open Bottle Relay
    print('--> Stopping Bottle valve...\n')
    pi.write( pins['BottleValvePin'], False )
    states['BottleValve'] = False
    print('--> Bottle Valve Closed\n')

def open_tank_valve( pi, pins ) -> None: # Trigger Open Tank Relay
    print('--> Opened Tank Valve...\n')
    pi.write( pins['TankValvePin'], True )
    states['TankValve'] = True
    print('--> Fill Tank Pin Opened\n')

def close_tank_valve( pi, pins ) -> None: # Trigger Open Tank Relay
    print('--> Closed Tank Valve...\n')
    pi.write( pins['TankValvePin'], False )
    states['TankValve'] = False
    print('--> Tank Valve Closed\n')

def close_vent( pi, pins ) -> None: # Trigger to Close Vent Relay
    print('--> Closing Vent...\n')
    pi.write( pins['VentValvePin'], True )
    states['VentValve'] = True
    print('--> Vent Closed\n')

def open_vent( pi, pins ) -> None: # Trigger to Open Vent Relay
    print('--> Opening Vent...\n')
    pi.write( pins['VentValvePin'], False )
    states['VentValve'] = False
    print('--> Vent Opened\n')


def open_gox( pi, pins ) -> None: # Trigger to Open GOX Relay
    print('--> Flowing GOX...\n')
    pi.write( pins['GOXValvePin'], True )
    states['GOXValve'] = True
    print('--> GOX Opened\n')

def close_gox( pi, pins ) -> None: # Trigger to Close GOX relay
    print('--> Closing GOX...\n')
    pi.write( pins['GOXValvePin'], False )
    states['GOXValve'] = False
    print('--> GOX Closed\n')

def ignition( pi, pins ) -> None: # Trigger to Start Ignition Relay
    print('--> Ignition!...\n')
    pi.write( pins['IgnitionPin'], True )
    states['IgnitionTrigger'] = True
    print('--> !!Ignition!!\n')

def stop_ignition( pi, pins ) -> None: # Trigger to Stop Ignition Relay
    print('--> Stopping Ignition...\n')
    pi.write( pins['IgnitionPin'], False )
    states['IgnitionTrigger'] = False
    print('--> Ignition Stopped\n')

def arm_ignition( pi, pins ) -> None: # Arm Ignition Sequence
    print('--> Arming Ignition Sequence...\n')
    pi.write( pins['ArmingPin'], True )
    states['ArmingTrigger'] = True
    print('--> Ignition Sequence Armed\n')

def disarm_ignition( pi, pins ) -> None: # Arm Ignition Sequence
    print('--> Disarming Ignition Sequence...\n')
    pi.write( pins['ArmingPin'], False )
    states['ArmingTrigger'] = False
    stop_ignition( pi, pins )
    close_gox( pi, pins)
    print('--> Ignition Sequence Disarmed\n')

def auto_ignition( pi, pins ) -> None: # Auto Ignition Sequence
    print('--> Auto Ignition Sequence Initiated!...\n')

def abort( pi, pins ) -> None: # Abort Sequence
    print('--> !!ABORTING!!...\n')

def get_pins( pi, pins ) -> None: # Get Pins
    print('--> Getting Pins...\n')
    print(pins)
    print('\n')

def get_pin_states( pi, pins ) -> None: # Get Pin States
    print('--> Getting Pin States...\n')
    print(states)
    print('\n')

def check_armed( pi, pins ) -> None: # Check if Armed
    print('--> Checking If Armed...\n')
    print(Get_State('ArmingTrigger'))
    print('\n')

def unknown_command( pi, pins ): # If no valid command
    print('--> Unknown command\n')
