from subprocess import getoutput
from ipaddress import IPv4Address
from time import sleep

def check_connection(stop, s, lock, timeout: int): # Watchdog runner function
    while not stop.is_set():
        with lock:
            check = check_ssh()
            if not check:
                sleep(timeout)
                if not check_ssh():
                    s.append('abort')
                    break
            s.append('pass')

def check_ssh() -> bool: # Check SSH connection
    ssh_alive = getoutput('w -i | awk \'NR==3{print $3}\'')
    if ssh_alive.__contains__('fe80'):
        return True
    if ssh_alive.__contains__(':'):
        ssh_alive = ssh_alive.split(sep=':')[0]
    try:
        IPv4Address(ssh_alive)
    except ValueError:
        return False
    return True
