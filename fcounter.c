#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define limit 100

//variables globales
char types[limit][4];
int sum[limit];
int n_type;
int total;

//funciones

char *getExt(char *arg){
	char *aux = strpbrk(arg,".");
	if(aux != NULL){						
		aux = strrchr(arg,'.') + 1; 
	}
	return aux;
}

int iguales(char *arg1, char *arg2){
	int i, len;
	
	if( strlen(arg1)  != strlen(arg2) )
		return 0;

	len = strlen(arg1);
	for(i = 0 ; i < len ; i++)
		if(arg1[i] != arg2[i])
			return 0;
	
	return 1;
}

void count_files(char *dir_currently){
	DIR *dir;
	struct dirent *data;
	struct stat info;
	char *extension;
	char *aux;
	int i, flag;
	flag = 1;
	int roots = 0;

	dir = opendir(dir_currently);
	if (dir != NULL)
		while( (data = readdir(dir)) != NULL ){
			if ( stat(data->d_name, &info) != -1)
				switch(info.st_mode & S_IFMT){
				//directorio
				case S_IFDIR:
					//comprobar que no se trate de . ni ..
					if(roots > 1){
						strcpy(aux,dir_currently);
						strcat(aux,"/");
						strcat(aux,data->d_name);	
						//recorrer el subdirectorio
						count_files(aux);
					}
					roots++;
					break;
				//archivo
				case S_IFREG: 
					total++;
					//obtener la extension
					extension = getExt(data->d_name);
					//buscar la extension dentro de las ya existentes
					//si no tiene extension el archivo (extension = NULL), lo suma al total, pero no lo registra
					if(extension != NULL){											
						if(n_type != 0){
						//1.- buscar la extension y sumar
							flag = 1;								
							for(i = 0 ; i < n_type ; i++){
								if( iguales(extension,types[i]) ){
									sum[i]++;
									flag = 0;
									break;
								}
							}	
						}
						//registrar si no existe
						 if(flag){
							strcpy(types[n_type],extension);
							sum[n_type] = 1;
							n_type++;
						}
					}
					break;
				default: break;
				}
		}
	closedir(dir);
}

int main(int argc, char *argv[]){
	//declaracion de variables
	char *dir_currently;	
	int cont;
	
	//obtenemos el directorio actual
	dir_currently = getcwd(NULL,0);

	//inicializacion de variables
	total = 0;
	n_type = 0;
					
	//recorrer directorio actual
	count_files(dir_currently);
	
	//mostrar resultados
	printf("Extension         Cantidad\n");
	for(cont = 0 ; cont < n_type ; cont++)
		switch( strlen(types[cont]) ){		
			case 1 :  printf("%s                   %d\n",types[cont],sum[cont]); break;
			case 2 :  printf("%s                  %d\n",types[cont],sum[cont]); break;
			default : printf("%s                 %d\n",types[cont],sum[cont]); break;
			}
	printf("Total : %d\n",total);
	return 1;
}
