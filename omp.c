#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <math.h>

# define N 1 // nombres de tableaux
# define K 20 // tailles des tableaux
float* bin[N]; // tableau qui stocke tout les bin
void init_bin(); // genere les tableaux
void generator(float bin[]);
void affiche(float bin[]);
void tri_merge(float bin1[],float bin2[]);
void tri (float b[]);
int main(int argc, char const *argv[])
{
	printf("init\n");
	init_bin();
	printf("generate\n");
	for (int i = 0; i < N; ++i)
		generator(bin[i]);
	printf("affiche\n");
	for (int i = 0; i < N; ++i){
		printf("affichage bin %d\n",i );
		affiche(bin[i]);
	}
	return 0;
}
void init_bin(){
	for (int i = 0; i < N; ++i)
		bin[i]=(float*)malloc(K*sizeof(float));
}

void generator(float tab[]){
    srand((unsigned int)time(NULL));
   float coef = 15.0; //coeffecient pour agrandir les floats
    for (int i = 0; i < K; ++i)
        tab[i]=((float)rand()/(float)(RAND_MAX))*coef ;
}
void affiche(float tab[]){
    for (int i = 0; i < K; ++i)
    	printf("bin[%d]=%f\n",i,tab[i] );
}
void tri_merge(float bin1[],float bin2[]){

	bout1=(float*)malloc(K*sizeof(float));
	bout2=(float*)malloc(K*sizeof(float));

}
void tri (float b[]){

}