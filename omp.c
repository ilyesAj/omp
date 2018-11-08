#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int N ; // nombres de tableaux
int K ; // tailles des tableaux

float** bin; // tableau qui stocke tout les bin
void init_bin(); // genere les tableaux
void generator(float bin[]);
void affiche(float bin[]);
void tri_merge(float bin1[],float bin2[]);
int calcul_gap(int a);
int partition(float tableau[], int deb, int fin);
void tri_rapide_bis(float tableau[],int debut,int fin);
void tri_rapide(float* tableau,int longueur);
void echanger(float tab[], int i, int j);
float maximum(float a,float b);
float minimum(float a , float b);
void tri_parallel (float** bloc);


int main(int argc, char const *argv[])
{
	clock_t begin, end;
	double time_spent;
	omp_set_num_threads(5);
	srand((unsigned int)time(NULL));
	N=atoi(argv[1]);
	K=atoi(argv[2]);
	printf("Base de données d'entiers de %d x %d éléments\n",N,K );
    printf("init\n");
	init_bin();
	printf("Generation du tableau de bloc \n");
	for (int i = 0; i < N; i++)
		generator(bin[i]);
	
	for (int i = 0; i < N; i++){
		printf("affichage bin [%d] ",i );
		affiche(bin[i]);
	}
	begin = clock();
	tri_parallel(bin);
	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("///////////////////////////////////////////////////////\n");
	printf("Le temps d'execution de affiche est %f\n",time_spent );
	printf("///////////////////////////////////////////////////////\n");

	printf("Affichage aprés le tri parallél\n");
	for (int i = 0; i < N; ++i){
	printf("affichage bin [%d] ",i );
	affiche(bin[i]);
	}

	/*
	    srand((unsigned int)time(NULL));

	printf("init\n");
	init_bin();
	printf("Generation du tableau de bloc triés \n");
	for (int i = 0; i < N; ++i){
		generator(bin[i]);
		tri_rapide(bin[i],K);
	}
	for (int i = 0; i < N; ++i){
		printf("affichage bin [%d] ",i );
		affiche(bin[i]);
	}
	printf("Affichage aprés tri_merge\n");
	tri_merge(bin[0],bin[1]);
		for (int i = 0; i < N; ++i){
		printf("affichage bin [%d] ",i );
		affiche(bin[i]);
	}
	*/
	return 0;
}

void init_bin(){
	bin=(float**)malloc(N*sizeof(float*));
	for (int i = 0; i < N; i++)
		bin[i]=(float*)malloc(K*sizeof(float));
}

void generator(float tab[]){
   float coef = 15.0; //coeffecient pour agrandir les floats
    for (int i = 0; i < K; ++i)
        tab[i]=((float)rand()/(float)(RAND_MAX))*coef ;
}
void affiche(float tab[]){
    for (int i = 0; i < K; ++i)
    	printf("| %f | ",tab[i] );
    printf("\n");
}
void tri_merge(float bin1[],float bin2[]){
	// la fonction ne donne pas necessairement en sortie deux sous tableaux triés !
	//pour l'instant je modifie directement bin1 et bin 2
	//float* bout1=(float*)malloc(K*sizeof(float));
	//float* bout2=(float*)malloc(K*sizeof(float));
	int gap=0; // le saut 
	int j;
	float tmp;
	//gap=calcul_gap(K);
	for (gap = calcul_gap(2*K); gap >0 ; gap=calcul_gap(gap))
	{

		//j=gap;
		// debug printf("gap=%d\n",gap );
		for (int i = K-gap; i < K; i++)
		{
			j=i+gap;
			//debug printf("i=%d et j=%d \n",i,j-K );
			if (i>=K && j < K)
			{
				//debug printf("i>K && j < K\n");
				if (bin2[i-K]>bin1[j])
				{
					// debug printf("changer [%d]=%f et [%d]=%f\n",i-K,bin2[i-K],j,bin1[j]);
					tmp=bin2[i-K];
					bin2[i-K]=bin1[j];
					bin1[j]=tmp;
				}
			}
			if (i<K && j >= K)
			{
				// debug printf("i<K && j > K\n");

				if (bin1[i]>bin2[j-K])
				{
					// debug printf("changer [%d]=%f et [%d]=%f\n",i,bin1[i],j-K,bin2[j-K]);
					tmp=bin1[i];
					bin1[i]=bin2[j-K];
					bin2[j-K]=tmp;
				}
			}
			
			if (j>(K*2)){
				//debug printf("breaking\n");
				break;
			}

		}
		/*debug
					printf("fin gap=%d\n",gap );
			affiche(bin1);
			printf("bin 2///\n");
			affiche(bin2);
			*/
	}
}
int calcul_gap(int a){
	int resultat;
	if (a==2)
		return 1;
	else if (a==1)
	{
		return 0;
	}
	else{
		resultat=(int)a/2;
		if (a%2==1)
			resultat++;
		return resultat;
	}
}

/*********** pour le besoin de la fonction tri merge j ai besoin de deux tableaux triés en entrée 
du coup j ai pris un truc provisoire d'internet */
int partition(float tableau[], int deb, int fin)
    {
    int compt=deb;
    float pivot=tableau[deb];
    int i;

    for(i=deb+1;i<=fin;i++)
        {
        if(tableau[i]<pivot)
            {
            compt++;
            echanger(tableau,compt,i);
            }
        }
    echanger(tableau,compt,deb);
    return(compt);
    }

void tri_rapide_bis(float tableau[],int debut,int fin)
    {
    if(debut<fin)
        {
        int pivot=partition(tableau,debut,fin);
        tri_rapide_bis(tableau,debut,pivot-1);
        tri_rapide_bis(tableau,pivot+1,fin);
        }
    }

void tri_rapide(float tableau[],int longueur)
     {
     tri_rapide_bis(tableau,0,longueur-1);
     }
void echanger(float tab[], int i, int j)
     {
     float memoire;
     memoire=tab[i];
     tab[i]=tab[j];
     tab[j]=memoire;
     }
float minimum(float a , float b)
{
	if (a>b)
		return b ;
	else
		return a ;
}
float maximum(float a , float b)
{
	if (a>b)
		return a ;
	else
		return b ;
}
void tri_parallel(float** bloc)
{
	int i,j,b1,b2,k ; 
	int min , max ;
	#pragma omp parallel for
	for (i=0;i<N;i++)
		tri_rapide(bloc[i],K);

	for (j=0;j<N;j++)
	{	k=1+(j%2) ; // Traitement des blocs deux à deux 
	#pragma omp parallel for
		for (i=0;i<N/2-1;i++)
		{
			b1=1+(k+2*i)%N;
			b2=1+(K+2*i+1)%N;
			min=minimum(b1,b2);
			max=maximum(b1,b2) ; 
			tri_merge(bin[min],bin[max]);
		}
	}


	/*printf("Le tri en paralléle \n");
	for (int i = 1; i <= N; ++i){
		generator(bin[i]);
		tri_rapide(bin[i],K);
	}
	for (int i = 0; i < N; ++i){
		printf("affichage bin [%d] ",i );
		affiche(bin[i]);
	}
		tri_merge(bin[0],bin[1]);
		for (int i = 0; i < N; ++i){
		printf("affichage bin [%d] ",i );
		affiche(bin[i]);
	}*/		

}