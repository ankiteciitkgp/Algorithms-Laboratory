#include <stdio.h>
#include <stdlib.h>
const int inf = 100000;
int cost_insert = 0,cost_delete = 0,cost_search = 0,cost_rehash = 0,cost_prime=0,collisions=0;
char mode;
int isPrime(int num) {
 int i;
 for(i = 2;i<num ; i++) {
    if( num%i == 0) return -1;
    cost_prime++;
 }
 return 0;
}
int nextPrime(int num) {
 int prime = num;
 while(isPrime(prime)!=0) {
   prime = num++;
   cost_prime++;
 }
 return prime;
}
int hashKey( int value, int tablesize) {
 return value%tablesize;
}

typedef struct node {
 int value;
 struct node *next;
 struct node *prev;
}node;

int chainSearch(node **table,int n, int val) {
    int key = hashKey(val,n);
    cost_search++;
    if(table[key]->value ==inf){
       if(mode !='S'&&mode !='s') printf("Element not found\n");
       return -1;
    }
    node *newnode = table[key];

    while(newnode!=NULL){
        cost_search++;
        if(newnode->value == val){
            if(mode !='S'&&mode !='s') printf("Element Present\n");
            return 1;
        }
        else newnode = newnode->prev;
    }
    if(mode !='S'&&mode !='s') printf("Element not found");
    return -1;
}
void chainInsert(node **table,int n,int *size,int val) {
    if(chainSearch(table,n, val)==-1){
    node *newnode = (node *)malloc(sizeof(node));
    newnode->value = NULL;
    newnode->next = NULL;
    int key = hashKey(val,n);
    table[key]->value = val;
    size[key]++;
    table[key]->next = newnode;
    newnode->prev = table[key];
    table[key] = newnode;
    if(mode !='S'&&mode !='s') printf("Element Inserted at key:%d\n",key);
    cost_insert++;
    }
    else {
        if(mode !='S'&&mode !='s') printf("Element already Present\n");
    }

}
void chainDelete(node **table,int n,int *size, int val) {
    int key = hashKey(val,n);
    if(chainSearch(table,n,val) == -1){
       return ;
    }
    cost_delete++;
    node *newnode = (node *)malloc(sizeof(node));
    newnode = table[key]->prev;
    while(newnode->prev!=NULL){
        cost_delete++;
        if(newnode->value == val){
            (newnode->prev)->next = newnode->next;
            newnode->next->prev = newnode->prev;
            newnode = NULL;
            size[key]--;
            if(mode !='S'||mode !='s') printf("Element Deleted\n");
            return;
        }
        else newnode = newnode->prev;
    }
    if(newnode->value ==val){
    (newnode->next)->prev = NULL;
    size[key]--;
    if(mode !='S'||mode !='s') printf("Element Deleted\n");
    }
}
void chainHash(){
    printf("Enter the min size of the table:");
    int n;
    scanf("%d",&n);
    n = nextPrime(n);
    int j;
    node **table = (node **)malloc(n*sizeof(node *));
    int *size = (int *)malloc(n*sizeof(int));
    for(j =0; j<n ;j++){
        table[j] = (node *)malloc(sizeof(node));
        table[j]->value = inf;
        table[j]->prev = NULL;
        table[j]->next = NULL;
        size[j] = 0;
    }
    char in;
    int value;
    while(1){
      printf("I-Insert, S-Search, D-Delete\nEnter the name of operation to be done:");
      scanf(" %c",&in);
      printf("\nEnter Value:");
      scanf(" %d",&value);
      if(in =='I'||in =='i') chainInsert(table,n,size,value);
      else if(in == 'S'||in == 's') chainSearch(table,n,value);
      else if(in =='D' || in =='d') chainDelete(table, n,size, value);
      else {
              printf("No such operation");
              break;
       }
    }
}
void schainHash(int n,int e){

    n = nextPrime(n);
    int j;
    node **table = (node **)malloc(n*sizeof(node *));
    int *size = (int *)malloc(n*sizeof(int));
    for(j =0; j<n ;j++){
        table[j] = (node *)malloc(sizeof(node));
        table[j]->value = inf;
        table[j]->prev = NULL;
        table[j]->next = NULL;
        size[j] = 0;
    }
     for(j=0;j<e;j++){
        chainInsert(table,n,size,rand());
    }
}

int linearSearch(int *table,int n,int val) {
 cost_search++;
 int key = hashKey(val,n);
 int i = key;

 while(table[i]!=val&&table[i]!=(-inf)){
   cost_search++;
   if(i == n-1) i = 0;
   else i++;
 }
 if(table[i] == val){
        if(mode !='S'&&mode !='s') printf("Element Found\n");
        return i;
 }
 else {
     if(mode !='S'&&mode !='s') printf("Element not Found\n");
     return -1;
 }
}
void linearInsert(int *table,int n,int val) {
  int key = hashKey(val,n);
  cost_insert++;
   if(linearSearch(table,n,val)!=-1) {
        if(mode !='S'&&mode !='s') printf("Element already present.\n");
        return;
 }

 while(table[key]!=(-inf) && table[key]!=inf){
    cost_insert++;
    collisions++;
   if(key == n-1) key = 0;
   else key++;
 }
 table[key] =val;
 if(mode !='S'&&mode !='s') printf("Element Inserted at key:%d\n",key);
}
void linearDelete(int *table,int n,int val) {
 int location = linearSearch(table,n,val);
 if(location>=0){ table[location] = inf;
 if(mode !='S'&&mode !='s') printf("Element Deleted");
 }
 cost_delete++;
}
int linearSize(int *table, int n) {
 int i, count = 0;
 for(i=0; i<n ; i++){
   if(table[i] != inf && table[i] != (-inf)){
        count++;
   }
 }
 return count;
}
int *linearRehash(int *table, int *a){
  if(mode !='S'&&mode !='s') printf("\nRehashing Started\n");
  int k;
  cost_rehash++;
  int n = *a;
  int nsize = nextPrime(2*n);
  int *ntable = (int *)malloc(nsize*sizeof(int *));
  for(k =0; k<nsize ;k++){
        ntable[k] = (-inf);
    }
  cost_rehash+=nsize;
  for(k = 0; k<n ;k++) {
   int temp = table[k];
   if(temp != inf && temp != (-inf)) linearInsert(ntable, nsize, temp);
  }
  cost_rehash+=n;
  if(mode !='S'&&mode !='s') printf("Rehashing done.New table size = %d\n",nsize);
  *a = nsize;
  return ntable;
}
void linearHash(){
    int j, n = 31;
    int *table = (int *)malloc(n*sizeof(int *));
    for(j =0; j<n ;j++){
        table[j] = (-inf);
    }
    char in;
    int value;
while(1){
    printf("Please Enter the name of operation to be done: I-Insert, S-Search, D-Delete:");
    scanf(" %c",&in);
    printf("\nEnter Value:");
    scanf(" %d",&value);
    if(in =='I'||in =='i') linearInsert(table,n,value);
    else if(in == 'S'||in == 's') linearSearch(table,n,value);
    else if(in =='D' || in =='d') linearDelete(table, n, value);
    else if(in == 'L'|| in =='l') printf("\nTable Size:%d\n",linearSize(table,n));
    else {
            printf("No such operation");
            break;
    }
    if(linearSize(table,n)> 0.75*n) {
            int temp_cost = cost_insert;
            table = linearRehash(table,&n);
            cost_rehash+=(cost_insert-temp_cost);
            cost_insert = temp_cost;
    }
   }
}
void slinearHash(int n,int e){
    n = nextPrime(n);
    int j;
    int *table = (int *)malloc(n*sizeof(int *));
    for(j =0; j<n ;j++){
        table[j] = (-inf);
    }
     for(j=0;j<e;j++){
        linearInsert(table,n,rand());
        if(linearSize(table,n)> 0.75*n) {
          int temp_cost = cost_insert;
          table = linearRehash(table,&n);
          cost_rehash+=(cost_insert-temp_cost);
          cost_insert = temp_cost;
      }
  }
}

int quadraticSearch(int *table,int n,int val) {
 int key = hashKey(val,n);
 cost_search++;
 int k = key;
 int i =0;
 while(table[k]!=val&&table[k]!=(-inf)){
   i++;
   k = (key+(i*i))%n;
   cost_search++;
 }
 if(table[k] == val){
        if(mode !='S'&&mode !='s') printf("Element Found\n");
        return i;
 }
 else {
     if(mode !='S'&&mode !='s') printf("Element not Found\n");
     return -1;
 }
}
void quadraticInsert(int *table,int n,int val) {
 int key = hashKey(val,n);
 cost_insert++;
   if(quadraticSearch(table,n,val)!=-1) {
        if(mode !='S'&&mode !='s') printf("Element already present.\n");
        return;
 }
 int k = key, i=0;
 while(table[k]!=(-inf) && table[k]!=inf){
   i++;
   k = (key+i*i)%n;
   cost_insert++;
   collisions++;
 }
 table[k] = val;
 if(mode !='S'&&mode !='s') printf("Element Inserted at key:%d\n",k);
}
void quadraticDelete(int *table,int n,int val) {
 int location = quadraticSearch(table,n,val);
 cost_delete++;
 if(location>=0){ table[location] = inf;
 if(mode !='S'&&mode !='s') printf("Element Deleted");
 }
}
int quadraticSize(int *table, int n) {
 int i, count = 0;
 for(i=0; i<n ; i++){
   if(table[i] != inf && table[i] != (-inf)){
        count++;
   }
 }
 return count;
}
int *quadraticRehash(int *table, int *a){
 if(mode !='S'&&mode !='s') printf("\nRehashing Started\n");
  int k;
  cost_rehash++;
  int n = *a;
  int nsize = nextPrime(2*n);
  int *ntable = (int *)malloc(nsize*sizeof(int *));
  for(k =0; k<nsize ;k++){
        ntable[k] = (-inf);
    }
  cost_rehash+=nsize;
  for(k = 0; k<n ;k++) {
   int temp = table[k];
   if(temp != inf && temp != (-inf)) linearInsert(ntable, nsize, temp);
  }
  cost_rehash+=n;
  if(mode !='S'&&mode !='s') printf("Rehashing done.New table size = %d\n",nsize);
  *a = nsize;
  return ntable;
}
void quadraticHash(){
    int j, n = 31;
    int *table = (int *)malloc(n*sizeof(int *));
    for(j =0; j<n ;j++){
        table[j] = (-inf);
    }
    char in;
    int value;
while(1){
    printf("I-Insert, S-Search, D-Delete\nEnter the name of operation to be done:");
    scanf(" %c",&in);
    printf("\nEnter Value:");
    scanf(" %d",&value);
    if(in =='I'||in =='i') quadraticInsert(table,n,value);
    else if(in == 'S'||in == 's') quadraticSearch(table,n,value);
    else if(in =='D' || in =='d') quadraticDelete(table, n, value);
    else if(in == 'L'|| in =='l') printf("\nTable Size:%d\n",quadraticSize(table,n));
    else {
            printf("No such operation");
            break;
    }
    if(quadraticSize(table,n)> 0.75*n){ table = quadraticRehash(table,&n);
    }
  }
}
void squadraticHash(int n,int e){
    n = nextPrime(n);
    int j;
    int *table = (int *)malloc(n*sizeof(int *));
    for(j =0; j<n ;j++){
        table[j] = (-inf);
    }
     for(j=0;j<e;j++){
        quadraticInsert(table,n,rand());
        if(quadraticSize(table,n)> 0.75*n) {
          int temp_cost = cost_insert;
          table = quadraticRehash(table,&n);
          cost_rehash+=(cost_insert-temp_cost);
          cost_insert = temp_cost;
      }
  }
}

int doubleSearch(int *table,int n,int val) {
 int key = hashKey(val,n);
 int key1 = 29 - (val%29);
 int k = key, i=0;
 cost_search++;
 while(table[k]!=val&&table[k]!=(-inf)){
   i++;
   k = (key+i*key1)%n;
   cost_search++;
 }
 if(table[k] == val){
        if(mode !='S'&&mode !='s') printf("Element Found\n");
        return k;
 }
 else {
     if(mode !='S'&&mode !='s') printf("Element not Found\n");
     return -1;
 }
}
void doubleInsert(int *table,int n,int val) {
 int key = hashKey(val,n);
 int key1 = 29 - (val%29);
 cost_insert++;
   if(doubleSearch(table,n,val)!=-1) {
        if(mode !='S'&&mode !='s') printf("Element already present.\n");
        return;
 }
 int k = key, i=0;
 while(table[k]!=(-inf) && table[k]!=inf){
   i++;
   k = (key+i*key1)%n;
   cost_insert++;
   collisions++;
 }
 table[k] = val;
 if(mode !='S'&&mode !='s') printf("Element Inserted at key:%d\n",k);
}
void doubleDelete(int *table,int n,int val) {
 int location = doubleSearch(table,n,val);
 if(location>=0){ table[location] = inf;
 printf("Element Deleted");
 }
 cost_delete++;
}
int doubleSize(int *table, int n) {
 int i, count = 0;
 for(i=0; i<n ; i++){
   if(table[i] != inf && table[i] != (-inf)){
        count++;
   }
 }
 return count;
}
int *doubleRehash(int *table, int *a){
  if(mode !='S'&&mode !='s') printf("Rehashing Started");
  int k;
  cost_rehash++;
  int n = *a;
  int nsize = nextPrime(2*n);
  int *ntable = (int *)malloc(nsize*sizeof(int *));
  for(k =0; k<nsize ;k++){
        ntable[k] = (-inf);
    }
  for(k = 0; k<n ;k++) {
   int temp = table[k];
   if(temp != inf && temp != (-inf)) doubleInsert(ntable, nsize, temp);
  }
  cost_rehash+= n + nsize;
  *a = nsize;
  if(mode !='S'&&mode !='s') printf("Rehashing done. New table size = %d\n",nsize);
  return ntable;
}
void doubleHash(){
    int j, n = 31;
    int *table = (int *)malloc(n*sizeof(int *));
    for(j =0; j<n ;j++){
        table[j] = (-inf);
    }
    char in;
    int value;
while(1){
    printf("I-Insert, S-Search, D-Delete\nEnter the name of operation to be done:");
    scanf(" %c",&in);
    printf("\nEnter Value:");
    scanf(" %d",&value);
    if(in =='I'||in =='i') doubleInsert(table,n,value);
    else if(in == 'S'||in == 's') doubleSearch(table,n,value);
    else if(in =='D' || in =='d') doubleDelete(table, n, value);
    else if(in == 'L'|| in =='l') printf("\nTable Size:%d\n",doubleSize(table,n));
    else {
            printf("No such operation");
            break;
    }
    if(doubleSize(table,n)> 0.75*n) table = doubleRehash(table,&n);
 }
}
void sdoubleHash(int n,int e){
    n = nextPrime(n);
    int j;
    int *table = (int *)malloc(n*sizeof(int *));
    for(j =0; j<n ;j++){
        table[j] = (-inf);
    }
     for(j=0;j<e;j++){
        doubleInsert(table,n,rand());
        if(doubleSize(table,n)> 0.75*n) {
          int temp_cost = cost_insert;
          table = doubleRehash(table,&n);
          cost_rehash+=(cost_insert-temp_cost);
          cost_insert = temp_cost;
      }
  }
}

int main(){
 cost_insert=0;cost_delete =0;cost_search=0;cost_rehash=0;
 printf("S-Statistics mode\nU-User input mode\nEnter the mode of operation:");
 scanf("%c",&mode);
 if(mode =='S'||mode =='s'){
 printf("\nEnter the min size of the table:");
 int n,e;
 scanf("%d",&n);
 printf("\nEnter number of Elements to be inserted:");
 scanf("%d",&e);
 printf("\nStatistics Mode output:");
 cost_insert=0;cost_delete =0;cost_search=0;cost_rehash=0;collisions=0;
 schainHash(n,e);
 printf("\nChain Hash:\nCost Insert:%d\nCost Delete:%d\nCost Rehash:%d\nCollisions:%d\n",cost_insert+cost_search,cost_delete,cost_rehash,collisions);
 cost_insert=0;cost_delete =0;cost_search=0;cost_rehash=0;collisions=0;
 slinearHash(n,e);
 printf("\nLinear Hash:\nCost Insert:%d\nCost Delete:%d\nCost Rehash:%d\nCollisions:%d\n",cost_insert+cost_search,cost_delete,cost_rehash,collisions);
 cost_insert=0;cost_delete =0;cost_search=0;cost_rehash=0;collisions=0;
 sdoubleHash(n,e);
 printf("\nDouble Hash:\nCost Insert:%d\nCost Delete:%d\nCost Rehash:%d\nCollisions:%d\n",cost_insert+cost_search,cost_delete,cost_rehash,collisions);
 cost_insert=0;cost_delete =0;cost_search=0;cost_rehash=0;collisions=0;
 squadraticHash(n,e);
 printf("\nQuadratic Hash:\nCost Insert:%d\nCost Delete:%d\nCost Rehash:%d\nCollisions:%d\n",cost_insert+cost_search,cost_delete,cost_rehash,collisions);
 }
 else {
 printf("\nC - Chain Hashing\nL - Linear Hashing\nQ - Quadratic Hashing\nD - Double Hashing\nEnter alphabet corrsponding to the Operation:");
 char input;
 scanf(" %c",&input);
 if(input == 'c'||input =='C') chainHash();
 else if(input == 'l'||input =='L') linearHash();
 else if(input == 'd'||input =='D') doubleHash();
 else if(input == 'q'||input =='Q') quadraticHash();
 else printf("No such operation");
 printf("Cost Insert:%d\nCost Delete:%d\nCost Rehash:%d\nCollisions:%d\n",cost_insert+cost_search,cost_delete,cost_rehash,collisions);

 }

 return 0;
}
