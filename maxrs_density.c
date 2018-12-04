#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "/usr/include/sqlite3.h"

int maxrs_x_location;
int maxrs_y_location;
int maxrs_x_length;
int maxrs_y_length;
int max_score_in_grid;
float maxrs_score;
int RANGE;
int SCOPE;


int maxrs(void){
  sqlite3 *conn = NULL;
  sqlite3_stmt *stmt_find_total_score_in_scope = NULL;
  sqlite3_stmt *stmt_find_total_score_in_grid = NULL;
  int ret, i, j, decision = 0;
  int x_reduction, y_reduction, x_length, y_length;
  char find_total_score_sql[256];
  int total_score_in_grid;
  float alpha = 0.5, total_score_in_scope, current_score;

  ret = sqlite3_open("study.sqlite", &conn);//SQLite3に接続
  if(ret != SQLITE_OK){
    printf("database is not open!\n");
    return -1;
  }


  //MaxRSの初期値として、XY座標全体を領域とした場合を求める
  sqlite3_prepare_v2(conn, "SELECT total(score) FROM point_table;", 64, &stmt_find_total_score_in_scope, NULL);
  while(sqlite3_step(stmt_find_total_score_in_scope) == SQLITE_ROW ){//座標を設定
    total_score_in_scope = sqlite3_column_int(stmt_find_total_score_in_scope, 0);
  }
  maxrs_x_location = 0;
  maxrs_y_location = 0;
  maxrs_x_length = SCOPE/RANGE;
  maxrs_y_length = SCOPE/RANGE;
  maxrs_score = (float)total_score_in_scope/maxrs_x_length/maxrs_y_length;


  //一つずつ領域を小さくしていき、最適解を求める
  for(x_reduction=1; x_reduction<SCOPE/RANGE; x_reduction++){//x_reductionはSCOPEに比べて何マス分範囲が縮小されているか
    for(y_reduction=0; y_reduction<SCOPE/RANGE; y_reduction++){

      x_length = SCOPE/RANGE - x_reduction;//領域のx方向の大きさ
      y_length = SCOPE/RANGE - y_reduction;//領域のy方向の大きさ

      for(i=0; i<=x_reduction; i++){//x方向に動かす回数
        for(j=0; j<=y_reduction; j++){//y方向に動かす回数

          sprintf(find_total_score_sql, "select sum(number) from grid_table where x_range >= %d and x_range <= %d and y_range >= %d and y_range <= %d;", 0+i, x_length+i-1, 0+j, y_length+j-1);
          sqlite3_prepare_v2(conn, find_total_score_sql, 512, &stmt_find_total_score_in_grid, NULL);

          while(sqlite3_step(stmt_find_total_score_in_grid) == SQLITE_ROW ){
            total_score_in_grid = sqlite3_column_int(stmt_find_total_score_in_grid, 0);//現在のグリッドの合計スコア
          }

          current_score = (float)total_score_in_grid/x_length/y_length;

          //現在のスコアが従来のmaxrs_scoreを超えたら行われる処理
          if(current_score > maxrs_score){
            maxrs_x_location = i;
            maxrs_y_location = j;
            maxrs_x_length = x_length;
            maxrs_y_length = y_length;
            maxrs_score = current_score;
          /*  decision = 1;//終了処理
            break;*/
          }
        }
        if(decision == 1){
          break;
        }
      }
      if(decision == 1){
        break;
      }
    }
    if(decision == 1){
      break;
    }
  }


  sqlite3_finalize(stmt_find_total_score_in_scope);
  sqlite3_finalize(stmt_find_total_score_in_grid);
  sqlite3_close(conn);
  return 0;
}
