#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "/usr/include/sqlite3.h"


int RANGE;//gridで区切る格子の大きさ(初期)
int SCOPE;//x,y座標の最大値
char datafile[];


int opencsv(int Grid[][SCOPE/RANGE+1]) {//csvファイルからテストデータ展開
  //x_location,y_location,score
  //1000x1000,100
  FILE *fp;
  int x_location, y_location, score;

  fp = fopen(datafile,"r");//テストデータ展開
  if (fp == NULL) {
    printf("[%s] is not open!\n",datafile);
    return -1;
  }else{
    printf("[%s] is opened!\n",datafile);
  }

  //ファイルの読み込み
  while(fscanf(fp,"%d,%d,%d", &x_location, &y_location, &score)!=EOF) {
    //gridに値を挿入
    Grid[x_location/RANGE][y_location/RANGE] += score;
  }

  //終了処理
  fclose(fp);
  return 0;
}
