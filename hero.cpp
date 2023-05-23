
#include <iostream>
using namespace std;
#include <cctype>
#include <string>

// returns true if given char is representing a color
// returns false if not
bool isColor(char c){
    if (c == 'G' ||c == 'g' || c == 'R' || c == 'r' || c == 'Y' || c == 'y' || c == 'B' || c == 'b' || c == 'O' || c == 'o'){
        return true;
        }
        else return false;
}

//returns the number of beats in tune from position begin to end
int countBeats(int begin, int end, string tune){
    int beats=0;
    while (begin<=end){
    if (tune.at(begin)=='/'){
        beats++;
        begin++;
    }
        else if ((isColor(tune.at(begin))) && (tune.at(begin+1)=='/')){
            beats++;
            begin+=2;
        }
       else if ((isColor(tune.at(begin))) && (isdigit(tune.at(begin+1))) && (tune.at(begin+2)=='/')){
            beats++;
            begin+=3;
        }
        else if ((isColor(tune.at(begin))) && (isdigit(tune.at(begin+1))) && (isdigit(tune.at(begin+2))) && (tune.at(begin+3)=='/')){
            beats++;
            begin+=4;
        }
    }
    //cerr << beats << " beats" << endl;
    return beats;
}


//returns true if tune is syntactically correct, false if not
bool hasProperSyntax(string tune){
    int syntax=0;
    if (tune == "") {//cerr << "has proper Syntax" << endl;
        return true;}
    if ((tune.at(tune.length()-1))!='/') {// cerr << "does not have proper Syntax" << endl;
        return false;}
    if (isdigit(tune.at(0))) {//cerr << "does not have proper Syntax" << endl;
        return false;}

    
    for(int i=0; i<tune.size(); i++){
        if (tune.at(i)=='/' ||
            ((isColor(tune.at(i))) && (tune.at(i+1)=='/')) ||
             ((isColor(tune.at(i))) && (isdigit(tune.at(i+1))) && (tune.at(i+2)=='/')) ||
            ((isColor(tune.at(i))) && (isdigit(tune.at(i+1))) && (isdigit(tune.at(i+2))) && (tune.at(i+3)=='/')) ||
            ((isdigit(tune.at(i))) && (isColor(tune.at(i-1))) && (tune.at(i+1)=='/')) ||
            ((isdigit(tune.at(i))) && (isColor(tune.at(i-1))) && (isdigit(tune.at(i+1)))) ||
             ((isdigit(tune.at(i))) && (isdigit(tune.at(i-1))) && (tune.at(i+1)=='/'))) {
            syntax=1;
        }
        
        else {//cerr << "does not have proper Syntax" << endl;
            return false;}
    }
              
    if (syntax==1){//cerr << "has proper Syntax" << endl;
        return true;}
    else {//cerr << "does not have proper Syntax" << endl;
        return false;}
}

//checks if a sustained note has the correct number of slashes
//returns the incorrect beat if slashes are incorrect
//returns -1 if slashes are correct
int checkSustainSlashes(string tune){
    int numSlashes;
    for (int i = 0; i<tune.length(); i++ ){
    if ((isdigit(tune.at(i))) && isdigit(tune.at(i+1))){
        numSlashes = 10*(tune.at(i)- '0') + (tune.at(i+1)- '0');
        for (int j = i+2; j< (i+2+numSlashes); j++){
            if (tune.at(j)!='/'){
                //cerr << "slashes are incorrect at beat " << countBeats(0, j, tune) << endl;
                return countBeats(0, j, tune);
            }
        }
    }
    
   else if (isdigit(tune.at(i))){
        numSlashes = tune.at(i) - '0';
        for (int j = i+1; j<(i+1+numSlashes) && j < tune.length(); j++){
            
            if (tune.at(j)!='/'){
                //cerr << "slashes are incorrect at beat " << countBeats(0, j, tune) << endl;
                return countBeats(0, j, tune);
            }
        }
    }
    }
    //cerr << "slashes are correct " << endl;
    return -1;
}

// checks if a sustained note length is less than 2
// if it is too short, returns the number beat which specifies the note which is too short
// if they are correct, returns -1
int checkSustainLength(string tune){
    for (int i = 0; i<tune.size(); i++){
        if (isColor(tune.at(i))){
            if (((tune.at(i+1)=='0') && (tune.at(i+2)=='/')) ||
                ((tune.at(i+1)=='1') && (tune.at(i+2)=='/')) ||
                ((tune.at(i+1)=='0') && (tune.at(i+2)=='0') && (tune.at(i+3)=='/')) ||
                 ((tune.at(i+1)=='0') && (tune.at(i+2)=='1') && (tune.at(i+3)=='/'))){
               // cerr << "sustained note is too short at beat " << countBeats(0, i, tune) << endl;
                return countBeats(0, i, tune);
            }
        }
    }
   // cerr << "sustained notes are long enough" << endl;
    return -1;
}

//checks if tune ends prematurely
//if it does, returns the number of beats of tune plus 1
//if not, it returns -1
int checkPremature(string tune){
    int numSlashes;
    for (int i = (tune.size()-1); i > 0; i--){
//        if ((tune.at(i)!='/') && (!isdigit(tune.at(i)))){
//            return 0;
//        }
        if (isdigit(tune.at(i)) && (isdigit(tune.at(i-1)))){
            numSlashes = (tune.at(i-1)- '0')*10 + (tune.at(i)- '0');
            if (numSlashes > (tune.size()-1-i)){
              //  cerr << "ends prematurely, bad beat is " << ((countBeats(0, tune.length()-1, tune))+1) << "beats";
                return (countBeats(0, tune.length()-1, tune) +1);
            }
        }
        else if (isdigit(tune.at(i))){
            numSlashes =(tune.at(i)- '0');
            if (numSlashes > (tune.size()-1-i)){
               // cerr << "ends prematurely, bad beat is " << ((countBeats(0, tune.length()-1, tune))+1) << "beats";
                return (countBeats(0, tune.length()-1, tune) +1);
            }
        }
    }
   // cerr << "does not end prematurely" << endl;
    return -1;
}

//converts tune to instructions
string convertible(string tune){
    string instructions="";
    int begin = 0;
    int end = tune.length();
    int numSlashes=0;
    if (tune == ""){return tune;}
    while (begin<end){
    
    if ((isColor(tune.at(begin))) && (isdigit(tune.at(begin+1))) && (isdigit(tune.at(begin+2))) && (tune.at(begin+3)=='/')){
        numSlashes=10*(tune.at(begin+1) - '0') + (tune.at(begin+2)- '0');
        for (int i =0; i<numSlashes; i++){
        instructions+=toupper(tune.at(begin));
        }
         begin+=(3+numSlashes);
    }
    else if ((isColor(tune.at(begin))) && (isdigit(tune.at(begin+1))) && (tune.at(begin+2)=='/')){
        numSlashes=(tune.at(begin+1)- '0');
        for (int i =0; i<numSlashes; i++){
        instructions+=toupper(tune.at(begin));
        }
         begin+=(2+numSlashes);
     }
        else if ((isColor(tune.at(begin))) && (tune.at(begin+1)=='/')){
            instructions+=tolower(tune.at(begin));
            begin+=2;
        }
       else if (tune.at(begin)=='/'){
            instructions+="x";
            begin++;
        }
    }
   // cerr << "instructions are " << instructions << endl;
    return instructions;
}

int convertTune(string tune, string& instructions, int& badBeat){
    
    //if syntax incorrect, badBeat unchanged, returns 1
    if (!hasProperSyntax(tune)){
        return 1;
    }
    // syntax correct but ends early, badbeat is 1 more than number of beats in string, returns 4
    if(hasProperSyntax(tune) && checkPremature(tune)!=-1){
        if (checkSustainSlashes(tune)==-1 && checkSustainLength(tune)==-1){
        badBeat = checkPremature(tune);
            return 4;}
    }
    // syntactically correct, but sustained note beat is incorrect, badBeat is set to number of that beat, returns 2
    if(hasProperSyntax(tune) && (checkSustainSlashes(tune)!=-1)){
        badBeat = checkSustainSlashes(tune);
        return 2;
    }
    
    // syntax correct, but sustained note length too short, badbeat is that beat, returns 3
    if(hasProperSyntax(tune) && (checkSustainLength(tune)!=-1)){
        badBeat = checkSustainLength(tune);
        return 3;
    }
    //if convertible, instructions=conversion, badBeat unchanged, returns 0
    else{
        instructions = convertible(tune);
        return 0;
    }
}

int main() {
    string instructions = " ";
    int badBeat = 0;
    cout << "Enter tune: ";
    string tune;
    getline(cin, tune);
    //cerr << hasProperSyntax(tune) << endl;
    cerr << convertTune(tune, instructions, badBeat);
    cerr << "instructions are " << instructions << " badBeat is " << badBeat << endl;
    
    
    return 0;
}
