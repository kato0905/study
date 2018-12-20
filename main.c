#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GLUT/glut.h>
#include "/usr/include/sqlite3.h"

int opencsv(char *datafile);
int init(void);
int maxrs(void);
void display(void);
int count_element( void * , int , char ** , char ** );

int GRID_RANGE = 10;//gridの幅
int SCOPE_RANGE;
int maxrs_x_location;
int maxrs_y_location;
int maxrs_x_length;//grid何幅分の大きさか
int maxrs_y_length;
float maxrs_score;
int RANGE;
int SCOPE;


int main(int argc, char *argv[]) {
  char datafile[] = "testdata_1000.txt";//点の位置とスコアデータ
  //testdata.csvはうまく読み込めないが、testdata.txtは問題なく読み込める

  opencsv(datafile);

  init();//RANGEの内容変更
  GRID_RANGE = RANGE;//display関数用(なぜかRANGEだと動かない)
  SCOPE_RANGE = SCOPE;//上に同じ
  printf("GRID_RANGE is %d\n",RANGE);
  printf("SCOPE_RANGE is %d\n",SCOPE);
  maxrs();//maxrsとなる領域を見つける
  printf("maxrs_score is %f\n", maxrs_score);
  printf("maxrs_x_location is %d\n",maxrs_x_location);
  printf("maxrs_y_location is %d\n",maxrs_y_location);
  printf("maxrs_x_length is %d\n",maxrs_x_length);
  printf("maxrs_y_length is %d\n",maxrs_y_length);

  glutInitWindowSize(640, 640);
  // OpenGL/GLUT 環境の初期化
  glutInit(&argc, argv);
  // ウィンドウを開く
  glutCreateWindow("MaxRS query");
  // ウィンドウへの描画関数の登録
  glutDisplayFunc(display);
  // メインループ開始
  glutMainLoop();

  return 0;
}

void display(void) {
  sqlite3 *conn = NULL;
  sqlite3_stmt *stmt = NULL;
  int ret;

  ret = sqlite3_open("study.sqlite", &conn);//SQLite3に接続
  if(ret != SQLITE_OK){
    printf("database is not open!\n");
    return;
  }

  sqlite3_prepare_v2(conn, "SELECT * FROM point_table", 64, &stmt, NULL);


  glClear(GL_COLOR_BUFFER_BIT);
  glPointSize(5.0);

	//点の描画
	glBegin(GL_POINTS);
	glColor3d(0.0,0.0,1.0);//点の色を指定
  while(sqlite3_step(stmt) == SQLITE_ROW ){//座標を設定
    int get_x_location = sqlite3_column_int(stmt, 1);
    int get_y_location = sqlite3_column_int(stmt, 2);
    glVertex2f((float)(get_x_location-500)*0.002,(float)(get_y_location-500)*0.002);
  }
	glEnd();//設定終了

  //MaxRSの四角形を描画(左下の座標を指定)
  glBegin(GL_LINE_LOOP);
  glColor3d(0.0,1.0,0);
  //(maxrs_x_location*GRID_RANGE-SCOPE_RANGE/2)で値は
  // -SCOPE_RANGE/2 ~ SCOPE_RANGE/2までの範囲に収まる
  //それに2/SCOPE_RANGEをかけることで、値を -1 ~ 1の範囲に収める
  glVertex2d((float)(maxrs_x_location*GRID_RANGE-SCOPE_RANGE/2)*2/SCOPE_RANGE,(float)(maxrs_y_location*GRID_RANGE-SCOPE_RANGE/2)*2/SCOPE_RANGE);
  glVertex2d((float)((maxrs_x_location+maxrs_x_length)*GRID_RANGE-SCOPE_RANGE/2)*2/SCOPE_RANGE,(float)(maxrs_y_location*GRID_RANGE-SCOPE_RANGE/2)*2/SCOPE_RANGE);
  glVertex2d((float)((maxrs_x_location+maxrs_x_length)*GRID_RANGE-SCOPE_RANGE/2)*2/SCOPE_RANGE,(float)((maxrs_y_location+maxrs_y_length)*GRID_RANGE-SCOPE_RANGE/2)*2/SCOPE_RANGE);
  glVertex2d((float)(maxrs_x_location*GRID_RANGE-SCOPE_RANGE/2)*2/SCOPE_RANGE,(float)((maxrs_y_location+maxrs_y_length)*GRID_RANGE-SCOPE_RANGE/2)*2/SCOPE_RANGE);
  glEnd();

  glFlush();

  glClearColor(1.0,1.0,1.0,1.0);//背景色

  sqlite3_finalize(stmt);
  sqlite3_close(conn);
}
