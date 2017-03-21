//
//  main.cpp
//  Project1
//
//  Created by John Maxey on 1/11/17.
//  Copyright © 2017 John Maxey. All rights reserved.
//
#include <getopt.h>
#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <sstream>
#include <string.h>
#include <string>
#include <algorithm>
#include <stdio.h>
using namespace std;

string getMode(int argc, char *argv[], string &beginWord, string &endWord,
               string &outputMode, bool &change, bool &swap, bool &length);
int createDictionary(vector<string> &dictionary, char &dict, int lines,
                     string beginWord, string endWord);
bool changeWord(string &current, string &compare);
bool swapWord(string &current, string &compare);
bool lengthWord(string current, string compare);
void outputResults(vector<string> &result, string &outputMode, ostringstream &os);

int main(int argc, char * argv[]) {
    ios_base::sync_with_stdio(false);
    ostringstream os;
    
    bool change = false;
    bool swap = false;
    bool length = false;
    string beginWord;
    string endWord;
    string outputMode = "W";
    string mode = getMode(argc, argv, beginWord, endWord, outputMode, change, swap, length);
    
    if((swap == true || change == true) && length == false &&
       (beginWord.length() != endWord.length())){
        cerr << "Error: invalid commands specified with begin and end strings" << endl;
        exit(1);
    }
    
    //read in type of dictionary and number of words in dictionary
    char dict;
    int lines;
    cin >> dict;
    cin >> lines;
    
    //read in all the words into dictionary vector
    vector <string> dictionary;
    if(dict == 'S'){
        dictionary.reserve(size_t(lines));
    }else if(dict == 'C'){
        dictionary.reserve(size_t(lines*2));
    }
    
    int firstWord = createDictionary(dictionary, dict, lines, beginWord, endWord);
    size_t dictSize = dictionary.size();
    vector<bool> visited(dictSize, false);
    vector<int> previous(dictSize, -1);
    vector<string> result;

    //decide what routing scheme and dictionary to use
    if(mode == "stack"){
        stack<string> stack; //create stack
        visited[size_t(firstWord)] = true; //make first word visited = true
        stack.push(dictionary[size_t(firstWord)]); //push first word onto stack
        vector<string>::iterator it;
        string current;
        size_t done = 0;
        //LOOKING FOR END WORD
        while(current != endWord){
            //if queue is empty then there is no solution
            if(stack.empty()){
                os << "No solution." << "\n";
                goto noSolution;
            }
            //current is next in queue, delete it from queue
            current = stack.top();
            //cout << current << endl;
            stack.pop();
            it = find(dictionary.begin(), dictionary.end(), current);
            auto pos = it - dictionary.begin();
            //start looking through dictionary for words to add to stack
            for(size_t a = 0; a < dictSize; ++a){
                bool found = false;
                
            //change setting is on, has not been added to stack
                if(change == true && visited[a] == false
                   && (current.length() == dictionary[a].length())){
                    if(changeWord(current, dictionary[a]) == true){
                        visited[a] = true;
                        previous[a] = int(pos);
                        if(dictionary[a] == endWord){
                            current = endWord;
                            done = a;
                            break;
                        }
                        stack.push(dictionary[a]);
                        found = true;
                    }
                }//if(change == true)
                
            //swap setting is on, has not been added to stack, was not found in change
                if(swap == true && visited[a] == false && found == false
                   && current.length() == dictionary[a].length()){
                    if(swapWord(current, dictionary[a]) == true){
                        visited[a] = true;
                        previous[a] = int(pos);
                        if(dictionary[a] == endWord){
                            current = endWord;
                            done = a;
                            break;
                        }
                        stack.push(dictionary[a]);
                        found = true;
                    }
                }//if(swap == true)
            //length setting is on, has not been added to stack, not found in change or swap
                if(length == true && visited[a] == false && found == false &&
                   (current.length() - dictionary[a].length() == 1 ||
                    dictionary[a].length()-current.length() == -1)){
                    if(lengthWord(current, dictionary[a]) == true){
                        visited[a] = true;
                        previous[a] = int(pos);
                        if(dictionary[a] == endWord){
                            current = endWord;
                            done = a;
                            break;
                        }
                        stack.push(dictionary[a]);
                    }
                }//if(length == true)
                
            }//for()
            
            
        }//while()
        
        //ADD USED WORDS TO RESULT VECTOR
        while(previous[done] != -1){
            result.push_back(dictionary[done]);
            done = size_t(previous[done]);
        }
        result.push_back(dictionary[size_t(firstWord)]);
        
        outputResults(result, outputMode, os);
        
    }else if(mode == "queue"){
        queue<string> queue;
        visited[size_t(firstWord)] = true;
        queue.push(dictionary[size_t(firstWord)]);
        vector<string>::iterator it;
        string current;
        size_t done = 0;
        //LOOKING FOR END WORD
        while(current != endWord){
            //if queue is empty then there is no solution
            if(queue.empty()){
                os << "No solution." << "\n";
                goto noSolution;
            }
            current = queue.front();
            queue.pop();
            it = find(dictionary.begin(), dictionary.end(), current);
            auto pos = it - dictionary.begin();
            //start looking through dictionary for words to add to stack
            for(size_t a = 0; a < dictSize; ++a){
                bool found = false;
                
                //change setting is on, has not been added to stack
                if(change == true && visited[a] == false
                   && current.length() == dictionary[a].length()){
                    if(changeWord(current, dictionary[a]) == true){
                        visited[a] = true;
                        previous[a] = int(pos);
                        if(dictionary[a] == endWord){
                            current = endWord;
                            done = a;
                            break;
                        }
                        queue.push(dictionary[a]);
                        found = true;
                    }
                }//if(change == true)
                
                //swap setting is on, has not been added to stack, was not found in change
                if(swap == true && visited[a] == false && found == false
                   && current.length() == dictionary[a].length()){
                    if(swapWord(current, dictionary[a]) == true){
                        visited[a] = true;
                        previous[a] = int(pos);
                        if(dictionary[a] == endWord){
                            current = endWord;
                            done = a;
                            break;
                        }
                        queue.push(dictionary[a]);
                        found = true;
                    }
                }//if(swap == true)
                //length setting is on, has not been added to stack, not found in change or swap
                if(length == true && visited[a] == false && found == false &&
                   (current.length() - dictionary[a].length() == 1 ||
                    dictionary[a].length()-current.length() == -1)){
                    if(lengthWord(current, dictionary[a]) == true){
                        visited[a] = true;
                        previous[a] = int(pos);
                        if(dictionary[a] == endWord){
                            current = endWord;
                            done = a;
                            break;
                        }
                        queue.push(dictionary[a]);
                    }
                }//if(length == true)
                
            }//for()
            
        }//while()
        
        //ADD USED WORDS TO RESULT VECTOR
        while(previous[done] != -1){
            result.push_back(dictionary[done]);
            done = size_t(previous[done]);
        }
        result.push_back(dictionary[size_t(firstWord)]);
        
        outputResults(result, outputMode, os);
    }
    
noSolution:
    cout << os.str();
    return 0;
}



///////////////////
//OUTPUT FUNCTION//
///////////////////
void outputResults(vector<string> &result, string &outputMode, ostringstream &os){
    size_t resultSize = result.size();
    //OUTPUT FINAL RESULT
    if(outputMode == "W"){
        os << "Words in morph: " << resultSize << "\n";
        for(size_t t = 0; t < resultSize; ++t){
            os << result[resultSize-t-1] << "\n";
        }
    }else if(outputMode == "M"){
        os << "Words in morph: " << resultSize << "\n";
        os << result[resultSize-1] << "\n";
        for(size_t t = 0; t < resultSize-1; ++t){
            if(result[resultSize-t-1].size() != result[resultSize-t-2].size()){
                bool last = false;
                //insert or delete (length)
                if(result[resultSize-t-1].length() - result[resultSize-t-2].length() == 1){
                    //delete
                    for(size_t s = 0; s < result[resultSize-t-2].size(); ++s){
                        if(result[resultSize-t-1][s] != result[resultSize-t-2][s]){
                            last = true;
                            os << "d," << s << "\n";
                            break;
                        }
                    }
                    if(last == false){
                        os << "d," << result[resultSize-t-2].size() << "\n";
                    }
                }else if(result[resultSize-t-2].length() -
                         result[resultSize-t-1].length() == 1){
                    //insert
                    for(size_t s = 0; s < result[resultSize-t-1].size(); ++s){
                        if(result[resultSize-t-1][s] != result[resultSize-t-2][s]){
                            last = true;
                            os << "i," << s << "," << result[resultSize-t-2][s] << "\n";
                            break;
                        }
                    }
                    if(last == false){
                        os << "i," << result[resultSize-t-2].size() - 1 << ",";
                        os << result[resultSize-t-2][result[resultSize-t-2].size() - 1] << "\n";
                    }
                }
            }else{
                for(size_t s = 0; s < result[resultSize-t-1].size(); ++s){
                    if(result[resultSize-t-1][s] != result[resultSize-t-2][s]){
                        //swap or change
                        if(result[resultSize-t-1][s] == result[resultSize-t-2][s+1] &&
                           result[resultSize-t-1][s+1] == result[resultSize-t-2][s]){
                            //swap
                            os << "s," << s << "\n";
                            break;
                        }else{
                            //change
                            os << "c," << s << "," << result[resultSize-t-2][s] << "\n";
                            break;
                        }
                        
                    }
                }//for()
            }//else()
        }//for()
    }//else if()
}
//////////////////////////
//END OF OUTPUT FUNCTION//
//////////////////////////



///////////////////
//CHANGE FUNCTION//
///////////////////
bool changeWord(string &current, string &compare){

    int lettersDiff = 0;
    for(size_t i = 0; i < current.length(); ++i){
        //if more than one letter is different it cannot be added
        if(lettersDiff > 1){
            return false;
        }
        if(current[i] != compare[i]){
            ++lettersDiff;
        }
    }
    
    if(lettersDiff == 1){
        return true;
    }
    return false;
}
//////////////////////////
//END OF CHANGE FUNCTION//
//////////////////////////



/////////////////
//SWAP FUNCTION//
/////////////////
bool swapWord(string &current, string &compare){

    int lettersDiff = 0;
    bool swapable = false;
    for (size_t i = 0; i < current.length(); ++i){
        if(lettersDiff > 2){
            return false;
        }
        if(current[i] != compare[i]){
            ++lettersDiff;
            if(current[i] == compare[i+1] && current[i+1] == compare[i]){
                swapable = true;
            }
        }
    }
    if(lettersDiff == 2 && swapable == true){
        return true;
    }
    return false;
}
////////////////////////
//END OF SWAP FUNCTION//
////////////////////////



///////////////////
//LENGTH FUNCTION//
///////////////////
bool lengthWord(string current, string compare){
    
    if(current.length() - compare.length() == 1){
        for(size_t i = 0; i < compare.length(); ++i){
            if(current[i] != compare[i]){
                current.erase(current.begin()+int(i));
                if(current == compare){
                    return true;
                }else{
                    return false;
                }
            }
        }
        return true;
    }else if(compare.length() - current.length() == 1){
        for(size_t i = 0; i < current.length(); ++i){
            if(current[i] != compare[i]){
                compare.erase(compare.begin()+int(i));
                if(current == compare){
                    return true;
                }else{
                    return false;
                }
            }
        }
        return true;
    }else{
        return false;
    }
    
    return false;
}
//////////////////////////
//END OF LENGTH FUNCTION//
//////////////////////////



//////////////////////////////
//CREATE DICTIONARY FUNCTION//
//////////////////////////////
int createDictionary(vector<string> &dictionary, char &dict, int lines,
                     string beginWord, string endWord){
    
    string next;
    bool first = false;
    bool last = false;
    int firstWord = 0;
    if(dict == 'S'){
        for(int i = 0; i < lines; ++i){
            cin >> next;
            if(next[0] == '/' && next[1] == '/'){
                getline(cin, next);
                i--;
            }else{
                dictionary.push_back(next);
            }
            if(next == beginWord){
                firstWord = i;
                first = true;
            }else if(next == endWord){
                last = true;
            }
        }
    }else if(dict == 'C'){
        for(int i = 0; i < lines; ++i){
            cin >> next;
            if(next[0] == '/' && next[1] == '/'){
                getline(cin, next);
                i--;
            }else if(next.find('&') != string::npos){
                next.erase(next.end()-1);
                dictionary.push_back(next);
                reverse(next.begin(), next.end());
                dictionary.push_back(next);
            }else if(next.find('[') != string::npos){
                size_t place = next.find('[');
                size_t place2 = next.find(']');
                string sub = next.substr(place+1, place2-place);
                next.erase(place, place2-place+1);
                for(size_t j = 0; j < sub.size()-1; ++j){
                    next.insert(next.begin()+int(place), sub[j]);
                    dictionary.push_back(next);
                    next.erase(next.begin()+int(place));
                }
            }else if(next.find('!') != string::npos){
                size_t place = next.find('!');
                next.erase(next.begin()+int(place));
                dictionary.push_back(next);
                swap(next[place-1], next[place-2]);
                dictionary.push_back(next);
            }else if(next.find('?') != string::npos){
                size_t place = next.find('?');
                next.erase(next.begin()+int(place));
                dictionary.push_back(next);
                next.insert(next.begin()+int(place), next[place-1]);
                dictionary.push_back(next);
            }else{
                dictionary.push_back(next);
            }
        }
        for(size_t k = 0; k < dictionary.size(); ++k){
            if(dictionary[k] == beginWord){
                firstWord = int(k);
                first = true;
            }else if(dictionary[k] == endWord){
                last = true;
            }
        }
    }
    
    if(first == false || last == false){
        cerr << "Error: begin or end word not in dictionary" << endl;
        exit(1);
    }
    return firstWord;
}
////////////////////////////
//END OF CREATE DICTIONARY//
////////////////////////////


////////////////////
//GETMODE FUNCTION//
////////////////////
string getMode(int argc, char * argv[], string &beginWord, string &endWord,
               string &outputMode, bool &change, bool &swap, bool &length)
{
    string mode = "stack";
    bool scheme = false;
    bool edit = false;
    // These are used with getopt_long()
    opterr = true; // Give us help with errors
    int choice;
    int option_index = 0;
    static struct option long_options[] = {
        { "stack",          no_argument,        nullptr, 's'},
        { "queue",          no_argument,        nullptr, 'q'},
        { "change",         no_argument,        nullptr, 'c'},
        { "swap",           no_argument,        nullptr, 'p'},
        { "length",         no_argument,        nullptr, 'l'},
        { "output",         required_argument,  nullptr, 'o'},
        { "begin",          required_argument,  nullptr, 'b'},
        { "end",            required_argument,  nullptr, 'e'},
        { "help",           no_argument,        nullptr, 'h'},
        { nullptr,          0,                  nullptr, '\0'}
    };
    
    // TODO: Fill in the double quotes, to match the mode and help options.
    while ((choice = getopt_long(argc, argv, "sqcplo:b:e:h", long_options, &option_index))
           != -1) {
        switch(choice) {
            case 's':
                if(scheme == true){
                    cerr << "Error: invalid routing scheme specification" << endl;
                    exit(1);
                }
                mode = "stack";
                scheme = true;
                break;
            case 'q':
                if(scheme == true){
                    cerr << "Error: invalid routing scheme specification" << endl;
                    exit(1);
                }
                mode = "queue";
                scheme = true;
                break;
            case 'c':
                change = true;
                if(edit == false){
                    edit = true;
                }
                break;
            case 'p':
                swap = true;
                edit = true;
                break;
            case 'l':
                length = true;
                edit = true;
                break;
            case 'o':
                outputMode = optarg;
                if(outputMode != "W" && outputMode != "M"){
                    cerr << "Error: invalid priority queue type: " << outputMode << endl;
                    exit(1);
                }
                break;
            case 'b':
                beginWord = optarg;
                if(beginWord.empty()){
                    cerr << "Error: beginning word not specified" << endl;
                    exit(1);
                }
                break;
            case 'e':
                endWord = optarg;
                if(endWord.empty()){
                    cerr << "Error: end word not specified" << endl;
                    exit(1);
                }
                break;
            case 'h':
                cerr << "This program navigates through a dictionary to change a word" << endl;
                exit(0);
        } // switch
    } // while
    
    if(scheme == false){
        cerr << "Error: invalid routing scheme specification" << endl;
        exit(1);
    }else if(edit == false){
        cerr << "Error: must specify either change, length, or swap setting" << endl;
        exit(1);
    }
    
    return mode;
}
///////////////////
//END OF GET MODE//
///////////////////



