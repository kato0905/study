#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "/usr/include/sqlite3.h"

int opencsv(char *datafile) {//csvファイルからテストデータ展開
  //x_location,y_location,score
  //1000x1000,100
  sqlite3 *conn = NULL;
  sqlite3_stmt *stmt = NULL;
  FILE *fp;
  int x_location, y_location, score;
  int ret = 0;
  const char *insert_to_point_table_sql = "insert into point_table(x_location, y_location, score) values(?, ?, ?)";

  fp = fopen(datafile,"r");//テストデータ展開
  if (fp == NULL) {
    printf("[%s] is not open!\n",datafile);
    return -1;
  }else{
    printf("[%s] is opened!\n",datafile);
  }


  ret = sqlite3_open("study.sqlite", &conn);//SQLite3に接続
  if(ret != SQLITE_OK){
    printf("database is not open!\n");
    return -1;
  }
  sqlite3_exec(conn, "delete from point_table;", NULL, NULL, NULL);//データをリセット
  sqlite3_prepare(conn, insert_to_point_table_sql, strlen(insert_to_point_table_sql), &stmt, NULL);


  //ファイルの読み込み
  while(fscanf(fp,"%d,%d,%d", &x_location, &y_location, &score)!=EOF) {
    //printf("%d,%d,%d\n", x_location, y_location, score);

    //データベース(point_table)に値を格納
    sqlite3_reset(stmt);
    //変数を設定
    sqlite3_bind_int(stmt, 1,  x_location);
    sqlite3_bind_int(stmt, 2,  y_location);
    sqlite3_bind_int(stmt, 3,  score);
    //SQL文の実行
    while(SQLITE_DONE != sqlite3_step(stmt)){}
  }

  //終了処理
  fclose(fp);
  sqlite3_finalize(stmt);
  sqlite3_close(conn);
  return 0;
}
