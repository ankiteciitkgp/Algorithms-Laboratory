#include<stdio.h>
#include<stdlib.h>
#include<stdlib.h>

#define INT_MAX    2147483647
#define maxlevel 6

typedef struct snode{
int key;
char* data;
struct snode **next;
}snode;

typedef struct skiplist{
int size;
int level;
snode *head;
}skiplist;

skiplist* newskiplist(){
    int i;
    skiplist* list = (skiplist *)malloc(sizeof(struct skiplist));
    snode *header = (snode *)malloc(sizeof(struct snode));
    list->head = header;
    header->key = INT_MAX;
    header->next = (snode **)malloc(sizeof(snode*) * (maxlevel+1));
    for (i = 0; i <= maxlevel; i++) {
        header->next[i] = list->head;
    }
    list->level = 1;
    list->size = 0;

    return list;
}

static int rand_level(){
    int level = 1;
    while (rand() < RAND_MAX/2 && level < maxlevel)
        level++;
    return level;
}

int insert(skiplist *list, int k, char* value){
    snode *update[maxlevel+1];
    snode *x = list->head;
    int i, level;

    for (i = list->level; i >= 1; i--) {
        while (x->next[i]->key < k)
            x = x->next[i];
        update[i] = x;
    }
    x = x->next[1];
    if (k == x->key) {
        strcat(x->data,", ");
        strcat(x->data,value);
        return 0;
    } else {
        level = rand_level();
        if (level > list->level) {
            for (i = list->level+1; i <= level; i++) {
                update[i] = list->head;
            }
            list->level = level;
        }
        x = (snode *)malloc(sizeof(snode));
        x->key = k;
        x->data = value;
        x->next = (snode **)malloc(sizeof(snode*) * (level + 1));
        for (i = 1; i <= level; i++) {
            x->next[i] = update[i]->next[i];
            update[i]->next[i] = x;
        }
    }
    return 0;
}

snode *search(skiplist *list, int key){
    snode *x = list->head;
    int i;
    for (i = list->level; i >= 1; i--) {
        while (x->next[i]->key < key)
            x = x->next[i];
    }
    if (x->next[1]->key == key) {
        return x->next[1];
    } else {
        return NULL;
    }
    return NULL;
}

static void freenode(snode *x){
    if (x) {
        free(x->next);
        free(x);
    }
}

int snodeDelete(skiplist *list, int key){
    int i;
    snode *update[maxlevel];
    snode *x = list->head;
    for (i = list->level; i >= 1; i--) {
        while (x->next[i]->key < key)
            x = x->next[i];
        update[i] = x;
    }

    x = x->next[1];
    if (x->key == key) {
        for (i = 1; i <= list->level; i++) {
            if (update[i]->next[i] != x)
                break;
            update[i]->next[1] = x->next[i];
        }
        freenode(x);

        while (list->level > 1 && list->head->next[list->level] == list->head)
            list->level--;
        return 0;
    }
    return 1;
}

static void ioTraverse(skiplist *list){
    snode *x = list->head;
    while (x && x->next[1] != list->head) {
        printf("%d[%s]->", x->next[1]->key, x->next[1]->data);
        x = x->next[1];
    }
    printf("NIL\n");
}

skiplist* merge(skiplist* a, skiplist* b){
 /*skiplist* list = newskiplist();
 snode *update[maxlevel+1];
 int i;
 list->level = max(a->level,b->level);
 for(i = 1;i<=list->level;i++)
    update[i] = list->head;

 while(a->head->next[1]!= NULL && b->head->next[1]!=NULL){
    int key1 = a->head->next[1]->key;
    int key2 = b->head->next[1]->key;
    if(key1>key2)
 }*/
 snode* x = b->head;
 skiplist* y = a;
 while(x && x->next[1] != b->head){
    insert(y, x->next[1]->key,x->next[1]->data);
    x = x->next[1];
 }
 return y;
}

int main( int argc, char *argv[] ){

    int i =1;
    skiplist* a = newskiplist();
    skiplist* b = newskiplist();

    int key;
    char* data;
    char* list;
    while(i < argc) {

            if(strcmp(argv[i],"I")==0) {
                    i++;
                    list = argv[i++];
                    key = atoi(argv[i++]);
                    data = argv[i++];
                    if(strcmp(list,"a") == 0){
                         insert(a,key,data);
                    }
                    else{
                         insert(b,key,data);
                    }
             }

             else if(strcmp(argv[i],"S")==0) {
                    i++;
                    list = argv[i++];
                    key = atoi(argv[i++]);
                    printf("Searching  for key:%d in skip list: %s\n",key,list);
                    snode* s;
                    if(strcmp(list,"a") == 0)
                        s = search(a,key);
                    else
                        s = search(b,key);
                    if(s==NULL)
                        printf("Element not found\n");
                    else
                        printf("Element found: %d[%s]\n",s->key,s->data);

             }
             else if(strcmp(argv[i],"D")==0){
                    i++;
                    list = argv[i++];
                    key = atoi(argv[i++]);
                    printf("Deleting key: %d in list: %s\n",key,list);
                    if(strcmp(list,"a") == 0)
                        snodeDelete(a,key);
                    else {

                        snodeDelete(b,key);
                    }
             }
             else if(strcmp(argv[i],"T")==0){
                    i++;
                    list = argv[i++];
                    printf("In order Traverse of list:%s started\n",list);
                    if(strcmp(list,"a") == 0)
                        ioTraverse(a);
                    else
                        ioTraverse(b);
             }

            else if(strcmp(argv[i],"M")==0){

                    i++;
                    printf("Merging list a and list b\n");
                    a = merge(a,b);
             }
             else break;
     }
 return 0;
}
