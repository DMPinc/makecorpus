//
//  main.cpp
//  train_midi
//
//  Created by Yohei Yamaguchi on 2014/02/15.
//  Copyright (c) 2014年 Yohei Yamaguchi. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <unordered_map>
#include <sstream>

int main(int argc, const char * argv[])
{
    using namespace std;
    
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " SMF training_data \n";
        return 1;
    }
    
    ifstream fin;
    fin.open(argv[1]);
    if (!fin) {
        std::cerr << "Can't open file: " << argv[1];
        return 1;
    }
    
    string line;
    bool isTrackStarting = false;
    bool isChordSilentDone = false;
    int countOfTracks = 0;
    int lastEndTime;
    unordered_map<string, int> table;
    vector<string> chordStack;
    while (std::getline(fin, line)) {
        //cerr << "dEbug:" << line << endl;
        
        //  skip sentence from #
        if (line.length() > 1 && line[0] == '#') {
            continue;
        }
        
        // begining of tack
        if (line == "MTrk") {
            isTrackStarting = true;
            cerr << "Track " << countOfTracks << endl;
            table.clear();
            continue;
        }
        // end of track
        else if (line == "TrkEnd") {
            isTrackStarting = false;
            ++countOfTracks;
            //cerr << "FINISH!!!" << endl << endl;
            cout << endl;
            table.clear();
            continue;
        }
        
        if (!isTrackStarting) {
            continue;
        }
        
        // data part of track
        istringstream iss(line);
        vector<string> tokens;
        copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter<vector<string> >(tokens));
        
        string type = tokens[1];
        if (type == "Par" || type == "Meta" || type == "Pb" || type == "PrCh" || type == "SysEx") {
            continue;
        }
        else if (type == "On") {
            int time = stoi(tokens[0]);
            //  int channel = stoi(tokens[2].substr(3, tokens[2].length()));
            string tone = tokens[3].substr(2, tokens[3].length());
            int volume = stoi(tokens[4].substr(2, tokens[4].length()));
            
            //  start plyaing sound
            if (volume == 100) {
                
                //  no sound duration
                if (!isChordSilentDone) {
                    int nosoundDuration = time - lastEndTime;
                    if (nosoundDuration > 0) {
                        cout << nosoundDuration << "+" << "sp" << " ";
                    }
                    isChordSilentDone = true;
                }
            
                table[tone] = time;
            }
            //  finish playing sound
            else if (volume == 0) {
                isChordSilentDone = false;
                lastEndTime = time;
                
                if (table.find(tone) == table.end()) {
                    //  not found
                } else {
                    int prevStartTime = table[tone];
                    table.erase(tone);
                    
                    //  和音処理. table の要素数が0でない場合、出力せず貯めこむ
                    if (table.empty()) {
                        int length = time - prevStartTime;
                        
                        string toneString;
                        if (chordStack.empty()) {
                            toneString = tone;
                        } else {
                            toneString = "{";
                            stringstream ss;
                            
                            chordStack.push_back(tone);
                            for(size_t i = 0; i < chordStack.size(); ++i)
                            {
                                if(i != 0)
                                    ss << ",";
                                ss << chordStack[i];
                            }
                            toneString += ss.str();
                            toneString += "}";
                            
                            chordStack.clear();
                        }

                        //  出力する
                        cout << length << "+" << toneString << " ";
                    }
                    else {
                        //  和音を貯めこむ
                        chordStack.push_back(tone);
                    }
                }
            }
        }
    }
    
    return 0;
}

