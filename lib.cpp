#include "header.hpp"
using namespace std;
int arrx[]={-1,-1,-1,0,0,1,1,1};
int arry[]={-1,0,1,-1,1,-1,0,1};

int n=20, m=20, fm=0, mc=0, w=32, cnt=60, time1=0, **arr = nullptr, **showarr = nullptr;
int game=0;
string str="";

int **initarr(int **arr){
	arr=(int**)calloc((100) ,sizeof(int*));
	for(int i=0;i<100;i++){
		arr[i]=(int*)calloc((100), sizeof(int));
	}
	return arr;
}

bool check(int x, int y){
	if(x>0 && x <=20 && y>0 && y <=21)return true;
	return false;
}
void bfstraversal(int **showarr, int **arr, int x, int y){
    queue<pair<int,int>>q;
    q.push({x,y});
    while (!q.empty()){
        int a = q.front().first;
        int b= q.front().second;
        showarr[a][b]=arr[a][b];
        //std::cout<< a << " " << b << " " << arr[a][b]<< std::endl;
        q.pop();
        for(int i=0;i<8;i++){
        	if(showarr[a+arrx[i]][b+arry[i]]==10 && check(a+arrx[i], b+arry[i]) && arr[a][b]==0){
        		q.push({a+arrx[i], b+arry[i]});
        	}
        }
    }
}

void generate(int **arr, int n, int m, int k){
	srand(time(NULL));
	while(k>0){
		int x=rand()%n+1, y=rand()%m+1;
		if(arr[x][y]!=9){
			arr[x][y]=9;
			k--;
		}
	}
	return;
}
void count(int **arr,int **showarr, int n, int m){
	for(int i=1;i<=n;i++){
		for(int j=1;j<=m+1;j++){
			showarr[i][j]=10;
			if(arr[i][j]!=9){
				for(int q=0;q<8;q++)
					if(arr[i+arrx[q]][j+arry[q]]==9)arr[i][j]++;	
			}
		}
	}
	return;
}
void exchange(int **arr, int x, int y, int i, int j){
	for(int q=0;q<8;q++){
		if(arr[x+arrx[q]][y+arry[q]]!=9)arr[x+arrx[q]][y+arry[q]]--;
		if(arr[i+arrx[q]][j+arry[q]]!=9)arr[i+arrx[q]][j+arry[q]]++;
	}
	return;
}
void newcnt(int **arr, int x, int y){
	arr[x][y]=0;
	for(int q=0;q<8;q++){
		if(arr[x+arrx[q]][y+arry[q]]==9)arr[x][y]++;
	}
	return;

}
void firstmove(int **arr, int x, int y, int n, int m){
	if(arr[x][y]==9){
		for(int i=1;i<=n;i++){
			for(int j=2;j<=m+1;j++){
				if(arr[i][j]!=9){
					exchange(arr, x, y, i, j);
					newcnt(arr, x, y);
					arr[i][j]=9;
					return;
				}
			}
		}
	}
	return;
}
