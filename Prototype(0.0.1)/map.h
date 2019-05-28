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

    void moveAction(int row, int col);
    //이동 코드 재사용성 향상을 위한 부분

    void undo();

    void targetOverwrite();

    int getRow();

    int getCol();

    int** getMapData();
};
#endif