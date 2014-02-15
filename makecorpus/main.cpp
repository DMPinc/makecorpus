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
    bool isParsingChord = false;
    vector<string> chordStack;
    int countOfTracks = 0;
    int lastEndTime;
    unordered_map<string, int> table;
    while (std::getline(fin, line)) {
        //cerr << line << endl;
        
        if (line == "MTrk") {
            isTrackStarting = true;
            cerr << "Track " << countOfTracks << endl;
            table.clear();
            continue;
        } else if (line == "TrkEnd") {
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
        
        //  parse data
        istringstream iss(line);
        vector<string> tokens;
        copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter<vector<string> >(tokens));
        //for(vector<string>::const_iterator i = tokens.begin(); i != tokens.end(); ++i) cout << *i << ' ';
        
        string type = tokens[1];
        if (type == "Par" || type == "Meta" || type == "Pb" || type == "PrCh") {
            continue;
        }
        else if (type == "On") {
            int time = stoi(tokens[0]);//ok
            //  int channel = stoi(tokens[2].substr(3, tokens[2].length()));
            string tone = tokens[3].substr(2, tokens[3].length());
            int volume = stoi(tokens[4].substr(2, tokens[4].length()));
            
            //  登録
            if (volume == 100) {
                
                //  無音処理
                int nosoundDuration = time - lastEndTime;
                if (nosoundDuration > 0) {
                    cout << nosoundDuration << "+" << "sp" << " ";
                }
                
                table.insert(make_pair(tone, time));
            }
            //  ゲートタイムを取得
            else if (volume == 0) {
                lastEndTime = time;
                
                //  和音処理.
                if (table.find(tone) == table.end()) {
                    isParsingChord = true;
                    
                } else {
                    int startTime = table[tone];
                    int length = time - startTime;
                    
                    //  出力する
                    cout << length << "+" << tone << " ";
                    
                    //  登録済みのを消す
                    table.erase(tone);
                }
            }
        }
    }
    
    return 0;
}

