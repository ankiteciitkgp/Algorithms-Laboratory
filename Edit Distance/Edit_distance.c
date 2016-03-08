#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include<Math.h>

int min(int a,int b,int c){
    if(a<b && a<c)
        return a;
    else if(b<a && b<c)
        return b;
    else
        return c;
}
void editDistace(char* a,char* b,int m,int n){
    int d[m+1][n+1];
    int i,j;
    for(i = 0;i <= m;i++){
        d[i][0] = i;
    }
    for(i = 0;i<= n;i++){
        d[0][i] = i;
    }

    for(j =1;j<=n;j++){
        for(i = 1;i<=m;i++){
            if(a[i-1] == b[j-1])
                d[i][j] = d[i-1][j-1];
            else
                d[i][j] = min(d[i-1][j],d[i][j-1],d[i-1][j-1]) + 1;
        }
    }
    printf("Edit Distance: %d\n",d[m][n]);

    i = m; j = n;
    int x = d[m][n];
    while(x > 0){
        if(a[i-1] == b[j-1]){
            i--;
            j--;
        }
        else{
            if(d[i][j] == d[i-1][j] + 1){
                printf("Deletion of %dth character which is %c\n",i,a[i-1]);
                i--;
            }
            else if(d[i][j] == d[i][j-1]+1){
               printf("Insertion of %dth character which is %c\n",i,b[j-1]);
               j--;
            }
            else if(d[i][j] == d[i-1][j-1]+1){
                printf("Substitution of %dth character which is %c with %c\n",i,a[i-1],b[j-1]);
                i--;
                j--;
            }
            x--;
        }
    }
}

void compareFiles(char** a,char** b){

}

int main(){
    int i,j;
    char d;
    char** c = (char**)malloc(10*sizeof(char *));
    for(i=0;i<10;i++){
        c[i] = (char *)malloc(100*sizeof(char));
    }
    FILE* fp = fopen("input1.txt","r");
    i = 0;
    j = 0;
    while(1){
        d = fgetc(fp);
        if(d == ' ')
            if(c[i][j-1] == ' '){
                continue;
            }
        c[i][j] = d;
        j++;
        if(d == '\n'){
            i++;
            j = 0;
        }
    printf("OK");
       if( feof(fp)){
            printf("OKbb");
            break ;
      }
    }
    printf("%s\n",c[1]);
    fclose(fp);
    char** v = (char**)malloc(10*sizeof(char *));
    for(i=0;i<10;i++){
        c[i] = (char *)malloc(100*sizeof(char));
    }
    fp = fopen("input2.txt","r");
    i = 0;
    j = 0;
    while(1){
        d = fgetc(fp);
        if(d == ' ')
            if(v[i][j-1] == ' '){
                continue;
            }
        v[i][j] = d;
        j++;
        if(d == '\n'){
            i++;
            j = 0;
        }
     printf("OK");
       if( feof(fp)){
            printf("OKbb");
            break ;
      }
    }
    printf("%s\n",v[1]);
    fclose(fp);


    return 0;
}
