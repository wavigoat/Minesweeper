//
// Created by katie on 4/15/2024.
//

#ifndef MINESWEEPER_BOARD_H
#define MINESWEEPER_BOARD_H
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <map>
#include <cctype>
#include <fstream>
#include <SFML/Graphics.hpp>
using namespace std;

struct DigitsManager{
    sf::Texture digits;
    sf::Texture *ptr;

    void loaddigits(){
        digits.loadFromFile("files/images/digits.png");
        ptr = &digits;
    };
    void setdigit(sf::Sprite &display, int num){
        display.setTexture(*ptr);
        display.setTextureRect(sf::IntRect(num*21, 0, 21, 32));
    }
    void alldigit(sf::Sprite &display, int num){
        if (num == 0){
            setdigit(display, 0);
        }
        else if (num == 1){
            setdigit(display, 1);
        }
        else if (num == 2){
            setdigit(display, 2);
        }
        else if (num == 3){
            setdigit(display, 3);
        }
        else if (num == 4){
            setdigit(display, 4);
        }
        else if (num == 5){
            setdigit(display, 5);
        }
        else if (num == 6){
            setdigit(display, 6);
        }
        else if (num == 7){
            setdigit(display, 7);
        }
        else if (num == 8){
            setdigit(display, 8);
        }
        else if (num == 9){
            setdigit(display, 9);
        }
    }
    void ifnegative(sf::Sprite &display){
        display.setTexture(*ptr);
        display.setTextureRect(sf::IntRect(210, 0, 21, 32));
    }

};

struct TextureManager{
    map<string, sf::Texture*> textmap;
    sf::Texture debugtexture;
    sf::Texture pausetexture;
    sf::Texture playtexture;
    sf::Texture leaderboard;
    sf::Texture happy;
    sf::Texture lose;
    sf::Texture win;

    sf::Texture hidden;
    sf::Texture revealed;
    sf::Texture flag;



    sf::Texture mine;
    sf::Texture one;
    sf::Texture two;
    sf::Texture three;
    sf::Texture four;
    sf::Texture five;
    sf::Texture six;
    sf::Texture seven;
    sf::Texture eight;

    void loadtextures() {
        debugtexture.loadFromFile("files/images/debug.png");
        textmap["debug"] = &debugtexture;
        pausetexture.loadFromFile("files/images/pause.png");
        textmap["pause"] = &pausetexture;
        playtexture.loadFromFile("files/images/play.png");
        textmap["play"] = &playtexture;
        leaderboard.loadFromFile("files/images/leaderboard.png");
        textmap["leaderboard"] = &leaderboard;

        happy.loadFromFile("files/images/face_happy.png");
        textmap["happy"] = &happy;
        lose.loadFromFile("files/images/face_lose.png");
        textmap["lose"] = &lose;
        win.loadFromFile("files/images/face_win.png");
        textmap["win"] = &win;


        hidden.loadFromFile("files/images/tile_hidden.png");
        textmap["hidden"] = &hidden;
        revealed.loadFromFile("files/images/tile_revealed.png");
        textmap["revealed"] = &revealed;
        flag.loadFromFile("files/images/flag.png");
        textmap["flag"] = &flag;

        mine.loadFromFile("files/images/mine.png");
        textmap["mine"] = &mine;

        one.loadFromFile("files/images/number_1.png");
        textmap["one"] = &one;
        two.loadFromFile("files/images/number_2.png");
        textmap["two"] = &two;
        three.loadFromFile("files/images/number_3.png");
        textmap["three"] = &three;
        four.loadFromFile("files/images/number_4.png");
        textmap["four"] = &four;
        five.loadFromFile("files/images/number_5.png");
        textmap["five"] = &five;
        six.loadFromFile("files/images/number_6.png");
        textmap["six"] = &six;
        seven.loadFromFile("files/images/number_7.png");
        textmap["seven"] = &seven;
        eight.loadFromFile("files/images/number_8.png");
        textmap["eight"] = &eight;
    }
};

void recursion6(vector<vector<int>> &topgrid, vector<vector<int>> &grid, int p, int q) {
    if (grid[p][q] == 0) {
        topgrid[p][q] = 0;
        if (q > 0) {
            topgrid[p][q-1] = 0;
            recursion6(topgrid, grid, p, q - 1);
            if (p > 0) {
                topgrid[p - 1][q - 1] = 0;
                recursion6(topgrid, grid, p - 1, q - 1);
            }
            if (p + 1 < topgrid.size()) {
                topgrid[p + 1][q - 1] = 0;
                recursion6(topgrid, grid, p + 1, q - 1);
            }
        }
    }
}
void recursion8(vector<vector<int>> &topgrid, vector<vector<int>> &grid, int p, int q){
    if(grid[p][q]==0) {
        topgrid[p][q] = 0;
        if (q + 1 < topgrid[p].size()) {
            topgrid[p][q+1] = 0;
            recursion8(topgrid, grid, p + 1, q+1);
            if (p > 0) {
                topgrid[p - 1][q + 1] = 0;
                recursion8(topgrid, grid, p - 1, q + 1);
            }
            if (q + 1 < topgrid[p].size()) {
                topgrid[p + 1][q + 1] = 0;
                recursion8(topgrid, grid, p + 1, q + 1);
            }
        }
    }
}
void recursion5(vector<vector<int>> &topgrid, vector<vector<int>> &grid, int p, int q) {
    if (grid[p][q] == 0) {
        topgrid[p][q] = 0;
        if (p > 0) {
            topgrid[p - 1][q] = 0;
            recursion5(topgrid, grid, p - 1, q);
            if (q > 0) {
                topgrid[p - 1][q - 1] = 0;
                recursion5(topgrid, grid, p - 1, q - 1);
            }
            if (q + 1 < topgrid[p].size()) {
                topgrid[p - 1][q + 1] = 0;
                recursion5(topgrid, grid, p - 1, q + 1);
            }
        }
    }
}
void recursion7(vector<vector<int>> &topgrid, vector<vector<int>> &grid, int p, int q){
    if(grid[p][q]==0) {
        topgrid[p][q] = 0;
        if (p + 1 < topgrid.size()) {
            topgrid[p + 1][q] = 0;
            recursion7(topgrid, grid, p + 1, q);
           if (q > 0) {
                topgrid[p + 1][q - 1] = 0;
                recursion7(topgrid, grid, p + 1, q - 1);
            }
            if (q + 1 < topgrid[p].size()) {
                topgrid[p + 1][q + 1] = 0;
                recursion7(topgrid, grid, p + 1, q + 1);
            }
        }
    }
    }
bool checkvictory(vector<vector<int>> &topgrid, vector<vector<int>> &grid, vector<vector<int>> &flaggrid){
    vector<vector<int>> temp = grid;
    for (int i =0; i < topgrid.size(); i++){
        for (int j =0; j < topgrid[i].size(); j++){
            if(temp[i][j] < 9){
                temp[i][j] = 0;
            }
        }
    }
    if (temp == topgrid){
        flaggrid = temp;
        return true;
    }
    else{
        return false;
    }
}

void leaderboardprint(vector<vector<string>> &ranks, fstream &file){
    ranks.clear();
    vector<string> temp;
    string name;
    string time;
    string singleLine;
    for (int i = 0; i < 5; i++) {
        getline(file, singleLine, '\n');
        istringstream stream(singleLine);
        getline(stream, time, ',');
        getline(stream, name, ',');
        temp.push_back(time);
        temp.push_back(name);
        ranks.push_back(temp);
        temp.clear();
    }
}

int leaderboardprintnew(vector<vector<string>> &ranks, fstream &file, int minutes, int seconds, string nametext){
    file.clear();
    file.seekg(0, ios::beg);
    ranks.clear();
    vector<string> temp;
    string testing;
    string name;
    string time;
    string singleLine;
    bool added = false;
    int savedrank = 10;
    for (int i = 0; i < 5; i++) {
        getline(file, singleLine, '\n');
        istringstream stream(singleLine);
        getline(stream, time, ',');
        getline(stream, name, ',');
        int compare =  stoi(time.substr(0, 2)) ;
        if (minutes < compare && !added){
            if (minutes < 10){
                testing = "0" + to_string(minutes) + ":";
            }
            else{
                testing = to_string(minutes) + ":";
            }
            if (seconds < 10){
                testing += "0" + to_string(seconds);
            }
            else{
                testing += to_string(seconds);
            }
            temp.push_back(testing);
            temp.push_back(nametext.substr(0, nametext.size() - 1));
            savedrank = i;
            added = true;
            ranks.push_back(temp);
            temp.clear();
            i += 1;
        }
        else if (minutes == compare && !added){
            if (seconds < stoi(time.substr(3, 2))) {
                if (minutes < 10) {
                    testing = "0" + to_string(minutes) + ":";
                } else {
                    testing = to_string(minutes) + ":";
                }
                if (seconds < 10) {
                    testing += "0" + to_string(seconds);
                } else {
                    testing += to_string(seconds);
                }
                temp.push_back(testing);
                temp.push_back(nametext.substr(0, nametext.size() - 1));
                savedrank = i;
                added = true;
                ranks.push_back(temp);
                temp.clear();
                i += 1;
            }
            else{
                temp.push_back(time);
                temp.push_back(name);
                ranks.push_back(temp);
                temp.clear();
                if (minutes < 10) {
                    testing = "0" + to_string(minutes) + ":";
                } else {
                    testing = to_string(minutes) + ":";
                }
                if (seconds < 10) {
                    testing += "0" + to_string(seconds);
                } else {
                    testing += to_string(seconds);
                }
                temp.push_back(testing);
                temp.push_back(nametext.substr(0, nametext.size() - 1));
                savedrank = i+1;
                added = true;
                ranks.push_back(temp);
                temp.clear();
                i += 1;
                continue;
            }
        }
        temp.push_back(time);
        temp.push_back(name);
        ranks.push_back(temp);
        temp.clear();
    }
    ofstream out("files/leaderboard.txt");
    for (int y=0;y<5;y++)
    {
        for (int x=0;x<2;x++)
        {
            if (x == 0) {
                out << ranks[y][x] << ",";
            }
            else{
                out << ranks[y][x];
            }
        }
        out<<"\n";
    }
    return savedrank;
}

string createstring(vector<vector<string>> &ranks, fstream &file){
    string leaderboard = "";
    leaderboardprint(ranks, file);
    int ranking = 1;
    for(int i = 0; i < 5; i++){
        leaderboard += to_string(ranking);
        leaderboard += ".";
        leaderboard += "\t";
        for(int j = 0; j < 2; j++){
            leaderboard += ranks[i][j];
            if (j == 0){leaderboard += "\t";}
        }
        if (i < 4){leaderboard += "\n";}
        if (i < 4){leaderboard += "\n";}
        ranking += 1;
    }
    return leaderboard;
}

string createstringnew(vector<vector<string>> &ranks, fstream &file,int minutes, int seconds, string nametext){
    string leaderboard = "";
    int savedrank = leaderboardprintnew(ranks, file, minutes, seconds, nametext);
    int ranking = 1;
    for(int i = 0; i < 5; i++){
        leaderboard += to_string(ranking);
        leaderboard += ".";
        leaderboard += "\t";
        for(int j = 0; j < 2; j++){
            leaderboard += ranks[i][j];
            if (j == 0){leaderboard += "\t";}
            else{
                if (i == savedrank){
                    leaderboard += "*";
                }
            }
        }
        if (i < 4){leaderboard += "\n";}
        if (i < 4){leaderboard += "\n";}
        ranking += 1;
    }
    return leaderboard;
}

#endif //MINESWEEPER_BOARD_H