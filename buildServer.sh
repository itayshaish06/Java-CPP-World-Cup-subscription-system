#!/bin/bash

# navigate to the server folder and compile the server
cd server
mvn compile

# start the server in a new terminal window
mvn exec:java -Dexec.mainClass="bgu.spl.net.impl.stomp.StompServer" -Dexec.args="7776 tpc"

echo ""
echo "Server terminated"

mvn clean
