#include "../include/StompProtocol.h"
#include "../include/ConnectionHandler.h"
#include "../include/event.h"
using namespace std;
//constructor
StompProtocol::StompProtocol() : receiptId(0), clientUser(nullptr), receiptToCommand(unordered_map<int,string>()),receiptToSubID(unordered_map<int,int>()),subID(0),commandLock(),subIDLock(),connected(false),userName()  {}

//copy constructor
StompProtocol::StompProtocol(const StompProtocol &other) : receiptId(other.receiptId), clientUser(nullptr), receiptToCommand(other.receiptToCommand),receiptToSubID(other.receiptToSubID),subID(other.subID),commandLock(),subIDLock(),connected(other.connected),userName(other.userName) {
    if (other.clientUser != nullptr) {
        clientUser = &((*other.clientUser));
    }
}

//copy assignment operator
StompProtocol &StompProtocol::operator=(const StompProtocol &other) {
    if (this != &other) {
        receiptId = other.receiptId;
        if (other.clientUser != nullptr) {
            clientUser = &((*other.clientUser));
         }
        receiptToCommand = other.receiptToCommand;
        receiptToSubID = other.receiptToSubID;
        subID = other.subID;
        connected = other.connected;
        userName = other.userName;
    }
    return *this;
}

//move constructor
StompProtocol::StompProtocol(StompProtocol &&other) : receiptId(other.receiptId), clientUser(other.clientUser), receiptToCommand(other.receiptToCommand),receiptToSubID(other.receiptToSubID),subID(other.subID),commandLock(),subIDLock(),connected(other.connected),userName(other.userName) {
    other.clientUser = nullptr;
}

//move assignment operator
StompProtocol &StompProtocol::operator=(StompProtocol &&other) {
    if (this != &other) {
        receiptId = other.receiptId;
        clientUser = other.clientUser;
        other.clientUser = nullptr;
        receiptToCommand = other.receiptToCommand;
        receiptToSubID = other.receiptToSubID;
        subID = other.subID;
        connected = other.connected;
        userName = other.userName;
    }
    return *this;
}

//destructor
StompProtocol::~StompProtocol() {
    delete (clientUser);
}

//get connected
volatile bool& StompProtocol::getConnected() {
    return connected;
}

//switch connected
void StompProtocol::switchConnected() {
    connected = !connected;
}

//handle commands:

//i/o function
void StompProtocol::handleIO(string inputFromUser, ConnectionHandler &connectionHandler) {
    string command = subStrUntilSpace(inputFromUser);
    if (command.compare("login") == 0) {
        subStrUntilSpace(inputFromUser);
        string userName = subStrUntilSpace(inputFromUser);
        string password = subStrUntilSpace(inputFromUser);
        handleConnect(userName, password, connectionHandler);
    }
     else if (command.compare("join") == 0) {
        string channel = subStrUntilSpace(inputFromUser);
        handleJoin(channel, connectionHandler);
    }
     else if (command.compare("exit") == 0) {
        string channel = subStrUntilSpace(inputFromUser);
        handleExit(channel, connectionHandler);
    }
     else if (command.compare("report") == 0){
        string filePath = subStrUntilSpace(inputFromUser);
        handleReport(filePath, connectionHandler);
    }
     else if (command.compare("logout") == 0) {
        handleDisconnect(connectionHandler);
    }
    else if (command.compare("summary") == 0) {
        handleSummary(inputFromUser);
     }else {
        cout << "Invalid command" << endl;
    }
}

//get command type
string StompProtocol::subStrUntilSpace(string& inputFromUser){
    int spaceIndex = inputFromUser.find(" ");
    string commandType = inputFromUser.substr(0,inputFromUser.find(" "));
    if (spaceIndex == -1){
        inputFromUser = "";
    }
    else{
        inputFromUser = inputFromUser.substr(inputFromUser.find(" ") + 1);
    }
    return commandType;
}

//connect to server
void StompProtocol::handleConnect(string userName, string password, ConnectionHandler &connectionHandler) {
    if(connected){
        cout << "The client is already logged in, log out before trying again" << endl;
        return;
    }
    if(userName.empty() || password.empty()){
        cout << "Invalid input - USERNAME or PASS are empty" << endl;
        return;
    }
    string connectFrame = "CONNECT\naccept-version:1.2\nhost:stomp.cs.bgu.ac.il\nlogin:" + userName + "\npasscode:" + password + "\n\n";
    this->userName = userName;
    if(connectionHandler.sendLine(connectFrame)){
        //cout << "message sent" << endl;
        return;
    }
    //cout << "message not sent" << endl;
}

//join channel
void StompProtocol::handleJoin(string channel,ConnectionHandler &connectionHandler) {
    string joinFrame = "SUBSCRIBE\ndestination:" + channel + "\nid:" + to_string(subID) + "\nreceipt:" + to_string(receiptId) + "\n\n";
    lock_guard<mutex> lock(commandLock);
    receiptToCommand.insert(pair<int,string>(receiptId,"Joined " + channel));
    lock_guard<mutex> lock2(subIDLock);
    receiptToSubID.insert(pair<int,int>(receiptId,subID));
    receiptId++;
    subID++;
    connectionHandler.sendLine(joinFrame);
}

//disconnect from server
void StompProtocol::handleDisconnect(ConnectionHandler &connectionHandler) {
    string disconnectFrame = "DISCONNECT\nreceipt:" + to_string(receiptId) + "\n\n";
    lock_guard<mutex> lock(commandLock);
    receiptToCommand.insert(pair<int,string>(receiptId,"disconnect"));
    receiptId++;
    connectionHandler.sendLine(disconnectFrame);
}

//send message
void StompProtocol::handleReport(string filePath, ConnectionHandler &connectionHandler) {
    names_and_events parsedFile = parseEventsFile(filePath);
    string channel = parsedFile.team_a_name + "_" + parsedFile.team_b_name;
    if(!clientUser->isSubscribed(channel)){
        cout << "You are not subscribed to this channel" << endl;
        return;
    }
    string userName = clientUser->getUserName();
    for (Event &event : parsedFile.events) {
        map<string,string> teamA_UpdatesMap  = (event.get_team_a_updates());
        map<string,string> teamB_UpdatesMap  = (event.get_team_b_updates());
        string teamA_Update;
        string teamB_Update;
        for (pair <string,string> update : teamA_UpdatesMap)
        {
            if (update.first.compare("goals")==0)
                teamA_Update = teamA_Update +"\n" + "    goals: " + update.second;
            if (update.first.compare("possession")==0)
                teamA_Update = teamA_Update +"\n"+ "    possession: " + update.second;
        }
        for (pair <string,string> update : teamB_UpdatesMap)
        {
            if (update.first.compare("goals")==0)
                teamB_Update = teamB_Update +"\n"+ "    goals: " + update.second ;
            if (update.first.compare("possession")==0)
                teamB_Update = teamB_Update +"\n"+ "    possession: " + update.second;
        }
        string time = to_string(event.get_time());
        string active = ((event.get_time() < 5400)) ? "true" : "false";
        string beforeHalfTime = ((event.get_time() < 2700)) ? "true" : "false";
        string sendFrame =
         "SEND\ndestination:/" + parsedFile.team_a_name + "_" + parsedFile.team_b_name +"\n\n" 
         +"user: "+ userName +"\n" +
          "team a: " + parsedFile.team_a_name +"\n"+
          "team b: " + parsedFile.team_b_name +"\n"+
          "event name: " + event.get_name() + "\n" +
          "time: " + time + "\n"+
          "general game updates:\n" + 
          "    active: " + active + "\n"+
          "    before halftime: " + beforeHalfTime + "\n"+
          "team a updates: " + teamA_Update + "\n" +
          "team b updates: " + teamB_Update + "\n" +
          "description: " + event.get_discription() + "\n";
          //cout << sendFrame << endl;
        connectionHandler.sendLine(sendFrame);
    }

}

//exit channel
void StompProtocol:: handleExit(string channel,ConnectionHandler& connectionHandler){
    int channelSubID = clientUser->getTheSubID(channel);
    string exitFrame = "UNSUBSCRIBE\nid:" + to_string(channelSubID) + "\nreceipt:" + to_string(receiptId) + "\n\n";
    lock_guard<mutex> lock(commandLock);
    receiptToSubID.insert(pair<int,int>(receiptId,channelSubID));
    receiptToCommand.insert(pair<int,string>(receiptId,"Exit " + channel));
    receiptId++;
    connectionHandler.sendLine(exitFrame);
}

//handle frames from server:
void StompProtocol::handleFrameFromServer(string frame, ConnectionHandler& connectionHandler){
    //cout << "frame from server:\n"<< frame << endl;
    string command = getMainHeaderFrame(frame);
    if (command.compare("CONNECTED") == 0) {
        handleConnectedFromServer(frame);
    } else if (command.compare("MESSAGE") == 0) {
        handleMessageFromServer(frame);
    } else if (command.compare("RECEIPT") == 0) {
        handleReceiptFromServer(frame, connectionHandler);
    } else if (command.compare("ERROR") == 0) {
        handleErrorFromServer(frame, connectionHandler);
    } else {
        cout << "Invalid frame" << endl;
    }
}

string StompProtocol::getMainHeaderFrame(string& frame){
    string header = frame.substr(0,frame.find("\n"));
    frame = frame.substr(frame.find("\n")+1);
    return header;
}

//handle receipt from server
void StompProtocol::handleReceiptFromServer(string frame, ConnectionHandler& connectionHandler){
    string receipt_id = getHeader(frame,"receipt-id:");
    int receiptIdInt = stoi(receipt_id);
    string command = receiptToCommand.at(receiptIdInt);
    //std::string::npos is thr value returned if the string is not found
    if (command.find("Joined")!= std::string::npos) {
        int joinedStringSize = 7;//length of "Joined " - with space
        int indexOfChannel = command.find("Joined") + joinedStringSize;
        int SubID = receiptToSubID.at(receiptIdInt);
        string channel = command.substr(indexOfChannel);
        cout << "Joined channel " << channel << endl;
        clientUser->subscribe(channel, SubID);
        receiptToCommand.erase(receiptIdInt);
        receiptToSubID.erase(receiptIdInt);

    } else if (command.find("Exit") != std::string::npos) {
        int ExitStringSize = 5;//length of "Exit " - with space
        int indexOfChannel = command.find("Exit") + ExitStringSize;
        int SubID = receiptToSubID.at(receiptIdInt);
        string channel = command.substr(indexOfChannel);
        cout << "Exited channel " << channel << endl;
        clientUser->unsubscribe(SubID);
        receiptToCommand.erase(receiptIdInt);
        receiptToSubID.erase(receiptIdInt);

    } else if (command.find("disconnect") != std::string::npos) {
        receiptToCommand.erase(receiptIdInt);
        receiptToSubID.erase(receiptIdInt);
        switchConnected();
        connectionHandler.close();
    }
}

// get header from frame
string StompProtocol::getHeader(string &frame, string header)
{
    string headerValue = "";
    if (eraseStringUntilHeader(frame, header))
    {
        // header found and frame starting with it
        int headerStart = 0;
        int headerLength = header.length();
        int from = headerStart + headerLength;
        while (frame[from] == ' ')
                from++;
        int endOfHeaderValue = frame.find("\n", from);
        headerValue = frame.substr(from, endOfHeaderValue - from);
        frame = frame.substr(frame.find("\n", frame.find(header)) + 1);
    }
    return headerValue;
}

//handle error from server
void StompProtocol::handleErrorFromServer(string errorMsg, ConnectionHandler& connectionHandler){
    string receiptIdString = getHeader(errorMsg,"receipt-id:");
    string commandCausedTheError = "";
    int receiptIdInt = -1;
    if(!receiptIdString.empty()){
        receiptIdInt = stoi(receiptIdString);
        commandCausedTheError = receiptToCommand.at(receiptIdInt);
    }
    string errorDetailString = getHeader(errorMsg,"message:");
    cout << "Error caused by command " << commandCausedTheError << "\n" 
    << "Error details: "<< errorDetailString  <<"\n"<< endl;
    receiptToCommand.erase(receiptIdInt);
    receiptToSubID.erase(receiptIdInt);
    if(connected){
        switchConnected();
        connectionHandler.close();
    }
}

//handle message from server
void StompProtocol::handleMessageFromServer(string messageMsg){
    string channel = getHeader(messageMsg,"destination:");
    string teamAName = channel.substr(channel.find("/") + 1, channel.find("_") - channel.find("/") - 1);
    string teamBName = channel.substr(channel.find("_") + 1);
    string teamA_VS_teamB = teamAName + " vs " + teamBName;
    channel = teamAName + "_" + teamBName;
    string userName = getHeader(messageMsg,"user:");
    string eventName = getHeader(messageMsg,"event name:");
    string time = getHeader(messageMsg,"time:");
    string teamAUpdates = getTeamAupdates(messageMsg);
    teamAUpdates = teamAName+" stats:" + reduceDoubleSpaces(teamAUpdates)+"\n";
    string teamBUpdates = getTeamBupdates(messageMsg);
    teamBUpdates = teamBName+" stats:" + reduceDoubleSpaces(teamBUpdates)+"\n";;
    string description = time+" - "+eventName+":\n\n"+getDesctiption(messageMsg)+"\n";
    clientUser->insertEvent(channel, teamA_VS_teamB, teamAName, teamBName, userName, time, teamAUpdates, teamBUpdates, description);
    //bool added = clientUser->insertEvent(channel, teamA_VS_teamB, teamAName, teamBName, userName, time, teamAUpdates, teamBUpdates, description);
    //cout << "Message received from channel " << channel<< " and saved "<< ((added) ? "Successfully" : "Unsuccessfully") << endl;
}

//handle connected from server
void StompProtocol::handleConnectedFromServer(string connectedMsg){
    cout << "Login successful" << endl;
    switchConnected();
    clientUser = new user(userName);
}

//handle summary
void StompProtocol::handleSummary(string summaryMsg){
    //summaryMsg = summaryMsg.substr(summaryMsg.find(" ") + 1);
    string gameName = summaryMsg.substr(0,summaryMsg.find(" "));
    summaryMsg = summaryMsg.substr(summaryMsg.find(" ") + 1);
    string user = summaryMsg.substr(0, summaryMsg.find(" "));
    summaryMsg = summaryMsg.substr(summaryMsg.find(" ") + 1);
    string filePath = summaryMsg;
    if(clientUser->summarize(gameName, user, filePath)){
        //cout << "Summary created successfully" << endl;
    }
    else{
        //cout << "Summary created unsuccessfully" << endl;
    }
}

//shouldterminate
bool StompProtocol::shouldTerminate(){
    return !connected;
}

string StompProtocol::getTeamAupdates(string &frame){
    string teamAupdates = "";
    string header = "team a updates:";
    eraseStringUntilHeader(frame, header);
    header = "\n";
    if(eraseStringUntilHeader(frame, header)){
        teamAupdates = frame.substr(0, frame.find("team b updates:")-1);
    }
    else{
        header = "possession:";
        if(eraseStringUntilHeader(frame, header)){
            teamAupdates = frame.substr(0, frame.find("team b updates:"));
        }
    }
    return teamAupdates;
}

string StompProtocol::getTeamBupdates(string &frame){
    string teamBupdates = "";
    string header = "team b updates:";
    eraseStringUntilHeader(frame, header);
    header = "\n";
    if(eraseStringUntilHeader(frame, header)){
        teamBupdates = frame.substr(0, frame.find("description:")-1);
    }
    else{
        header = "possession:";
        if(eraseStringUntilHeader(frame, header)){
            teamBupdates = frame.substr(0, frame.find("description:"));
        }
    }
    return teamBupdates;
}

string StompProtocol::getDesctiption(string &frame){
    string description = "";
    string header = "description:";
    int from = header.length()+1;
    if(eraseStringUntilHeader(frame, header)){
        description = frame.substr(from);
    }
    return description;
}

string StompProtocol::reduceDoubleSpaces(string str){
    int i = 0;
    int str_len = str.length();
    while(i < str_len-1){
        if(str[i] == ' ' && str[i+1] == ' '){
            str.erase(i, 2);
        }
        else{
            i++;
        }
    }
    return str;
}

bool StompProtocol::eraseStringUntilHeader(string& frame, string& header){
    int headerStart = frame.find(header);
    int frame_len = frame.length();
    if(headerStart > -1 && headerStart < frame_len){
        frame = frame.substr(headerStart);
        return true;
    }
    return false;
}
