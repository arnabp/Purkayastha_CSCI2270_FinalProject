#include <vector>
#ifndef BRACKET_H
#define BRACKET_H

struct player{
    bool defeated;
    std::string name;
    int pRank;
};

struct match{
    int matchnumber;
    player *p1;
    player *p2;
    match *nextMatch;
    match *backMatch;
    match *forwardMatch;

    match(){}; // default constructor

    match(int initmatchNumber)
    {
        matchnumber = initmatchNumber;
        p1 = NULL;
        p2 = NULL;
    }
};

class Bracket
{
    public:
        Bracket(std::string thefilename);
        ~Bracket();
        void createBracket();
        void destroyBracket();
        void printLineup();
        void submitResult(std::string playerWinner);
        void printWinners();
        match * findMatch(std::string playerFind);
        void printRound(int roundNumb);
        void undoMatch(match * currentMatch);
        match * specificSearch(std::string player1, std::string player2);
    protected:
    private:
        std::vector<player> playerList;
        int totalMatches;
        int round1Count;
        match *root;
        int rowDifference(int players);
        int rowNumbers(int players);
        int addRounds(int lastround);

};

#endif // BRACKET_H
