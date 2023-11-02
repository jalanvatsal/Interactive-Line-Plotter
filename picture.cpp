//
//  main.cpp
//  Project 3
//
//  Created by Vatsal Jalan on 21/10/2566 BE.
//
#include <iostream>
#include<cctype>
#include<cassert>
using namespace std;
#include "grid.h"

const int HORIZ = 0;
const int VERT = 1;
const int FG = 0;
const int BG = 1;

// line plotting functions
void plotHorizontalLine(int r, int c, int distance, char ch);
void plotVerticalLine(int r, int c, int distance, char ch);
void plotRectangle(int r, int c, int height, int width, char ch);
bool plotLine(int r, int c, int distance, int dir, char plotChar, int fgbg);

//command string proccessing functions
bool processLetterStep(string text, int& pos);
bool processModeStep(string text, int& pos);
bool processClearStep(string text, int& pos);

// execution of commands in command string
bool isValidCommandString(string commandstring);
int performCommands(string commandString, char& plotChar, int& mode, int& badPos);

int main(){
    setSize(8, 20);
    char pc = '%';
    int m = FG;
    int bad = 99;
    assert(performCommands("v2h3h-33", pc, m, bad) == 3 && bad == 4);
    cout << "All tests succeeded " << endl;
}

void plotHorizontalLine(int r, int c, int distance, char ch){
    

    if(distance >= 0){
        // checking if columns exceeds number of columns in grid
        if(c + distance > getCols()){
            for(int i = c; i <= getCols() ; i++){
                setChar(r, i, ch);
            }
        // checking if column argument is negative but its sum with distance is positive
        } else if((c < 0) && (c + distance >= 1)){
            for(int i = 1; i <= c + distance ; i++){
                setChar(r, i, ch);
            }
        } else {
            for(int i = c; i <= c + distance ; i++){
                setChar(r, i, ch);
            }
        }
        
    }
    
}

// similar in structure to plotHorizontalLine
void plotVerticalLine(int r, int c, int distance, char ch){
    
    if(distance >= 0){
        if(r + distance > getRows()){
            for(int i = r; i <= getRows() ; i++){
                setChar(i, c, ch);
            }
        } else if((r < 0) && (r + distance >= 1)){
            for(int i = 1; i <= r + distance ; i++){
                setChar(i, c, ch);
            }
        } else {
            for(int i = r; i <= r + distance ; i++){
                setChar(i, c, ch);
            }
        }
    }
}


// Calling plotHorizontalLine and plotVerticalLine
void plotRectangle(int r, int c, int height, int width, char ch){
    
    plotHorizontalLine(r, c, width, ch);
    plotVerticalLine(r, c + width, height, ch);
    plotHorizontalLine(r + height, c, width, ch);
    plotVerticalLine(r, c, height, ch);
}


bool plotLine(int r, int c, int distance, int dir, char plotChar, int fgbg){
    
    // error checking
    if(!isprint(plotChar)){
        return false;
    }
    
    if(dir != 0 && dir != 1){
        return false;
    }
    
    if(fgbg != 0 && fgbg != 1){
        return false;
    }
    
    if(r < 1 || r > getRows() || c < 1 || c > getCols()){
        return false;
    }
    
    // drawing the actual line (printing characters on screen)
    if(distance > 0 && dir == 0){
        if(c + distance <= getCols()){
            if(fgbg == 0){
                for(int i = c; i <= c + distance; i++){
                    setChar(r, i, plotChar);
                }
            } else if(fgbg == 1){
                for(int i = c; i <= c + distance; i++){
                    if(getChar(r, i) == ' '){
                        setChar(r, i, plotChar);
                    }
                }
            }
        }
    } else if(distance > 0 && dir == 1){
        if(r + distance <= getRows()){
            if(fgbg == 0){
                for(int i = r; i <= r + distance; i++){
                    setChar(i, c, plotChar);
                }
            } else if(fgbg == 1){
                for(int i = r; i <= r + distance; i++){
                    if(getChar(i, c) == ' '){
                        setChar(i, c, plotChar);
                    }
                }
            }
        }
    } else if(distance < 0 && dir == 0){
        if(c + distance >= 1){
            if(fgbg == 0){
                for(int i = c; i >= c + distance; i--){
                    setChar(r, i, plotChar);
                }
            } else if(fgbg == 1){
                for(int i = c; i >= c + distance; i--){
                    if(getChar(r, i) == ' '){
                        setChar(r, i, plotChar);
                    }
                }
            }
        }
    } else if(distance < 0 && dir == 1){
        if(r + distance >= 1){
            if(fgbg == 0){
                for(int i = r; i >= r + distance; i--){
                    setChar(i, c, plotChar);
                }
            } else if(fgbg == 1){
                for(int i = r; i >= r + distance; i--){
                    if(getChar(i, c) == ' '){
                        setChar(i, c, plotChar);
                    }
                }
            }
        }
    } else {
        if(fgbg == 0){
            setChar(r, c, plotChar);
        } else if(fgbg == 1){
            if(getChar(r, c) == ' '){
                setChar(r, c, plotChar);
            }
        }
    }
    
    return true;
}


bool processLetterStep(string text, int& pos){
    // we know that we are at h or v at this point
    
    //checking if h or v is given at the end of command string
    if(pos == text.size() - 1){
        return false;
    }
    
    // count how many characters until next command letter which is allowed to start the next command
    int counter = 0;
    for(int i = pos + 1; i != text.size(); i++){
        if(text.at(i) != 'h' && text.at(i) != 'v'&& text.at(i) != 'b' && text.at(i) != 'f' && text.at(i) != 'c'){
            counter++;
        } else {
            break;
        }
    }
    
    if(counter > 3 || counter == 0){
        return false;
    } else if(counter == 1){
        if(!isdigit(text.at(pos+1))){
            return false;
        }
        pos += 2;
    } else if(counter == 2){
        
        if(text.at(pos + 1) != '-' && !isdigit(text.at(pos + 1))){
            return false;
        }
        
        if(text.at(pos + 1) == '-' && !isdigit(text.at(pos + 2))){
            return false;
        }
        
        if(isdigit(text.at(pos + 1))){
            if(!isdigit(text.at(pos + 2))){
                return false;
            }
        }
        
        pos += 3;
        
    } else if(counter == 3){
        if(text.at(pos + 1) != '-'){
            return false;
        }
        if(!isdigit(text.at(pos + 2)) || !isdigit(text.at(pos + 3))){
            return false;
        }
        pos += 4;
    }
    return true;
}


bool processModeStep(string text, int& pos){
    
    // if 'b' or 'f' is at end of string
    if(pos == text.size() - 1){
        return false;
    }
    
    if(!isprint(text.at(pos + 1))){
        return false;
    }
    
    pos += 2;
    
    return true;
    
}

bool processClearStep(string text, int& pos){
    
    //if c is at end of string that's allowed
    if(pos == text.size() - 1){
        return true;
    }
    
    if(text.at(pos + 1) != 'h' && text.at(pos + 1) != 'v' && text.at(pos + 1) != 'b' && text.at(pos + 1) != 'f' && text.at(pos + 1) != 'c'){
        return false;
    }
    
    pos++;
    
    return true;
}


bool isValidCommandString(string commandstring){
    
    // pre-proccessing of string
    for(int i = 0; i != commandstring.size(); i++){
        commandstring.at(i) = tolower(commandstring.at(i));
    }
    
    int k = 0;
    
    while(k != commandstring.size()){
        
        if(commandstring.at(k) == 'h' || commandstring.at(k) == 'v'){
            if(!processLetterStep(commandstring, k)){
                return false;
            }
        } else if(commandstring.at(k) == 'b' || commandstring.at(k) == 'f'){
            if(!processModeStep(commandstring, k)){
                return false;
            }
        } else if(commandstring.at(k) == 'c'){
            if(!processClearStep(commandstring, k)){
                return false;
            }
        } else {
            return false;
        }
        
    }
    
    return true;
}



int performCommands(string commandString, char& plotChar, int& mode, int& badPos){
    
    if(mode != 0 && mode != 1){
        return 2;
    }
    
    if(!isprint(plotChar)){
        return 2;
    }
    
    int current_row = 1;
    int current_col = 1;
    
    if(!isValidCommandString(commandString)){
        // pre-proccessing of string
        for(int i = 0; i != commandString.size(); i++){
            commandString.at(i) = tolower(commandString.at(i));
        }
        
        int k = 0;
        
        while(k != commandString.size()){
            
            if(commandString.at(k) == 'h' || commandString.at(k) == 'v'){
                
                //checking if h or v is given at the end of command string
                if(k == commandString.size() - 1){
                    badPos = commandString.size();
                    return 1;
                }
                
                // count how many characters until next command letter which is allowed to start the next command
                
                int counter = 0;
                for(int i = k + 1; i != commandString.size(); i++){
                    if(commandString.at(i) != 'h' && commandString.at(i) != 'v'&& commandString.at(i) != 'b' && commandString.at(i) != 'f' && commandString.at(i) != 'c'){
                        counter++;
                    } else {
                        break;
                    }
                }
                
                if(counter == 0){
                    badPos = k;
                    return 1;
                }
    
                if(counter == 1){
                    if(!isdigit(commandString.at(k + 1))){
                        badPos = k + 1;
                        return 1;
                    }
                    k += 2;
                }
                
                if(counter == 2){
                    
                    if(commandString.at(k + 1) != '-' && !isdigit(commandString.at(k + 1))){
                        badPos = k + 1;
                        return 1;
                    }
                    
                    if(commandString.at(k + 1) == '-' && !isdigit(commandString.at(k + 2))){
                        badPos = k + 2;
                        return 1;
                    }
                    
                    if(isdigit(commandString.at(k + 1))){
                        if(!isdigit(commandString.at(k + 2))){
                            badPos = k + 2;
                            return 1;
                        }
                    }
                    
                    k += 3;
                }
                
                
                if(counter >= 3){
                    
                    string substring = commandString.substr(k,4);
                    
                    if(isValidCommandString(substring)){
                        badPos = k + 4;
                        return 1;
                    } else if(!isValidCommandString(substring)){
                        
                        for(int i = 1; i <= 3; i++){
                            if(i == 1){
                                if(!isdigit(substring.at(1)) && substring.at(1) != '-'){
                                    badPos = k + 1;
                                    return 1;
                                }
                            } else if(i == 2){
                                if(!isdigit(substring.at(2))){
                                    badPos = k + 2;
                                    return 1;
                                }
                            } else if(i == 3){
                                if(substring.at(1) == '-' && isdigit(substring.at(2)) && !isdigit(substring.at(3))){
                                    badPos = k + 3;
                                    return 1;
                                } else if(isdigit(substring.at(1)) && isdigit(substring.at(2))){
                                    badPos = k + 3;
                                    return 1;
                                }
                            }
                        }
                        
                    }
                    
                }
                
            } else if(commandString.at(k) == 'b' || commandString.at(k) == 'f'){
                // if 'b' or 'f' is at end of string
                if(k == commandString.size() - 1){
                    badPos = commandString.size();
                    return 1;
                }
                
                if(!isprint(commandString.at(k + 1))){
                    badPos = k+1;
                    return 1;
                }
                
                k += 2;
                
            } else if(commandString.at(k) == 'c'){
                if(commandString.at(k + 1) != 'h' && commandString.at(k + 1) != 'v' && commandString.at(k + 1) != 'b' && commandString.at(k + 1) != 'f' && commandString.at(k + 1) != 'c'){
                    badPos = k + 1;
                    return 1;
                }
                
                k += 1;
                
            } else {
                badPos = k;
                return 1;
            }
        }
    } else if(isValidCommandString(commandString)){
        
        //check if we are going to plot anything out of bounds (return 3 case)
        
        // pre-proccessing of string
        for(int i = 0; i != commandString.size(); i++){
            commandString.at(i) = tolower(commandString.at(i));
        }
        
        
        int k = 0;
        while(k != commandString.size()){
            
            if(commandString.at(k) == 'h' || commandString.at(k) == 'v'){
                
                int counter = 0;
                
                for(int i = k + 1; i != commandString.size(); i++){
                    if(commandString.at(i) != 'h' && commandString.at(i) != 'v'&& commandString.at(i) != 'b' && commandString.at(i) != 'f' && commandString.at(i) != 'c'){
                        counter++;
                    } else {
                        break;
                    }
                }
                
                if(counter == 1 && commandString.at(k) == 'h'){
                    
                    int ones_place = (9 - ('9' - commandString.at(k+1)));
                    
                    // check if it goes more than max col. val
                    if((current_col + ones_place) > getCols()){
                        badPos = k;
                        return 3;
                    } else {
                        plotLine(current_row, current_col, ones_place, 0, plotChar, mode);
                        current_col = current_col + ones_place;
                    }
                    
                    k+= 2;
                    
                } else if(counter == 1 && commandString.at(k) == 'v'){
                    
                    int ones_place = (9 - ('9' - commandString.at(k+1)));
                    
                    // check if it goes more than max row. val
                    if((current_row + ones_place) > getRows()){
                        badPos = k;
                        return 3;
                    } else {
                        plotLine(current_row, current_col, ones_place, 1, plotChar, mode);
                        current_row = current_row + ones_place;
                    }
                    
                    k+= 2;
                    
                } else if(counter == 2 && commandString.at(k + 1) == '-'){
                    if(commandString.at(k) == 'h'){
                        
                        int ones_place = (9 - ('9' - commandString.at(k+2)));
                        
                        // check if it goes less than min col. val
                        if((current_col - ones_place) < 1){
                            badPos = k;
                            return 3;
                        } else {
                            plotLine(current_row, current_col, -1 * ones_place, 0, plotChar, mode);
                            current_col = current_col - ones_place;
                        }
                        
                        k+= 3;
                        
                    } else if(commandString.at(k) == 'v'){
                        
                        int ones_place = (9 - ('9' - commandString.at(k+2)));
                        
                        // check if it less than min row. val
                        if((current_row - ones_place) < 1){
                            badPos = k;
                            return 3;
                        } else {
                            plotLine(current_row, current_col, -1 * ones_place, 1, plotChar, mode);
                            current_row = current_row - ones_place;
                        }
                        
                        k+= 3;
                    }
                } else if(counter == 2 && commandString.at(k + 1) != '-'){
                    
                    if(commandString.at(k) == 'h'){
                        
                        int tens_place = 10 * (9 - ('9' - commandString.at(k+1)));
                        int ones_place = (9 - ('9' - commandString.at(k+2)));
                        int distance = tens_place + ones_place;
                        
                        // check if it goes more than max col. val
                        if((current_col + distance) > getCols()){
                            badPos = k;
                            return 3;
                        } else {
                            plotLine(current_row, current_col, distance , 0 , plotChar, mode);
                            current_col = current_col + distance;
                        }
                        
                        k+= 3;
                        
                    } else if(commandString.at(k) == 'v'){
                        
                        int tens_place = 10 * (9 - ('9' - commandString.at(k+1)));
                        int ones_place = (9 - ('9' - commandString.at(k+2)));
                        int distance = tens_place + ones_place;
                        
                        // check if it goes more than max row. val
                        if((current_row + distance) > getRows()){
                            badPos = k;
                            return 3;
                        } else {
                            plotLine(current_row, current_col, distance , 1, plotChar, mode);
                            current_row = current_row + distance;
                        }
                        
                        k+= 3;
                    }
                    
                } else if(counter == 3){
                    
                    if(commandString.at(k) == 'h'){
                        
                        int tens_place = 10 * (9 - ('9' - commandString.at(k+2)));
                        int ones_place = (9 - ('9' - commandString.at(k+3)));
                        int distance = tens_place + ones_place;
                        
                        // check if it goes less than min col. val
                        if((current_col - distance) < 1){
                            badPos = k;
                            return 3;
                        } else {
                            plotLine(current_row, current_col, -1 * distance , 0 , plotChar, mode);
                            current_col = current_col - distance;
                        }
                        
                        k += 4;
                        
                    } else if(commandString.at(k) == 'v'){
                        
                        int tens_place = 10 * (9 - ('9' - commandString.at(k+2)));
                        int ones_place = (9 - ('9' - commandString.at(k+3)));
                        int distance = tens_place + ones_place;
                        
                        // check if it goes less than min row. val
                        if((current_row - distance) < 1){
                            badPos = k;
                            return 3;
                        } else {
                            plotLine(current_row, current_col, -1 * distance , 1 , plotChar, mode);
                            current_row = current_row - distance;
                        }
                        
                        k+= 4;
                    }
                }
            } else if(commandString.at(k) == 'f' || commandString.at(k) == 'b'){
                
                if(commandString.at(k) == 'f'){
                    mode = FG;
                    plotChar = commandString.at(k+1);
                } else if(commandString.at(k) == 'b'){
                    mode = BG;
                    plotChar = commandString.at(k+1);
                }
                k += 2;
            } else if(commandString.at(k) == 'c'){
                
                // set all grid chars to space chars
                for(int i = 1; i <= getRows(); i++){
                    plotLine(i, 1, getCols() - 1, HORIZ, ' ', FG);
                }
                
                current_row = 1;
                current_col = 1;
                plotChar = '*';
                mode = FG;
                
                k += 1;
            }
        }
    }
    return 0;
}
