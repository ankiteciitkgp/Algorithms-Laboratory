#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void plotSimulation()
{
    int i = 0;
    int commandNum = 4;
    char * commandsForGnuplot[] =
    {
        "set title \"PLOT\"",
        "set style line 1 lc rgb '#0060ad' lt 1 lw 1 pt 7 ps 1.5",
        "set style line 2 lc rgb '#dd181f' lt 1 lw 2 pt 5 ps 1.5",
        "plot 'data.temp' using 1:2 title 'Signal1' with lines, 'data.temp' using 1:3 title 'Signal2' with lines"
    };
    FILE * gnuplotPipe = popen ("gnuplot -persistent", "w");
    for (i=0; i < commandNum; i++)
    {
        fprintf(gnuplotPipe, "%s \n", commandsForGnuplot[i]);
    }
}

int sam = 512;  //samples

typedef struct complex{
    double real;
    double imag;
}complex;

complex* newComplex(double a, double b){
    complex* c = (complex *)malloc(sizeof(complex));
    c->real = a;
    c->imag = b;
    return c;
}

complex* addComplex(complex* a,complex* b){
    return newComplex(a->real+b->real, a->imag+b->imag);
}

complex* subComplex(complex* a,complex* b){
    return newComplex(a->real- b->real, a->imag - b->imag);
}

complex* prodComplex(complex* a,complex* b){
    double real = a->real*b->real - a->imag*b->imag;
    double imag = a->real*b->imag + a->imag*b->real;
    return newComplex(real,imag);
}

complex* conjComplex(complex* a){
    return newComplex(a->real,-1*a->imag);
}

double magComplex(complex* a){
    return sqrt(a->real*a->real + a->imag*a->imag);
}

complex** FFT(complex** x,int N){
    int i;
    if(N == 1) {
        complex** temp = (complex **)malloc(sizeof(complex*));
        temp[0] = x[0];
        return temp;
    }

    if(N%2 !=0 ) {
            printf("N is not power of 2");
            return ;
    }
    complex** even = (complex **)malloc((N/2)*sizeof(complex *));
    for(i=0;i<N/2;i++){
        even[i] = (complex *)malloc(sizeof(complex));
        even[i] = x[2*i];
    }
    complex** q = FFT(even, N/2);

    complex** odd = (complex **)malloc((N/2)*sizeof(complex *));
    for(i=0;i<N/2;i++){
        odd[i] = (complex *)malloc(sizeof(complex));
        odd[i] = x[2*i +1];
    }
    complex** r = FFT(odd, N/2);

    complex** y = (complex **)malloc(N*sizeof(complex *));
    for(i = 0;i<N/2;i++){
        y[i] = (complex *)malloc(sizeof(complex));
        double kth = -2*i*(3.14)/N;
        complex* wk = newComplex(cos(kth),sin(kth));
        y[i] = addComplex(q[i],prodComplex(wk,r[i]));
        y[i + N/2] = subComplex(q[i],prodComplex(wk,r[i]));
    }
    return y;
}

complex** iFFT(complex** x,int N){
    complex** y = (complex** )malloc(N*sizeof(complex *));
    int i;
    for(i = 0;i<N;i++){
        y[i] = conjComplex(x[i]);
    }
    y = FFT(y,N);
    for(i = 0;i<N;i++){
        y[i] = conjComplex(y[i]);
    }

    for(i = 0;i<N;i++){
        y[i] = prodComplex(y[i],newComplex(1.0/N,0.0));
    }
    return y;
}

int main(){
    int f1,f2,i;
    printf("Enter frequencies F1 and F2\n");
    scanf("%d",&f1);
    scanf("%d",&f2);

    complex** x1 = (complex **)malloc(sam*sizeof(complex *));
    for(i = 0;i<sam;i++){
        x1[i] = (complex *)malloc(sizeof(complex));
    }
    complex** x2 = (complex **)malloc(sam*sizeof(complex *));
    for(i = 0;i<sam;i++){
        x2[i] = (complex *)malloc(sizeof(complex));
    }
    FILE *fu1 = fopen("u1.txt", "w+");
    fprintf(fu1,"x\ty\n");
    FILE *fu2 = fopen("u2.txt", "w+");
    fprintf(fu2,"x\ty\n");
    double k1 = 2*(3.14)*f1/sam;
    double k2 = 2*(3.14)*f2/sam;
    for(i = 0; i<sam;i++){
        x1[i]->real = sin(k1*i);
        x1[i]->imag = 0;
        fprintf(fu1,"%d\t%0.4f\n",i,x1[i]->real);
        x2[i]->real = sin(k2*i);
        x2[i]->imag = 0;
        fprintf(fu2,"%d\t%0.4f\n",i,x2[i]->real);
    }

    x1  = FFT(x1,sam);
    FILE *fftfu1 = fopen("fftu1.txt", "w+");
    fprintf(fftfu1,"x\ty\n");
    for(i = 0;i<sam;i++){
        fprintf(fftfu1,"%d\t%0.4f\n",i,magComplex(x1[i]));
    }

    x2  = FFT(x2,sam);
    FILE *fftfu2 = fopen("fftu2.txt", "w+");
    fprintf(fftfu2,"x\ty\n");
    for(i = 0;i<sam;i++){
        fprintf(fftfu2,"%d\t%0.4f\n",i,magComplex(x2[i]));
    }


    x1  = iFFT(x1,sam);
    FILE *ifftfu1 = fopen("ifftu1.txt", "w+");
    fprintf(ifftfu1,"x\ty\n");
    for(i = 0;i<sam;i++){
        fprintf(ifftfu1,"%d\t%0.4f\n",i,x1[i]->real);
    }

     x2  = iFFT(x2,sam);
    FILE *ifftfu2 = fopen("ifftu2.txt", "w+");
    fprintf(ifftfu2,"x\ty\n");
    for(i = 0;i<sam;i++){
        fprintf(ifftfu2,"%d\t%0.4f\n",i,x2[i]->real);
    }
    int w = 64;
    complex** sx1 = (complex **)malloc(w*sam*sizeof(complex *));
    for(i = 0;i<sam*w;i++){
        sx1[i] = (complex *)malloc(sizeof(complex));
        sx1[i]->imag = 0;
        sx1[i]->real = 0;
    }
    complex** sx2 = (complex **)malloc(w*sam*sizeof(complex *));
    for(i = 0;i<sam*w;i++){
        sx2[i] = (complex *)malloc(sizeof(complex));
        sx2[i]->imag = 0;
        sx2[i]->real = 0;
    }
    for(i = 0;i<sam/2;i++){
        sx1[i + 8*sam] = x1[i];
        sx1[i + 56*sam] = x1[i + sam/2];
        sx2[i + 16*sam] = x2[i];
        sx1[i + 48*sam] = x2[i + sam/2];
    }
    for(i = 0;i<sam*w;i++){
        sx1[i] = addComplex(sx1[i],sx2[i]);
    }


    FILE *sfftfu = fopen("sfftu.txt", "w+");
    fprintf(sfftfu,"x\ty\n");
    for(i = 0;i<sam*w;i++){
        fprintf(sfftfu,"%d\t%0.4f\n",i,magComplex(sx1[i]));
    }

    sx1 = iFFT(sx1,sam*w);

    FILE *sifftfu = fopen("data.temp", "w+");
    fprintf(sifftfu,"x\ty\n");
    for(i = 0;i<sam*w;i++){
        fprintf(sifftfu,"%d\t%0.4f\n",i,magComplex(sx1[i]));
    }
    plotSimulation();







    /*complex** x1 = (complex **)malloc(4*sizeof(complex *));
    for(i = 0;i<4;i++){
        x1[i] = (complex *)malloc(sizeof(complex));
    }
    x1[0]->real = -0.03480425839330703;
    x1[1]->real = 0.07910192950176387;
    x1[2]->real = 0.7233322451735928;
    x1[3]->real = 0.1659819820667019;
    x1[0]->imag = 0;
    x1[1]->imag = 0;
    x1[2]->imag = 0;
    x1[3]->imag = 0;
    x1  = FFT(x1,4);
    for(i = 0;i<4;i++){
        printf("%lf %lf\n", x1[i]->real,x1[i]->imag);
    }
    x1  = iFFT(x1,4);
    for(i = 0;i<4;i++){
        printf("%lf %lf\n", x1[i]->real,x1[i]->imag);
    }*/


    return 0;
}
