/***************************************************************************
 *            main.c
 *
 *  Tue Jun 13 16:54:01 2006
 *  Copyright  2006  John Edgar Congote Calle
 *  Email jcongote@gmail.com
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/***************************************************************************
 *
 *  Modified: March 2008 by John Edgar Congote Calle
 *  The root operation was change to work better with integers.
 * 
 ****************************************************************************/

#include <stdio.h>
#include <math.h>
#include <allegro.h>

typedef enum _operacion {	suma,
							resta,
							multiplicacion,
							division,
							raiz,
							potencia,
							resultado}
operacion;


typedef struct _Ejercicio{
	int nejercicio;	//Identifica el numero del ejercicio
	int resultado; //Guarda el resultado del ejercicio
	int mostrable; //Define si el ejercicio puede ser utilizado en el concurso o no
	int nsegmentos; //Indica el numero de elementos que contiene un ejercicio

	int *valores;
	operacion *operaciones;
} Ejercicio;

typedef struct _ListaEjercicios{
	int n;
	Ejercicio *ejercicios;
} ListaEjercicios;

int oper_potencia(int base, int p){
    int i;
    int r=1;
    for (i=0;i<p;i++)
        r*=base;
	return r;
}

int oper_raiz(int base, int r){
    //printf("Raiz: base %i, raiz: %i, resultado: %e-%i\n",base,r,powf(base,1.0/r),(int)powf(base,1.0/r));
	//return powf(base,1.0/r);

	if (base>0){
        int res;
        for (res=1;res<base;res++){
            if (oper_potencia(res,r)==base)
            {
                return res;
            }
        }
	}
    return 0;
}

void leer_ejercicios(const char *narc, ListaEjercicios *j){
	FILE *arc;
	int i;

	arc = fopen(narc,"r");

	fscanf(arc,"%d",&(j->n));

	j->ejercicios = (Ejercicio*) malloc (sizeof(Ejercicio)*j->n);

	for (i=0;i<j->n;i++){
		char t[100];
		int numero[100];
		int m,k=0;
		int OrgResult;
		do{
			fscanf(arc,"%d %c",&(numero[k]),&(t[k]));
			k++;
		} while (t[k-1]!='=');

		fscanf(arc,"%i",&OrgResult);

		j->ejercicios[i].nejercicio = i+1;
		j->ejercicios[i].mostrable = 1;
		j->ejercicios[i].nsegmentos = k;
		j->ejercicios[i].valores = (int*) malloc (sizeof(int)*k);
		j->ejercicios[i].operaciones = (operacion*) malloc(sizeof(operacion)*k);

		j->ejercicios[i].resultado=numero[0];

		for (m=0;m<k;m++){
			j->ejercicios[i].valores[m]=numero[m];
			switch (t[m]){
				case '+':
					j->ejercicios[i].operaciones[m]=suma;
					j->ejercicios[i].resultado += numero[m+1];
					break;
				case '-':
					j->ejercicios[i].operaciones[m]=resta;
					j->ejercicios[i].resultado -= numero[m+1];
					break;
				case '*':
					j->ejercicios[i].operaciones[m]=multiplicacion;
					j->ejercicios[i].resultado *= numero[m+1];
					break;
				case '/':
					j->ejercicios[i].operaciones[m]=division;
					if(j->ejercicios[i].resultado % numero[m+1] != 0){
                            fprintf(stderr,"Error en el ejercicio div %i-%i:%i\n",i,m,j->ejercicios[i].resultado);
                    }
					j->ejercicios[i].resultado /= numero[m+1];
					break;
				case 'r':
					j->ejercicios[i].operaciones[m]=raiz;
					j->ejercicios[i].resultado = oper_raiz(j->ejercicios[i].resultado, numero[m+1]);
					break;
				case 'p':
					j->ejercicios[i].operaciones[m]=potencia;
					j->ejercicios[i].resultado = oper_potencia(j->ejercicios[i].resultado, numero[m+1]);
					break;
				case '=':
					j->ejercicios[i].operaciones[m]=resultado;
					break;
			}
        }
        if(j->ejercicios[i].resultado != OrgResult){
            fprintf(stderr, "Error en el ejericico res %i: %i-%i\n",i,j->ejercicios[i].resultado,OrgResult);
        }
	}
	fclose(arc);
}

void mostrar_imagen(BITMAP *img){
	if (img!=NULL){
		acquire_screen();
		blit(img,screen,0,0,0,0,img->w, img->h);
        release_screen();
	}
}

void inicializacion_allegro(){
	/*
	Inicializacion de la libreria Alegro
	*/

}

int main(int argc, char **argv){

	ListaEjercicios ejercicios;
	FONT *f80,*f150;
	BITMAP  //*img_carga,
            *img_fondo,
            *img_ejercicio,
            *img_ejercicios,
            *img_resultado,
            *img_espresultado;

    PALETTE pal;

	if(allegro_init()!=0){
		//No se puede inicializar la libreria
		exit(1);
	}
	if (	install_timer() != 0 ||
			install_keyboard() !=0 ){
		//No se pueden inicializar correctamente los subsistemas
		exit(1);
	};

	set_color_depth(32);

	if (set_gfx_mode(GFX_AUTODETECT_FULLSCREEN,640,480,0,0)!=0){
		//No se puede implementar el modo grafico
		exit(1);
	}

	//img_carga = load_tga("img_carga.tga",pal);

	//mostrar_imagen(img_carga);

	img_fondo = load_tga("img_fondo.tga",pal);
	img_ejercicio = load_tga("img_ejercicio.tga",pal);
	img_ejercicios = load_tga("img_ejercicios.tga",pal);
	img_resultado = load_tga("img_resultado.tga",pal);
	img_espresultado = load_tga("img_espresultado.tga",pal);


    //printf("Leyendo ejercicios...\n");

	if (argc>1){
		leer_ejercicios(argv[1],&ejercicios);
	} else {
		exit(1);
	}

	f80= load_font("t80_fnt.pcx", NULL, NULL);
	f150= load_font("t1502_fnt.pcx", NULL, NULL);

	mostrar_imagen(img_fondo);

	clear_keybuf();

    textprintf_centre_ex(screen, font, 320, 400, makecol(0,0,0),-1,"Entre Semilla");

    srand(readkey());
	while ( ( readkey() >> 8 ) != KEY_ESC){
		int eje=0;
		
		mostrar_imagen(img_ejercicio);
		//clear_bitmap(screen);
		//Seleccionar ejercicio

		do{
			eje = rand() %  ejercicios.n;
		} while (ejercicios.ejercicios[eje].mostrable==0);
		ejercicios.ejercicios[eje].mostrable=0;

		textprintf_centre_ex(screen, f80, 500, 20, makecol(0,0,0),-1,"Ejercicio:");
		textprintf_centre_ex(screen, f150, 450, 200, makecol(0,0,0),-1,"%i", eje+1);

		//Esperar tecla (se puede elegir una tecla especifica para no mostrar
		// ejercicio en particular)

		if (( readkey() >> 8 ) != KEY_ESC )	{
		// Iniciar el ciclo donde se muestran los diferentes segmentos del
		// ejercicio
			int i;
			char soper[7][3]={"+\0","-\0","*\0","/\0","r\0","p\0","=\0"};
			char ejercicio[200]="\0";

			for (i=0;i<ejercicios.ejercicios[eje].nsegmentos;i++){
				mostrar_imagen(img_ejercicios);
				if (i==0){
				    sprintf(ejercicio,"%s %i",ejercicio,ejercicios.ejercicios[eje].valores[i]);
					textprintf_centre_ex(screen, f150, 440, 240, makecol(0,0,255),-1,"%i",ejercicios.ejercicios[eje].valores[i]);
				} else {
                    //Otra operacion
					sprintf(ejercicio,"%s %s %i",ejercicio,soper[ejercicios.ejercicios[eje].operaciones[i-1]],ejercicios.ejercicios[eje].valores[i]);
   					textprintf_centre_ex(screen, f150, 440, 240, makecol(0,0,255),-1,"%s %i",soper[ejercicios.ejercicios[eje].operaciones[i-1]],ejercicios.ejercicios[eje].valores[i]);
                }
                textprintf_centre_ex(screen, font, 320, 20, makecol(0,0,255),-1,"%s",ejercicio);
                readkey();
			}
			mostrar_imagen(img_espresultado);
			textprintf_centre_ex(screen, font, 320, 20, makecol(0,255,0),-1,"%s =",ejercicio);
			//textprintf_centre_ex(screen, f150, 300, 220, makecol(255,0,0),-1,"=");
		//Esperar tecla donde se va a mostrar el resultado (se puede definir
		// una tecla para no mostrar el resultado)
		    readkey();
		//Mostrar resultado
			mostrar_imagen(img_resultado);
			textprintf_centre_ex(screen, font, 320, 20, makecol(0,0,0),makecol(255,255,255),"%s = %i",ejercicio,ejercicios.ejercicios[eje].resultado);
			textprintf_centre_ex(screen, f150, 200, 200, makecol(128,64,128),-1,"%i",ejercicios.ejercicios[eje].resultado);
		    readkey();
			mostrar_imagen(img_fondo);
		}
		//Si todos los ejercicios fueron mostrados, volver a comenzar

		//Limpiar pantalla y dejar el fondo predeterminado
	};

    //destroy_bitmap(img_carga);
    destroy_bitmap(img_fondo);
    destroy_bitmap(img_ejercicio);
    destroy_bitmap(img_ejercicios);
    destroy_bitmap(img_resultado);
    destroy_bitmap(img_espresultado);
	//Guardar el listado de ejercicios


	return 0;
}
END_OF_MAIN()
