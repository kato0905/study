#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GLUT/glut.h>
#include "/usr/include/sqlite3.h"
#include <time.h>


void display(void);

int GRID_RANGE;//gridの幅
int SCOPE_RANGE;
int Current_best_score[16] = {};//0で初期化
int Current_best_x_location[16] = {};
int Current_best_y_location[16] = {};

//rの値を変更する際は注意
int x_length = 50;//ユーザー入力の四角形
int y_length = 50;
int RANGE = 10;
int SCOPE = 10000;
//testdata.csvはうまく読み込めないが、testdata.txtは問題なく読み込める
char datafile[] = "testdata_3000.txt";//点の位置とスコアデータ
int Grid[1000+1][1000+1] = {};
int opencsv(int Grid[][1000+1]);
int maxrs(int Grid[][1000+1]);


int main(int argc, char *argv[]) {
  clock_t start, end;
  int i;

  opencsv(Grid);//配列Gridの中にx_location,y_location,scoreを挿入

  GRID_RANGE = RANGE;//display関数用(なぜかRANGEだと動かない)
  SCOPE_RANGE = SCOPE;//上に同じ
  printf("GRID_RANGE is %d\n",RANGE);
  printf("SCOPE_RANGE is %d\n",SCOPE);
  start = clock();
  maxrs(Grid);//maxrsとなる領域を見つける
  end = clock();
  for(i=0;i<16;i++){
    printf("%d,",Current_best_score[i]);
  }
  for(i=0;i<16;i++){
    printf("\n[%d,%d]%f,",Current_best_x_location[i],Current_best_y_location[i],(0.5+(double)i/10)*x_length);
  }
  printf("\n");
  printf("計算時間:%f\n",(double)(end - start) / CLOCKS_PER_SEC);


  glutInitWindowSize(640, 640);
  // OpenGL/GLUT 環境の初期化
  glutInit(&argc, argv);
  // ウィンドウを開く
  glutCreateWindow("MaxRS query");
  // ウィンドウへの描画関数の登録
  glutDisplayFunc(display);
  // メインループ開始
  glutMainLoop();
  printf("lfdajsfk");

  return 0;
}

void display(void) {
  FILE *fp;
  int x_location, y_location, score;

  fp = fopen(datafile,"r");//テストデータ展開
  if (fp == NULL) {
    printf("[%s] is not open!\n",datafile);
    return;
  }else{
    printf("[%s] is opened!\n",datafile);
  }

  glClear(GL_COLOR_BUFFER_BIT);
  glPointSize(5.0);
  //点の描画
  glBegin(GL_POINTS);
  glColor3d(0.0,0.0,1.0);//点の色を指定
  //ファイルの読み込み
  while(fscanf(fp,"%d,%d,%d", &x_location, &y_location, &score)!=EOF) {
    glVertex2f((float)(x_location-SCOPE_RANGE/2)*2/SCOPE_RANGE,(float)(y_location-SCOPE_RANGE/2)*2/SCOPE_RANGE);
  }

  //終了処理
  fclose(fp);
	glEnd();//設定終了
/*
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
*/
  glFlush();

  glClearColor(1.0,1.0,1.0,1.0);//背景色


}
