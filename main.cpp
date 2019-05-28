#include <ncurses.h>
#include <fstream>
#include <vector>

//키보드로 입력받은 숫자가 아스키 코드라는 것을 항상 명심할 것(-48)

using namespace std;

int stepCnt, pushCnt, numStage;

class map {
public:
    int r, c, **initMapData, **mapData, **undoData, **targetData, currCharRow, currCharCol;

    map(int arr[]) {
        r = arr[0];
        c = arr[1];
        mapData = new int *[r];
        for (int i = 0; i < r; i++)
            mapData[i] = new int[c];
        int cnt = 2;
        for(int i = 0; i < r; i++) {
            for(int j = 0; j < c; j++) {
                mapData[i][j] = arr[cnt];
                if(arr[cnt] == 5) {
                    currCharRow = i;
                    currCharCol = j;
                }
                cnt++;
            }
        }
        initMapData = new int *[r];
        for (int i = 0; i < r; i++)
            initMapData[i] = new int[c];
        for(int i = 0; i < r; i++) {
            for (int j = 0; j < c; j++) {
                initMapData[i][j] = mapData[i][j];
            }
        }
        undoData = new int *[r];
        for (int i = 0; i < r; i++)
            undoData[i] = new int[c];
        for(int i = 0; i < r; i++) {
            for (int j = 0; j < c; j++) {
                undoData[i][j] = mapData[i][j];
            }
        }
        targetData = new int *[r];
        for (int i = 0; i < r; i++)
            targetData[i] = new int[c];
        for(int i = 0; i < r; i++) {
            for (int j = 0; j < c; j++) {
                if(mapData[i][j] == 3) targetData[i][j] = 1;
                else targetData[i][j] = 0;
            }
        }
    }

    void restartMap() {
        for(int i = 0; i < r; i++) {
            for (int j = 0; j < c; j++) {
                mapData[i][j] = initMapData[i][j];
            }
        }
        for(int i = 0; i < r; i++) {
            for (int j = 0; j < c; j++) {
                if(mapData[i][j] == 5) {
                    currCharRow = i;
                    currCharCol = j;
                }
            }
        }
        stepCnt = 0;
        pushCnt = 0;
    }

    void saveUndoData() {
        for(int i = 0; i < r; i++) {
            for (int j = 0; j < c; j++) {
                undoData[i][j] = mapData[i][j];
            }
        }
    }

    bool clearChk() {
        for(int i = 0; i < r; i++) {
            for (int j = 0; j < c; j++) {
                if(mapData[i][j] == 3) return false;
                if(mapData[i][j] == 2) {
                    if(targetData[i][j] != 1) return false;
                }
            }
        }
        return true;
    }

    void upside() {
        saveUndoData();
        if(mapData[currCharRow - 1][currCharCol] == 0) {
            mapData[currCharRow - 1][currCharCol] = 5;
            mapData[currCharRow][currCharCol] = 0;
            currCharRow--;
            stepCnt++;
        } else if(mapData[currCharRow - 1][currCharCol] == 3) {
            mapData[currCharRow - 1][currCharCol] = 5;
            mapData[currCharRow][currCharCol] = 0;
            currCharRow--;
            stepCnt++;
        } else if(mapData[currCharRow - 1][currCharCol] == 1) {
            return;
        } else if(mapData[currCharRow - 1][currCharCol] == 2) {
            if(mapData[currCharRow - 2][currCharCol] == 0 || mapData[currCharRow - 2][currCharCol] == 3) {
                mapData[currCharRow - 2][currCharCol] = 2;
                mapData[currCharRow - 1][currCharCol] = 5;
                mapData[currCharRow][currCharCol] = 0;
                currCharRow--;
                stepCnt++;
                pushCnt++;
            } else return;
        }
        targetOverwrite();
    }

    void downside() {
        saveUndoData();
        if(mapData[currCharRow + 1][currCharCol] == 0) {
            mapData[currCharRow + 1][currCharCol] = 5;
            mapData[currCharRow][currCharCol] = 0;
            currCharRow++;
            stepCnt++;
        } else if(mapData[currCharRow + 1][currCharCol] == 3) {
            mapData[currCharRow + 1][currCharCol] = 5;
            mapData[currCharRow][currCharCol] = 0;
            currCharRow++;
            stepCnt++;
        } else if(mapData[currCharRow + 1][currCharCol] == 1) {
            return;
        } else if(mapData[currCharRow + 1][currCharCol] == 2) {
            if(mapData[currCharRow + 2][currCharCol] == 0 || mapData[currCharRow + 2][currCharCol] == 3) {
                mapData[currCharRow + 2][currCharCol] = 2;
                mapData[currCharRow + 1][currCharCol] = 5;
                mapData[currCharRow][currCharCol] = 0;
                currCharRow++;
                stepCnt++;
                pushCnt++;
            } else return;
        }
        targetOverwrite();
    }

    void rightside() {
        saveUndoData();
        if(mapData[currCharRow][currCharCol + 1] == 0) {
            mapData[currCharRow][currCharCol + 1] = 5;
            mapData[currCharRow][currCharCol] = 0;
            currCharCol++;
            stepCnt++;
        } else if(mapData[currCharRow][currCharCol + 1] == 3) {
            mapData[currCharRow][currCharCol + 1] = 5;
            mapData[currCharRow][currCharCol] = 0;
            currCharCol++;
            stepCnt++;
        } else if(mapData[currCharRow][currCharCol + 1] == 1) {
            return;
        } else if(mapData[currCharRow][currCharCol + 1] == 2) {
            if(mapData[currCharRow][currCharCol + 2] == 0 || mapData[currCharRow][currCharCol + 2] == 3) {
                mapData[currCharRow][currCharCol + 2] = 2;
                mapData[currCharRow][currCharCol + 1] = 5;
                mapData[currCharRow][currCharCol] = 0;
                currCharCol++;
                stepCnt++;
                pushCnt++;
            } else return;
        }
        targetOverwrite();
    }

    void leftside() {
        saveUndoData();
        if(mapData[currCharRow][currCharCol - 1] == 0) {
            mapData[currCharRow][currCharCol - 1] = 5;
            mapData[currCharRow][currCharCol] = 0;
            currCharCol--;
            stepCnt++;
        } else if(mapData[currCharRow][currCharCol - 1] == 3) {
            mapData[currCharRow][currCharCol - 1] = 5;
            mapData[currCharRow][currCharCol] = 0;
            currCharCol--;
            stepCnt++;
        } else if(mapData[currCharRow][currCharCol - 1] == 1) {
            return;
        } else if(mapData[currCharRow][currCharCol - 1] == 2) {
            if(mapData[currCharRow][currCharCol - 2] == 0 || mapData[currCharRow][currCharCol - 2] == 3) {
                mapData[currCharRow][currCharCol - 2] = 2;
                mapData[currCharRow][currCharCol - 1] = 5;
                mapData[currCharRow][currCharCol] = 0;
                currCharCol--;
                stepCnt++;
                pushCnt++;
            } else return;
        }
        targetOverwrite();
    }

    void moveAction(int row, int col) {
        //이동 코드 재사용성 향상을 위한 부분
    }

    void undo() {
        int differCnt = 0;
        for(int i = 0; i < r; i++) {
            for (int j = 0; j < c; j++) {
                if(mapData[i][j] != undoData[i][j]) {
                    mapData[i][j] = undoData[i][j];
                    differCnt++;
                }
            }
        }
        for(int i = 0; i < r; i++) {
            for (int j = 0; j < c; j++) {
                if(mapData[i][j] == 5) {
                    currCharRow = i;
                    currCharCol = j;
                }
            }
        }
        if(differCnt >= 3) {
            stepCnt--;
            pushCnt--;
        } else if(differCnt == 2) {
            stepCnt--;
        }
    }

    void targetOverwrite() {
        for(int i = 0; i < r; i++) {
            for (int j = 0; j < c; j++) {
                if(mapData[i][j] == 0 && targetData[i][j] == 1) mapData[i][j] = 3;
            }
        }
    }
};

vector<map> stages;

void refreshMap(int stage);
void playGame(int stage);
void startScreen(int stage);
void baseUI();
void helpScreen();
void clearScreen();

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
    mvprintw(4, 7, "Move Character");
    mvprintw(5, 20, "- W, A, S, D");
    mvprintw(13, 15, "Press any key to return");
    attroff(COLOR_PAIR(1));
}

void refreshMap(int stage) {
    for(int i = 0; i < stages.at(stage - 1).r; i++) {
        for(int j = 0; j < stages.at(stage - 1).c; j++) {
            switch (stages.at(stage - 1).mapData[i][j]) {
                case 0:
                    attron(COLOR_PAIR(5));
                    mvprintw(3 + i, 5 + j * 2, "  ");
                    attroff(COLOR_PAIR(5));
                    break;
                case 1:
                    attron(COLOR_PAIR(4));
                    mvprintw(3 + i, 5 + j * 2, "  ");
                    attroff(COLOR_PAIR(4));
                    break;
                case 2:
                    attron(COLOR_PAIR(6));
                    mvprintw(3 + i, 5 + j * 2, "  ");
                    attroff(COLOR_PAIR(6));
                    break;
                case 3:
                    attron(COLOR_PAIR(8));
                    mvprintw(3 + i, 5 + j * 2, "  ");
                    attroff(COLOR_PAIR(8));
                    break;
                case 4:
                    attron(COLOR_PAIR(5));
                    mvprintw(3 + i, 5 + j * 2, "  ");
                    attroff(COLOR_PAIR(5));
                    break;
                case 5:
                    attron(COLOR_PAIR(7));
                    mvprintw(3 + i, 5 + j * 2, "  ");
                    attroff(COLOR_PAIR(7));
                    break;
            }
        }
    }
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
                attron(COLOR_PAIR(1));
                b = to_string(stepCnt);
                a = b.c_str();
                mvprintw(5, 33, a);
                d = to_string(pushCnt);
                c = d.c_str();
                mvprintw(9, 33, c);
                attroff(COLOR_PAIR(1));
                break;
            case 83:
            case 115:
                stages.at(stage - 49).downside();
                refreshMap(stage - 48);
                attron(COLOR_PAIR(1));
                b = to_string(stepCnt);
                a = b.c_str();
                mvprintw(5, 33, a);
                d = to_string(pushCnt);
                c = d.c_str();
                mvprintw(9, 33, c);
                attroff(COLOR_PAIR(1));
                break;
            case 68:
            case 100:
                stages.at(stage - 49).rightside();
                refreshMap(stage - 48);
                attron(COLOR_PAIR(1));
                b = to_string(stepCnt);
                a = b.c_str();
                mvprintw(5, 33, a);
                d = to_string(pushCnt);
                c = d.c_str();
                mvprintw(9, 33, c);
                attroff(COLOR_PAIR(1));
                break;
            case 65:
            case 97:
                stages.at(stage - 49).leftside();
                refreshMap(stage - 48);
                attron(COLOR_PAIR(1));
                b = to_string(stepCnt);
                a = b.c_str();
                mvprintw(5, 33, a);
                d = to_string(pushCnt);
                c = d.c_str();
                mvprintw(9, 33, c);
                attroff(COLOR_PAIR(1));
                break;
            case 85:
            case 117:
                stages.at(stage - 49).undo();
                refreshMap(stage - 48);
                attron(COLOR_PAIR(1));
                b = to_string(stepCnt);
                a = b.c_str();
                mvprintw(5, 33, a);
                d = to_string(pushCnt);
                c = d.c_str();
                mvprintw(9, 33, c);
                attroff(COLOR_PAIR(1));
                break;
            case 82:
            case 114:
                stages.at(stage - 49).restartMap();
                refreshMap(stage - 48);
                attron(COLOR_PAIR(1));
                b = to_string(stepCnt);
                a = b.c_str();
                mvprintw(5, 33, "   ");
                mvprintw(5, 33, a);
                d = to_string(pushCnt);
                c = d.c_str();
                mvprintw(9, 33, "   ");
                mvprintw(9, 33, c);
                attroff(COLOR_PAIR(1));
                break;
            case 81:
            case 113:
                chkquit = 1;
                break;
        }
    }
    if(chkquit == 0) clearScreen();
    stages.at(stage - 49).restartMap();
    clear();
    startScreen(numStage);
}

void clearScreen() {
    clear();
    baseUI();
    attron(COLOR_PAIR(2));
    mvprintw(4, 12, "Congraturation!!");
    mvprintw(5, 14, "Stage Clear!");
    mvprintw(7, 15, "Your Score");
    attroff(COLOR_PAIR(2));
    attron(COLOR_PAIR(1));
    mvprintw(13, 15, "Press any key to return");
    attroff(COLOR_PAIR(1));
    attron(COLOR_PAIR(3));
    mvprintw(8, 14, " Step : ");
    string b = to_string(stepCnt);
    const char *a = b.c_str();
    mvprintw(8, 23, "   ");
    mvprintw(8, 22, a);
    mvprintw(9, 14, " Push : ");
    string d = to_string(pushCnt);
    const char *c = d.c_str();
    mvprintw(9, 23, "   ");
    mvprintw(9, 22, c);
    attroff(COLOR_PAIR(3));
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
    inStream.open("/Users/simonkim/CLionProjects/cppcode/input.txt");
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
    init_pair(1, COLOR_BLUE, COLOR_BLACK); init_pair(2, COLOR_RED, COLOR_BLACK); init_pair(3, COLOR_BLACK, COLOR_WHITE);
    init_pair(4, COLOR_WHITE, COLOR_WHITE); //벽
    init_pair(5, COLOR_BLACK, COLOR_BLACK); //빈공간
    init_pair(6, COLOR_RED, COLOR_RED); //박스
    init_pair(7, COLOR_BLUE, COLOR_BLUE); //캐릭터
    init_pair(8, COLOR_GREEN, COLOR_GREEN); //타겟
    startScreen(numStage);
    endwin();
    return 0;
}