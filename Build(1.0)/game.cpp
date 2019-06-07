#include <fstream>
#include <ncurses.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include "map.h"

using namespace std;

vector<map> stages;

void refreshMap(int stage);
void playGame(int stage);
void startScreen(int stage);
void baseUI();
void helpScreen();
void clearScreen(int stage);
void drawGraphic(int row, int col, int color);
void refreshScore();
void saveScore(int stage, int step, int push);  // Save user score

int input;

void baseUI() {
    bkgd(COLOR_PAIR(1));
    attron(COLOR_PAIR(1));
    mvprintw(1, 2, "Push Box Game");
    attroff(COLOR_PAIR(1));
    attron(COLOR_PAIR(2));
    border('|', '|', '-', '-', '*', '*', '*', '*');
    attroff(COLOR_PAIR(2));
}

void helpScreen() {
    attron(COLOR_PAIR(1));
    mvprintw(4, 10, "Move Character ");
    attron(COLOR_PAIR(7));
    mvprintw(4, 25, "  ");
    attroff(COLOR_PAIR(7));
    mvprintw(5, 18, "- W, A, S, D");
    mvprintw(7, 10, "Move the ");
    attron(COLOR_PAIR(6));
    mvprintw(7, 19, "  ");
    attroff(COLOR_PAIR(6));
    mvprintw(8, 21, "to the ");
    attron(COLOR_PAIR(8));
    mvprintw(8, 28, "  ");
    attroff(COLOR_PAIR(8));
    attron(COLOR_PAIR(4));
    mvprintw(10, 10, "  ");
    attroff(COLOR_PAIR(4));
    mvprintw(10, 12, " cant pass through");
    mvprintw(13, 15, "Press any key to return");
    attroff(COLOR_PAIR(1));
}

void drawGraphic(int row, int col, int color) {
    attron(COLOR_PAIR(color));
    mvprintw(3 + row, 5 + col * 2, "  ");
    attroff(COLOR_PAIR(color));
}

void refreshMap(int stage) {
    for(int i = 0; i < stages.at(stage - 1).getRow(); i++) {
        for(int j = 0; j < stages.at(stage - 1).getCol(); j++) {
            switch (stages.at(stage - 1).getMapData()[i][j]) {
                case 0:
                case 4:
                    drawGraphic(i, j, 5);
                    break;
                case 1:
                    drawGraphic(i, j, 4);
                    break;
                case 2:
                    drawGraphic(i, j, 6);
                    break;
                case 3:
                    drawGraphic(i, j, 8);
                    break;
                case 5:
                    drawGraphic(i, j, 7);
                    break;
            }
        }
    }
}

void refreshScore() {
    attron(COLOR_PAIR(1));
    string b = to_string(stepCnt);
    const char *a = b.c_str();
    mvprintw(5, 33, a);
    string d = to_string(pushCnt);
    const char *c = d.c_str();
    mvprintw(9, 33, c);
    attroff(COLOR_PAIR(1));
}

void saveScore(int stage, int step, int push) {
    string file = "/your_root/Build(1.0)/scores/stage" + to_string(stage) + ".txt";
    ifstream fin(file);
    vector<int> v;
    v.clear();
    v.push_back(step + push);
    int total;
    while(fin >> total) {
        if (total != 0) {
            v.push_back(total);
        }
    }
    sort(v.begin(), v.end());
    while(v.size() < 3) {
        v.push_back(0);
    }
    ofstream fout(file);
    fout << v[0] << "\n" << v[1] << "\n"<< v[2] << "\n";
    fout.close();
    fin.close();
}

void playGame(int stage) {
    int control;
    attron(COLOR_PAIR(3));
    string f = to_string(stage - 48);
    const char *e = f.c_str();
    mvprintw(1, 36, e);
    mvprintw(1, 30, " Stage");
    mvprintw(1, 37, " ");
    mvprintw(4, 31, " Step ");
    mvprintw(8, 31, " Push ");
    attroff(COLOR_PAIR(3));
    attron(COLOR_PAIR(1));
    mvprintw(13, 7, "Q : Quit  R : Restart  U : Undo");
    attroff(COLOR_PAIR(1));
    refreshMap(stage - 48);
    attron(COLOR_PAIR(1));
    string b = to_string(stepCnt);
    const char *a = b.c_str();
    mvprintw(5, 33, a);
    string d = to_string(pushCnt);
    const char *c = d.c_str();
    mvprintw(9, 33, c);
    attroff(COLOR_PAIR(1));
    int chkquit = 0;
    while(!stages.at(stage - 49).clearChk()) {
        if(chkquit == 1) break;
        refresh();
        control = getch();
        switch (control) {
            case 87:
            case 119:
                stages.at(stage - 49).upside();
                refreshMap(stage - 48);
                refreshScore();
                break;
            case 83:
            case 115:
                stages.at(stage - 49).downside();
                refreshMap(stage - 48);
                refreshScore();
                break;
            case 68:
            case 100:
                stages.at(stage - 49).rightside();
                refreshMap(stage - 48);
                refreshScore();
                break;
            case 65:
            case 97:
                stages.at(stage - 49).leftside();
                refreshMap(stage - 48);
                refreshScore();
                break;
            case 85:
            case 117:
                stages.at(stage - 49).undo();
                refreshMap(stage - 48);
                refreshScore();
                break;
            case 82:
            case 114:
                stages.at(stage - 49).restartMap();
                refreshMap(stage - 48);
                attron(COLOR_PAIR(1));
                mvprintw(5, 33, "   ");
                mvprintw(9, 33, "   ");
                attroff(COLOR_PAIR(1));
                refreshScore();
                break;
            case 81:
            case 113:
                chkquit = 1;
                break;
        }
    }
    if(chkquit == 0) {
        clearScreen(stage);
    }
    stages.at(stage - 49).restartMap();
    clear();
    startScreen(numStage);
}

void clearScreen(int stage) {
    saveScore(stage - 48, stepCnt, pushCnt);
    clear();
    baseUI();
    attron(COLOR_PAIR(2));
    mvprintw(3, 6, "Congraturation! ");
    mvprintw(3, 22, "Stage Clear!");
    mvprintw(5, 15, "Your Score");
    attroff(COLOR_PAIR(2));
    attron(COLOR_PAIR(1));
    mvprintw(13, 15, "Press any key to return");
    attroff(COLOR_PAIR(1));
    attron(COLOR_PAIR(9));
    mvprintw(6, 8, " Step :     ");
    string b = to_string(stepCnt);
    const char *a = b.c_str();
    mvprintw(6, 16, a);
    mvprintw(6, 20, " Push :     ");
    string d = to_string(pushCnt);
    const char *c = d.c_str();
    mvprintw(6, 28, c);
    mvprintw(7, 8, "       Total:           ");
    string f = to_string(pushCnt + stepCnt);
    const char *e = f.c_str();
    mvprintw(7, 22, e);
    attroff(COLOR_PAIR(9));
    attron(COLOR_PAIR(10));
    mvprintw(9, 13, " Rank  1:     ");
    mvprintw(10, 13, "       2:     ");
    mvprintw(11, 13, "       3:     ");
    string file = "/your_root/Build(1.0)/scores/stage" + to_string(stage - 48) + ".txt";
    int total;
    ifstream fin(file);
    for(int i = 0; i < 3; i++) {
        fin >> total;
        string rank = to_string(total);
        const char *strrank = rank.c_str();
        mvprintw(9 + i, 23, strrank);
    }
    fin.close();
    attroff(COLOR_PAIR(10));
    refresh();
    getch();
}

void startScreen(int stage) {
    baseUI();
    attron(COLOR_PAIR(2));
    mvprintw(6, 5, "Please enter the stage to play");
    mvprintw(7, 10, "Choose between 1 ~ ");
    string b = to_string(stage);
    const char *a = b.c_str();
    mvprintw(7, 29, a);
    attroff(COLOR_PAIR(2));
    attron(COLOR_PAIR(1));
    mvprintw(13, 13, "Q : Quit  H : How to play");
    attroff(COLOR_PAIR(1));
    refresh();
    input = getch();
    if (input == 72 || input == 104) {
        clear();
        baseUI();
        helpScreen();
        refresh();
        input = getch();
        clear();
        startScreen(stage);
    } else if (input == 81 || input == 113) {
        clear();
        endwin();
    } else if (input > 48 && input < stage + 49) {
        clear();
        stepCnt = 0;
        pushCnt = 0;
        baseUI();
        playGame(input);
    } else {
        clear();
        startScreen(stage);
    }
}

int main() {
    ifstream inStream;
    // 0: 벽 내부의 빈공간, 1: 벽, 2: 박스, 3: 타겟, 4: 빈공간, 5: 캐릭터초기위치
    // 여유공간을 계산, 맵 데이터의 최대 크기는 가로 10 세로 9으로 제한
    inStream.open("/your_root/Build(1.0)/input.txt");
    inStream >> numStage;
    for(int i = 0; i < numStage; i++) {
        int r, c;
        int data;
        inStream >> r >> c;
        int temp[r * c + 2];
        temp[0] = r; temp[1] = c;
        for(int j = 2; j < r * c + 2; j++) {
            inStream >> data;
            temp[j] = data;
        }
        map tempMap = map(temp);
        stages.push_back(tempMap);
    }
    int row = 15;
    int col = 40;
    initscr();
    resize_term(row, col);
    curs_set(0);
    noecho();
    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_BLACK, COLOR_WHITE);
    init_pair(4, COLOR_WHITE, COLOR_WHITE); //벽
    init_pair(5, COLOR_BLACK, COLOR_BLACK); //빈공간
    init_pair(6, COLOR_RED, COLOR_RED); //박스
    init_pair(7, COLOR_BLUE, COLOR_BLUE); //캐릭터
    init_pair(8, COLOR_GREEN, COLOR_GREEN); //타겟
    init_pair(9, COLOR_GREEN, COLOR_BLACK);
    init_pair(10, COLOR_BLACK, COLOR_GREEN);
    startScreen(numStage);
    endwin();
    return 0;
}
