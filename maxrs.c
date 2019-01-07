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

  int i, j, is;
  double r;
  int x_point, y_point;
  int x_reduction, y_reduction;
  int current_2_score=0, current_score=0, before_score=0;
  int grid_subtraction, grid_addition;

  //初めの一つを検索する
  x_point = 0;
  y_point = 0;
  //r=2の時のスコアを求める
  for(i=x_point;i<=x_point+2*x_length;i++){
    for(j=y_point;j<=y_point+2*y_length;j++){
      current_2_score += Grid[i][j];//r=2の時のスコアを求める
    }
  }
  Current_best_score[15] = current_2_score;
  Current_best_x_location[15] = x_point;
  Current_best_y_location[15] = y_point;
  //r=0.5~1.9までを探索
  for(is=0;is<15;is++){
    r = 0.5+(double)is/10;
    //範囲を動かす
    for(y_reduction=0; y_reduction<=(2-r)*y_length; y_reduction++){
      for(x_reduction=0; x_reduction<=(2-r)*x_length; x_reduction++){
        //スコアを求める
        current_score = 0;
        if(x_reduction != 0){
          grid_subtraction = 0;
          grid_addition = 0;
          for(j=y_point+y_reduction; j<=y_point+y_reduction+r*y_length; j++){
            grid_subtraction += Grid[x_point+x_reduction-1][j];
            grid_addition += Grid[x_point+x_reduction+(int)(r*x_length)][j];
          }
          current_score = before_score-grid_subtraction+grid_addition;
        }else if(x_reduction == 0){
          for(i=x_point+x_reduction;i<=x_point+x_reduction+r*x_length;i++){
            for(j=y_point+y_reduction;j<=y_point+y_reduction+r*y_length;j++){
              current_score += Grid[i][j];
            }
          }
        }
        if(Current_best_score[is] < current_score){
          Current_best_score[is] = current_score;
          Current_best_x_location[is] = x_point+x_reduction;
          Current_best_y_location[is] = y_point+y_reduction;
        }
        before_score=current_score;
      }
    }
  }
  current_2_score = 0;
  current_score = 0;



//次に,追加した部分のみの探索を行う
for(y_point=0;y_point<=SCOPE/RANGE-2*y_length;y_point++){
  for(x_point=0;x_point<=SCOPE/RANGE-2*x_length;x_point++){
//if(x_point == 0 && y_point == 0){
//  x_point++;
//}
    //探索する価値があると判断したら探索(検索範囲がS0.9より小さければ,r=0.5,~0.8を探索)
    //r=2の時のスコアを求める
    for(i=x_point;i<=x_point+2*x_length;i++){
      for(j=y_point;j<=y_point+2*y_length;j++){
        current_2_score += Grid[i][j];//r=2の時のスコアを求める
      }
    }
    if(Current_best_score[15] < current_2_score){
      Current_best_score[15] = current_2_score;
      Current_best_x_location[15] = x_point;
      Current_best_y_location[15] = y_point;
    }

    //r=0.5~1.9までを探索
    for(is=0;is<15;is++){
      if(Current_best_score[is] < current_2_score){
        r = 0.5+(double)is/10;
        //範囲を動かす
        if(x_point != 0){//横に探索が広がる時(x_pointが一つ増える時)
          for(y_reduction=0; y_reduction<=(2-r)*y_length; y_reduction++){
            if(y_reduction == 0){//下端
              for(i=x_point+2*x_length-(int)(r*x_length);i<=x_point+2*x_length;i++){
                for(j=y_point+y_reduction;j<=y_point+y_reduction+r*y_length;j++){
                  current_score += Grid[i][j];
                }
              }
            }else if(y_reduction != 0){
              grid_addition = 0;
              grid_subtraction = 0;
              for(i=x_point+2*x_length-(int)(r*x_length);i<=x_point+2*x_length;i++){
                grid_addition += Grid[i][y_point+y_reduction+(int)(r*y_length)];
                grid_subtraction += Grid[i][y_point+y_reduction-1];
              }
              current_score = before_score + grid_addition - grid_subtraction;
            }
            if(Current_best_score[is] < current_score){
              Current_best_score[is] = current_score;
              Current_best_x_location[is] = x_point+2*x_length-(int)(r*x_length);
              Current_best_y_location[is] = y_point+y_reduction;
            }
            before_score = current_score;
            current_score = 0;
          }

        }else if(x_point == 0){//探索が一段上がる時(y_pointが一つ増える時)

          for(x_reduction=0; x_reduction<=(2-r)*x_length; x_reduction++){
            if(x_reduction == 0){
              for(i=x_point+x_reduction;i<=x_point+x_reduction+r*x_length;i++){
                for(j=y_point+2*y_length-(int)(r*y_length);j<=y_point+2*y_length;j++){
                  current_score += Grid[i][j];
                }
              }
            }else if(x_reduction != 0){
              grid_addition = 0;
              grid_subtraction = 0;
              for(j=y_point+2*y_length-(int)(r*y_length);j<=y_point+2*y_length;j++){
                grid_addition = Grid[x_point+x_reduction+(int)(r*x_length)][j];
                grid_subtraction = Grid[x_point+x_reduction-1][j];
              }
              current_score = before_score + grid_addition - grid_subtraction;
            }
            if(Current_best_score[is] < current_score){
              Current_best_score[is] = current_score;
              Current_best_x_location[is] = x_point+x_reduction;
              Current_best_y_location[is] = y_point+2*y_length-(int)(r*y_length);
            }
            before_score = current_score;
            current_score = 0;
          }

        }
      }else{
        //もうCurrent_best_scoreが更新される余地がない
        break;
      }
    }
    current_2_score = 0;
  }
}

  return 0;
}
