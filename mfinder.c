#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<dirent.h>
#include<sys/stat.h>
#include<unistd.h>

int main ()
{
	DIR * directorio;
	struct dirent * entry;
	struct stat info;
	int priv;		//pa probar por mientras los valores de entrada
	int uid;

	priv = 0700;
	uid = 1000;

	//se abre directorio actual
	directorio = opendir(".");

	if(directorio == NULL)
	{
		printf("No se puede acceder a directorio");
		exit(1);
	}
	
	while ( (entry = readdir(directorio)) != NULL)
	{
		if ( (strcmp(entry->d_name,".") ) != 0 && (strcmp(entry->d_name,"..") ) != 0 )
			//se verfica estado del archivo
			if ( stat(entry->d_name , &info) == -1 )
			{
				perror("stat");
				exit(EXIT_FAILURE);
			}

			else 
			{
				printf("nepe");
				//printf("name: %s\t st_uid : %d\n",entry->d_name,info.st_uid);
				//muestra archivo en el cual uid y los permisos sean los correspondientes
				if (uid == info.st_uid)
				{
					printf("st_mode: %o\n",info.st_mode);
					printf("archivo:\t%s\n",entry->d_name);
					if(info.st_mode & S_IRUSR) printf("\tpropietario tiene permiso r\n");
					if(info.st_mode & S_IWUSR) printf("\tpropietario tiene permiso w\n");
					if(info.st_mode & S_IXUSR) printf("\tpropietario tiene permiso x\n");
					if(info.st_mode & S_IROTH) printf("\totro tiene permiso r\n");
					if(info.st_mode & S_IWOTH) printf("\totro tiene permiso w\n");
					if(info.st_mode & S_IXOTH) printf("\totro tiene permiso x\n");
					int masked;
					masked = (info.st_mode & 000777);
					printf("\tcalza pollo: masked: %o, verify: %o\n", masked, (masked == priv)? 1 : 0 );
//					if( access(entry->d_name,R_OK) == 0 ) printf("\tpermisos de lectura\n");
//					if( access(entry->d_name,W_OK) == 0 ) printf("\tpermisos de escritura\n");
//					if( access(entry->d_name,X_OK) == 0 ) printf("\tpermisos de ejecucion\n");

				}
			}
	
	}

	closedir (directorio);

	return 1;
}
