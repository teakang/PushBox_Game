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
    if(mapData[currCharRow - 1][currCharCol] == 0) {
        mapData[currCharRow - 1][currCharCol] = 5;
        mapData[currCharRow][currCharCol] = 0;
        currCharRow--;
        stepCnt++;
        saveUndoData();
    } else if(mapData[currCharRow - 1][currCharCol] == 3) {
        mapData[currCharRow - 1][currCharCol] = 5;
        mapData[currCharRow][currCharCol] = 0;
        currCharRow--;
        stepCnt++;
        saveUndoData();
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
            saveUndoData();
        } else return;
    }
    targetOverwrite();
}

void map::downside() {
    if(mapData[currCharRow + 1][currCharCol] == 0) {
        mapData[currCharRow + 1][currCharCol] = 5;
        mapData[currCharRow][currCharCol] = 0;
        currCharRow++;
        stepCnt++;
        saveUndoData();
    } else if(mapData[currCharRow + 1][currCharCol] == 3) {
        mapData[currCharRow + 1][currCharCol] = 5;
        mapData[currCharRow][currCharCol] = 0;
        currCharRow++;
        stepCnt++;
        saveUndoData();
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
            saveUndoData();
        } else return;
    }
    targetOverwrite();
}

void map::rightside() {
    if(mapData[currCharRow][currCharCol + 1] == 0) {
        mapData[currCharRow][currCharCol + 1] = 5;
        mapData[currCharRow][currCharCol] = 0;
        currCharCol++;
        stepCnt++;
        saveUndoData();
    } else if(mapData[currCharRow][currCharCol + 1] == 3) {
        mapData[currCharRow][currCharCol + 1] = 5;
        mapData[currCharRow][currCharCol] = 0;
        currCharCol++;
        stepCnt++;
        saveUndoData();
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
            saveUndoData();
        } else return;
    }
    targetOverwrite();
}

void map::leftside() {
    if(mapData[currCharRow][currCharCol - 1] == 0) {
        mapData[currCharRow][currCharCol - 1] = 5;
        mapData[currCharRow][currCharCol] = 0;
        currCharCol--;
        stepCnt++;
        saveUndoData();
    } else if(mapData[currCharRow][currCharCol - 1] == 3) {
        mapData[currCharRow][currCharCol - 1] = 5;
        mapData[currCharRow][currCharCol] = 0;
        currCharCol--;
        stepCnt++;
        saveUndoData();
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
            saveUndoData();
        } else return;
    }
    targetOverwrite();
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

void map::moveAction(int row, int col) {

}
