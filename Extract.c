#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <math.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include<sys/stat.h>


void mkdirs(const char *muldir) 
{
    int i,len;
    char str[512];    
    strncpy(str, muldir, 512);
    len=strlen(str);
    for( i=0; i<len; i++ )
    {
        if( str[i]=='/' )
        {
            str[i] = '\0';
            if( access(str,0)!=0 )
            {
                mkdir(str, 0775 );
            }
            str[i]='/';
        }
    }
    if( len>0 && access(str,0)!=0 )
    {
        mkdir(str, 0775 );
    }
    return;
}

int file_exist(const char *path)
{
    DIR *dp;
    if ((dp = opendir(path)) == NULL)
    {
        mkdirs(path);
        printf("path = %s\n", path);
        return 0;
    }
    else
    {   
        closedir(dp);
        printf("Directory already exists! Please specify a new path.");
//        exit(0);
        return -1;
    }
}

int Find_Max_index (double *p, int samples) // function for pccs-based strategies to find the index of row or column has maximum 1s
{
	int i;
	int Maxid=0;
	for(i=0;i<samples;i++)
	{
		if(p[Maxid]<p[i])Maxid=i;
	}
	return Maxid;
}

int Find_Min_index (double *p, int samples) // function for pccs-based strategies to find the index of row or column has maximum 1s
{
	int i;
	int Maxid=0;
	for(i=0;i<samples;i++)
	{
		if(p[Maxid]>p[i])Maxid=i;
	}
	return Maxid;
}

int FindMaxindex (int *p, int samples) // function for pccs-based strategies to find the index of row or column has maximum 1s
{
	int i;
	int Maxid=0;
	for(i=0;i<samples;i++)
	{
		if(p[Maxid]<p[i])Maxid=i;
	}
	return Maxid;
}

int FindMinindex (int *p, int samples) // function for pccs-based strategies to find the index of row or column has maximum 1s
{
	int i;
	int Maxid=0;
	for(i=0;i<samples;i++)
	{
		if(p[Maxid]>p[i])Maxid=i;
	}
	return Maxid;
}

double Max (double *p, int samples)
{
	int i;
	int Max=0;
	for(i=0;i<samples;i++)
	{
		if(p[Max]<p[i])Max=p[i];
	}
	return Max;
}

char flip (char p)
{
	char a;
	a=p;
	if(a==48)p=49;
	if(a==49)p=48;
	
	return p;
}

char ** origins (char **p, int samples, int sites)
{
	return p;
}

char ** Row (char **p, int samples, int sites) // function of rowed frequency-based sorting 
{
	int m,n;
	int *count = (int*)malloc(sizeof(int)*samples);
	
	for(m=0;m<samples;m++)
	{
		count[m] = 0;
		for(n=0;n<sites;n++)
		{
			if(p[m][n] == 49)count[m]++;
		}
	}
	
	int temple;
	char *temp;
	
	for(m=0;m<samples;m++)
	{
		for(n=m+1;n<samples;n++)
		{
			if(count[m] < count[n])
			{
				temp = p[m];
				p[m] = p[n];
				p[n] = temp;
	
				temple = count[m];
				count[m] = count[n];
				count[n] = temple;
			}
		}
	}
	
	for(m=0;m<samples;m++)
	{
//		printf("%d\n", count[m]);
	}
	
	free(count);
	
	return p;
}

char ** Column (char **p, int samples, int sites) // function of columned frequency-based sorting
{
	int k,m,n;
	int *count = (int*)malloc(sizeof(int)*sites);
		
	for(m=0;m<sites;m++)
	{
		count[m] = 0;
		for(n=0;n<samples;n++)
		{
			if(p[n][m] == 49)count[m]++;
		}
	}
	

		
	int temple;
	char temp;
		
	for(m=0;m<sites;m++)
	{
		for(n=m+1;n<sites;n++)
		{
			if(count[m] < count[n])
			{
				for(k=0;k<samples;k++)
				{
					temp = p[k][m];
					p[k][m] = p[k][n];
					p[k][n] = temp;
				}
					
				temple = count[m];
				count[m] = count[n];
				count[n] = temple;
			}
		}
	}
	
	for(k=0;k<sites;k++)
	{
//		printf("%d\n", count[k]);
	}
	
	free(count);
	
	return p;
}

char ** Pccrow (char **p, int samples, int sites) // function of rowed pccs-based sorting
{
	int *P_A = (int*)calloc(samples, sizeof(int));
	int *P_a = (int*)calloc(samples, sizeof(int));
	int i,k,m,n,j;
		
	// create the matrices to store PCC between two rows and the sum of PCC of each row
	double **PCC_matrix = (double**)malloc(sizeof(double*)*samples);
	double *sum = (double*)calloc(samples, sizeof(double));

		
	for(i=0;i<samples;i++)
	{
		PCC_matrix[i] = (double*)malloc(sizeof(double)*(samples));
	}
	
	char **p_c = (char**)malloc(sizeof(char*)*samples);
	for(i=0;i<samples;i++)
	{
		p_c[i] = (char*)malloc(sizeof(char)*sites);
	}
	int *fixed=(int*)calloc(samples, sizeof(int));
	int a, b;
	int *c= (int*)calloc(samples, sizeof(int));
	double temple1;
	int tem;
	char *temp1;
	
	for(i=0;i<samples;i++)
	{
		for(j=0;j<sites;j++)
		{
			if(p[i][j]==48)p_c[i][j]=48;
			if(p[i][j]==49)p_c[i][j]=49;				
		}	
	}
	
	n=0;

	for(i=0;i<samples;i++)
	{
		fixed[i]=samples+1;
	}
	
	for(i=0;i<samples;i++)
	{
		m=0;
		k=0;
		for(j=0;j<sites;j++)
		{
			if(p_c[i][j]==49)m+=1;
			if(p_c[i][j]==48)k+=1;
		}
		if(m == sites || k == sites)
		{
			n+=1;
		}
	}
	
	if(n==0)
	{
		for(i=0;i<samples;i++)
		{
			for(k=0;k<sites;k++)
			{
				if(p[i][k] == 49)P_A[i]++;
			}
			P_a[i] = sites - P_A[i];
	//		printf("%d %d\n", P_A[i], P_a[i]);
		}
		for(i=0;i<samples;i++)
		{
			PCC_matrix[i][i] = 1.000001;
			for(k=i+1;k<samples;k++)
			{
				int P_AB=0;
				for(m=0;m<sites;m++)
				{
					if(p[i][m] == 49 && p[k][m] == 49)P_AB++;
				}
				PCC_matrix[i][k] = (double)((double)P_AB/(double)sites-(double)P_A[i]/(double)sites*(double)P_A[k]/(double)sites)*((double)P_AB/(double)sites-(double)P_A[i]/(double)sites*(double)P_A[k]/(double)sites)/(double)(P_A[i]/(double)sites*(double)(1-(double)P_A[i]/(double)sites)*(double)P_A[k]/(double)sites*(double)(1-(double)P_A[k]/(double)sites));
				if(P_A[i] == 0 || P_A[k] == 0)PCC_matrix[i][k]=0;
				//PCC_matrix[k][i] = PCC_matrix[i][k];
				PCC_matrix[k][i] = PCC_matrix[i][k];
	//			printf("%d %d %d %d %d %d ", P_AB, sites, P_A[i], P_A[k], P_a[i], P_a[k]);
	//			printf("%f\n", PCC_matrix[i][k]);
			}
		}
			
		for(i=0;i<samples;i++)
		{
			for(k=0;k<samples;k++)
			{
				sum[i]+=PCC_matrix[i][k];
				printf("%lf ", PCC_matrix[i][k]);
			}
			printf("%lf\n", sum[i]);
		}
			
		int Maxid=0;
			
		// find the max index and sort with row
		Maxid = Find_Max_index(sum, samples);
	//	printf("%d %lf\n", Maxid, sum[Maxid]);
			
		double temple1;
		char *temp1;
	//	for(i=0;i<samples;i++)
	//	{
	//		printf("p:%f\n", PCC_matrix[i]);
	//		printf("*p:%s\n", &PCC_matrix[i]);
	//	}	
		for(m=0;m<samples;m++)
		{
			for(n=m+1;n<samples;n++)
			{
				if(PCC_matrix[Maxid][m] < PCC_matrix[Maxid][n])
				{
					temp1 = p[m];
					p[m] = p[n];
					p[n] = temp1;
						
					temple1 = PCC_matrix[Maxid][m];
					PCC_matrix[Maxid][m] = PCC_matrix[Maxid][n];
					PCC_matrix[Maxid][n] = temple1;
				}
			}
		}
	}
	
	
	
	
	
	
	
	
	
	
	
	
	else
	{
		for(i=0;i<samples;i++)
		{
			for(k=0;k<sites;k++)
			{
				if(p[i][k] == 49)P_A[i]++;
			}
			P_a[i] = sites - P_A[i];
	//		printf("%d %d\n", P_A[i], P_a[i]);
		}
		
		for(i=0;i<samples;i++)
		{
			PCC_matrix[i][i] = 1.00001;
			for(k=i+1;k<samples;k++)
			{
				int P_AB=0;
				for(m=0;m<sites;m++)
				{
					if(p[i][m] == 49 && p[k][m] == 49)P_AB++;
				}
				PCC_matrix[i][k] = (double)((double)P_AB/(double)sites-(double)P_A[i]/(double)sites*(double)P_A[k]/(double)sites)*((double)P_AB/(double)sites-(double)P_A[i]/(double)sites*(double)P_A[k]/(double)sites)/(double)(P_A[i]/(double)sites*(double)(1-(double)P_A[i]/(double)sites)*(double)P_A[k]/(double)sites*(double)(1-(double)P_A[k]/(double)sites));
				if(P_A[i] == 0 || P_A[k] == 0)PCC_matrix[i][k]=0;
				//PCC_matrix[k][i] = PCC_matrix[i][k];
				PCC_matrix[k][i] = PCC_matrix[i][k];
	//			printf("%d %d %d %d %d %d ", P_AB, sites, P_A[i], P_A[k], P_a[i], P_a[k]);
	//			printf("%f\n", PCC_matrix[i][k]);
			}
		}
		
		n=0;
		for(i=0;i<samples;i++)
		{
			m=0;
			k=0;
			for(j=0;j<sites;j++)
			{
				if(p_c[i][j]==49)m+=1;
				if(p_c[i][j]==48)k+=1;
			}
			if(m == sites || k == sites)
			{
				for(j=0;j<sites;j++)
				{
					p[n][j]=p_c[i][j];
				}
				fixed[n]=i;
				n+=1;
				for(k=0;k<samples;k++)
				{
					PCC_matrix[k][i]=0;
				}
			}
		}
		
		printf("%d %d\n", n, m);
		
		for(i=0;i<samples;i++)
		{
	//		printf("%d \n", fixed[i]);
		}
		
		for(i=0;i<samples;i++)
		{
			for(k=0;k<samples;k++)
			{
				for(m=0;m<n;m++)
				{
					if(i==fixed[m])sum[i]=2*samples;
					else sum[i]+=PCC_matrix[i][k];
				}			
	//			printf("%lf ", PCC_matrix[i][k]);
			}
	//		printf("%lf\n", sum[i]);
		}
		
		
			
		
		
		for(a=0;a<samples;a++)
		{
			c[a]=a;
		}
		
		for(a=0;a<samples;a++)
		{
			for(b=a+1;b<samples;b++)
			{
				if(sum[a] < sum[b])
				{
					tem = c[a];
					c[a] = c[b];
					c[b] = tem;
					
					temple1 = sum[a];
					sum[a] = sum[b];
					sum[b] = temple1;
				}
			}
		}
		
		for(a=0;a<samples;a++)
		{
			printf("%f\n", sum[a]);
		}
		
		
	//	printf("%d %lf\n", Maxid, sum[Maxid]);
			
		
	//	for(i=0;i<samples;i++)
	//	{
	//		printf("p:%f\n", PCC_matrix[i]);
	//		printf("*p:%s\n", &PCC_matrix[i]);
	//	}
		k=n;
		a=c[n];
		printf("%d\n", a);
		for(m=0;m<samples;m++)
		{
	//		printf("%f\n", PCC_matrix[a][m]);
		}
		for(m=0;m<samples;m++)
		{
			for(n=m+1;n<samples;n++)
			{
				if(PCC_matrix[a][m] < PCC_matrix[a][n])
				{
					temp1 = p_c[m];
					p_c[m] = p_c[n];
					p_c[n] = temp1;
						
					temple1 = PCC_matrix[a][m];
					PCC_matrix[a][m] = PCC_matrix[a][n];
					PCC_matrix[a][n] = temple1;
				}
			}
		}
		
		for(i=0;i<samples-k;i++)
		{
			for(j=0;j<sites;j++)
			{
				if(p_c[i][j]==48)p[i+k][j]=48;
				if(p_c[i][j]==49)p[i+k][j]=49;
			}
		}
	}
	
	
	free(P_A);
	free(P_a);
	free(sum);
	for(i=0;i<samples;i++)
	{
		free(PCC_matrix[i]);
		free(p_c[i]);
	}
	free(p_c);
	free(c);
	free(fixed);
	free(PCC_matrix);
	
	return p;
}

char ** Pccrow1 (char **p, int samples, int sites) // function of rowed pccs-based sorting
{
	int *P_A = (int*)calloc(samples, sizeof(int));
	int *P_a = (int*)calloc(samples, sizeof(int));
	int i,k,m,n,j;
	
	
		
	for(i=0;i<samples;i++)
	{
		for(k=0;k<sites;k++)
		{
			if(p[i][k] == 49)P_A[i]++;
		}
		P_a[i] = sites - P_A[i];
//		printf("%d %d\n", P_A[i], P_a[i]);
	}
		
	// create the matrices to store PCC between two rows and the sum of PCC of each row
	double **PCC_matrix = (double**)malloc(sizeof(double*)*samples);
	double *sum = (double*)calloc(samples, sizeof(double));

		
	for(i=0;i<samples;i++)
	{
		PCC_matrix[i] = (double*)malloc(sizeof(double)*(samples));
	}
		
	for(i=0;i<samples;i++)
	{
		PCC_matrix[i][i] = 1.000001;
		for(k=i+1;k<samples;k++)
		{
			int P_AB=0;
			for(m=0;m<sites;m++)
			{
				if(p[i][m] == 49 && p[k][m] == 49)P_AB++;
			}
			PCC_matrix[i][k] = (double)((double)P_AB/(double)sites-(double)P_A[i]/(double)sites*(double)P_A[k]/(double)sites)*((double)P_AB/(double)sites-(double)P_A[i]/(double)sites*(double)P_A[k]/(double)sites)/(double)(P_A[i]/(double)sites*(double)(1-(double)P_A[i]/(double)sites)*(double)P_A[k]/(double)sites*(double)(1-(double)P_A[k]/(double)sites));
			if(P_A[i] == 0 || P_A[k] == 0)PCC_matrix[i][k]=0;
			//PCC_matrix[k][i] = PCC_matrix[i][k];
			PCC_matrix[k][i] = PCC_matrix[i][k];
//			printf("%d %d %d %d %d %d ", P_AB, sites, P_A[i], P_A[k], P_a[i], P_a[k]);
//			printf("%f\n", PCC_matrix[i][k]);
		}
	}
		
	for(i=0;i<samples;i++)
	{
		for(k=0;k<samples;k++)
		{
			sum[i]+=PCC_matrix[i][k];
			printf("%lf ", PCC_matrix[i][k]);
		}
		printf("%lf\n", sum[i]);
	}
		
	int Maxid=0;
		
	// find the max index and sort with row
	Maxid = Find_Max_index(sum, samples);
//	printf("%d %lf\n", Maxid, sum[Maxid]);
		
	double temple1;
	char *temp1;
//	for(i=0;i<samples;i++)
//	{
//		printf("p:%f\n", PCC_matrix[i]);
//		printf("*p:%s\n", &PCC_matrix[i]);
//	}	
	for(m=0;m<samples;m++)
	{
		for(n=m+1;n<samples;n++)
		{
			if(PCC_matrix[Maxid][m] < PCC_matrix[Maxid][n])
			{
				temp1 = p[m];
				p[m] = p[n];
				p[n] = temp1;
					
				temple1 = PCC_matrix[Maxid][m];
				PCC_matrix[Maxid][m] = PCC_matrix[Maxid][n];
				PCC_matrix[Maxid][n] = temple1;
			}
		}
	}
	
	free(P_A);
	free(P_a);
	free(sum);
	for(i=0;i<samples;i++)
	{
		free(PCC_matrix[i]);
	}
	free(PCC_matrix);
	
	return p;
}

char ** Pcccolumn (char **p, int samples, int sites) // function of columned pccs-based sorting
{
	int *P_A = (int*)calloc(sites, sizeof(int));
	int *P_a = (int*)calloc(sites, sizeof(int));
	int i,k,m,n;
		
	for(i=0;i<sites;i++)
	{
		for(k=0;k<samples;k++)
		{
			if(p[k][i] == 49)P_A[i]++;
		}
		P_a[i] = samples - P_A[i];
//		printf("%d %d\n", P_A[i], P_a[i]);
	}
		
	double **PCC_matrix = (double**)malloc(sizeof(double*)*sites);
	double *sum = (double*)calloc(sites, sizeof(double));

		
	for(i=0;i<sites;i++)
	{
		PCC_matrix[i] = (double*)malloc(sizeof(double)*(sites));
	}
		
	for(i=0;i<sites;i++)
	{
		PCC_matrix[i][i] = 1.000001;
		for(k=i+1;k<sites;k++)
		{
			int P_AB=0;
			for(m=0;m<samples;m++)
			{
				if(p[m][i] == 49 && p[m][k] == 49)P_AB++;
			}
			PCC_matrix[i][k] = (double)((double)P_AB/(double)samples-(double)P_A[i]/(double)samples*(double)P_A[k]/(double)samples)*((double)P_AB/(double)samples-(double)P_A[i]/(double)samples*(double)P_A[k]/(double)samples)/(double)(P_A[i]/(double)samples*(double)(1-(double)P_A[i]/(double)samples)*(double)P_A[k]/(double)samples*(double)(1-(double)P_A[k]/(double)samples));
			if(P_A[i] == 0 || P_A[k] == 0)PCC_matrix[i][k]=0;
			PCC_matrix[k][i] = PCC_matrix[i][k];
//			printf("%d %d %d %d %d %d ", P_AB, samples, P_A[i], P_A[k], P_a[i], P_a[k]);
//			printf("%f\n", PCC_matrix[i][k]);
		}
	}
		
	for(i=0;i<sites;i++)
	{
		for(k=0;k<sites;k++)
		{
			sum[i]+=PCC_matrix[i][k];
			printf("%lf ", PCC_matrix[i][k]);
		}
		printf("%lf\n", sum[i]);
	}
		
	int Maxid=0;

	Maxid = Find_Max_index(sum, sites);
//	printf("%d %lf\n", Maxid, sum[Maxid]);
		
	double temple;
	char temp;
		
	for(m=0;m<sites;m++)
	{
		for(n=m+1;n<sites;n++)
		{
			if(PCC_matrix[Maxid][m] < PCC_matrix[Maxid][n])
			{
				for(k=0;k<samples;k++)
				{
					temp = p[k][m];
					p[k][m] = p[k][n];
					p[k][n] = temp;
				}
					
				temple = PCC_matrix[Maxid][m];
				PCC_matrix[Maxid][m] = PCC_matrix[Maxid][n];
				PCC_matrix[Maxid][n] = temple;
			}
		}
	}
	
	free(P_A);
	free(P_a);
	free(sum);
	for(i=0;i<sites;i++)
	{
		free(PCC_matrix[i]);
	}
	free(PCC_matrix);
	
	return p;
}

char ** Pcccenter (char **p, int samples, int sites)
{
	int center = (int)(round(samples/2));
	int i,j;
//	center = (int)(sample/2);
	int **p_c = (int**)malloc(sizeof(int*)*samples);
	for(i=0;i<samples;i++)
	{
		p_c[i] = (int*)malloc(sizeof(int)*sites);
	}
	
//	p_c[center]=p[0];
	for(j=0;j<sites;j++)
	{
		if(p[0][j]==48)p_c[center][j]=0;
		if(p[0][j]==49)p_c[center][j]=1;
	}
//	printf("p:0x%lx\n", (unsigned long)p_c);
//	printf("%d\n", center);
	for(i=1;i<samples;i++)
	{
		for(j=0;j<sites;j++)
		{
			if(i%2 == 1)
			{
				if(p[i][j]==48)p_c[center-(int)(i/2)-1][j]=0;
				if(p[i][j]==49)p_c[center-(int)(i/2)-1][j]=1;
//				p_c[center-(int)(i/2)-1]=p[i];
//				printf("%d ", center-(int)(i/2)-1);
			}
	//		if(i%2 == 0)p_c[center+(int)(i/2)]=p[i];
			if(i%2 == 0)
			{
				if(p[i][j]==48)p_c[center+(int)(i/2)][j]=0;
				if(p[i][j]==49)p_c[center+(int)(i/2)][j]=1;
//				p_c[center+(int)(i/2)]=p[i];
//				printf("%d ", center+(int)(i/2));
			}
//			printf("%d a\n", i);
		}
	}
	for(i=0;i<samples;i++)
	{
		for(j=0;j<sites;j++)
		{
			if(p_c[i][j]==0)p[i][j]=48;
			if(p_c[i][j]==1)p[i][j]=49;
		}
	}
//	p=p_c;
//	printf("p:0x%lx\n", (unsigned long)p);
	for(i=0;i<samples;i++)
	{
//		&p[i] = &p_c[i];
//		printf("p:%s\n", p[i]);
//		printf("*p:%d\n", *p[i]);
	}
//	printf("\n");
	for(i=0;i<samples;i++)
	{
		free(p_c[i]);
//		printf("p:%s\n", p_c[i]);
//		printf("*p:%d\n", *p_c[i]);
	}
	free(p_c);
	
	return p;
}

char ** Hamrow (char **p, int samples, int sites) // function of rowed pccs-based sorting
{

	int i,k,m,n;
		
		
	// create the matrices to store PCC between two rows and the sum of PCC of each row
	int **PCC_matrix = (int**)malloc(sizeof(int*)*samples);
	double *sum = (double*)calloc(samples, sizeof(double));

		
	for(i=0;i<samples;i++)
	{
		PCC_matrix[i] = (int*)malloc(sizeof(int)*(samples));
	}
		
	for(i=0;i<samples;i++)
	{
		PCC_matrix[i][i] = 0;
		for(k=i+1;k<samples;k++)
		{
			int P_AB=0;
			for(m=0;m<sites;m++)
			{
				if(p[i][m] != p[k][m])P_AB++;
//				if(p[i][m] == 49 && p[k][m] == 48)P_AB++;
				
			}
			PCC_matrix[i][k] = P_AB;
			PCC_matrix[k][i] = PCC_matrix[i][k];
//			printf("%f\n", PCC_matrix[i][k]);
		}
	}
		
	for(i=0;i<samples;i++)
	{
		for(k=0;k<samples;k++)
		{
			sum[i]+=PCC_matrix[i][k];
//			printf("%d ", PCC_matrix[i][k]);
		}
//		printf("%lf\n", sum[i]);
	}
		
	int Maxid=0;
		
	// find the max index and sort with row
	Maxid = Find_Min_index(sum, samples);
//	printf("%d %lf\n", Maxid, sum[Maxid]);
		
	int temple1;
	char *temp1;
//	for(i=0;i<samples;i++)
//	{
//		printf("p:%f\n", PCC_matrix[i]);
//		printf("*p:%s\n", &PCC_matrix[i]);
//	}	
	for(m=0;m<samples;m++)
	{
		for(n=m+1;n<samples;n++)
		{
			if(PCC_matrix[Maxid][m] > PCC_matrix[Maxid][n])
			{
				temp1 = p[m];
				p[m] = p[n];
				p[n] = temp1;
					
				temple1 = PCC_matrix[Maxid][m];
				PCC_matrix[Maxid][m] = PCC_matrix[Maxid][n];
				PCC_matrix[Maxid][n] = temple1;
			}
		}
	}
	
	
	free(sum);
	for(i=0;i<samples;i++)
	{
		free(PCC_matrix[i]);
	}
	free(PCC_matrix);
	
	return p;
}

char ** Hamcolumn (char **p, int samples, int sites) // function of rowed pccs-based sorting
{

	int i,k,m,n;
		
		
	// create the matrices to store PCC between two rows and the sum of PCC of each row
	int **PCC_matrix = (int**)malloc(sizeof(int*)*sites);
	double *sum = (double*)calloc(sites, sizeof(double));

		
	for(i=0;i<sites;i++)
	{
		PCC_matrix[i] = (int*)malloc(sizeof(int)*(sites));
	}
		
	for(i=0;i<sites;i++)
	{
		PCC_matrix[i][i] = 0;
		for(k=i+1;k<sites;k++)
		{
			int P_AB=0;
			for(m=0;m<samples;m++)
			{
				if(p[m][i] != p[m][k])P_AB++;
//				if(p[i][m] == 49 && p[k][m] == 48)P_AB++;
				
			}
			PCC_matrix[i][k] = P_AB;
			PCC_matrix[k][i] = PCC_matrix[i][k];
//			printf("%f\n", PCC_matrix[i][k]);
		}
	}
		
	for(i=0;i<sites;i++)
	{
		for(k=0;k<sites;k++)
		{
			sum[i]+=PCC_matrix[i][k];
//			printf("%d ", PCC_matrix[i][k]);
		}
//		printf("%lf\n", sum[i]);
	}
		
	int Maxid=0;
		
	// find the max index and sort with row
	Maxid = Find_Min_index(sum, sites);
	printf("%d %lf\n", Maxid, sum[Maxid]);
		
	int temple1;
	char temp1;
//	for(i=0;i<samples;i++)
//	{
//		printf("p:%f\n", PCC_matrix[i]);
//		printf("*p:%s\n", &PCC_matrix[i]);
//	}	
	for(m=0;m<sites;m++)
	{
		for(n=m+1;n<sites;n++)
		{
			if(PCC_matrix[Maxid][m] > PCC_matrix[Maxid][n])
			{
				for(k=0;k<samples;k++)
				{
					temp1 = p[k][m];
					p[k][m] = p[k][n];
					p[k][n] = temp1;
				}
					
				temple1 = PCC_matrix[Maxid][m];
				PCC_matrix[Maxid][m] = PCC_matrix[Maxid][n];
				PCC_matrix[Maxid][n] = temple1;
			}
		}
	}
	
	
	free(sum);
	for(i=0;i<sites;i++)
	{
		free(PCC_matrix[i]);
	}
	free(PCC_matrix);
	
	return p;
}

char ** Minorcolumn (char **p, int samples, int sites) // function of columned frequency-based sorting
{
	int k,m,n;
	int *count = (int*)malloc(sizeof(int)*sites);
		
	for(m=0;m<sites;m++)
	{
		count[m] = 0;
		for(n=0;n<samples;n++)
		{
			if(p[n][m] == 49)count[m]++;
		}
		if(count[m]>samples/2)count[m]=samples-count[m];
	}
	

		
	int temple;
	char temp;
		
	for(m=0;m<sites;m++)
	{
		for(n=m+1;n<sites;n++)
		{
			if(count[m] > count[n])
			{
				for(k=0;k<samples;k++)
				{
					temp = p[k][m];
					p[k][m] = p[k][n];
					p[k][n] = temp;
				}
					
				temple = count[m];
				count[m] = count[n];
				count[n] = temple;
			}
		}
	}
	
	for(k=0;k<sites;k++)
	{
//		printf("%d\n", count[k]);
	}
	
	free(count);
	
	return p;
}

char ** Minorcolumnflip (char **p, int samples, int sites) // function of columned frequency-based sorting
{
	int k,m,n,lab;
	int *count = (int*)malloc(sizeof(int)*sites);
	int *lable = (int*)malloc(sizeof(int)*sites);
		
	for(m=0;m<sites;m++)
	{
		count[m] = 0;
		for(n=0;n<samples;n++)
		{
			if(p[n][m] == 49)count[m]++;
		}
		lab=count[m];
		if(lab>samples/2)
		{
			count[m]=samples-count[m];
			lable[m]=0;
		}
		if(lab<=samples/2)
		{
			lable[m]=1;
		}
//		printf("%d\n", lable[m]);
	}
	

		
	int temple, temple1;
	char temp;
		
	for(m=0;m<sites;m++)
	{
		for(n=m+1;n<sites;n++)
		{
			if(count[m] > count[n])
			{
				for(k=0;k<samples;k++)
				{
					temp = p[k][m];
					p[k][m] = p[k][n];
					p[k][n] = temp;
				}
					
				temple = count[m];
				count[m] = count[n];
				count[n] = temple;
				
				temple1 = lable[m];
				lable[m] = lable[n];
				lable[n] = temple1;				
			}
		}
//		printf("%d\n", lable[m]);
	}
	
	
	for(k=1;k<sites;k++)
	{
		if(lable[k]!=lable[0])
		{
			for(m=0;m<samples;m++)
			{
				p[m][k]=flip(p[m][k]);
			}
		}
	}
	
	free(count);
	free(lable);
	
	return p;
}

char ** Minorcolumnflipbase (char **p, int samples, int sites) // function of columned frequency-based sorting
{
	int k,m,n,lab;
	int *count = (int*)malloc(sizeof(int)*sites);
	int *lable = (int*)malloc(sizeof(int)*sites);
		
	for(m=0;m<sites;m++)
	{
		count[m] = 0;
		for(n=0;n<samples;n++)
		{
			if(p[n][m] == 49)count[m]++;
		}
		lab=count[m];
		if(lab>samples/2)
		{
			count[m]=samples-count[m];
			lable[m]=0;
		}
		if(lab<=samples/2)
		{
			lable[m]=1;
		}
//		printf("%d\n", lable[m]);
	}
	

		
	int temple, temple1;
	char temp;
		
	for(m=0;m<sites;m++)
	{
		for(n=m+1;n<sites;n++)
		{
			if(count[m] > count[n])
			{
				for(k=0;k<samples;k++)
				{
					temp = p[k][m];
					p[k][m] = p[k][n];
					p[k][n] = temp;
				}
					
				temple = count[m];
				count[m] = count[n];
				count[n] = temple;
				
				temple1 = lable[m];
				lable[m] = lable[n];
				lable[n] = temple1;				
			}
		}
//		printf("%d\n", lable[m]);
	}
	
	
	for(k=0;k<sites;k++)
	{
		if(lable[k]!=0)
		{
			for(m=0;m<samples;m++)
			{
				p[m][k]=flip(p[m][k]);
			}
		}
	}
	
	free(count);
	free(lable);
	
	return p;
}

char ** RowDecimal (char **p, int samples, int sites)
{
	int m;
	int n=0;
	long *count = (long*)malloc(sizeof(long)*samples);
	
	for(m=0;m<samples;m++)
	{
		count[m] = 0;
		for(n=0;n<sites;n++)
		{
			if(p[m][n]=='1')
			{
				count[m]+=pow(2, sites-n-1);
			}
		}
	}
	
	int temple;
	char *temp;
	
	for(m=0;m<samples;m++)
	{
		for(n=m+1;n<samples;n++)
		{
			if(count[m] < count[n])
			{
				temp = p[m];
				p[m] = p[n];
				p[n] = temp;
	
				temple = count[m];
				count[m] = count[n];
				count[n] = temple;
			}
		}
	}
	
	for(m=0;m<samples;m++)
	{
		printf("%ld\n", count[m]);
	}
	
	free(count);
	
	return p;
}

char ** RowHamWeiDis (char **p, int samples, int sites)
{
	int i,j,k,m,n;
		
	double *weight_matrix = (double*)calloc(samples, sizeof(double));	
	// create the matrices to store PCC between two rows and the sum of PCC of each row
	double **PCC_matrix = (double**)malloc(sizeof(double*)*samples);
	for(i=0;i<samples;i++)
	{
		PCC_matrix[i] = (double*)malloc(sizeof(double)*(samples));
	}
	
	double *sum = (double*)calloc(samples, sizeof(double));
	
	int **p_c = (int**)malloc(sizeof(int*)*samples);
	for(i=0;i<samples;i++)
	{
		p_c[i] = (int*)malloc(sizeof(int)*sites);
	}
	
//	p_c[center]=p[0];
	for(i=0;i<samples;i++)
	{
		weight_matrix[i]=0;
		for(j=0;j<sites;j++)
		{
			if(p[i][j]==48)p_c[i][j]=0;
			if(p[i][j]==49)
			{
				p_c[i][j]=1;
				weight_matrix[i]+=1;
			}
				
		}
//		printf("%f\n", weight_matrix[i]);	
	}
	
		
	
		
	for(i=0;i<samples;i++)
	{
		PCC_matrix[i][i] = 0;
		for(k=i+1;k<samples;k++)
		{
			int P_AB=0;
			for(m=0;m<sites;m++)
			{
				if(p[i][m] != p[k][m])P_AB++;
//				if(p[i][m] == 49 && p[k][m] == 48)P_AB++;
				
			}
			PCC_matrix[i][k] = P_AB;
			PCC_matrix[k][i] = PCC_matrix[i][k];
//			printf("%f\n", PCC_matrix[i][k]);
		}
	}
		
	for(i=0;i<samples;i++)
	{
		for(k=0;k<samples;k++)
		{
			sum[i]+=PCC_matrix[i][k];
//			printf("%d ", PCC_matrix[i][k]);
		}
//		printf("%lf\n", sum[i]);
	}
		
	int Maxid=0;
	double max;
	int *fixed=(int*)calloc(samples, sizeof(int));
	
	for(i=0;i<samples;i++)
	{
		fixed[i]=samples+1;
	}
	
	// find the max index and sort with row
	Maxid = Find_Max_index(weight_matrix, samples);
//	printf("%d %lf\n", Maxid, sum[Maxid]);
		
	
	for(i=0;i<sites;i++)
	{
		if(p_c[Maxid][i]==0)p[0][i]=48;
		if(p_c[Maxid][i]==1)p[0][i]=49;
	}
	
	fixed[0]=Maxid;
	Maxid=Find_Min_index(PCC_matrix[Maxid], samples);
	
	for(i=1;i<samples;i++)
	{
//		max=Max(PCC_matrix[Maxid], samples);
//		printf("\n%f\n\n", max);
//		printf("%d\n", Maxid);
		for(j=0;j<samples;j++)
		{
			for(m=0;m<samples;m++)
			{
				if(j==fixed[m])PCC_matrix[Maxid][j]=samples;
			}
//			printf("%f\n", PCC_matrix[Maxid][j]);
		}
		Maxid=Find_Min_index(PCC_matrix[Maxid], samples);
		for(j=0;j<sites;j++)
		{
			if(p_c[Maxid][j]==0)p[i][j]=48;
			if(p_c[Maxid][j]==1)p[i][j]=49;
		}
		fixed[i]=Maxid;
//		printf("%d\n", fixed[i]);
	}
	
	
	
	free(sum);
	free(fixed);
	free(weight_matrix);
	for(i=0;i<samples;i++)
	{
		free(PCC_matrix[i]);
		free(p_c[i]);
	}
	free(PCC_matrix);
	free(p_c);
	
	return p;
}

char ** AdjacentCorrelation (char **p, int samples, int sites) // function of columned pccs-based sorting
{
	int *P_A = (int*)calloc(sites, sizeof(int));
	int *P_a = (int*)calloc(sites, sizeof(int));
	int i,k,m,n,j,a,b;
	double max;
	
	int *fixed = (int*)calloc(samples, sizeof(int));
		
	for(i=0;i<sites;i++)
	{
		for(k=0;k<samples;k++)
		{
			if(p[k][i] == 49)P_A[i]++;
		}
		P_a[i] = samples - P_A[i];
//		printf("%d %d\n", P_A[i], P_a[i]);
	}
		
	double **PCC_matrix = (double**)malloc(sizeof(double*)*sites);

		
	for(i=0;i<sites;i++)
	{
		PCC_matrix[i] = (double*)malloc(sizeof(double)*(sites));
	}
	
	char **p_c = (char**)malloc(sizeof(char*)*samples);
	for(i=0;i<samples;i++)
	{
		p_c[i] = (char*)malloc(sizeof(char)*sites);
	}

	for(i=0;i<samples;i++)
	{
		for(j=0;j<sites;j++)
		{
			if(p[i][j]==48)p_c[i][j]=48;
			if(p[i][j]==49)p_c[i][j]=49;			
		}	
	}
		
	for(i=0;i<sites;i++)
	{
		PCC_matrix[i][i] = 0.00001;
		for(k=i+1;k<sites;k++)
		{
			int P_AB=0;
			for(m=0;m<samples;m++)
			{
				if(p[m][i] == 49 && p[m][k] == 49)P_AB++;
			}
			PCC_matrix[i][k] = (double)((double)P_AB/(double)samples-(double)P_A[i]/(double)samples*(double)P_A[k]/(double)samples)*((double)P_AB/(double)samples-(double)P_A[i]/(double)samples*(double)P_A[k]/(double)samples)/(double)(P_A[i]/(double)samples*(double)(1-(double)P_A[i]/(double)samples)*(double)P_A[k]/(double)samples*(double)(1-(double)P_A[k]/(double)samples));
			if(P_A[i] == 0 || P_A[k] == 0)PCC_matrix[i][k]=0;
			PCC_matrix[k][i] = PCC_matrix[i][k];
//			printf("%d %d %d %d %d %d ", P_AB, samples, P_A[i], P_A[k], P_a[i], P_a[k]);
//			printf("%f\n", PCC_matrix[i][k]);
		}
	}
	
	max=1.0001;	
	for(i=0;i<sites;i++)
	{
		for(k=i+1;k<sites;k++)
		{
			if(PCC_matrix[i][k] < max)
			{
				max=PCC_matrix[i][k];
				a=i;
				b=k;
			}
		}
	}
	fixed[0]=a;
	fixed[1]=b;
	
	printf("%f %d %d\n", max, a, b);
	
	for(i=0;i<sites;i++)
	{
		PCC_matrix[i][a]=0;
		PCC_matrix[i][b]=0;
	}
	
	for(i=0;i<samples;i++)
	{
		if(p_c[i][a]==48)p[i][0]=48;
		if(p_c[i][a]==49)p[i][0]=49;
		
		if(p_c[i][b]==48)p[i][sites-1]=48;
		if(p_c[i][b]==49)p[i][sites-1]=49;
	}		

		
	for(m=1;m<sites/2;m++)
	{
		a=Find_Max_index(PCC_matrix[a], sites);
		for(i=0;i<samples;i++)
		{
			if(p_c[i][a]==48)p[i][m]=48;
			if(p_c[i][a]==49)p[i][m]=49;
		}
		for(i=0;i<sites;i++)
		{
			PCC_matrix[i][a]=0;
		}
		
		b=Find_Max_index(PCC_matrix[b], sites);
		for(i=0;i<samples;i++)
		{
			if(p_c[i][b]==48)p[i][sites-m-1]=48;
			if(p_c[i][b]==49)p[i][sites-m-1]=49;
		}
		for(i=0;i<sites;i++)
		{
			PCC_matrix[i][b]=0;
		}
		
	}
	
	free(P_A);
	free(P_a);
	free(fixed);
	for(i=0;i<sites;i++)
	{
		free(PCC_matrix[i]);
	}
	for(i=0;i<samples;i++)
	{
		free(p_c[i]);
	}
	free(p_c);
	free(PCC_matrix);
	
	return p;
}

char ** SideAdjacentCorrelation (char **p, int samples, int sites) // function of columned pccs-based sorting
{
	int *P_A = (int*)calloc(sites, sizeof(int));
	int *P_a = (int*)calloc(sites, sizeof(int));
	int i,k,m,n,j,a,b;
	double max;
	
	int *fixed = (int*)calloc(sites, sizeof(int));
		
	for(i=0;i<sites;i++)
	{
		fixed[i] = sites+1;
		for(k=0;k<samples;k++)
		{
			if(p[k][i] == 49)P_A[i]++;
		}
		P_a[i] = samples - P_A[i];
//		printf("%d %d\n", P_A[i], P_a[i]);
	}
		
	double **PCC_matrix = (double**)malloc(sizeof(double*)*sites);

		
	for(i=0;i<sites;i++)
	{
		PCC_matrix[i] = (double*)malloc(sizeof(double)*(sites));
	}
	
	char **p_c = (char**)malloc(sizeof(char*)*samples);
	for(i=0;i<samples;i++)
	{
		p_c[i] = (char*)malloc(sizeof(char)*sites);
	}

	for(i=0;i<samples;i++)
	{
		for(j=0;j<sites;j++)
		{
			if(p[i][j]==48)p_c[i][j]=48;
			if(p[i][j]==49)p_c[i][j]=49;			
		}	
	}
		
	for(i=0;i<sites;i++)
	{
		PCC_matrix[i][i] = 0.00001;
		for(k=i+1;k<sites;k++)
		{
			int P_AB=0;
			for(m=0;m<samples;m++)
			{
				if(p[m][i] == 49 && p[m][k] == 49)P_AB++;
			}
			PCC_matrix[i][k] = (double)((double)P_AB/(double)samples-(double)P_A[i]/(double)samples*(double)P_A[k]/(double)samples)*((double)P_AB/(double)samples-(double)P_A[i]/(double)samples*(double)P_A[k]/(double)samples)/(double)(P_A[i]/(double)samples*(double)(1-(double)P_A[i]/(double)samples)*(double)P_A[k]/(double)samples*(double)(1-(double)P_A[k]/(double)samples));
			if(P_A[i] == 0 || P_A[k] == 0)PCC_matrix[i][k]=0;
			PCC_matrix[k][i] = PCC_matrix[i][k];
//			printf("%d %d %d %d %d %d ", P_AB, samples, P_A[i], P_A[k], P_a[i], P_a[k]);
//			printf("%f\n", PCC_matrix[i][k]);
		}
	}
	
	max=1.0001;	
	for(i=0;i<sites/2-1;i++)
	{
		for(k=sites/2;k<sites;k++)
		{
			if(PCC_matrix[i][k] < max)
			{
				max=PCC_matrix[i][k];
				a=i;
				b=k;
			}
		}
	}
	fixed[0]=a;
	fixed[sites-1]=b;
	
	printf("%f %d %d\n", max, a, b);
	
	for(i=0;i<sites;i++)
	{
		PCC_matrix[i][a]=0;
		PCC_matrix[i][b]=0;
	}
	
	for(i=0;i<samples;i++)
	{
		if(p_c[i][a]==48)p[i][0]=48;
		if(p_c[i][a]==49)p[i][0]=49;
		
		if(p_c[i][b]==48)p[i][sites-1]=48;
		if(p_c[i][b]==49)p[i][sites-1]=49;
	}		
	
	for(i=0;i<sites/2;i++)
	{
		for(j=sites/2;j<sites;j++)
		{
			PCC_matrix[i][j]=0;
		}
	}
	
	for(i=sites/2;i<sites;i++)
	{
		for(j=0;j<sites/2;j++)
		{
			PCC_matrix[i][j]=0;
		}
	}
	
	for(i=0;i<sites;i++)
	{
		for(j=0;j<sites;j++)
		{
			printf("%f ", PCC_matrix[i][j]);
		}
		printf("\n");
	}
	
		
	for(m=1;m<sites/2;m++)
	{
		a=Find_Max_index(PCC_matrix[a], sites);
		for(k=0;k<sites;k++)
		{
			if(a==fixed[k])
			{
				a=0;
				break;
			}
		}
		for(k=0;k<sites;k++)
		{
			if(a==fixed[k])
			{
				a++;
			}
		}
		
		fixed[m]=a;
		
		
		for(i=0;i<samples;i++)
		{
			if(p_c[i][a]==48)p[i][m]=48;
			if(p_c[i][a]==49)p[i][m]=49;
		}
		for(i=0;i<sites;i++)
		{
			PCC_matrix[i][a]=0;
		}
		
		b=Find_Max_index(PCC_matrix[b], sites);
		for(k=0;k<sites;k++)
		{
			if(b==fixed[k])
			{
				b=sites/2;
				break;
			}
		}
		for(k=0;k<sites;k++)
		{
			if(b==fixed[k])
			{
				b++;
			}
		}
		
		fixed[sites-m-1]=b;
		
		
		for(i=0;i<samples;i++)
		{
			if(p_c[i][b]==48)p[i][sites-m-1]=48;
			if(p_c[i][b]==49)p[i][sites-m-1]=49;
		}
		for(i=0;i<sites;i++)
		{
			PCC_matrix[i][b]=0;
		}
		
	}
	
	for(i=0;i<sites;i++)
	{
		for(j=0;j<sites;j++)
		{
			printf("%f ", PCC_matrix[i][j]);
		}
		printf("\n");
	}
	
	free(P_A);
	free(P_a);
	for(i=0;i<sites;i++)
	{
		free(PCC_matrix[i]);
		printf("%d ", fixed[i]);
	}
	printf("\n");
	for(i=0;i<samples;i++)
	{
		free(p_c[i]);
	}
	free(fixed);
	free(p_c);
	free(PCC_matrix);
	
	return p;
}


int main (int argc, char ** argv)
{
	int opt;
	const char *optstring = "i:a:b:c:w:s:l:r:g:o:";
	int  win_snp=0, win_site=0, length=0, min_snp=1, grid_size=1, modeA=0, modeB=0;
//	char *modeA, *modeB;
	char *center;
	char *out_dir;
	char *file_name;
//	mode = "noopt";
	center = "bp";
	
	while (-1 != (opt = getopt(argc, argv, optstring))) // get opts
	{
		switch (opt) {
			case 'i': ;
				file_name = optarg;
				break;
			case 'a': 
				modeA = atoi(optarg);
				break;
			case 'b': 
				modeB = atoi(optarg);
				break;
			case 'c': ;
				center = optarg;
				break;
			case 'w':
				win_snp = atoi(optarg);
				break;
			case 's':
				win_site = atoi(optarg);
				break;
			case 'l':
				length = atoi(optarg);
				break;
			case 'r':
				min_snp = atoi(optarg);
				break;
			case 'g':
				grid_size = atoi(optarg);
				break;
			case 'o': ;
				out_dir = optarg;
				break;
			default:
				printf("No option is detected");
				break;
		}
	}
	
	FILE *fp = fopen(file_name, "r");	
	FILE *fout = NULL;
	char tstring [1000];
	char filename [200];
	char flag [100];
	int samples, populations;
	int skip;
	
	file_exist(out_dir);
	
	fscanf(fp, "%s", flag);
	if(strcmp(flag, "command:") == 0)
	{
		fscanf(fp, "%s", tstring);
		fscanf(fp, "%s", tstring);
		samples = atoi(tstring);
		printf("number of samples: %d\n", samples);
		for(skip=0;skip<10;skip++)fscanf(fp, "%s", tstring);
		populations = atoi(tstring);
		printf("number of populations: %d\n", populations);
	}
	else
	{
		fscanf(fp, "%s", tstring);			
		// get the number of samples
		samples = atoi(tstring);
		printf("number of samples: %d\n", samples);
		
		fscanf(fp, "%s", tstring);
		
		//get the number of populations
		populations = atoi(tstring);
		printf("number of populations: %d\n", populations);
	}
	
	if(win_site>length)win_site=length;
	
	int j;
	
	// scan each population
	for(j=0;j<populations;j++)
	{
		while(strcmp(tstring, "segsites:"))
			fscanf(fp, "%s", tstring);
		
		printf("New population start!\n");
		
		// get the sites information of this population		
		fscanf(fp, "%s", tstring);
		int sites = atoi(tstring);
		if(strcmp(flag, "command:") == 0)sites = sites - 1;
		printf("site of the population %d is: %d\n", j+1, sites);

		fscanf(fp, "%s", tstring);
		int i,k,index_center;
		int *positions_ori = (int*)calloc(sites, sizeof(int));
		float *position = (float*)malloc(sizeof(float)*sites);
		if(strcmp(flag, "command:") == 0)
		{	
			for(i=0;i<sites;i++)
			{
				fscanf(fp, "%s", tstring);
				positions_ori[i] = atoi(tstring);
//				printf("%d ", positions[i]);
			}
		}
		else
		{	
			for(i=0;i<sites;i++)
			{
				fscanf(fp, "%s", tstring);
				position[i] = atof(tstring);
//				printf("%f ", position[i]);
			}
//			int *positions = (int*)malloc(sizeof(int)*(sites+1));
			for(i=0;i<sites;i++)
			{
				positions_ori[i] = position[i] * length;
//				printf("%d ", positions[i]);
			}
		}
		printf("\n");
		
		// create the matrix to store the data and write it to the txt file
		char **matrix_ori = (char**)malloc(sizeof(char*)*samples);
		for(i=0;i<samples;i++)
		{
			matrix_ori[i] = (char*)malloc(sizeof(char)*(sites+1));
			fscanf(fp, "%s", matrix_ori[i]);
		}
		
		
		int count=0, m, n=0;
		
		for(i=0;i<sites;i++)
		{
			for(m=0;m<samples;m++)
			{
				if(matrix_ori[m][i]==48)count++;
			}
			if(count==0 || count==samples)continue;
			else n++;
			
			count=0;
		}
		
		printf("%d %d\n", n, sites);
		
		char **matrix = (char**)malloc(sizeof(char*)*samples);
		for(i=0;i<samples;i++)
		{
			matrix[i] = (char*)malloc(sizeof(char)*n);
//			fscanf(fp, "%s", matrix[i]);
		}
		int *positions = (int*)calloc(n, sizeof(int));
		
		
		n=0;
		for(i=0;i<sites;i++)
		{
			for(m=0;m<samples;m++)
			{
				if(matrix_ori[m][i]==48)count++;
			}
			if(count==0 || count==samples)continue;
			else
			{
				for(m=0;m<samples;m++)
				{
					matrix[m][n]=matrix_ori[m][i];
					positions[n]=positions_ori[i];
				}
				n++;
			}
			
			count=0;
		}
		
		sites=n;
		
		printf("%d %d\n", n, sites);
		
		
		if((strcmp(center, "snp") == 0 || strcmp(center, "bp") == 0) && win_snp>sites)
		{
			free(position);
			free(positions);
			for(i=0;i<samples;i++)
			{
				free(matrix[i]);
			}
			free(matrix);
			printf("The window size is larger than total number of sites of this population, so do not count!\n");
			continue;
		}
		
		
		
//		for(i=0;i<sites-1;i++)
//		{
//			if(positions[i]<=length/2 && positions[i+1]>length/2)index_center=i;
//		}
		
		
			
		// proccess the data by specified sorting strategy
		if (win_snp==sites)
		{	
			// create the txt file
			sprintf(filename, "%s/%d.txt", out_dir, j);
			fout = fopen(filename, "w");
			
			if (modeA == 0)
			{
				matrix = origins(matrix, samples, win_snp);
			}
			
			if (modeA == 1)
			{
				matrix = Row(matrix, samples, win_snp);
			}
			
			if (modeA == 2)
			{
				matrix = RowDecimal(matrix, samples, win_snp);
			}
			
			if (modeA == 3)
			{
				matrix = Pccrow(matrix, samples, win_snp);
			}
			
			if (modeA == 4)
			{
				matrix = Hamrow(matrix, samples, win_snp);
			}
			
			if (modeA == 5)
			{
				matrix = RowHamWeiDis(matrix, samples, win_snp);
			}
			
			if (modeA == 6)
			{
				matrix = Column(matrix, samples, win_snp);
			}
			
			if (modeA == 7)
			{
				matrix = Pcccolumn(matrix, samples, win_snp);
			}
				
			if (modeA == 8)
			{
				matrix = Minorcolumn(matrix, samples, win_snp);
			}
			
			if (modeA == 9)
			{
				matrix = Minorcolumnflip(matrix, samples, win_snp);
			}
			
			if (modeA == 10)
			{
				matrix = Hamcolumn(matrix, samples, win_snp);
			}
			
			if (modeA == 11)
			{
				matrix = AdjacentCorrelation(matrix, samples, win_snp);
			}
			
			if (modeA == 12)
			{
				matrix = SideAdjacentCorrelation(matrix, samples, win_snp);
			}
			
			//
			
			if (modeB == 0)
			{
				matrix = origins(matrix, samples, win_snp);
			}
			
			if (modeB == 1)
			{
				matrix = Row(matrix, samples, win_snp);
			}
			
			if (modeB == 2)
			{
				matrix = RowDecimal(matrix, samples, win_snp);
			}
			
			if (modeB == 3)
			{
				matrix = Pccrow(matrix, samples, win_snp);
			}
			
			if (modeB == 4)
			{
				matrix = Hamrow(matrix, samples, win_snp);
			}
			
			if (modeB == 5)
			{
				matrix = RowHamWeiDis(matrix, samples, win_snp);
			}
			
			if (modeB == 6)
			{
				matrix = Column(matrix, samples, win_snp);
			}
			
			if (modeB == 7)
			{
				matrix = Pcccolumn(matrix, samples, win_snp);
			}
				
			if (modeB == 8)
			{
				matrix = Minorcolumn(matrix, samples, win_snp);
			}
			
			if (modeB == 9)
			{
				matrix = Minorcolumnflip(matrix, samples, win_snp);
			}
			
			if (modeB == 10)
			{
				matrix = Hamcolumn(matrix, samples, win_snp);
			}
			
			if (modeB == 11)
			{
				matrix = AdjacentCorrelation(matrix, samples, win_snp);
			}
			
			if (modeB == 12)
			{
				matrix = SideAdjacentCorrelation(matrix, samples, win_snp);
			}
			
			for(i=0;i<samples;i++)
			{
				printf("%s\n", matrix[i]);
				fprintf(fout, "%s\n", matrix[i]);
			}	
			free(position);
			free(positions);
			free(positions_ori);
			for(i=0;i<samples;i++)
			{
				free(matrix[i]);
				free(matrix_ori[i]);
			}
			free(matrix);
			free(matrix_ori);
			fclose(fout);
//			printf("The window size is larger than total number of sites of this population, so do not count!\n");
			continue;
			
		}
		
		// process the data by specified centering strategy
		if (strcmp(center, "snp") == 0) // snp centering strategy
		{
			int g;
			
			if (win_snp == 0)
			{
				printf("\nThe size of snps window is not specified!\n");
				free(position);
				free(positions_ori);
				free(positions);
				for(i=0;i<samples;i++)
				{
					free(matrix[i]);
					free(matrix_ori[i]);
				}
				free(matrix);
				free(matrix_ori);
				fclose(fp);
				return -1;
			}
			for(g=1;g<grid_size+1;g++)
			{
				for(i=0;i<sites-1;i++)
				{
					if(positions[i]<=(int)(length*g)/(grid_size+1) && positions[i+1]>(int)(length*g)/(grid_size+1))index_center=i;
				}
				
				printf("center is:%d\n", index_center);
//				printf("%d %d %s\n", win_snp/2, win_site, mode);
				
				char **snp_matrix = (char**)malloc(sizeof(char*)*samples);
				for(i=0;i<samples;i++)
				{
					snp_matrix[i] = (char*)calloc(win_snp+1, sizeof(char));
				}
				
				if ((index_center-win_snp/2+1)<0)index_center=win_snp/2-1;
				
				for(k=0;k<samples;k++)
				{
					for(i=0;i<win_snp;i++)snp_matrix[k][i]=matrix[k][i+index_center-win_snp/2+1];
				}
				
				sprintf(filename, "%s/%d_S%d_C%d_E%d.txt", out_dir, j, positions[index_center-win_snp/2+1], (int)(length*g)/(grid_size+1), positions[index_center+win_snp/2+1]);
				fout = fopen(filename, "w");

				if (modeA == 0)
				{
					snp_matrix = origins(snp_matrix, samples, win_snp);
				}
				
				if (modeA == 1)
				{
					snp_matrix = Row(snp_matrix, samples, win_snp);
				}
				
				if (modeA == 2)
				{
					snp_matrix = RowDecimal(snp_matrix, samples, win_snp);
				}
				
				if (modeA == 3)
				{
					snp_matrix = Pccrow(snp_matrix, samples, win_snp);
				}
				
				if (modeA == 4)
				{
					snp_matrix = Hamrow(snp_matrix, samples, win_snp);
				}
				
				if (modeA == 5)
				{
					snp_matrix = RowHamWeiDis(snp_matrix, samples, win_snp);
				}
				
				if (modeA == 6)
				{
					snp_matrix = Column(snp_matrix, samples, win_snp);
				}
				
				if (modeA == 7)
				{
					snp_matrix = Pcccolumn(snp_matrix, samples, win_snp);
				}
					
				if (modeA == 8)
				{
					snp_matrix = Minorcolumn(snp_matrix, samples, win_snp);
				}
				
				if (modeA == 9)
				{
					snp_matrix = Minorcolumnflip(snp_matrix, samples, win_snp);
				}
				
				if (modeA == 10)
				{
					snp_matrix = Hamcolumn(snp_matrix, samples, win_snp);
				}
				
				if (modeA == 11)
				{
					snp_matrix = AdjacentCorrelation(snp_matrix, samples, win_snp);
				}
				
				if (modeA == 12)
				{
					snp_matrix = SideAdjacentCorrelation(snp_matrix, samples, win_snp);
				}
				
				//
				
				if (modeB == 0)
				{
					snp_matrix = origins(snp_matrix, samples, win_snp);
				}
				
				if (modeB == 1)
				{
					snp_matrix = Row(snp_matrix, samples, win_snp);
				}
				
				if (modeB == 2)
				{
					snp_matrix = RowDecimal(snp_matrix, samples, win_snp);
				}
				
				if (modeB == 3)
				{
					snp_matrix = Pccrow(snp_matrix, samples, win_snp);
				}
				
				if (modeB == 4)
				{
					snp_matrix = Hamrow(snp_matrix, samples, win_snp);
				}
				
				if (modeB == 5)
				{
					snp_matrix = RowHamWeiDis(snp_matrix, samples, win_snp);
				}
				
				if (modeB == 6)
				{
					snp_matrix = Column(snp_matrix, samples, win_snp);
				}
				
				if (modeB == 7)
				{
					snp_matrix = Pcccolumn(snp_matrix, samples, win_snp);
				}
					
				if (modeB == 8)
				{
					snp_matrix = Minorcolumn(snp_matrix, samples, win_snp);
				}
				
				if (modeB == 9)
				{
					snp_matrix = Minorcolumnflip(snp_matrix, samples, win_snp);
				}
				
				if (modeB == 10)
				{
					snp_matrix = Hamcolumn(snp_matrix, samples, win_snp);
				}
				
				if (modeB == 11)
				{
					snp_matrix = AdjacentCorrelation(snp_matrix, samples, win_snp);
				}
				
				if (modeB == 12)
				{
					snp_matrix = SideAdjacentCorrelation(snp_matrix, samples, win_snp);
				}
				
				for(i=0;i<samples;i++)
				{
					printf("%s\n", snp_matrix[i]);
					fprintf(fout, "%s\n", snp_matrix[i]);
				}
				for(i=0;i<samples;i++)
				{
					free(snp_matrix[i]);
				}
				free(snp_matrix);
				fclose(fout);
			}
		}
			
		if (strcmp(center, "bp") == 0) // bp-based window strategy
		{
			
			if (win_snp == 0)
			{
				printf("\nThe size of snps window is not specified!\n");
				free(position);
				free(positions_ori);
				free(positions);
				for(i=0;i<samples;i++)
				{
					free(matrix[i]);
					free(matrix_ori[i]);
				}
				free(matrix);
				free(matrix_ori);
				fclose(fp);
				return -1;
			}
//			printf("center is:%d\n", index_center);
//			printf("%d %d %s\n", win_snp/2, win_site, mode);
			
			if (win_site == 0 || length ==0)
			{
				printf("\nThe size of bp window and rigion length both need to be specified!\n");
				free(position);
				free(positions_ori);
				free(positions);
				for(i=0;i<samples;i++)
				{
					free(matrix[i]);
					free(matrix_ori[i]);
				}
				free(matrix);
				free(matrix_ori);
				fclose(fp);
				return -1;
			}
			
			
			if (min_snp > win_snp/2)
			{
				printf("\nThe minimum number of snps is unworkable!\n");
				free(position);
				free(positions_ori);
				free(positions);
				for(i=0;i<samples;i++)
				{
					free(matrix[i]);
					free(matrix_ori[i]);
				}
				free(matrix);
				free(matrix_ori);
				fclose(fp);
				return -1;
			}
			int g;
			for(g=1;g<grid_size+1;g++)
			{
				char **snp_matrix = (char**)malloc(sizeof(char*)*samples);
				for(i=0;i<samples;i++)
				{
					snp_matrix[i] = (char*)calloc(win_snp+1, sizeof(char));
				}
				for(i=0;i<sites-1;i++)
				{
					if(positions[i]<=(int)(length*g)/(grid_size+1) && positions[i+1]>(int)(length*g)/(grid_size+1))index_center=i;
				}
				
				printf("center is:%d\n", index_center);
//				printf("%d %d %s\n", win_snp/2, win_site, mode);
				
				int width, left_boundry, right_boundry;
				int left_snp=0, right_snp=0, left_index=0, right_index=0, total;
				width = win_site/2;
				left_boundry = (int)(length*g)/(grid_size+1) - width;
				right_boundry = (int)(length*g)/(grid_size+1) + width;
				printf("%d %d\n", left_boundry, right_boundry);
				for(i=0;i<sites;i++)
				{
					if (positions[i]>=left_boundry && positions[i]<=(int)(length*g)/(grid_size+1))left_snp++;
					if (positions[i]>(int)(length*g)/(grid_size+1) && positions[i]<=right_boundry)right_snp++;
	//				if (positions[i]<left_boundry && positions[i+1]>=left_boundry)left_index=i+1; // check
	//				if (positions[i]<=right_boundry && positions[i+1]>right_boundry)right_index=i; // check
				}
				for(i=0;i<sites-1;i++)
				{
					if (positions[i]<=(int)(length*g)/(grid_size+1) && positions[i+1]>(int)(length*g)/(grid_size+1))
					{
						left_index = i;
						right_index = i+1;
					}
				}
				printf("%d %d %d %d\n", left_index, right_index, left_snp, right_snp);
				total = left_snp + right_snp;
				if (left_snp<min_snp || right_snp<min_snp)total=0;
				
				while (total != win_snp)
				{
					if (total>win_snp)
					{
						left_snp = 0;
						right_snp = 0;
						left_boundry = left_boundry + 0.01 * length;
						right_boundry = right_boundry - 0.01 * length;
						if (left_boundry<0)left_boundry = positions[0];
						if (right_boundry>length)right_boundry = positions[sites-1];
						for(i=0;i<sites;i++)
						{
							if (positions[i]>=left_boundry && positions[i]<=(int)(length*g)/(grid_size+1))left_snp++;
							if (positions[i]>(int)(length*g)/(grid_size+1) && positions[i]<=right_boundry)right_snp++;
	//						if (positions[i]<left_boundry && positions[i+1]>=left_boundry)left_index=i+1;
	//						if (positions[i]<=right_boundry && positions[i+1]>right_boundry)right_index=i;
						}
						total = left_snp + right_snp;
						if (total=win_snp)
						{
							for(i=0;i<sites;i++)
							{
								if (positions[i]<=left_boundry && positions[i+1]>left_boundry)left_index=i+1;
								if (positions[i]<=right_boundry && positions[i+1]>right_boundry)right_index=i;
							}
							if ((left_index+win_snp)>=(sites-1))left_index=sites-1-win_snp;
							if ((right_index-win_snp)<=0)left_index=0;
						}
						if (left_snp<min_snp || right_snp<min_snp)total=0;
						printf("%d %d %d %d %d %d %d %d\n", left_boundry, right_boundry, left_snp, right_snp, left_index, right_index, total, win_snp);
					}
					
					if (total<win_snp)
					{
						if (total==0)
						{
							left_index = index_center - min_snp + 1;
							right_index = index_center + min_snp;
							if (left_index<=0)
							{
								left_index=0;
							}
							if (right_index>=sites-1)
							{
								left_index=sites-1-win_snp;
							}
							total = 2 * min_snp;
							left_snp = min_snp;
							right_snp = min_snp;
							printf("%d %d %d\n", left_index, right_index, total);
						}
						while (total != win_snp)
						{
							if ((int)(length*g)/(grid_size+1)-positions[left_index-1]>=positions[right_index+1]-(int)(length*g)/(grid_size+1))
							{
								right_index=right_index+1;
	//							printf("%d\n", right_index);
							}					
							else 
							{
								left_index=left_index-1;
	//							printf("%d\n", left_index);
							}
							total = right_index - left_index + 1;
							left_snp = index_center - left_index + 1;
							right_snp = right_index - index_center;
							if (left_index<=0)total=win_snp;
							if (right_index>=sites-1)
							{
								total=win_snp;
								left_index=sites-1-win_snp;
	//							printf("\n%d\n\n", left_index);
							}
							
						}
					}
				}
				if (left_index<0)left_index=0;
				if ((left_index+win_snp+1)>sites)left_index=0;
				
				printf("%d %d %d %d %d %d %d %d\n", left_boundry, right_boundry, left_snp, right_snp, left_index, right_index, total, win_snp);

				for(k=0;k<samples;k++)
				{
					for(i=0;i<win_snp;i++)snp_matrix[k][i]=matrix[k][i+left_index];
				}
				
				sprintf(filename, "%s/%d_S%d_C%d_E%d.txt", out_dir, j, positions[left_index], (int)(length*g)/(grid_size+1), positions[left_index+win_snp]);
				fout = fopen(filename, "w");
				
				if (modeA == 0)  //Original-no-sorting
				{
					snp_matrix = origins(snp_matrix, samples, win_snp);
				}
				
				if (modeA == 1)  //Row-decreasing-hamming-weight
				{
					snp_matrix = Row(snp_matrix, samples, win_snp);
				}
				
				if (modeA == 2)  //Row-decreasing-decimal-value
				{
					snp_matrix = RowDecimal(snp_matrix, samples, win_snp);
				}
				
				if (modeA == 3)  //Row-decreasing-correlation
				{
					snp_matrix = Pccrow(snp_matrix, samples, win_snp);
				}
				
				if (modeA == 4)  //Row-increasing-hamming-distance
				{
					snp_matrix = Hamrow(snp_matrix, samples, win_snp);
				}
				
				if (modeA == 5)  //Row-hamming-weight-increasing-hamming-distance
				{
					snp_matrix = RowHamWeiDis(snp_matrix, samples, win_snp);
				}
				
				if (modeA == 6)  //Column-decreasing-derived-allele-frequency
				{
					snp_matrix = Column(snp_matrix, samples, win_snp);
				}
				
				if (modeA == 7)  //Column-decreasing-correlation
				{
					snp_matrix = Pcccolumn(snp_matrix, samples, win_snp);
				}
					
				if (modeA == 8)  //Column-decreasing-minor-allele-frequency
				{
					snp_matrix = Minorcolumn(snp_matrix, samples, win_snp);
				}
				
				if (modeA == 9)  //Column-decreasing-minor-allele-frequency-with-bit-flipping
				{
					snp_matrix = Minorcolumnflip(snp_matrix, samples, win_snp);
				}
				
				if (modeA == 10)  //Column-decreasing-hamming-distance
				{
					snp_matrix = Hamcolumn(snp_matrix, samples, win_snp);
				}
				
				if (modeA == 11)  //Column-adjacent-max-correlation
				{
					snp_matrix = AdjacentCorrelation(snp_matrix, samples, win_snp);
				}
				
				if (modeA == 12)  //Column-adjacent-max-correlation-on-different-sides
				{
					snp_matrix = SideAdjacentCorrelation(snp_matrix, samples, win_snp);
				}
				
				
				
				//
				
				if (modeB == 0)
				{
					snp_matrix = origins(snp_matrix, samples, win_snp);
				}
				
				if (modeB == 1)
				{
					snp_matrix = Row(snp_matrix, samples, win_snp);
				}
				
				if (modeB == 2)
				{
					snp_matrix = RowDecimal(snp_matrix, samples, win_snp);
				}
				
				if (modeB == 3)
				{
					snp_matrix = Pccrow(snp_matrix, samples, win_snp);
				}
				
				if (modeB == 4)
				{
					snp_matrix = Hamrow(snp_matrix, samples, win_snp);
				}
				
				if (modeB == 5)
				{
					snp_matrix = RowHamWeiDis(snp_matrix, samples, win_snp);
				}
				
				if (modeB == 6)
				{
					snp_matrix = Column(snp_matrix, samples, win_snp);
				}
				
				if (modeB == 7)
				{
					snp_matrix = Pcccolumn(snp_matrix, samples, win_snp);
				}
					
				if (modeB == 8)
				{
					snp_matrix = Minorcolumn(snp_matrix, samples, win_snp);
				}
				
				if (modeB == 9)
				{
					snp_matrix = Minorcolumnflip(snp_matrix, samples, win_snp);
				}
				
				if (modeB == 10)
				{
					snp_matrix = Hamcolumn(snp_matrix, samples, win_snp);
				}
				
				if (modeB == 11)
				{
					snp_matrix = AdjacentCorrelation(snp_matrix, samples, win_snp);
				}
				
				if (modeB == 12)
				{
					snp_matrix = SideAdjacentCorrelation(snp_matrix, samples, win_snp);
				}
				
				for(i=0;i<samples;i++)
				{
					printf("%s\n", snp_matrix[i]);
					//printf("%s\n", result[i]);
					fprintf(fout, "%s\n", snp_matrix[i]);
				}
				for(i=0;i<samples;i++)
				{
					free(snp_matrix[i]);
				}
				free(snp_matrix);
				fclose(fout);
			}
		}
		
		
		
		free(position);
		free(positions_ori);
		free(positions);
		for(i=0;i<samples;i++)
		{
			free(matrix[i]);
			free(matrix_ori[i]);
		}
		free(matrix);
		free(matrix_ori);
		printf("Population ends!\n\n");
	
	}
	fclose(fp);
	

	return 0;
}
