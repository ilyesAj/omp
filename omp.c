#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <omp.h>       
#include <unistd.h>
#include <time.h>

int N ; // nombres de tableaux
int K ; // tailles des tableaux


float** bin; // tableau qui stocke tout les bin

void verif_tri(float** bloc);
void fusion(float tableau[],int deb1,int fin1,int fin2);
void tri_fusion_bis(float tableau[],int deb,int fin);
void tri_fusion(float tableau[],int longueur);
void create_marks_csv(char *filename,int N,int K, float time,int nb_threads );
void init_bin(); // genere les tableaux
void generator(float bin[]);
void affiche(float bin[]);
void tri_merge(float bin1[],float bin2[]);
float maximum(float a,float b);
float minimum(float a , float b);
void tri_parallel (float** bloc,int nb_threads);


int main(int argc, char const *argv[])
{
	int nb_threads;
	double t1,t2;
	double time_spent;
	

	srand((unsigned int)time(NULL));
	N=atoi(argv[1]);
	K=atoi(argv[2]);
	printf("Base de données d'entiers de %d x %d éléments\n",N,K );

	printf("Entrez le nombre de threads qui vont exécuté le tri parallél \n");
	
	scanf("\n%d",&nb_threads);

	init_bin();
	printf("Generation du tableau de bloc \n");
	for (int i = 0; i < N; i++)
		generator(bin[i]);
	/*
	for (int i = 0; i < N; i++){
		printf("affichage bin [%d] ",i );
		affiche(bin[i]);
	}
	*/
	t1=omp_get_wtime();
	printf("ICI\n");
	tri_parallel(bin,nb_threads);
	printf("ICI 2\n");
	t2=omp_get_wtime();
	time_spent = (double)(t2-t1);
	/*
	printf("Affichage aprés le tri parallél\n");
	for (int i = 0; i < N; ++i){
	printf("affichage bin [%d] ",i );
	affiche(bin[i]);
	}*/
	//verif_tri(bin);*/
	printf("///////////////////////////////////////////////////////\n");
	printf("Le temps d'execution du tri parallél est %f\n",time_spent );
	printf("///////////////////////////////////////////////////////\n");
	
	create_marks_csv("lesvaleurs.csv",N,K,time_spent,nb_threads);
	
	return 0;
}

void init_bin(){
	bin=(float**)malloc(N*sizeof(float*));
	for (int i = 0; i < N; i++)
		bin[i]=(float*)malloc(K*sizeof(float));
}

void generator(float tab[]){
   float coef = 1000; //coeffecient pour agrandir les floats
    for (int i = 0; i < K; ++i)
        tab[i]=((float)rand()/(float)(RAND_MAX))*coef ;
}
void affiche(float tab[]){
	printf("\n");
    for (int i = 0; i < K; ++i)
    	printf("| %f | \n",tab[i] );
    printf("\n");
}
int nextGap(int gap) 
{ 
    if (gap <= 1) 
        return 0; 
    return (gap / 2) + (gap % 2); 
} 

void swap(float a, float b)
{
	float memoire;
     memoire=a;
     a=b;
     b=memoire;
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

void tri_merge(float *bin1, float *bin2)
{	
	int i,j,k;
	i=0;j=0;k=0;
	float* tab =(float*)malloc(2*K*sizeof(float));
				
	for (k=0;k<2*K;k++)
	{
		if (i>=K)
		{
			tab[k]=bin2[j];
			j++;
		}
		else
		if (j>=K)
		{
			tab[k]=bin1[i];
			i++;
		}
		else
		if ((bin1[i]<bin2[j])&&(i<K))
		{
			tab[k]=bin1[i];
			i++;
		}
		else
		if ((bin2[j]<bin1[i])&&(j<K))
		{
			tab[k]=bin2[j];
			j++;	
		}
		
	}
	for (i=0;i<k;i++)
	{
		if (i<K)
			bin1[i]=tab[i];
		else
			bin2[i-K]=tab[i];
	}


}																																																											

void fusion(float tableau[],int deb1,int fin1,int fin2)
        {
        float *table1;
        int deb2=fin1+1;
        int compt1=deb1;
        int compt2=deb2;
        int i;																																																													
        
        table1=malloc((fin1-deb1+1)*sizeof(float));

        //on recopie les éléments du début du tableau
        for(i=deb1;i<=fin1;i++)
            {
            table1[i-deb1]=tableau[i];
            }
                        
        for(i=deb1;i<=fin2;i++)
            {        
            if (compt1==deb2) //c'est que tous les éléments du premier tableau ont été utilisés
                {
                break; //tous les éléments ont donc été classés
                }
            else if (compt2==(fin2+1)) //c'est que tous les éléments du second tableau ont été utilisés
                {
                tableau[i]=table1[compt1-deb1]; //on ajoute les éléments restants du premier tableau
                compt1++;
                }
            else if (table1[compt1-deb1]<tableau[compt2])
                {
                tableau[i]=table1[compt1-deb1]; //on ajoute un élément du premier tableau
                compt1++;
                }
            else
                {
                tableau[i]=tableau[compt2]; //on ajoute un élément du second tableau
                compt2++;
                }
            }
        free(table1);
        }
        

void tri_fusion_bis(float tableau[],int deb,int fin)
        {
        if (deb!=fin)
            {
            int milieu=(fin+deb)/2;
            tri_fusion_bis(tableau,deb,milieu);
            tri_fusion_bis(tableau,milieu+1,fin);
            fusion(tableau,deb,milieu,fin);
            }
        }

void tri_fusion(float tableau[],int longueur)
     {
     if (longueur>0)
            {
            tri_fusion_bis(tableau,0,longueur-1);
            }
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
void tri_parallel(float** bloc,int nb_threads)
{
	omp_set_num_threads(nb_threads);
	int i,j,b1,b2,k ; 
	int min , max ;
	#pragma omp parallel for
	for (i=0;i<N;i++)
		tri_fusion(bloc[i],K);

	for (j=0;j<=N;j++)
	{	
		k=1+(j%2) ; // Traitement des blocs deux à deux 
		#pragma omp parallel for private(b1,b2,min,max)
			for (i=0;i<=N/2+1;i++)
			{
				b1=(1+(k+(2*i)))%N;
				b2=(1+(k+(2*i)+1))%N;
				min=minimum(b1,b2);
				max=maximum(b1,b2) ; 
				#pragma omp critical
				{
				tri_merge(bloc[min],bloc[max]);
				}
			
			}
	}

}
void verif_tri(float **bloc)
{
	float * tab = (float*)malloc(K*N*sizeof(float));
	int b=1 ;//initialement vrai
	int i,j,k;
	k=0; 
	i=0 ; 
	j=0 ; 
	for (i=0;i<N;i++)
		for(j=0;j<K;j++)
			{
				tab[k]=bloc[i][j];
				k++;
			}
	printf("k= %d \n",k );			
	i=0;
	while ((b==1)&&(i<k-1))
		{
			if (tab[i]>tab[i+1])
				{
					b=0;
					printf("Probléme au niveau de la ligne %d",i);
				}
			else
				{

						i++;	
				}
		}
	if (b==1) 
		printf("Tri parallél fonctionnel à 100/100 \n ");
					
}
void create_marks_csv(char *filename,int N,int K, float time ,int nb_threads){
  
FILE *fp;

  if( access( filename, F_OK ) != -1 ) {
	
	fp=fopen(filename,"a+");
    fprintf(fp, "\n%d,%d,%f,%d,%f", N, K, time, nb_threads , time/omp_get_num_procs());

} else {
	    fp=fopen(filename,"a+");
	    fprintf(fp,"N, K, time, nb_threads , charge du programme");  
		fprintf(fp, "\n%d,%d,%f,%d,%f", N, K, time, nb_threads,time/omp_get_num_procs());

}
  
fclose(fp); 
}