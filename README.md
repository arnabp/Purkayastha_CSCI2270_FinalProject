# Purkayastha_CSCI2270_FinalProject

This program was creating for the Final Project of CSCI 2270's Spring 2016 class by Arnab Purkayastha.

### Project Summary

This program is a *bracket creator*. Using pre-seeded player info, the program can sort players by their seed and subsequently create a table to show the match order for a fair bracket. This program supports brackets with hundreds of players and will create brackets with uneven numbers of players. Users can then submit match reports in the bracket to fill out the bracket, track progress of individual players within the bracket, view the winners of the bracket, and cancel matches within the bracket.

### How to Run

To run the program, download the zip of the project, unpack, and navigate to the folder within terminal. Execute with:

<pre><code> ./FinalProject bracket0.txt </code></pre>

For the program to work, include a txt file in the directory of the program. To help, a test file "bracket0.txt" is included. The txt file must contain each players name followed by their rank, separated by a comma. Players must be pre-sorted by rank starting from 1. The name of the test file must then be included as a command argument when running the file. All commands are listed in the menu that is displayed when running the program, and select options by inputting a number for the appropriate option.

### Dependencies

No additional libraries are required.

### System Requirements

Program must run on 64-bit Linux architecture.

### Group Members

None

### Contributers

None

### Known Bugs

- Deleting result of player that does not exist will cause segmentation fault
- Finding next matchup or specific matchup of players that do not exist will cause segmentation fault
