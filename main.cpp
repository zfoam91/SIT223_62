#include "header.hpp"
#include <SFML/Graphics.hpp>
using namespace sf;
int n=20, m=20, fm=0, mc=0, w=32, cnt=60, time1=0, **arr = nullptr, **showarr = nullptr;
int game=0;
string str="";

void init();
int main(){

	//textures and inits
	Clock clock;
	arr=initarr(arr);
	showarr=initarr(showarr);
	Texture t, ttimer, tboom, tgamestat, tretry;; 
    t.loadFromFile("tiles.png");
    ttimer.loadFromFile("timer.png");
    tboom.loadFromFile("boom.png");
    tgamestat.loadFromFile("game.png");
    tretry.loadFromFile("game.png");
    Sprite s(t);
    Sprite timer1(ttimer);
	Sprite timer2(ttimer);
	Sprite timer3(ttimer);
	Sprite mcnt1(ttimer);
	Sprite mcnt2(ttimer);
	Sprite gamestat(tgamestat);
	Sprite boom(tboom);
	Sprite retry(tretry);
	mcnt1.setPosition(64,0);
	mcnt2.setPosition(96,0);
	timer1.setPosition(544,0);
	timer2.setPosition(576,0);
	timer3.setPosition(608, 0);
	retry.setPosition(334,0);
 	
 	//creating the window and generating table
 	generate(arr, n,m, 60);
	count(arr, showarr, n, m);
    RenderWindow app(VideoMode(704, 736), "Minesweeper");
	
	while (app.isOpen()){
        Vector2i pos = Mouse::getPosition(app);
        int x = pos.x/w;
        int y = pos.y/w;
        Event e;
        app.clear(Color{ 0, 0, 0 });
        while(app.pollEvent(e)){
        	if(x > 22 || y > 23 || x < 0 || y < 0)continue;
            if(e.type == Event::Closed)app.close();
            if(e.type == Event::MouseButtonPressed)
              if(e.key.code == Mouse::Left){

              	//dont open the cell if it is flagged
              	if(showarr[x][y]==11)continue;

              	//opening a mine
              	if(arr[x][y]==9){
              		if(fm==0){
              			firstmove(arr, x, y,n,m);
              		}else{
              			game=1;
              		}
              	}

              	bfstraversal(showarr, arr, x, y);
              	fm++;

              	//pressing retry
              	if(x >=10 && x <=12 && y <=1){
              		clock.restart();
					arr=initarr(arr);
					showarr=initarr(showarr);
					mc=0, cnt=60, time1=0;
					generate(arr, n,m, 60);
					count(arr, showarr, n, m);
              		game=0;
              		retry.setTextureRect(IntRect(0, 0, 2*w, 2*w));
		           	app.draw(retry);
              	}

              }else if(e.key.code == Mouse::Right){

              	//flagging a cell
              	if(showarr[x][y]==10){
              		showarr[x][y]=11;
              		if(arr[x][y]==9)mc++;
              		cnt--;
              	}else if(showarr[x][y]==11){
              		showarr[x][y]=10;
              		if(arr[x][y]==9)mc--;
              		cnt++;
              	}
              }
        }
        //if(x > 22 || y > 23 || x < 0 || y < 0)continue;
        switch(game){

        	//if game is still going on
        	case 0:{
	        	for (int i=1;i<=n;i++){
		        	for (int j=2;j<=m+1;j++){
		            	//if(showarr[x][y]==9)showarr[i][j]=arr[i][j];
		           		s.setTextureRect(IntRect(showarr[i][j]*w,0,w,w));
		           		s.setPosition(i*w, j*w);
		           		retry.setTextureRect(IntRect(w*2, 0, 2*w, 2*w));
		           		app.draw(retry);
		           		app.draw(s);
		          	}
				}
				break;
        	}

        	//if the player lost
        	case 1:{
        		Texture tx;
				tx.loadFromFile("boom.png");
				Sprite sp(tx);
				for (int i=1;i<=n;i++){
		        	for (int j=2;j<=m+1;j++){
		            	if(arr[i][j]==9){
		            		sp.setPosition(i*w, j*w);
		            		sp.setTextureRect(IntRect(31,0, 32, 32));
		            		app.draw(sp);
		            	}else{
		            		if(showarr[i][j]==11){
		            			sp.setPosition(i*w, j*w);
		            			sp.setTextureRect(IntRect(0,0, 32, 32));
		            			app.draw(sp);
		            		}else{
				           		s.setTextureRect(IntRect(arr[i][j]*w,0,w,w));
				           		s.setPosition(i*w, j*w);
				           		app.draw(s);
				           	}
		           		}
		          	}
				}
				retry.setTextureRect(IntRect(w*2*3, 0, 2*w, 2*w));
		        app.draw(retry);
				break;
        	}

        	//if the player won
        	case 2:{
        		for (int i=1;i<=n;i++){
		        	for (int j=2;j<=m+1;j++){
		            	if(showarr[i][j]==11)arr[i][j]=11;
		           		s.setTextureRect(IntRect(arr[i][j]*w,0,w,w));
		           		s.setPosition(i*w, j*w);
		           		app.draw(s);
		          	}
				}
				retry.setTextureRect(IntRect(w*2*4, 0, 2*w, 2*w));
		        app.draw(retry);
        		break;
        	}
        }
        if(game==0){
			Time time= clock.getElapsedTime();
			time1 = (int)time.asSeconds();
		}

		//timer and mine counter
		timer3.setTextureRect(IntRect((time1%10)*w,0, 32, 64));
		timer2.setTextureRect(IntRect((time1%100/10)*w,0, 32, 64));
		timer1.setTextureRect(IntRect((time1/100)*w,0, 32, 64));
		mcnt1.setTextureRect(IntRect(cnt/10*w, 0, 32, 64));
		mcnt2.setTextureRect(IntRect(cnt%10*w, 0, 32, 64));
        app.draw(timer1);
		app.draw(timer2);
		app.draw(timer3);
		app.draw(mcnt1);
		app.draw(mcnt2);

		if(mc==60)game=2;
        app.display();
    }	
    for(int i=0; i<100;i++){
    	free(arr[i]);
    	free(showarr[i]);
    }
    free(arr);
    free(showarr);
    return 0;
}