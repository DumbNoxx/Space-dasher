#include <allegro.h>
#include <iostream>
#include <math.h>


void pintarNave(float,float,BITMAP*);//Crear la lineas de la nave
void caidaNave(float&,float&,float&,float&);//Para crear la caida de la nave
void rotar(float&,float&,float,float,float);
void aceleracion(float,float&,float&);//Acelera hacia arriba, der o izq
void pintarFuego(float,float,float,BITMAP*);//Crea el propulsor
void medidorCombustible(bool,float&,BITMAP*,bool);//Crea barra de combustible
void crearNivel(int,BITMAP*,bool);//Crea los niveles
void colision(float,float,BITMAP*,int,bool);//Hace la colision
bool gameOver(float,float,BITMAP*,int,bool);//Game over
bool aterrizar(float,float,float,float,BITMAP*,int,bool);//Funcion para aterrizar
bool choque(float,float,float,float,float,float,float,float);//Hace la que la nave choque con los obstaculos
bool choqueNave(int,float,float);//Hace que la nave choque
void botonClose(){
	 int result = alert("Salir","Seguro que quieres salir?", NULL,"&SI","&No",'s','n');
	 bool comprobar = false;
	 if(result == 1){
	 		   comprobar = true;
				} else {
				  	   comprobar = false;
						 }
	 }	




int main() {
	
	int depth, res,des;
	bool enable = true;
	allegro_init();
	
	depth = desktop_color_depth();
	if (depth == 0) depth = 64;//Selecciona 64 bits(en tal caso ser 64 bits)
	set_color_depth(depth);
	set_window_close_button(botonClose());
	res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, 740, 500, 0, 0);/*Es el tamanio
		  											 	  	 	de la ventana*/	 	
	if (res != 0) {												
		allegro_message(allegro_error);//Por si allegro llega a fallar
		exit(-1);
	}
	BITMAP *buffer = create_bitmap(740,500);//crea el mapa
	
	int x1=0,x2=30,y1=0,y2=30;
	
	
	install_timer();
	install_keyboard();//Para poder usar el teclado
	install_mouse();
	
	float cx=60,cy=217;
	float vx,vy;
	bool iniciar = false;
	float combustible = 100;
	int numNivel = 1;
	
	while (!key[KEY_ESC] && !gameOver(cx,cy,buffer,numNivel,iniciar) && !aterrizar(cx,cy,vx,vy,buffer,numNivel,iniciar)){
		bool gastarCombustible = false;  
		  
		clear_to_color(buffer,0x000000);
		crearNivel(numNivel,buffer,iniciar);
		
		
		if(key[KEY_SPACE]){//Esto inicia el juego si se le da al espacio
						   iniciar = true;
						   }				   
		switch(iniciar){
							   case true:caidaNave(cx,cy,vx,vy);
							   }
		
		if(key[KEY_UP]&&combustible > 0){
					aceleracion(0,vx,vy);
					pintarFuego(0,cx,cy,buffer);
					gastarCombustible = true;
					}
		if(key[KEY_RIGHT]&&combustible > 0){
						   aceleracion(-90,vx,vy);
						   pintarFuego(-90,cx,cy,buffer);
						   gastarCombustible = true;
						   }
 		if(key[KEY_LEFT]&&combustible > 0){
						   aceleracion(90,vx,vy);
						   pintarFuego(90,cx,cy,buffer);
						   gastarCombustible = true;
						   }
						   
						         
       medidorCombustible(gastarCombustible,combustible,buffer,iniciar);
       pintarNave(cx,cy,buffer);
	   blit(buffer, screen,0,0,0,0,740,500);
  		rest(10);//Hace un reset para que no se llene la memoria				   
  						   
	}
	
	clear_keybuf();//Limpia todo
	exit(0);
	return 0;
}
END_OF_MAIN()


void pintarNave(float cx, float cy, BITMAP *buffer){
	 line(buffer, cx-20,cy+20,cx-20,cy+10,0x1FB6B3);
	 line(buffer, cx-20,cy+20,cx-20,cy+10,0x1FB6B3);
	 line(buffer, cx-20,cy+10,cx-10,cy,0x1FB6B3);
	 line(buffer, cx-10,cy,cx-10,cy-10,0x1FB6B3);
	 line(buffer, cx-10,cy-10,cx,cy-15,0x1FB6B3);
	 line(buffer, cx,cy-15,cx+10,cy-10,0x1FB6B3);
	 line(buffer, cx+10,cy-10,cx+10,cy,0x1FB6B3);
	 line(buffer, cx+10,cy,cx+20,cy+10,0x1FB6B3);
	 line(buffer, cx+20,cy+10,cx+20,cy+20,0x1FB6B3);
	 line(buffer, cx-10,cy,cx+10,cy,0x1FB6B3);
	 }

void caidaNave(float &cx,float &cy,float &vx,float &vy){
	 float ax,ay;
	 
	 ax =0.0;
	 ay =0.1;
	 
	 vx+=ax;
	 vy+=ay;
	 
	 cx+=vx;
	 cy+=vy;
	 
	 }

void rotar(float &x,float &y,float cx,float cy,float da){
	 float dx=x-cx;
	 float dy=y-cy;
	 float r=sqrt(dx*dx+dy*dy);
	 float a=atan2(dy,dx);
	 
	 float PI = 3.14159265358979323846;
	 
	 float da_rad=da/180*PI;
	 
	 a-=da_rad;
	 
	 x=cx+r*cos(a);
	 y=cy+r*sin(a);	 
	 
	 }
	 

void aceleracion(float da, float &vx, float &vy){
	 float ax = 0, ay = -0.15;
	 rotar(ax,ay,0,0,da);
	 vx += ax;
 	 vy += ay;
				 
	 }
void pintarFuego(float da,float cx,float cy,BITMAP *buffer){
	 float c1;
	 float c2;
	 c1 = cx;
	 c2 = cy;
	 if(da != 0) c2+=9;
	 float fire[14]={c1-5,c2+5,c1-10,c2+20,c1-5,c2+20,c1,c2+35,c1+5,c2+20,c1+10,c2+20,c1+5,c2+5};
	 for(int i = 0;i<=12;i+=2){
	 		 rotar(fire[i],fire[i+1],cx,cy,da);
			 }
	 for(int i = 0;i<=10;i+=2){
	 		 line(buffer, fire[i],fire[i+1],fire[i+2],fire[i+3],0xFF5733);
			 }
	 }
void medidorCombustible(bool gastarCombustible,float &combustible,BITMAP *buffer,bool iniciar){
	 textout_centre_ex(buffer,font,"Combustible",100,30,0xFFFFFF,0x000000);
	 rectfill(buffer,50,50,50+combustible,55,0xFFFFFF);
	 if(iniciar == true){
	 			if(gastarCombustible == true) combustible -= 0.2;
				 }
	 }
	 
void crearNivel(int numLevel,BITMAP *buffer,bool iniciar){
	 if(numLevel == 0){
	 			 rectfill(buffer,610,450,705,500,0x999999);
	 			 textout_centre_ex(buffer,font,"TUTORIAL 0",350,30,0xFFFFFF,0x000000);
	 			 textout_centre_ex(buffer,font,"Tienes que aterrizar con suavidad en la plataforma",200,60,0x999999,0x000000);
				 textout_centre_ex(buffer,font,"antes de que se acabe el combustible.",150,70,0x999999,0x000000);
				 if(iniciar == false){
				 			rectfill(buffer,10,250,100,240,0x999999);textout_centre_ex(buffer,font,"PRESIONA ESPACIO PARA INICIAR EL NIVEL",200,80,0xFFFFFF,0x000000);
							 }			 
				 }
	 if(numLevel == 1){
	 			 textout_centre_ex(buffer,font,"TUTORIAL 1",350,30,0xFFFFFF,0x000000);
	 			 textout_centre_ex(buffer,font,"Esquiva los obstaculos",500,60,0xE7F3F1,0x000000);textout_centre_ex(buffer,font, "y aterriza con suavidad en la plataforma",550,70,0xE7F3F1,0x000000);
				 textout_centre_ex(buffer,font,"antes de que se acabe el combustible.",550,80,0xE7F3F1,0x000000);

	 			 
	 			 
	 			 //Obstaculos abajo
	 			 triangle(buffer, 110,100,300,500,110,500,0x999999);
	 			 triangle(buffer, 620,500,700,500,620,300,0x999999);
	 			 triangle(buffer, 500,700,900,700,620,300,0x999999);
	 			 
	 			 //Obstaculos arriba
	 			 triangle(buffer, 200,0,400,350,400,0,0x999999);
	 			 
	 			 //plataforma de aterrizaje
	 			 rectfill(buffer,600,250,705,200,0x999999);
	 			 
	 			 //Iniciar el juego
	  			 if(iniciar == false){
				 			rectfill(buffer,20,250,100,240,0x999999);textout_centre_ex(buffer,font,"PRESIONA ESPACIO PARA INICIAR EL NIVEL",200,80,0xFFFFFF,0x000000);
							 }	
				 }
	 }
	 
void colision(float cx,float cy,BITMAP *buffer,int numLevel,bool iniciar){
	 float x[12] = {cx-10,cx+10,cx,cx,cx+15,cx-15,cx+5,cx-10,cx+10,cx-5,cx-10,cx+10};
	 float y[12] = {cy,cy,cy-15,cy+15,cy-15,cy+15,cy+5,cy-10,cy-10,cy+10,cy,cy,};
	 float mx[6] = {7, 7,0,-7,-7,0}; 
	 float my[6] = {0,-7,-7,-7,0,7};
	 
	 clear(screen);
	 do{
		clear(buffer);
		crearNivel(numLevel,buffer,iniciar);
		int j = 0;
		
		for(int i = 0; i<=10;i+=2){
				line(buffer,x[i],y[i],x[i+1],y[i+1],0xFFFFFF);
				rotar(x[i+1],y[i+1],x[i],y[i],14);
				
				x[i] += mx[j];//Movimiento de la explosion
				y[i] += my[j];
				x[i+1] += mx[j];
				y[i+1] += my[j];
				j++;
				}
		blit(buffer,screen,0,0,0,0,740,500);
		rest(20);
		}while(!key[KEY_ESC]);
	 }
	 
bool gameOver(float cx,float cy,BITMAP *buffer,int numLevel,bool iniciar){
	 if(cx+20 >= 740 || cx-20 <= 0 || cy-15 <= 0 || cy+20 >= 500){
	 		  colision(cx,cy,buffer,numLevel,iniciar);
	 		  return true;
			  }
			  
	if(choqueNave(numLevel,cx,cy) == true){
							  colision(cx,cy,buffer,numLevel,iniciar);
							  return true;
							  }		  		  
	 return false;
	 }
bool aterrizar(float cx,float cy,float vx,float vy,BITMAP *buffer,int numLevel,bool iniciar){
	 if(numLevel == 0){
	 			 if(cy+20 >= 450 && cx-20 >= 610 && cx+20 <=710){
	 		  if(vy <= 4.7){
			  		return true;
					}else {
						  colision(cx,cy,buffer,numLevel,iniciar);
						  }
			  }
				  } else if(numLevel == 1){
				  		 if(cy+20 >= 205 && cx-20 >= 600 && cx+20 <=705){
	 		  if(vy <= 4.7){
			  		return true;
					}else {
						  colision(cx,cy,buffer,numLevel,iniciar);
						  }
			  }
						   }
	 return false;
	 
	 }

bool choque(float x1,float y1,float x2,float y2,float p1x,float p1y,float p2x,float p2y){
	 float m = (y2-y1)/(x2-x1);
	 // m>0 triangulo derecho, m<0 triangulo izquierdo
	 float b = y1-m*x1;
	 //ec de una recta es y=mx+b
	 if(m>0){
	 		 if(x1 <= p1x && p1x <= x2){
			 			if(p2y >= m*p1x + b) return true;
						 }else{
			 	  if(p1x <= x1 && x1 <= p2x){
			 	  		 if(y1 <= p2y) return true;
  	   		  			 	   	  	   		  }
						 	   }		 
		     }else if(m<0){
	 		 if(x1 <= p2x && p2x >= x2){
			 			if(p2y >= m*p2x + b) return true;
						 }
			 else{
			 	  if(p1x <= x2 && x2 <= p2x){
			 	  		 if(y2 <= p2y) return true;
						 	   	  	   		  }
				  }			 		 		 
			  }
	 return false;		  		  
	 }
	 
bool choqueNave(int numLevel,float cx,float cy){
	 //pata izquierda
	 float r1x = cx-20, r1y = cy;
	 float r2x = cx+20, r2y = cy+20;
	 //pata derecha
	 float p1x = cx-10, p1y = cy;
	 float p2x = cx+20, p2y = cy+20;
	 //cabeza
	 float q1x = cx-10, q1y = cy-15;
	 float q2x = cx+10, q2y = cy;
	 
	 
	 
	 if(numLevel == 1){
	 			 if(choque(110,100,300,500,r1x,r1y,r2x,r2y) == true || choque(110,100,300,500,p1x,p1y,p2x,p2y) == true || choque(110,100,300,500,q1x,q1y,q2x,q2y) == true) return true;										
	 			 if(choque(620,500,700,500,r1x,r1y,r2x,r2y) == true || choque(620,500,700,500,p1x,p1y,p2x,p2y) == true || choque(620,500,700,500,q1x,q1y,q2x,q2y) == true) return true;										
				 if(choque(500,700,900,700,r1x,r1y,r2x,r2y) == true || choque(500,700,900,700,p1x,p1y,p2x,p2y) == true || choque(500,700,900,700,q1x,q1y,q2x,q2y) == true) return true;										

				}			  
	 return false;
	 }
	 
 
	 
