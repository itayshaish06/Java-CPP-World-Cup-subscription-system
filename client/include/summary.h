#pragma once
#include <vector>
#include <mutex>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class summary{
    private:
        string active;
        string beforeHalfTime;
        string teamAstats;
        string teamBstats;
        string teamAname;
        string teamBname;
        string teamA_vs_teamB;
        string userName;
        vector<string> gameEventReports;

    public:
        //constructor
        summary();

        //copy assignment operator
        summary& operator=(const summary& other);

        //add message to vector
        void addToVector(string msg);

        //set active
        void setActive(string active);

        //set beforeHalfTime
        void setBeforeHalfTime(string beforeHalfTime);

        //set teamAstats
        void setTeamAstats(string teamAstats);

        //set teamBstats
        void setTeamBstats(string teamBstats);

        //set teamAname
        void setTeamAname(string teamAname);

        //set teamBname
        void setTeamBname(string teamBname);

        //set teamA_vs_teamB
        void setTeamA_vs_teamB(string teamA_vs_teamB);

        //set userName
        void setUserName(string userName);

        //summarize Game
        void summarizeGame(std::string json_path);

};