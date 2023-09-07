import pigpio # GPIO access
import threading as th # To run the console
from time import sleep # For delays
from os import _exit

from config import * # Import config, commands and Any type
from watchdog import check_connection # Check connection

class RepeatTimer(th.Timer):
    def run(self):
        while not self.finished.wait(self.interval):
            self.function(*self.args, **self.kwargs)

def exit_message(hard_abort: bool = False, before_cmd: bool = False):
    if Enable_Watchdog:
        stop_event.set()
        watchdog_thread.join()

    if not hard_abort and not before_cmd:
        Default_Pins( pi, PINS )

    pi.stop()
    print(SECTION_SEP)
    print('Byee!!')
    sleep(1)
    _exit(0)

def check_wd(s: list):
    got = ''
    if s.__len__() != 0 : got = s.pop()
    if got == 'abort':
        abort( pi, PINS )
        Default_Pins( pi, PINS )
        print('-->!!ABORTED!!\n')
        system('touch ~/PICode/ABORTED')
        exit_message(hard_abort=True)

clear()
print(MOTD)

print('Setting up RPI board...\n')
pi = pigpio.pi() # Set rpi board

for pin in PINS.values(): # Iterate through relay pins and make each an output
    pi.set_mode(pin, pigpio.OUTPUT)

print('All Set!')
print(SECTION_SEP)
print('Are these values correct?\n')

print(f' -  Watchdog Enable: {Enable_Watchdog}')
if Enable_Watchdog:
    print(f'        Watchdog Check Interval: {Watchdog_Check_Interval} seconds')
    print(f'        Watchdog Timeout: {Watchdog_Timout_Delay} seconds')

    print()

    print(f'  - GOX Enabled: {Enable_Gox}')
if Enable_Gox:
    print(f'        GOX Open Delay (After Ignition): {GOX_Open_Delay} seconds\n')
    print(f'  - Pins Close Delay (After GOX Opens): {Pins_Close_Delay} seconds')
else:
    print()
    print(f'  - Pins Close Delay (After Ignition): {Pins_Close_Delay} seconds')

confirm_config = input('\n[Y/n]: ')

if confirm_config == 'n':
    print('Please edit `config.py`')
    print(SECTION_SEP)
    exit_message(before_cmd=True)

print(SECTION_SEP)

stop_event: th.Event = th.Event() # Stop Event handler
watchdog_stack: list = []
watchdog_lock: th.Lock = th.Lock()
watchdog_thread = th.Thread(target=check_connection, args=(stop_event, watchdog_stack, watchdog_lock, Watchdog_Timout_Delay)) # Instantiate watchdog thread

if Enable_Watchdog:
    watchdog_thread.start()
    print("Started Watchdog Timer...")

print(SECTION_SEP)
Default_Pins( pi, PINS )
print(SECTION_SEP)
print('Launching Console...')
sleep(1)
clear()
print(MOTD)

try:
    while not stop_event.is_set(): # Main Loop

        rt = RepeatTimer(Watchdog_Check_Interval, check_wd, (watchdog_stack,))
        rt.start()

        cmd: str = input('> ').lower()
        rt.cancel()

        if cmd in ['quit', 'q', 'exit']: # If quit/q
            break

        if cmd in ['?', 'help']: # If help/?
            list_commands(COMMANDS.keys())
            continue

        if cmd.__contains__('gox') and not Enable_Gox:
            print('\n--> You do not have GOX enabled, please close the program edit `config.py` to enable it\n')
            continue

        if not Get_State('ArmingTrigger') and ( cmd in ['open gox valve', 'start ignition', 'auto ignition'] ): # If not armed
            print('\n--> IGNITION IS NOT ARMED\n')
            continue

        action: Any = unknown_command # Default to unknown
        for com in list(COMMANDS.keys()):
            if com[0] == cmd:
                action: Any = COMMANDS.get(com) # If command exists in list
                break

        action( pi, PINS )

        if action == clear: # Clear with motd
            print(MOTD)

        if cmd == 'auto ignition': # Auto Ignition Sequence
            ignition( pi, PINS )
            if Enable_Gox:
                print(f'Wating for {GOX_Open_Delay} seconds to open GOX...')
                sleep( GOX_Open_Delay )
                open_gox( pi, PINS )
            print(f'Waiting for {Pins_Close_Delay} seconds to close pins...')
            sleep( Pins_Close_Delay )
            print(SECTION_SEP)
            Default_Pins( pi, PINS )
            print(SECTION_SEP)
            print('--> Auto Ignition Sequence Completed\n')

        if cmd.__contains__('abort'): # All Abort Sequences
            Default_Pins( pi, PINS )
            print('-->!!ABORTED!!\n')

            if not cmd.__contains__('soft'): # If NOT 'Soft Abort' Sequence
                system('touch ~/PICode/ABORTED')
                exit_message(hard_abort=True)
                break

except Exception:
    sleep(0)
exit_message()