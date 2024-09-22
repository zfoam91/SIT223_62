#ifndef HEADER_H 
#define HEADER_H

#include <bits/stdc++.h>
using namespace std;

void count(int **arr, int **showarr, int n, int m);
void generate(int **arr, int n, int m, int k);
void bfstraversal(int **showarr, int **arr, int x, int y);
void exchange(int **arr, int x, int y, int i, int j);
void firstmove(int **arr, int x, int y, int n, int m);
bool check(int x, int y);
int **initarr(int **arr);

extern int n;
extern int m;
extern int fm;
extern int mc;
extern int w;
extern int cnt;
extern int time1;
extern int **arr;
extern int **showarr;
extern int game;
extern string str;

#endif 
