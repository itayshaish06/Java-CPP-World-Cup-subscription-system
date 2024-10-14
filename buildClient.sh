#!/bin/bash
#!/bin/bash

# navigate to the client folder and compile the client
cd client
sudo apt-get update
sudo apt-get install libboost-all-dev
make

# navigate to bin folder
cd bin

# get the IP address
ip4=$(/sbin/ip -o -4 addr list eth0 | awk '{print $4}' | cut -d/ -f1)

# run the client
./StompWCIClient $ip4 7776 

# navigate back to the main folder
cd ..

# clean 
make clean