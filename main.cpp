#include <string>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <chrono>
#include "board.h"

using namespace std::chrono;

//TODO: implement add new time to leaderboard

void setText(sf::Text &text, float x, float y){
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
                   textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

int main() {
    ifstream configuration("../files/config.cfg");
    string line;
    getline(configuration, line);
    int colCount = stoi(line);
    getline(configuration, line);
    int rowCount = stoi(line);
    getline(configuration, line);
    int mineCount = stoi(line);

    sf::RenderWindow startscreen(sf::VideoMode(colCount*32, (rowCount*32)+100), "Minesweeper", sf::Style::Close);
    sf::Font font;
    if (!font.loadFromFile("files/font.ttf"))
        return EXIT_FAILURE;
    bool startgame = false;
    sf::Text welcome;
    welcome.setFont(font);
    welcome.setString("WELCOME TO MINESWEEPER!");
    welcome.setCharacterSize(24);
    welcome.setFillColor(sf::Color::White);
    welcome.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(welcome, (colCount*32)/2, ((rowCount*32)+100)/2 - 150);

    sf::Text enter;
    enter.setFont(font);
    enter.setString("Enter your name:");
    enter.setCharacterSize(20);
    enter.setFillColor(sf::Color::White);
    enter.setStyle(sf::Text::Bold);
    setText(enter, (colCount*32)/2, ((rowCount*32)+100)/2 - 75);

    sf::Text lbtitle;
    lbtitle.setFont(font);
    lbtitle.setString("LEADERBOARD");
    lbtitle.setCharacterSize(20);
    lbtitle.setFillColor(sf::Color::White);
    lbtitle.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(lbtitle, (colCount*8), ((rowCount*16)+50)/2 -120);

    sf::Text lbrankings;
    lbrankings.setFont(font);
    lbrankings.setCharacterSize(18);
    lbrankings.setFillColor(sf::Color::White);
    lbrankings.setStyle(sf::Text::Bold);
    setText(lbrankings, (colCount*3), ((rowCount*16)+50)/5 + 20);


    sf::Text name;
    string nametext = "|";

    while(startscreen.isOpen()) {
        sf::Event event;
        startscreen.clear(sf::Color::Blue);
        while(startscreen.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                startscreen.close();
            }

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Backspace)
                {
                    if (nametext != "|") {
                        nametext = nametext.substr(0, nametext.size() - 2);
                        nametext += "|";
                    }
                }

                if (event.key.code == sf::Keyboard::Enter && nametext != "|"){
                    startscreen.close();
                    startgame = true;
                }
            }
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128 && event.text.unicode != 8 && isalpha(event.text.unicode)) {
                    if (nametext.size() <= 10) {
                        char c = static_cast<char>(event.text.unicode);
                        if (nametext == "|"){
                            c = toupper(c);
                        }
                        else{
                            c = tolower(c);
                        }
                        nametext = nametext.substr(0, nametext.size()-1);
                        nametext += c;
                        nametext += "|";
                    }
                }
            }
        }
        name.setFont(font);
        name.setString(nametext);
        name.setCharacterSize(18);
        name.setFillColor(sf::Color::Yellow);
        name.setStyle(sf::Text::Bold);
        setText(name, (colCount*32)/2, ((rowCount*32)+100)/2 - 45);
        startscreen.draw(name);
        startscreen.draw(welcome);
        startscreen.draw(enter);
        startscreen.display();
    }

    if (startgame) {
        bool addnew = false;
        vector<vector<string>> rankings;
        bool victory = false;
        fstream file;
        file.open("files/leaderboard.txt");
        string ranks = createstring(rankings, file);
        lbrankings.setString(ranks);
        auto start_time = chrono::high_resolution_clock::now();
        auto pauseTime = chrono::high_resolution_clock::now();
        auto elapsed_paused_time = chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now()-pauseTime).count();
        bool debugger=false;
        int ones;
        int tens;
        bool lose = false;
        bool pause = false;
        bool templ = false;
        bool tempb = pause;
        TextureManager textures;
        DigitsManager alldigits;
        textures.loadtextures();
        alldigits.loaddigits();
        int flagnum;
        if (mineCount < 100){
            flagnum = mineCount;
        }
        else{
            flagnum = 99;
        }

        sf::Sprite debugsprite;
        debugsprite.setTexture(*textures.textmap["debug"]);
        debugsprite.setPosition(sf::Vector2f((colCount * 32) - 304, 32 * (rowCount + 0.5)));

        sf::Sprite pausesprite;
        pausesprite.setTexture(*textures.textmap["pause"]);
        pausesprite.setPosition(sf::Vector2f((colCount * 32) - 240, 32 * (rowCount + 0.5)));

        sf::Sprite lbsprite;
        lbsprite.setTexture(*textures.textmap["leaderboard"]);
        lbsprite.setPosition(sf::Vector2f((colCount * 32) - 176, 32 * (rowCount + 0.5)));

        sf::Sprite happyf;
        happyf.setTexture(*textures.textmap["happy"]);
        happyf.setPosition(sf::Vector2f(((colCount / 2) * 32) - 32, 32 * (rowCount + 0.5)));

        sf::Sprite mine(*textures.textmap["mine"]);
        sf::Sprite one(*textures.textmap["one"]);
        sf::Sprite two(*textures.textmap["two"]);
        sf::Sprite three(*textures.textmap["three"]);
        sf::Sprite four(*textures.textmap["four"]);
        sf::Sprite five(*textures.textmap["five"]);
        sf::Sprite six(*textures.textmap["six"]);
        sf::Sprite seven(*textures.textmap["seven"]);
        sf::Sprite eight(*textures.textmap["eight"]);

        sf::Sprite tiler(*textures.textmap["revealed"]);
        sf::Sprite tileh(*textures.textmap["hidden"]);
        sf::Sprite flag(*textures.textmap["flag"]);
        sf::Sprite flagdigitone;
        flagdigitone.setPosition(54, 32 * (rowCount + 0.5) + 16);
        sf::Sprite flagdigitten;
        flagdigitten.setPosition(33, 32 * (rowCount + 0.5) + 16);
        sf::Sprite negcheck;
        negcheck.setPosition(12, 32 * (rowCount + 0.5) + 16);

        sf::Sprite timeminuteten;
        timeminuteten.setPosition(colCount*32 - 97, 32 * (rowCount + 0.5) + 16);
        sf::Sprite timeminuteone;
        timeminuteone.setPosition(colCount*32 - 76, 32 * (rowCount + 0.5) + 16);
        sf::Sprite timesecondten;
        timesecondten.setPosition(colCount*32 - 55, 32 * (rowCount + 0.5) + 16);
        sf::Sprite timesecondone;
        timesecondone.setPosition(colCount*32 - 34, 32 * (rowCount + 0.5) + 16);

        sf::RenderWindow minesweeper(sf::VideoMode(colCount*32, (rowCount*32)+100), "Minesweeper", sf::Style::Close);

        vector<vector<int>> topgrid;
        vector<int> temp;
        for (int i = 0; i < colCount; i++) {
            for (int j = 0; j < rowCount; j++) {
                temp.push_back(9);
            }
            topgrid.push_back(temp);
            temp.clear();
        }

        vector<vector<int>> flaggrid;
        for (int i = 0; i < colCount; i++) {
            for (int j = 0; j < rowCount; j++) {
                temp.push_back(0);
            }
            flaggrid.push_back(temp);
            temp.clear();
        }

        // ASK HOW TO TURN THIS INTO A FUNCTION!!!
        vector<vector<int>> grid;
        int minenum = 0;
        for (int i = 0; i < colCount; i++) {
            for (int j = 0; j < rowCount; j++) {
                temp.push_back(0);
            }
            grid.push_back(temp);
            temp.clear();
        }
        while (minenum < mineCount) {
            int ranr = rand() % rowCount;
            int ranc = rand() % colCount;
            if (grid[ranc][ranr] != 9) {
                grid[ranc][ranr] = 9;
                minenum += 1;
            }
        }

        for (int i = 0; i < colCount; i++) {
            for (int j = 0; j < rowCount; j++) {
                int n = 0;
                if (grid[i][j] == 9) {
                    continue;
                }

                if (i < colCount -1){
                    if (grid[i + 1][j] == 9) {
                        n += 1;
                    }
                    if (j < rowCount -1){
                        if (grid[i + 1][j + 1] == 9) {
                            n += 1;
                        }
                    }
                    if (j > 0){
                        if (grid[i + 1][j - 1] == 9) {
                            n += 1;
                        }
                    }

                }

                if (i > 0) {
                    if (grid[i - 1][j] == 9) {
                        n += 1;
                    }
                    if (j < rowCount - 1) {
                        if (grid[i - 1][j + 1] == 9) {
                            n += 1;
                        }
                    }
                    if (j > 0) {
                        if (grid[i - 1][j - 1] == 9) {
                            n += 1;
                        }
                    }
                }
                if (j < rowCount-1) {
                    if (grid[i][j + 1] == 9) {
                        n += 1;
                    }
                }
                if (j > 0) {
                    if (grid[i][j - 1] == 9) {
                        n += 1;
                    }
                }

                grid[i][j] = n;
            }
        }

        while(minesweeper.isOpen()) {
            sf::Vector2i clicker = sf::Mouse::getPosition(minesweeper);
            int x = clicker.x;
            int y = clicker.y;
            sf::Event gevent;
            minesweeper.clear(sf::Color::White);
            while (minesweeper.pollEvent(gevent)) {
                if (gevent.type == sf::Event::Closed) {
                    minesweeper.close();
                }
                if (gevent.type == sf::Event::MouseButtonPressed) {
                    if (gevent.mouseButton.button == sf::Mouse::Left && x <= colCount * 32 && y <= rowCount * 32 &&
                        !pause && !lose) {
                        int p = x / 32;
                        int q = y / 32;
                        if (flaggrid[p][q] != 9) {
                            topgrid[p][q] = 0;
                            if (grid[p][q] == 9) {
                                lose = true;
                                happyf.setTexture(*textures.textmap["lose"]);
                                for (int i = 0; i < colCount; i++) {
                                    for (int j = 0; j < rowCount; j++) {
                                        topgrid[i][j] = 0;
                                    }
                                }
                            } else if (grid[p][q] == 0) {
                                recursion7(topgrid, grid, p, q);
                                recursion5(topgrid, grid, p, q);
                                recursion6(topgrid, grid, p, q);
                                recursion8(topgrid, grid, p, q);
                            }
                        }
                        if (checkvictory(topgrid, grid, flaggrid)) {
                            victory = true;
                            happyf.setTexture(*textures.textmap["win"]);
                            flagnum = 0;
                            templ= true;
                            pause= true;
                            pauseTime = chrono::high_resolution_clock::now();
                            lose = true;
                        }
                    } else if (gevent.mouseButton.button == sf::Mouse::Left && x > colCount && y > rowCount) {
                        //debug
                        if ((((colCount * 32) - 304) < x && x < ((colCount * 32) - 240)) &&
                            (32 * (rowCount + 0.5)) < y && y < (32 * (rowCount + 0.5) + 64) && !pause && !lose) {
                            if (!debugger) {
                                for (int i = 0; i < colCount; i++) {
                                    for (int j = 0; j < rowCount; j++) {
                                        if (grid[i][j] == 9) {
                                            flaggrid[i][j] = 8;
                                        }
                                    }
                                }
                                debugger = true;
                            } else if (debugger) {
                                for (int i = 0; i < colCount; i++) {
                                    for (int j = 0; j < rowCount; j++) {
                                        if (grid[i][j] == 9) {
                                            flaggrid[i][j] = 0;
                                        }
                                    }
                                }
                                debugger = false;
                            }

                        }
                            //pause
                        else if (((colCount * 32) - 240) < x && x < ((colCount * 32) - 176) &&
                                 (32 * (rowCount + 0.5)) < y && y < (32 * (rowCount + 0.5) + 64) && !lose) {
                            if (!pause) {
                                pausesprite.setTexture(*textures.textmap["play"]);
                                pauseTime = chrono::high_resolution_clock::now();
                                pause = true;
                            } else if (pause) {
                                auto unPausedTime = chrono::high_resolution_clock::now();
                                elapsed_paused_time += chrono::duration_cast<chrono::seconds>(
                                        unPausedTime - pauseTime).count();
                                pausesprite.setTexture(*textures.textmap["pause"]);
                                pause = false;
                            }
                        }
                            //leaderboard
                        else if (((colCount * 32) - 176) < x && x < ((colCount * 32) - 92) &&
                                 (32 * (rowCount + 0.5)) < y && y < (32 * (rowCount + 0.5) + 64)) {
                            templ = true;
                            tempb = pause;
                            pause = true;
                            if (!tempb) {
                                pauseTime = chrono::high_resolution_clock::now();
                            }
                        }
                            //smiley
                        else if (((colCount / 2) * 32) - 32 < x && x < ((colCount / 2) * 32) + 32 &&
                                 (32 * (rowCount + 0.5)) < y && y < (32 * (rowCount + 0.5) + 64)) {
                            start_time = chrono::high_resolution_clock::now();
                            pauseTime = chrono::high_resolution_clock::now();
                            elapsed_paused_time = chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now()-pauseTime).count();
                            if (lose) {
                                happyf.setTexture(*textures.textmap["happy"]);
                                lose = false;
                            }
                            if (victory) {
                                happyf.setTexture(*textures.textmap["happy"]);
                                victory = false;
                                lose = false;
                            }
                            debugger=false;
                            lose = false;
                            pause = false;
                            templ = false;
                            tempb = pause;
                            addnew = false;
                            victory = false;
                            for (int i = 0; i < colCount; i++) {
                                for (int j = 0; j < rowCount; j++) {
                                    topgrid[i][j] = 9;
                                }
                            }
                            for (int i = 0; i < colCount; i++) {
                                for (int j = 0; j < rowCount; j++) {
                                    flaggrid[i][j] = 0;
                                }
                            }
                            flagnum = mineCount;
                            minenum = 0;
                            for (int i = 0; i < colCount; i++) {
                                for (int j = 0; j < rowCount; j++) {
                                    grid[i][j] = 0;
                                }
                            }
                            while (minenum < mineCount) {
                                int ranr = rand() % rowCount;
                                int ranc = rand() % colCount;
                                if (grid[ranc][ranr] != 9) {
                                    grid[ranc][ranr] = 9;
                                    minenum += 1;
                                }
                            }

                            for (int i = 0; i < colCount; i++) {
                                for (int j = 0; j < rowCount; j++) {
                                    int n = 0;
                                    if (grid[i][j] == 9) {
                                        continue;
                                    }

                                    if (i < colCount - 1) {
                                        if (grid[i + 1][j] == 9) {
                                            n += 1;
                                        }
                                        if (j < rowCount - 1) {
                                            if (grid[i + 1][j + 1] == 9) {
                                                n += 1;
                                            }
                                        }
                                        if (j > 0) {
                                            if (grid[i + 1][j - 1] == 9) {
                                                n += 1;
                                            }
                                        }

                                    }

                                    if (i > 0) {
                                        if (grid[i - 1][j] == 9) {
                                            n += 1;
                                        }
                                        if (j < rowCount - 1) {
                                            if (grid[i - 1][j + 1] == 9) {
                                                n += 1;
                                            }
                                        }
                                        if (j > 0) {
                                            if (grid[i - 1][j - 1] == 9) {
                                                n += 1;
                                            }
                                        }
                                    }
                                    if (j < rowCount - 1) {
                                        if (grid[i][j + 1] == 9) {
                                            n += 1;
                                        }
                                    }
                                    if (j > 0) {
                                        if (grid[i][j - 1] == 9) {
                                            n += 1;
                                        }
                                    }

                                    grid[i][j] = n;
                                }
                            }
                        }
                    } else if (gevent.mouseButton.button == sf::Mouse::Right && x <= colCount * 32 &&
                               y <= rowCount * 32 && !pause) {
                        int p = x / 32;
                        int q = y / 32;
                        if (flaggrid[p][q] != 9) {
                            flaggrid[p][q] = 9;
                            flagnum -= 1;
                        } else if (flaggrid[p][q] == 9) {
                            flaggrid[p][q] = 0;
                            flagnum += 1;
                        }
                    }
                }
            }

            auto game_duration = std::chrono::duration_cast<std::chrono::seconds>(
                    chrono::high_resolution_clock::now() - start_time);
            int total_time = game_duration.count();
            int minutes;
            int seconds;
            if (!pause) {
                total_time = total_time - elapsed_paused_time;
                minutes = total_time / 60;
                seconds = total_time % 60;
            }
            if (victory) {
                pauseTime = chrono::high_resolution_clock::now();
                if (!addnew) {
                    addnew = true;
                    ranks = createstringnew(rankings, file, minutes, seconds, nametext);
                }
                lbrankings.setString(ranks);
            }
            int minutes0 = minutes / 10 % 10;
            int minutes1 = minutes % 10;
            int seconds0 = seconds / 10 % 10;
            int seconds1 = seconds % 10;
            if (!victory) {
                alldigits.alldigit(timeminuteten, minutes0);
                alldigits.alldigit(timeminuteone, minutes1);
                alldigits.alldigit(timesecondten, seconds0);
                alldigits.alldigit(timesecondone, seconds1);
            }
            minesweeper.draw(timeminuteten);
            minesweeper.draw(timeminuteone);
            minesweeper.draw(timesecondten);
            minesweeper.draw(timesecondone);

            int tempdig = flagnum;
            if (flagnum >= 0) {
                alldigits.alldigit(negcheck, 0);
            } else {
                alldigits.ifnegative(negcheck);
                tempdig = flagnum * -1;
            }

            ones = tempdig % 10;
            alldigits.alldigit(flagdigitone, ones);
            tens = (tempdig - ones) / 10;
            alldigits.alldigit(flagdigitten, tens);
            minesweeper.draw(negcheck);
            minesweeper.draw(flagdigitten);
            minesweeper.draw(flagdigitone);

            minesweeper.draw(debugsprite);
            minesweeper.draw(pausesprite);
            minesweeper.draw(lbsprite);
            minesweeper.draw(happyf);

            for (int i = 0; i < colCount; i++) {
                for (int j = 0; j < rowCount; j++) {
                    tiler.setPosition(i * 32, j * 32);
                    minesweeper.draw(tiler);
                    if (grid[i][j] == 9) {
                        mine.setPosition(i * 32, j * 32);
                        minesweeper.draw(mine);
                    } else if (grid[i][j] == 1) {
                        one.setPosition(i * 32, j * 32);
                        minesweeper.draw(one);
                    } else if (grid[i][j] == 2) {
                        two.setPosition(i * 32, j * 32);
                        minesweeper.draw(two);
                    } else if (grid[i][j] == 3) {
                        three.setPosition(i * 32, j * 32);
                        minesweeper.draw(three);
                    } else if (grid[i][j] == 4) {
                        four.setPosition(i * 32, j * 32);
                        minesweeper.draw(four);
                    } else if (grid[i][j] == 5) {
                        five.setPosition(i * 32, j * 32);
                        minesweeper.draw(five);
                    } else if (grid[i][j] == 6) {
                        six.setPosition(i * 32, j * 32);
                        minesweeper.draw(six);
                    } else if (grid[i][j] == 7) {
                        seven.setPosition(i * 32, j * 32);
                        minesweeper.draw(seven);
                    } else if (grid[i][j] == 8) {
                        eight.setPosition(i * 32, j * 32);
                        minesweeper.draw(eight);
                    }

                    if (topgrid[i][j] == 9) {
                        tileh.setPosition(i * 32, j * 32);
                        minesweeper.draw(tileh);
                    }

                    if (flaggrid[i][j] == 9) {
                        flag.setPosition(i * 32, j * 32);
                        minesweeper.draw(flag);
                    }
                    if (flaggrid[i][j] == 8) {
                        mine.setPosition(i * 32, j * 32);
                        minesweeper.draw(mine);
                    }

                }
            }

            if (pause) {
                for (int i = 0; i < colCount; i++) {
                    for (int j = 0; j < rowCount; j++) {
                        tiler.setPosition(i * 32, j * 32);
                        minesweeper.draw(tiler);
                    }
                }
            }

            minesweeper.display();
            if (templ) {
                sf::RenderWindow leaderboard(sf::VideoMode(colCount * 16, (rowCount * 16) + 50), "Leaderboard",
                                             sf::Style::Close);
                while (leaderboard.isOpen() && templ) {
                    for (int i = 0; i < colCount; i++) {
                        for (int j = 0; j < rowCount; j++) {
                            tiler.setPosition(i * 32, j * 32);
                            minesweeper.draw(tiler);
                        }
                    }
                    leaderboard.clear(sf::Color::Blue);
                    sf::Event levent;
                    while (leaderboard.pollEvent(levent)) {
                        if (levent.type == sf::Event::Closed) {
                            templ = false;
                            pause = tempb;
                            if (!tempb) {
                                auto unPausedTime = chrono::high_resolution_clock::now();
                                elapsed_paused_time += chrono::duration_cast<chrono::seconds>(
                                        unPausedTime - pauseTime).count();
                            }
                            leaderboard.close();
                        }
                    }
                    leaderboard.draw(lbtitle);
                    leaderboard.draw(lbrankings);
                    leaderboard.display();

                }
            }
        }
    }


    return 0;
}