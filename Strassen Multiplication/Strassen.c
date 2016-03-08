#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <time.h>
float **matrixsum(float **mat1,float **mat2,int n,int *a)
{
 int i,j,count =0;
 float **matsum= (float **)malloc(n*sizeof(float *));
 for(i=0;i<n;i++)
 {
   matsum[i] = (float *)malloc(n*sizeof(float));
 }
 for(i=0;i<n;i++)
 {
   for(j=0;j<n;j++)
   {
     matsum[i][j]=mat1[i][j]+mat2[i][j];
     count++;
   }
 }
 *a+=count;
  return matsum;
}
float **matrixsub(float **mat1,float **mat2,int n,int *a)
{
 int i,j,count = 0;
 float **matsub= (float **)malloc(n*sizeof(float *));
 for(i=0;i<n;i++)
 {
   matsub[i] = (float *)malloc(n*sizeof(float));
 }
 for(i=0;i<n;i++)
 {
   for(j=0;j<n;j++)
   {
     matsub[i][j]=mat1[i][j]-mat2[i][j];
     count++;
   }
 }
  *a += count;
 return matsub;
}

float **matmultiply(float **mat1,float **mat2,int n,int *a,int q)
 {
   int i,j,k,count=0;
   float **matprod = (float **)malloc(n*sizeof(float *));
   for(i=0;i<n;i++)
    matprod[i] = (float *)malloc(n*sizeof(float));
   if(n<=pow(2,q))
   {
     for(i=0;i<n;i++)
     {
      for(j=0;j<n;j++)
      {
       for(k=0;k<n;k++)
       {
        matprod[i][j]+=mat1[i][k]*mat2[k][j];
        count+=2;
       }
       count--;
      }
     }
   }
   else
   {
   float **p1 = (float **)malloc(n/2*sizeof(float *));
   float **p2 = (float **)malloc(n/2*sizeof(float *));
   float **p3 = (float **)malloc(n/2*sizeof(float *));
   float **p4 = (float **)malloc(n/2*sizeof(float *));
   float **p5 = (float **)malloc(n/2*sizeof(float *));
   float **p6 = (float **)malloc(n/2*sizeof(float *));
   float **p7 = (float **)malloc(n/2*sizeof(float *));
   float **c11 = (float **)malloc(n/2*sizeof(float *));
   float **c12 = (float **)malloc(n/2*sizeof(float *));
   float **c21 = (float **)malloc(n/2*sizeof(float *));
   float **c22 = (float **)malloc(n/2*sizeof(float *));
   float **a11 = (float **)malloc(n/2*sizeof(float*));
   float **a12 = (float **)malloc(n/2*sizeof(float *));
   float **a21 = (float **)malloc(n/2*sizeof(float *));
   float **a22 = (float **)malloc(n/2*sizeof(float *));
   float **b11 = (float **)malloc(n/2*sizeof(float *));
   float **b12 = (float **)malloc(n/2*sizeof(float *));
   float **b21 = (float **)malloc(n/2*sizeof(float *));
   float **b22 = (float **)malloc(n/2*sizeof(float *));
   for(i=0;i<n/2;i++)
    {
     p1[i] = (float *)malloc(n/2*sizeof(float));
     p2[i] = (float *)malloc(n/2*sizeof(float));
     p3[i] = (float *)malloc(n/2*sizeof(float));
     p4[i] = (float *)malloc(n/2*sizeof(float));
     p5[i] = (float *)malloc(n/2*sizeof(float));
     p6[i] = (float *)malloc(n/2*sizeof(float));
     p7[i] = (float *)malloc(n/2*sizeof(float));
     c11[i] = (float *)malloc(n/2*sizeof(float));
     c12[i] = (float *)malloc(n/2*sizeof(float));
     c21[i] = (float *)malloc(n/2*sizeof(float));
     c22[i] = (float *)malloc(n/2*sizeof(float));
     a11[i] = (float *)malloc(n/2*sizeof(float));
     a12[i] = (float *)malloc(n/2*sizeof(float));
     a21[i] = (float *)malloc(n/2*sizeof(float));
     a22[i] = (float *)malloc(n/2*sizeof(float));
     b11[i] = (float *)malloc(n/2*sizeof(float));
     b12[i] = (float *)malloc(n/2*sizeof(float));
     b21[i] = (float *)malloc(n/2*sizeof(float));
     b22[i] = (float *)malloc(n/2*sizeof(float));
    }
   for(i=0;i<n/2;i++)
     {
       for(j=0;j<n/2;j++)
       {
        a11[i][j] = mat1[i][j];
        b11[i][j] = mat2[i][j];
        a12[i][j] = mat1[i][j+n/2];
        b12[i][j] = mat2[i][j+n/2];
        a21[i][j] = mat1[i+n/2][j];
        b21[i][j] = mat2[i+n/2][j];
        a22[i][j] = mat1[i+n/2][j+n/2];
        b22[i][j] = mat2[i+n/2][j+n/2];
       }
     }
   p1 = matmultiply(matrixsum(a11,a22,n/2,a),matrixsum(b11,b22,n/2,a),n/2,a,q);
   p2 = matmultiply(matrixsum(a21,a22,n/2,a),b11,n/2,a,q);
   p3 = matmultiply(a11,matrixsub(b12,b22,n/2,a),n/2,a,q);
   p4 = matmultiply(a22,matrixsub(b21,b11,n/2,a),n/2,a,q);
   p5 = matmultiply(matrixsum(a11,a12,n/2,a),b22,n/2,a,q);
   p6 = matmultiply(matrixsub(a21,a11,n/2,a),matrixsum(b11,b12,n/2,a),n/2,a,q);
   p7 = matmultiply(matrixsub(a12,a22,n/2,a),matrixsum(b21,b22,n/2,a),n/2,a,q);
   c11 = matrixsum(matrixsum(p1,p7,n/2,a),matrixsub(p4,p5,n/2,a),n/2,a);
   c12 = matrixsum(p3,p5,n/2,a);
   c21 = matrixsum(p2,p4,n/2,a);
   c22 = matrixsum(matrixsum(p1,p6,n/2,a),matrixsub(p3,p2,n/2,a),n/2,a);

   for(i=0;i<n/2;i++)
   {
    for(j=0;j<n/2;j++)
    {
        matprod[i][j] = c11[i][j];
        matprod[i][j+n/2] = c12[i][j];
        matprod[i+n/2][j] = c21[i][j];
        matprod[i+n/2][j+n/2] = c22[i][j];
    }
   }
   free(p1);free(p2);free(p3);free(p4);free(p5);free(p6);free(p7);
   free(c11);free(c12);free(c21);free(c22);
   }
   *a+=count;
   return matprod;
 }

int main()
{
 int n,i,j;
 printf("Enter the value n=");
 scanf("%d",&n);
 float **mat1 = (float **)malloc(n*sizeof(float *));
 float **mat2 = (float **)malloc(n*sizeof(float *));
 float **matpro = (float **)malloc(n*sizeof(float *));
 for(i=0;i<n;i++)
 {
   mat1[i] = (float *)malloc(n*sizeof(float));
   mat2[i] = (float *)malloc(n*sizeof(float));
   matpro[i] = (float *)malloc(n*sizeof(float));
 }
 printf("Matrix 1\n");
 drand48();
 drand48();
 for(i=0;i<n;i++)
 {
   for(j=0;j<n;j++)
    {
      mat1[i][j] = drand48();
      printf("%.2f ",mat1[i][j]);
    }
   printf("\n");
 }
 printf("\n");
 printf("Matrix 2\n");
 for(i=0;i<n;i++)
 {
   for(j=0;j<n;j++)
    {
      mat2[i][j] = drand48();
      printf("%0.2f ",mat2[i][j]);
    }
   printf("\n");
 }
 printf("\n");
 int flops = 0;
 int minq,curr;
 int min = INT_MAX;
 for(i=1;i<=log2(n);i++)
 {
     flops =0;
     matpro = matmultiply(mat1,mat2,n,&flops,i);
     curr = flops;
     if(min>curr)
     {
         min = curr;
         minq = i;
     }
 }
 printf("\nBase Case(Value of q)=%d\n\n",minq);
 printf("Matrix Product\n");
 for(i=0;i<n;i++)
 {
   for(j=0;j<n;j++)
    {
      printf("%0.2f ",matpro[i][j]);
    }
    printf("\n");
 }
  printf("\n");
  free(mat1);
  free(mat2);
  free(matpro);
  return 0;
}
