#include "../include/summary.h"
#include "../include/json.hpp"

using json = nlohmann::json;

using namespace std;

//constructor
summary::summary() : active(""), beforeHalfTime(""),teamAstats(""),teamBstats(""),teamAname(""), teamBname(""),teamA_vs_teamB(""), userName(""),gameEventReports({}){
}

//copy assignment operator
summary& summary::operator=(const summary& other){
    if(this != &other){
        this->gameEventReports = other.gameEventReports;
        //this->vector_lock = other.vector_lock;
        this->active = other.active;
        this->beforeHalfTime = other.beforeHalfTime;
        this->teamAstats = other.teamAstats;
        this->teamBstats = other.teamBstats;
        this->teamAname = other.teamAname;
        this->teamBname = other.teamBname;
        this->teamA_vs_teamB = other.teamA_vs_teamB;
        this->userName = other.userName;
    }
    return *this;
}


//add message to vector
void summary::addToVector(string msg){
    gameEventReports.push_back(msg);
}

//set active
void summary::setActive(string active){
    this->active = active;
}

//set beforeHalfTime
void summary::setBeforeHalfTime(string beforeHalfTime){
    this->beforeHalfTime = beforeHalfTime;
}

//set teamAstats
void summary::setTeamAstats(string teamAstats){
    this->teamAstats = teamAstats;
}

//set teamBstats
void summary::setTeamBstats(string teamBstats){
    this->teamBstats = teamBstats;
}

//set teamAname
void summary::setTeamAname(string teamAname){
    this->teamAname = teamAname;
}

//set teamBname
void summary::setTeamBname(string teamBname){
    this->teamBname = teamBname;
}

//set teamA_vs_teamB
void summary::setTeamA_vs_teamB(string teamA_vs_teamB){
    this->teamA_vs_teamB = teamA_vs_teamB;
}

//set userName
void summary::setUserName(string userName){
    this->userName = userName;
}

//summarize Game
void summary::summarizeGame(std::string jsonPath){
    ofstream json_file;
    json_file.open(jsonPath);
    json_file << teamA_vs_teamB << "\n";
    json_file << "Game stats:\n";
    json_file << "General stats:\n";
    json_file << "active: " << active << "\n";
    json_file << "before halftime: " << beforeHalfTime << "\n";
    json_file << teamAstats;
    json_file << teamBstats;
    json_file << "Game event reports:\n";
    int gameEventReportsSize = gameEventReports.size();
    for(int i = 0; i < gameEventReportsSize; i++){
        json_file << gameEventReports[i]+"\n" ;
    }
    
    json_file.close();
}

