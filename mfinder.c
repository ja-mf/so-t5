#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<dirent.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/types.h>

int mfinder(int uid,int priv,DIR * directorio);

int main (int argc, char **argv)
{
	//argc -> 2 , argv -> dueño y privilegios
	DIR * directorio;
	int priv;	
	int uid;

	//UID de usuario efectivo en caso de no ingresar dueño.
	if (argv[1] == NULL) uid = geteuid();
	else uid = atoi(argv[1]);

	//Privilegios asociados por umask en caso de no ingresarlos.
	if (argv[2] == NULL) priv = umask(022); 
	else priv = strtol(argv[2],NULL,8);
	
	//printf("arg1 = %d, arg2 = %i, arg3 = %o\n",argc,uid,priv);

	directorio = opendir(".");
	mfinder(uid,priv,directorio);
	return 0;
}



int mfinder(int uid, int priv,DIR * directorio)
{
	//DIR * directorio;
	struct dirent * entry;
	struct stat info;

	//se abre directorio actual
	//directorio = opendir(".");
	
	if(directorio == NULL)
	{
		printf("No se puede acceder a directorio");
		return (0);
	}
	//se itera en directorio
	while ( (entry = readdir(directorio)) != NULL)
	{		printf("dir : %s\n",entry->d_name);
	
		if ( (strcmp(entry->d_name,".") ) != 0 && (strcmp(entry->d_name,"..") ) != 0 )

			//se verfica estado del archivo distinto a "." y a ".."
			if ( stat(entry->d_name , &info) == -1 )
			{	
				perror("stat");
				exit(EXIT_FAILURE);
			}

			else 
			{
				//printf("name: %s\t st_uid : %d\n",entry->d_name,info.st_uid);
				//muestra archivo en el cual uid y los permisos sean los correspondientes
				if (uid == info.st_uid && priv == (info.st_mode & 000777))
				{
					printf("st_mode:\t%o\n",info.st_mode);
					printf("archivo:\t%s\n",entry->d_name);
					if(info.st_mode & S_IRUSR) printf("\tpropietario, permiso r\n");
					if(info.st_mode & S_IWUSR) printf("\tpropietario, permiso w\n");
					if(info.st_mode & S_IXUSR) printf("\tpropietario, permiso x\n");
					if(info.st_mode & S_IRGRP) printf("\tgrupo, permiso r\n");
					if(info.st_mode & S_IWGRP) printf("\tgrupo, permiso w\n");
					if(info.st_mode & S_IXGRP) printf("\tgrupo, permiso x\n");
					if(info.st_mode & S_IROTH) printf("\totro, permiso r\n");
					if(info.st_mode & S_IWOTH) printf("\totro, permiso w\n");
					if(info.st_mode & S_IXOTH) printf("\totro, permiso x\n");
					if(S_ISDIR(info.st_mode))
					{
						printf( "\tes un directorio\n");
						directorio = opendir(entry->d_name);
						mfinder(uid,priv,directorio);

					}

				}
			}
	
	}

	closedir (directorio);

	return 1;
}
