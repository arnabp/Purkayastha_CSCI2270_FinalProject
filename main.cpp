#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include "Bracket.h"

using namespace std;

void printMenu(){
    cout << "======Main Menu======" << endl;
    cout << "1. Print current lineup" << endl;
    cout << "2. Submit result" << endl;
    cout << "3. Find next matchup" << endl;
    cout << "4. Find specific matchup" << endl;
    cout << "5. Matches in round" << endl;
    cout << "6. Undo a match" << endl;
    cout << "7. Print Winners" << endl;
    cout << "8. Delete bracket" << endl;
    cout << "9. Create bracket" << endl;
    cout << "10. Quit" << endl;
}

int main(int argc,char *argv[])
{
    std::string thefilename;
    if (argc>1){
        thefilename = argv[1];
    }
    else{
        cout<<"ERROR: NO FILE NAME GIVEN"<<endl;
    }

    Bracket mainBracket(thefilename);

    int selection=0;
    while(selection != 11){
        printMenu();
        string ss;
        getline(cin,ss);
        selection = std::atoi(ss.c_str());

        if(selection == 1){
            mainBracket.printLineup();
        }
        else if(selection == 2){
            cout<<"Enter winner of match"<<endl;
            std::string matchWinner;
            getline(cin, matchWinner);
            mainBracket.submitResult(matchWinner);
        }
        else if(selection == 3){
            cout<<"Who are you searching for?"<<endl;
            std::string matchSearch;
            getline(cin, matchSearch);
            mainBracket.findMatch(matchSearch);
        }
        else if(selection == 4){
            cout<<"Who are you searching for?"<<endl;
            cout<<"Player 1: ";
            std::string Search1;
            getline(cin, Search1);
            cout<<"Player 2: ";
            std::string Search2;
            getline(cin, Search2);
            mainBracket.specificSearch(Search1,Search2);
        }
        else if(selection ==5){
            cout<<"What round would you like to view?"<<endl;
            std::string matchSearch;
            getline(cin, matchSearch);
            int roundnumb = atoi(matchSearch.c_str());
            mainBracket.printRound(roundnumb);
        }
        else if(selection == 6){
            cout<<"Which match would you like to undo?"<<endl;
            cout<<"Player 1: ";
            std::string Search1;
            getline(cin, Search1);
            cout<<"Player 2: ";
            std::string Search2;
            getline(cin, Search2);
            match * matchCancel = mainBracket.specificSearch(Search1,Search2);
            mainBracket.undoMatch(matchCancel);
        }
        else if(selection == 7){
            mainBracket.printWinners();
        }
        else if(selection == 8){
            mainBracket.destroyBracket();
        }
        else if(selection == 9){
            mainBracket.createBracket();
        }
        else if(selection == 10){
            cout	<<	"Goodbye!"	<<	endl;
            return 0;
        }
        else{
            cout << "Please select valid selection" <<endl;
        }
    }

}
