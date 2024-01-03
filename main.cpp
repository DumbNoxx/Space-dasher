#include <allegro.h>
#include <iostream>
#include <math.h>


void pintarNave(float,float,BITMAP*);//Crear la lineas de la nave
void caidaNave(float&,float&,float&,float&);//Para crear la caida de la nave
void rotar(float&,float&,float,float,float);
void aceleracion(float,float&,float&);//Acelera hacia arriba, der o izq
void pintarFuego(float,float,float,BITMAP*);//Crea el propulsor
void medidorCombustible(bool,float&,BITMAP*);//Crea barra de combustible
void crearNivel(int,BITMAP*);//Crea los niveles
void colision(float,float,BITMAP*,int);//Hace la colision
bool gameOver(float,float,BITMAP*,int);//Game over
bool aterrizar(float,float,float,float,BITMAP*,int);//Funcion para aterrizar
bool choque(float,float,float,float,float,float,float,float);
bool choqueNave(int,float,float);



int main() {
	
	int depth, res;
	allegro_init();
	depth = desktop_color_depth();
	if (depth == 0) depth = 64;//Selecciona 64 bits(en tal caso ser 64 bits)
	set_color_depth(depth);
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
	
	float cx=80,cy=30;
	float vx,vy;
	float combustible = 100;
	int numNivel = 2;
	
	while (!key[KEY_ESC] && !gameOver(cx,cy,buffer,numNivel) && !aterrizar(cx,cy,vx,vy,buffer,numNivel)){
		bool gastarCombustible = false;  
		  
		clear_to_color(buffer,0x000000);
		crearNivel(numNivel, buffer);
		caidaNave(cx,cy,vx,vy);
		
		
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
						   
						         
       medidorCombustible(gastarCombustible,combustible,buffer);
       pintarNave(cx,cy,buffer);
	   blit(buffer, screen,0,0,0,0,740,500);
  		rest(10);//Hace un reset para que no se llene la memoria				   
  						   
	}
	clear_keybuf();//Limpia todo
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
void medidorCombustible(bool gastarCombustible,float &combustible,BITMAP *buffer){
	 textout_centre_ex(buffer,font,"Combustible",100,30,0xFFFFFF,0x000000);
	 rectfill(buffer,50,50,50+combustible,55,0xFFFFFF);
	 if(gastarCombustible == true) combustible -= 0.2;
	 }
	 
void crearNivel(int numLevel,BITMAP *buffer){
	 if(numLevel == 1){
	 			 rectfill(buffer,10,450,100,500,0x999999);
				 }
	 if(numLevel == 2){
	 			 triangle(buffer, 400,500,300,500,300,200,0x999999);
	 			 triangle(buffer, 300,0,500,0,500,400,0x999999);
	 			 triangle(buffer, 620,500,700,500,620,300,0x999999);
	 			 triangle(buffer, 110,100,300,500,110,500,0x999999);
	 			 
				  rectfill(buffer,10,450,100,500,0x999999);
				 }
	 }
	 
void colision(float cx,float cy,BITMAP *buffer,int numLevel){
	 float x[12] = {cx-10,cx+10,cx,cx,cx+15,cx-15,cx+5,cx-10,cx+10,cx-5,cx-10,cx+10};
	 float y[12] = {cy,cy,cy-15,cy+15,cy-15,cy+15,cy+5,cy-10,cy-10,cy+10,cy,cy,};
	 float mx[6] = {7, 7,0,-7,-7,0}; 
	 float my[6] = {0,-7,-7,-7,0,7};
	 
	 clear(screen);
	 do{
		clear(buffer);
		crearNivel(numLevel,buffer);
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
	 
bool gameOver(float cx,float cy,BITMAP *buffer,int numLevel){
	 if(cx+20 >= 740 || cx-20 <= 0 || cy-15 <= 0 || cy+20 >= 500){
	 		  colision(cx,cy,buffer,numLevel);
	 		  return true;
			  }
			  
	if(choqueNave(numLevel,cx,cy) == true){
							  colision(cx,cy,buffer,numLevel);
							  return true;
							  }		  		  
	 return false;
	 }
bool aterrizar(float cx,float cy,float vx,float vy,BITMAP *buffer,int numLevel){
	 if(cy+20 >= 450 && cx-20 >=10 && cx+20 <=100){
	 		  if(vy <= 4.7){
			  		return true;
					}else {
						  colision(cx,cy,buffer,numLevel);
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
	 		 if(x1<=p1x && p1x >= x2){
			 			if(p2y >= m*p1x+b) return true;
						 }
			 else{
			 	  if(p1x <= x1 && x1 <= p2x)
			 	  		 if(y1 <= p2y) return true;
				  }			 		 		 
			  }else if(m<0){
	 		 if(x1<=p2x && p2x >= x2){
			 			if(p2y >= m*p2x+b) return true;
						 }
			 else{
			 	  if(p1x <= x2 && x2 <= p2x)
			 	  		 if(y2 <= p2y) return true;
				  }			 		 		 
			  }
	 return false;		  		  
	 }
	 
bool choqueNave(int numLevel,float cx,float cy){
	 float r1x = cx-20, r1y = cy-15;
	 float r2x = cx+20, r2y = cy+20;
	 
	 if(numLevel == 2){
	 			 if(choque(110,100,300,500,r1x,r1y,r2x,r2y) == true){
				 	return true;										
					 }
				  }
	 return false;
	 }
	 
	 
	 
