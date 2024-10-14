#pragma once
#include <mutex>
#include <unordered_map>
#include <string>

using namespace std;

#include "summary.h"

class user{
    private:
        string userName;
        unordered_map<string,int> channelToSubId;
        unordered_map<int,string> subIdToChannel;
        mutex hash_lock;
        unordered_map<string,unordered_map<string,summary>> channelToUserSummary;
        mutex hash_lock2;
        int subID;
        
    public:
        //constructor
        user(string username);

        //get userName
        string getUserName() const;

        //insert to channelToSubId
        void subscribe(string channel, int subId);

        //get subId from channelToSubId
        int getTheSubID(string channel);

        //get channel from subIdToChannel
        string& getTheChannel(int subId);

        //remove from channelToSubId and channelToUserSummary
        void unsubscribe(int subId);

        //insert to channelToUserSummary
        bool insertEvent(string channel, string teamA_VS_teamB,string teamAName,string teamBName, string eventUserName,string time,string teamAStats,string teamBStats ,string description);

        //get summary from channelToUserSummary
        bool summarize(string channel, string userName, string filePath);

        //check if the user is subscribed to the channel
        bool isSubscribed(string channel);

};