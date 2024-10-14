#pragma once
#include <mutex>
#include <string>
#include <unordered_map>
using namespace std;
#include "../include/user.h"

class ConnectionHandler;

class StompProtocol
{
private:
    int receiptId;
    user* clientUser;
    unordered_map<int, string> receiptToCommand;
    unordered_map<int, int> receiptToSubID;
    int subID;
    mutex commandLock;
    mutex subIDLock;
    volatile bool connected;
    string userName;

public:
    //constructor
    StompProtocol();

    //copy constructor
    StompProtocol(const StompProtocol &other);

    //copy assignment operator
    StompProtocol &operator=(const StompProtocol &other);

    //move constructor
    StompProtocol(StompProtocol &&other);

    //move assignment operator
    StompProtocol &operator=(StompProtocol &&other);

    //destructor
    ~StompProtocol();

    //get connected
    volatile bool& getConnected();

    //switch connected
    void switchConnected();

    //handle commands:

    //i/o function
    void handleIO(string inputFromUser, ConnectionHandler& connectionHandler);
    
    //sub string until space
    string subStrUntilSpace(string& inputFromUser);

    //connect to server
    void handleConnect(string userName, string passcode, ConnectionHandler& connectionHandler);

    //disconnect from server
    void handleDisconnect(ConnectionHandler& connectionHandler);

    //send message
    void handleReport(string filePath,ConnectionHandler& connectionHandler);

    //join channel
    void handleJoin(string channel,ConnectionHandler& connectionHandler);

    //exit channel
    void handleExit(string channel,ConnectionHandler& connectionHandler);

    //frames from server function
    void handleFromServer(string frame);

    //handle receipt
    void handleReceipt(string receiptId);

    //handle error
    void handleError(string error);

    //handle message
    void handleMessage(string message);

    //handle connected
    void handleConnected(string connected);
    
    //handle summary
    void handleSummary(string summary);

    //handle frames from server:
    void handleFrameFromServer(string frame, ConnectionHandler& connectionHandler);

    string getMainHeaderFrame(string& frame);

    //handle receipt from server
    void handleReceiptFromServer(string receiptMsg, ConnectionHandler& connectionHandler);

    //handle error from server
    void handleErrorFromServer(string errorMsg, ConnectionHandler& connectionHandler);

    //handle message from server
    void handleMessageFromServer(string messageMsg);

    //handle connected from server
    void handleConnectedFromServer(string connectedMsg);

    //get header from frame
    string getHeader(string& frame, string header);

    //shuldterminate
    bool shouldTerminate();

    bool eraseStringUntilHeader(string& frame, string& header);

    string getTeamAupdates(string &frame);

    string getTeamBupdates(string &frame);

    string getDesctiption(string &frame);

    string reduceDoubleSpaces(string str);

};

