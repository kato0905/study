#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "/usr/include/sqlite3.h"

int count_element( void * , int , char ** , char ** );

int element_number = -1;//テストデータのデータ数
int RANGE = 10;//gridで区切る格子の大きさ(初期)
int SCOPE = 1000;//x,y座標の最大値
int max_score_in_grid = 0;//グリッド内の最大のスコア





int init(void) {//データベースのデータを格子状に整理する
  sqlite3 *conn = NULL;
  sqlite3_stmt *stmt = NULL;
  sqlite3_stmt *stmt_select_point_table_by_id = NULL;
  int ret = 0, i, j, get_x_location, get_y_location, get_score;
  int grid_matrix[SCOPE/RANGE+1][SCOPE/RANGE+1];//テストデータの最大の大きさ/RANGE
  char find_id_sql[256];
  const char *insert_to_grid_table_sql = "insert into grid_table(x_range, y_range, number) values(?, ?, ?)";

  //gridの初期化
  for(i = 0; i <= SCOPE/RANGE; i++){
    for(j = 0; j <= SCOPE/RANGE; j++){
      grid_matrix[i][j] = 0;
    }
  }

  ret = sqlite3_open("study.sqlite", &conn);//SQLite3に接続
  if(ret != SQLITE_OK){
    printf("database is not open!\n");
    return -1;
  }
  sqlite3_exec(conn, "delete from grid_table;", NULL, NULL, NULL);//データをリセット
  sqlite3_prepare(conn, insert_to_grid_table_sql, strlen(insert_to_grid_table_sql), &stmt, NULL);
  sqlite3_exec(conn, "select count(score) from point_table;", count_element, NULL, NULL);//データの要素数のカウント


  for(i=1;i<element_number+1;i++){//grid_matrixにセット
    //idがiの要素のx_location,y_location,scoreを取得
    sprintf(find_id_sql, "select * from point_table where id = %d;", i);
    sqlite3_prepare_v2(conn, find_id_sql, 128, &stmt_select_point_table_by_id, NULL);

    while(sqlite3_step(stmt_select_point_table_by_id) == SQLITE_ROW ){
      get_x_location = sqlite3_column_int(stmt_select_point_table_by_id, 1);
      get_y_location = sqlite3_column_int(stmt_select_point_table_by_id, 2);
      get_score = sqlite3_column_int(stmt_select_point_table_by_id, 3);

      //gridに値を挿入
      grid_matrix[get_x_location/RANGE][get_y_location/RANGE] += get_score;

      if(grid_matrix[get_x_location/RANGE][get_y_location/RANGE] > max_score_in_grid){
        max_score_in_grid = grid_matrix[get_x_location/RANGE][get_y_location/RANGE];
      }
    }
  }

printf("max_score_in_grid is %d\n",max_score_in_grid);

  //grid_tableにセット
  for(i=0; i<=SCOPE/RANGE; i++){
    for(j=0; j<=SCOPE/RANGE; j++){
      if(grid_matrix[i][j] != 0){
        sqlite3_reset(stmt);
        sqlite3_bind_int(stmt, 1,  i);
        sqlite3_bind_int(stmt, 2,  j);
        sqlite3_bind_int(stmt, 3,  grid_matrix[i][j]);
        while(SQLITE_DONE != sqlite3_step(stmt)){}
      }
    }
  }

  //終了処理
  sqlite3_finalize(stmt);
  sqlite3_close(conn);
  return 0;
}



//コールバック関数
int count_element(void *get_prm, int col_cnt, char **row_txt, char **col_name){
  element_number = atoi(row_txt[0]);//要素数をグローバル関数にセット
  return 0;
}
