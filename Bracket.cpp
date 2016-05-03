#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include "Bracket.h"

using namespace std;

Bracket::Bracket(std::string thefilename)
{
    string data, playerName, ssplayerELO;
    int i=0, playerELO;
    ifstream inFile;
    inFile.open(thefilename.c_str());
    if(inFile.good()){
		while(getline(inFile, data)){
		    stringstream ss(data);

            getline(ss, playerName, ',');
            getline(ss, ssplayerELO, ',');
            playerELO = std::atoi(ssplayerELO.c_str());

            player newPlayer;
            newPlayer.defeated = false;
            newPlayer.name = playerName;
            newPlayer.pRank = playerELO;

            playerList.push_back(newPlayer);
            i++;
		}
    }

    if(playerELO != i){
        cout<<"Please check numbering of players"<<endl;
        return;
    }

    createBracket();

}

Bracket::~Bracket()
{
    destroyBracket();
    for(int i=0; i < playerList.size(); i++){
        playerList.pop_back();
    }
}

void Bracket::createBracket(){

    int sorter;
    int playerCount = playerList.size();
    round1Count = rowDifference(playerCount);
    int round2Players = playerCount - round1Count*2;
    round2Count = rowNumbers(round2Players);
    int additionalRounds = addRounds(round2Count);
    if(round1Count==0){
        round1Count = round2Count/2;
        round2Players=0;
        round2Count=0;
        additionalRounds = addRounds(round1Count);
    }
    totalMatches = round1Count+round2Count+additionalRounds;

    //cout<<"Total Players: "<<playerCount<<endl;
    //cout<<"Matches in Round 1: "<<round1Count<<endl;
    //cout<<"Matches in Round 2: "<<round2Count<<endl;

    match * currentMatch = new match(1);
    currentMatch->backMatch = NULL;
    root = currentMatch;

    for(int cc=1; cc < totalMatches; cc++){
        match * newMatch = new match(cc+1);
        currentMatch->forwardMatch = newMatch;
        newMatch->backMatch = currentMatch;
        currentMatch = newMatch;
    }

    currentMatch = root;
    while(currentMatch->matchnumber <= round1Count) currentMatch = currentMatch->forwardMatch;
    int matchCounter=1;

    while(matchCounter <= round2Players){
        if(currentMatch->p1){
            currentMatch->p2 = &playerList[matchCounter-1];
            //cout<<"In match #"<<currentMatch->matchnumber<<": P2 ";
            //cout<<currentMatch->p2->name<<" has been added"<<endl;
            currentMatch = currentMatch->backMatch;
        }
        else{
            currentMatch->p1 = &playerList[matchCounter-1];
            //cout<<"In match #"<<currentMatch->matchnumber<<": P1 ";
            //cout<<currentMatch->p1->name<<" has been added"<<endl;
            if(currentMatch->matchnumber != round1Count+round2Count){
                currentMatch = currentMatch->forwardMatch;
            }
        }
        matchCounter++;
    }

    currentMatch = root;
    while(currentMatch->matchnumber < round1Count) currentMatch = currentMatch->forwardMatch;
    while(matchCounter <= playerCount){
        if(currentMatch->p1){
            currentMatch->p2 = &playerList[matchCounter-1];
            //cout<<"In match #"<<currentMatch->matchnumber<<": P1 is already"<<currentMatch->p1->name<<endl;
            //cout<<"In match #"<<currentMatch->matchnumber<<": P2 ";
            //cout<<currentMatch->p2->name<<" has been added"<<endl;
            currentMatch = currentMatch->forwardMatch;
        }
        else{
            currentMatch->p1 = &playerList[matchCounter-1];
            //cout<<"In match #"<<currentMatch->matchnumber<<": P1 ";
            //cout<<currentMatch->p1->name<<" has been added"<<endl;
            if(currentMatch->matchnumber != 1) currentMatch = currentMatch->backMatch;
        }
        matchCounter++;
    }

    currentMatch = root;
    match * winnersmatch = root;
    sorter = round1Count;
    if(round2Count != 0){
        sorter = round2Count;
        while(winnersmatch->matchnumber <= round1Count) winnersmatch = winnersmatch->forwardMatch;
        for(int cc=1; cc <= round1Count; cc++){
            currentMatch->nextMatch = winnersmatch;
            currentMatch = currentMatch->forwardMatch;
            winnersmatch = winnersmatch->forwardMatch;
        }
    }

    while(winnersmatch->matchnumber <= round1Count+round2Count) winnersmatch = winnersmatch->forwardMatch;
    match * currentSecondary = currentMatch;
    for(int k=1; k<sorter; k++){
        currentSecondary = currentSecondary->forwardMatch;
    }
    //cout<<currentMatch->matchnumber<<endl;
    //cout<<currentSecondary->matchnumber<<endl;
    //cout<<winnersmatch->matchnumber<<endl;
    while(winnersmatch->matchnumber <= totalMatches){
        if(currentMatch->matchnumber >= currentSecondary->matchnumber){
            for(int k=1; k<=sorter; k++){
                currentSecondary = currentSecondary->forwardMatch;
            }
            sorter = sorter/2;
            for(int k=1; k<=sorter; k++){
                currentMatch = currentMatch->forwardMatch;
            }
        }
        currentMatch->nextMatch = winnersmatch;
        currentSecondary->nextMatch = winnersmatch;
        //cout<<"Going from match #"<<currentMatch->matchnumber<<" and "<<currentSecondary->matchnumber<<" to match #"<<winnersmatch->matchnumber<<endl;
        currentMatch = currentMatch->forwardMatch;
        currentSecondary = currentSecondary->backMatch;
        if(winnersmatch->matchnumber == totalMatches) break;
        winnersmatch = winnersmatch->forwardMatch;
    }
}

void Bracket::destroyBracket(){
    match * currentMatch = root;
    match * nextMatch = root->forwardMatch;
    root = NULL;

    while(nextMatch->forwardMatch){
        delete currentMatch;
        currentMatch = nextMatch;
        nextMatch = currentMatch->forwardMatch;
    }
    delete nextMatch;
}

void Bracket::printLineup(){
    match * currentMatch = root;
    for(int i=1; i<=totalMatches; i++){
        if(currentMatch->p1 && currentMatch->p2){
            if(!currentMatch->p1->defeated && !currentMatch->p2->defeated){
                cout<<"Match #"<<currentMatch->matchnumber<<": "<<currentMatch->p1->name<<" vs "<<currentMatch->p2->name<<endl;
            }
        }
        currentMatch = currentMatch->forwardMatch;
    }
}

void Bracket::submitResult(std::string playerWinner){
    match * currentMatch = findMatch(playerWinner);
    if(currentMatch == NULL)            //if match not found prints out "match not found" and then returns, stopping seg fault bug
    {
        return;
    }
    else
    {
        while(currentMatch->p1->defeated || currentMatch->p2->defeated){
            currentMatch = currentMatch->nextMatch;
            if(!currentMatch->p1 || !currentMatch->p2){
                cout<<"Next match is not ready yet"<<endl;
                return;
            }
        }
        player newPlayer;
        newPlayer.defeated = false;
        newPlayer.name = playerWinner;
        int playerRank, fromMatch, toMatch;
        if(currentMatch->p1->name == playerWinner){
            playerRank = currentMatch->p1->pRank;
            currentMatch->p2->defeated = true;
        }
        else{
            playerRank = currentMatch->p2->pRank;
            currentMatch->p1->defeated = true;
        }
        cout<<playerWinner<<" wins!"<<endl;
        fromMatch = currentMatch->matchnumber;
        if(currentMatch->nextMatch){
            currentMatch = currentMatch->nextMatch;
        }
        else{
            cout<<"Tournament has ended!"<<endl;
            return;
        }
        toMatch = currentMatch->matchnumber;
        cout<<"Going from match #"<<fromMatch<<" to match #"<<toMatch<<endl;
        if(currentMatch->p1){
            currentMatch->p2 = &playerList[playerRank-1];
        }
        else{
            currentMatch->p1 = &playerList[playerRank-1];
        }
    }	//end of check if/else statement to stop seg fault bug
}

void Bracket::printWinners(){
    match * currentMatch = root;
    string first, second, third1, third2;
    while(currentMatch->nextMatch) currentMatch = currentMatch->nextMatch;
    if(currentMatch->p1->defeated){
        first = currentMatch->p2->name;
        second = currentMatch->p1->name;
    }
    else{
        first = currentMatch->p1->name;
        second = currentMatch->p2->name;
    }
    currentMatch = currentMatch->backMatch;
    if(currentMatch->p1->name == first || currentMatch->p1->name == second){
        third1 = currentMatch->p2->name;
    }
    else{
        third1 = currentMatch->p1->name;
    }
    currentMatch = currentMatch->backMatch;
    if(currentMatch->p1->name == first || currentMatch->p1->name == second){
        third2 = currentMatch->p2->name;
    }
    else{
        third2 = currentMatch->p1->name;
    }

    cout<<first<<" places in First Place."<<endl;
    cout<<second<<" places in Second Place."<<endl;
    cout<<third1<<" and "<<third2<<" tie for Third Place."<<endl;
}

match * Bracket::findMatch(std::string playerFind){
    match * currentMatch = root;
    for(int i=1; i<=totalMatches; i++){
        if(currentMatch->p1){
            if(currentMatch->p1->name == playerFind){
                if(!currentMatch->p1->defeated && !currentMatch->p2->defeated){
                    cout<<currentMatch->p1->name<<" vs "<<currentMatch->p2->name<<endl;
                    return currentMatch;
                }
            }
        }
        if(currentMatch->p2){
            if(currentMatch->p2->name == playerFind){
                if(!currentMatch->p1->defeated && !currentMatch->p2->defeated){
                    cout<<currentMatch->p1->name<<" vs "<<currentMatch->p2->name<<endl;
                    return currentMatch;
                }
            }
        }
        currentMatch = currentMatch->forwardMatch;
    }
    cout<<"Match not found"<<endl;
    return NULL;
}

void Bracket::printRound(int roundNumb){
    int matches, i;
    string player1, player2;
    match * currentMatch = root;
    if(roundNumb == 1){
        matches = round1Count;
    }
    else if(roundNumb > 1){
        for(int j=0; j < round1Count; j++){
            currentMatch = currentMatch->forwardMatch;
        }
        roundNumb = roundNumb-2;
        matches = round2Count;
        for(i=0; i < roundNumb; i++){
            for(int j=0; j < matches; j++){
                currentMatch = currentMatch->forwardMatch;
            }
            matches = matches/2;
        }
        cout<<"Round #"<<i+2<<":"<<endl;
    }
    else{
        cout<<"Not a valid roundNumb"<<endl;
        return;
    }
    for(int j=0; j < matches; j++){
        if(currentMatch->p1){
            player1 = currentMatch->p1->name;
        }
        else player1 = "<no player>";

        if(currentMatch->p2){
            player2 = currentMatch->p2->name;
        }
        else player2 = "<no player>";
        cout<<"Match #"<<currentMatch->matchnumber<<": "<<player1<<" vs "<<player2<<endl;
        currentMatch = currentMatch->forwardMatch;
    }
}

void Bracket::undoMatch(match * currentMatch){
    if(!currentMatch){
        cout<<"Unplayed matches cannot be undone"<<endl;
        return;
    }

     if(currentMatch->nextMatch->p1 && currentMatch->nextMatch->p2){
        if(currentMatch->nextMatch->p1->defeated || currentMatch->nextMatch->p2->defeated){
            cout<<"Cannot cancel a match with already completed following matches. Please delete active matches first."<<endl;
            return;
        }
     }
    if(currentMatch->nextMatch->p1 || currentMatch->nextMatch->p2){
        if(currentMatch->p1->defeated){
            if(currentMatch->nextMatch->p1->name == currentMatch->p2->name){
                currentMatch->nextMatch->p1 = NULL;
                cout<<"Winner removed"<<endl;
            }
            else{
                currentMatch->nextMatch->p2 = NULL;
                cout<<"Winner removed"<<endl;
            }
        }
        else if(currentMatch->p2->defeated){
            if(currentMatch->nextMatch->p1->name == currentMatch->p1->name){
                currentMatch->nextMatch->p1 = NULL;
                cout<<"Winner removed"<<endl;
            }
            else{
                currentMatch->nextMatch->p2 = NULL;
                cout<<"Winner removed"<<endl;
            }
        }
     }

     if(currentMatch->p1->defeated){
        currentMatch->p1->defeated = false;
        cout<<"Result redacted"<<endl;
     }
     else{
        currentMatch->p2->defeated = false;
        cout<<"Result redacted"<<endl;
     }


}

match * Bracket::specificSearch(std::string player1, std::string player2){
    match * currentMatch = root;
    while(currentMatch->forwardMatch){
        if(currentMatch->p1->name == player1 && currentMatch->p2->name == player2){
            cout<<"Match #"<<currentMatch->matchnumber<<": "<<currentMatch->p1->name<<" vs "<<currentMatch->p2->name<<endl;
            if(currentMatch->p2->defeated){
                cout<<currentMatch->p1->name<<" won!"<<endl;
            }
            else if(currentMatch->p1->defeated){
                cout<<currentMatch->p2->name<<" won!"<<endl;
            }
            else{
                cout<<"No one has won this match yet"<<endl;
                return NULL;
            }
            return currentMatch;
        }
        else if(currentMatch->p1->name == player2 && currentMatch->p2->name == player1){
            cout<<"Match #"<<currentMatch->matchnumber<<": "<<currentMatch->p1->name<<" vs "<<currentMatch->p2->name<<endl;
            if(currentMatch->p2->defeated){
                cout<<currentMatch->p1->name<<" won!"<<endl;
            }
            else if(currentMatch->p1->defeated){
                cout<<currentMatch->p2->name<<" won!"<<endl;
            }
            else{
                cout<<"No one has won this match yet"<<endl;
                return NULL;
            }
            return currentMatch;
        }
        currentMatch = currentMatch->forwardMatch;
    }
    return NULL;
}

int Bracket::rowDifference(int players){
    int powers = 1;
    while(powers <= players){
        powers = powers*2;
    }
    powers = powers/2;
    return players-powers;
}

int Bracket::rowNumbers(int players){
    int powers = 1;
    while(powers <= players){
        powers = powers*2;
    }
    powers = powers/2;
    return powers;
}

int Bracket::addRounds(int lastround){
    int counter=0;
    while(lastround > 1){
        lastround = lastround/2;
        counter = counter + lastround;
    }
    return counter;

}
