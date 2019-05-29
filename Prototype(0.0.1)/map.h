#ifndef _MAP_H_
#define _MAP_H_

extern int stepCnt, pushCnt, numStage;

class map {

private:
    int r, c, currCharRow, currCharCol;
    int **initMapData, **mapData, **undoData, **targetData;

public:
    map(const int arr[]);

    void restartMap();

    void saveUndoData();

    bool clearChk();

    void upside();

    void downside();

    void rightside();

    void leftside();

    void moveAction(int rowChange, int colChange);

    void moveCharacter(int rowChange, int colChange);

    void undo();

    void targetOverwrite();

    int getRow();

    int getCol();

    int** getMapData();
};
#endif