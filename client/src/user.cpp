#include "../include/user.h"


//constructor
user::user(string username) : userName(username),channelToSubId(unordered_map<string,int>()),subIdToChannel(unordered_map<int,string>()), hash_lock(),channelToUserSummary(unordered_map<string,unordered_map<string,summary>>()), hash_lock2(),subID(0) {}

//get userName
string user::getUserName() const{
    return userName;
}

//insert to channelToSubId
void user::subscribe(string channel, int subId){
    lock_guard<mutex> lock(hash_lock);
    if(channelToSubId.count(channel) == 0){
        channelToSubId.insert({channel, subID});
        subIdToChannel.insert({subID, channel});
        lock_guard<mutex> lock2(hash_lock2);
        channelToUserSummary.insert({channel,unordered_map<string,summary>()});
    }
}

//get subId from channelToSubId
int user::getTheSubID(string channel){
    lock_guard<mutex> lock(hash_lock);
    return channelToSubId.at(channel);
}

//remove from channelToSubId
void user::unsubscribe(int subId){
    lock_guard<mutex> lock(hash_lock);
    if(subIdToChannel.find(subId) != subIdToChannel.end()){
        string channel = subIdToChannel.at(subId);
        subIdToChannel.erase(subId);
        channelToSubId.erase(channel);
        lock_guard<mutex> lock2(hash_lock2);
        channelToUserSummary.erase(channel);
    }
}

// insert to channelToUserSummary
bool user::insertEvent(string channel, string teamA_VS_teamB,string teamAName,string teamBName, string eventUserName,string time,string teamAStats,string teamBStats ,string description){
    lock_guard<mutex> lock(hash_lock2);
    if(channelToUserSummary.count(channel) == 0){
        return false;
    }
    if(channelToUserSummary.at(channel).count(eventUserName) == 0){
        summary newSummary;
        channelToUserSummary[channel].emplace(eventUserName, newSummary);
    }
    summary& summaryOfUser = channelToUserSummary.at(channel).at(eventUserName);
    summaryOfUser.setTeamA_vs_teamB(teamA_VS_teamB);
    summaryOfUser.setTeamAname(teamAName);
    summaryOfUser.setTeamBname(teamBName);
    summaryOfUser.setUserName(eventUserName);
    int timeInt = stoi(time);
    summaryOfUser.setActive(((timeInt < 5400)) ? "true" : "false");
    summaryOfUser.setBeforeHalfTime(((timeInt < 2700)) ? "true" : "false");
    summaryOfUser.setTeamAstats(teamAStats);
    summaryOfUser.setTeamBstats(teamBStats);
    summaryOfUser.addToVector(description);
    return true;
}

//get summary from channelToUserSummary, check if exist before calling this function
bool user::summarize(string channel, string userName, string filePath){
    lock_guard<mutex> lock(hash_lock2);
    if(channelToUserSummary.count(channel)>0 && channelToUserSummary.at(channel).count(userName) > 0){
        summary& tmp = channelToUserSummary.at(channel).at(userName);
        //summary& tmp = channelToUserSummary[channel][userName];
        tmp.summarizeGame(filePath);
        return true;
    }
    return false;
}

//get channel from subIdToChannel
string& user::getTheChannel(int subId){
    lock_guard<mutex> lock(hash_lock);
    return subIdToChannel.at(subId);
}

//check if user is subscribed to channel
bool user::isSubscribed(string channel){
    lock_guard<mutex> lock(hash_lock);
    return channelToSubId.count(channel) > 0;
}


