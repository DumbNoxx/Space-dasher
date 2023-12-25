#include <allegro.h>
#include <iostream>
#include <math.h>


void pintarNave(float,float,BITMAP*);//Crear la lineas de la nave
void caidaNave(float&,float&,float&,float&);//Para crear la caida de la nave
void aceleracion(float,float&,float&);//Acelera hacia arriba, der o izq
void pintarFuego(float,float,float,BITMAP*);//Crea el propulsor
void medidorCombustible(bool,float&,BITMAP*);

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
	
	float cx=100,cy=100;
	float vx,vy;
	float combustible = 100;
	
	while (!key[KEY_ESC]) {
		bool gastarCombustible = false;  
		  
		clear_to_color(buffer,0x000000);
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
