#!/bin/bash

echo "Checking if pigpiod is active..."
pigpiod_status=$(systemctl is-active pigpiod)
echo "Pigpiod status: $pigpiod_status"
if [[ $pigpiod_status != 'active' ]]
then

echo "Starting pigpiod..."
sudo systemctl start pigpiod
echo "...Started pigpiod"

fi
echo "You're good to go!"

sleep 1s

hd=$PWD
cd ~/PICode

if [ -e ./ABORTED ] 
then
    rm ./ABORTED
fi

if ! screen -list | grep -q "picode"
then
    screen -A -m -d -S picode ./.venv/bin/python ./src/main.py
fi

screen -r

cd $hd
unset hd
