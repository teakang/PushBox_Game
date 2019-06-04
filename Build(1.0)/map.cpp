#include "map.h"

int stepCnt, pushCnt, numStage;

map::map(const int *arr) {
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

void map::restartMap() {
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

void map::saveUndoData() {
    for(int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            undoData[i][j] = mapData[i][j];
        }
    }
}

bool map::clearChk() {
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

void map::upside() {
    moveAction(-1, 0);
}

void map::downside() {
    moveAction(1, 0);
}

void map::rightside() {
    moveAction(0, 1);
}

void map::leftside() {
    moveAction(0, -1);
}

void map::undo() {
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

void map::targetOverwrite() {
    for(int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            if(mapData[i][j] == 0 && targetData[i][j] == 1) mapData[i][j] = 3;
        }
    }
}

int map::getRow() {
    return r;
}

int map::getCol() {
    return c;
}

int** map::getMapData() {
    return mapData;
}

void map::moveAction(int rowChange, int colChange) {
    int row = currCharRow + rowChange;
    int col = currCharCol + colChange;
    if(mapData[row][col] == 0) {
        saveUndoData();
        mapData[row][col] = 5;
        mapData[currCharRow][currCharCol] = 0;
        moveCharacter(rowChange, colChange);
        stepCnt++;
    } else if(mapData[row][col] == 3) {
        saveUndoData();
        mapData[row][col] = 5;
        mapData[currCharRow][currCharCol] = 0;
        moveCharacter(rowChange, colChange);
        stepCnt++;
    } else if(mapData[row][col] == 1) {
        return;
    } else if(mapData[row][col] == 2) {
        if(mapData[row + rowChange][col + colChange] == 0 || mapData[row + rowChange][col + colChange] == 3) {
            saveUndoData();
            mapData[row + rowChange][col + colChange] = 2;
            mapData[row][col] = 5;
            mapData[currCharRow][currCharCol] = 0;
            moveCharacter(rowChange, colChange);
            stepCnt++;
            pushCnt++;
        } else return;
    }
    targetOverwrite();
}

void map::moveCharacter(int rowChange, int colChange) {
    if(rowChange == 1) currCharRow++;
    else if(rowChange == -1) currCharRow--;
    else if(colChange == 1) currCharCol++;
    else if(colChange == -1) currCharCol--;
}