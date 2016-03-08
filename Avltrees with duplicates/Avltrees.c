#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//Node creation
typedef struct node{
    int key;
    char *value;
    int height;
    struct node *left;
    struct node *right;
}node;
node* newNode(int k,char *val){
    node *n = (node *)malloc(sizeof(node));
    n->key = k;
    n->value = val;
    n->height = 1;
    n->left = NULL;
    n->right = NULL;
    return n;
};

int max(int a,int b){
    if(a>b) return a;
    else return b;
}
node *leftRotate(node *n){
    node *r = n->right;

    n->right = r->left;
    r->left = n;
    r->height = max(r->left->height,r->right->height)+1;
    n->height = max(n->left->height,n->right->height)+1;
    return r;
}
node *rightRotate(node *n){
    node *l = n->left;

    n->left = l->right;
    l->right = n;
    l->height = max(l->left->height,l->right->height)+1;
    n->height = max(n->left->height,n->right->height)+1;
    return l;
}

int getBal(node* left,node* right){
   int hl,hr;
   if(left == NULL) hl = 0;
   else hl = left->height;

   if(right == NULL) hr = 0;
   else hr = right->height;

   return hl - hr;
}

node* insertion(node *n,int k,char *val){
    if(n == NULL)
        return newNode(k,val);

    if(k <= n->key) n->left = insertion(n->left,k,val);
    else n->right = insertion(n->right,k,val);
    int balance  = getBal(n->left,n->right);

    if(balance>1){
            if(k <= n->left->key)
                return rightRotate(n);
            else {
                n->left = leftRotate(n->left);
                return rightRotate(n);
            }
    }
    else if(balance < -1){
        if(k<=n->right->key){
            n->right = rightRotate(n->right);
            return leftRotate(n);
        }
        else
            return leftRotate(n);
    }
  return n;
}

void ioTraverse(node *n){
   if(n == NULL) return;
   if(n->left != NULL)
       ioTraverse(n->left);

   printf("%d->%s\n",n->key,n->value);

   if(n->right!= NULL)
      ioTraverse(n->right);
}
int search(node *n,int k){
    if(n==NULL) return 0;
    int count = 0;
    if(n->key==k){
        count++;
        printf("%s\n",n->value);
    }
    if(k <= n->key ) count+=search(n->left,k);
    if(k >= n->key ) count+=search(n->right,k);
    return count;
}
node* minimumNode(node* n){
    node* current = n;
    while(current->left!=NULL)
        current = current->left;
    return current;
}
node* nodeDelete(node *n,int k){
    if(n == NULL) return NULL;
    if(k < n->key) n->left = nodeDelete(n->left,k);
    else if(k> n->key) n->right = nodeDelete(n->right,k);
    else{
        if(n->left==NULL && n->right==NULL){
            n = NULL;
            return NULL;
        }
        else if(n->left!=NULL && n->right==NULL){
            n->left = nodeDelete(n->left,k);
            n = n->left;
            return n;
        }
        else if(n->left==NULL && n->right!=NULL){
            n->right = nodeDelete(n->right,k);
            n = n->right;
            return n;
        }
        else{
            if(n->left->key == k)n->left = nodeDelete(n->left,k);
            if(n->right->key == k)n->right = nodeDelete(n->right,k);
            node* temp = minimumNode(n->right);
            n->key = temp->key;
            n->value = temp->value;
            n->right = nodeDelete(n->right,temp->key);
            temp = NULL;
        }
    }
    int balance  = getBal(n->left,n->right);

    if(balance>1){
            if(k <= n->left->key)
                return rightRotate(n);
            else {
                n->left = leftRotate(n->left);
                return rightRotate(n);
            }
    }
    else if(balance < -1){
        if(k<=n->right->key){
            n->right = rightRotate(n->right);
            return leftRotate(n);
        }
        else
            return leftRotate(n);
    }
  return n;
}
int main( int argc, char *argv[] ){
    int i =1;
    node* n = NULL;
    int key;
    char* value;
    while(i < argc) {
             if(strcmp(argv[i],"I")==0) {
                    i++;
                    key = atoi(argv[i++]);
                    value = argv[i++];
                    n = insertion(n,key,value);
             }
             else if(strcmp(argv[i],"S")==0) {
                    i++;
                    key = atoi(argv[i++]);
                    printf("Search Results for key = %d:\n",key);
                    if(search(n,key)==0) printf("No element found\n");
             }
             else if(strcmp(argv[i],"D")==0){
                    i++;
                    key = atoi(argv[i++]);
                    printf("Deleting key = %d:\n",key);
                    n = nodeDelete(n,key);
             }
             else if(strcmp(argv[i],"T")==0){
                    i++;
                    printf("In order Traverse started\n");
                    ioTraverse(n);
             }
             else break;
     }
 return 0;
}
