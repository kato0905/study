
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "/usr/include/sqlite3.h"
#include <math.h>

int Current_best_score[16];
int Current_best_x_location[16];
int Current_best_y_location[16];
int x_length;//ユーザー入力の四角形
int y_length;
int RANGE;
int SCOPE;


int maxrs(int Grid[][SCOPE/RANGE+1]){
  int xp,yp,x,y,currentscore=0,maxscore=0;
  int max_x,max_y;
  float r=1.1;
  for(yp=0;yp<=SCOPE/RANGE-r*y_length;yp++){
    for(xp=0;xp<=SCOPE/RANGE-r*x_length;xp++){
      //スコアを計算
      for(y=yp;y<=yp+r*y_length;y++){
        for(x=xp;x<=xp+r*x_length;x++){
          currentscore += Grid[x][y];
        }
      }

      if(currentscore > maxscore){
        maxscore = currentscore;
        max_x = xp;
        max_y = yp;
      }
      currentscore = 0;
    }
  }
  printf("rが%fの時のMaxScoreは%dです\n",r,maxscore);
  printf("[%d,%d]\n",max_x,max_y);
  return 0;
}
