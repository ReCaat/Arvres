#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "rbt.h"

typedef struct node_ {
    int value;
    int color;
    struct node_* parent;
    struct node_* left;
    struct node_* right;
} Node;

struct bin_ {
    Node *root;
};

void insertFixUp(RBT *T, Node *new);
void transplant(RBT *T, Node *u, Node *v);
void removeFixUp(RBT *T, Node *fix);
void preOrder(Node* root);
void posOrder(Node** root);

//User functions
RBT *rbCreate(void) {
    RBT* newT = (RBT*) malloc(sizeof(RBT));
    
    if (newT) newT->root = NULL;
    else printf("Error on create");
    
    return newT;
}

bool rbInsert(RBT *T, int key) {
    if(!T) return false;
    Node *newN = createNode(key);
    if(!newN) return false;

    Node *y = NULL, *x = T->root;
    while(x) {
        y = x;
        if(key == x->value) {
            free(newN);
            return false;
        } else
        if(key < x->value) x = x->left;
        else               x = x->right;
    }

    newN->parent = y;
    if(!y)                  T->root = newN;
    else if(key < y->value) y->left = newN;
    else                    y->right = newN;

    insertFixUp(T, newN);
    return true;
}

bool rbBelong(RBT *T, int key) {
    if(!T) return false;

    Node *aux = T->root;
    while(aux) {
        if(aux->value == key) return true;
        (key < aux->value) ? (aux = aux->left) : (aux = aux->right);    
    }

    return false;
}

bool rbRemove(RBT *T, int key) {
    if(!T) return false;

    //find the node to be deleted
    Node *del = T->root;
    while(del) { 
        if(del->value == key) break;
        (key < del->value) ? (del = del->left) : (del = del->right);    
    }
    if(!del) return false; //guarantees the node exist


    Node *x;
    Node *sub = del;
    int originalColor = del->color;
    if(!del->left) { //Case 1
        x = del->right;
        transplant(T, del, x);

    } else if(!del->right) { //Case 2
        x = del->left;
        transplant(T, del, x);
    
    } else {
        sub = del->right; //will find the min of right subtree
        while(sub->left)
            sub = sub->left;

        originalColor = sub->color;
        x = sub->right;

        if(sub->parent == del && x) 
            x->parent = sub;
        else if(sub->parent != del) {
            transplant(T, sub, x);
            sub->right = del->right;
            sub->right->parent = sub;
        }
        
        transplant(T, del, sub);
        sub->left = del->left;
        sub->left->parent = sub; //fix final pointers
        sub->color = del->color;
    }

    if(originalColor == BLACK) removeFixUp(T, x);

    del->parent = NULL; //safe
    del->left = NULL;
    del->right = NULL;
    free(del);
    
    return true;
}

void rbPrint(RBT *T) {
    if(T) {
        preOrder(T->root);
        printf("\n");
    } else 
        printf("Error, the Tree does not exist.");
}

bool rbEmpty(RBT *T) {
    return T == NULL || T->root == NULL;
}

void rbDelete(RBT *T) {
    if(T) {
        posOrder(&T->root);

        free(T);
        T = NULL;
    }
}
//End of user functions

//Create
Node *createNode(int key) {
    Node *newN = (Node*) malloc(sizeof(Node));
    if(newN) {
        newN->value = key;
        newN->color = RED;
        newN->left = NULL;
        newN->right = NULL;
    }
    return newN;
}

//FixUp methods
bool isRed(Node* root) {
    if(!root) return false;
    return root->color == RED;
}

//0 : leftSon / 1 : rightSon / -2 : first node
int side(Node *root) {
    if(!root->parent) return -2;
    if(root == root->parent->left) return 0;
    return 1;
}

void rotateLeft(RBT *T, Node* x){
    Node* y = x->right;
    
    x->right = y->left;
    if(y->left) y->left->parent = x;
    y->parent = x->parent;
    
    int k = side(x);
    if(k == -2) T->root = y;
    else if(!k) x->parent->left = y;
    else        x->parent->right = y;  
    
    y->left = x;
    x->parent = y;
}

void rotateRight(RBT *T, Node* x){
    Node* y = x->left;
    
    x->left = y->right;
    if(y->right) y->right->parent = x;
    y->parent = x->parent;
    
    int k = side(x);
    if(k == -2) T->root = y;
    else if(!k) x->parent->left = y;
    else        x->parent->right = y;  
    
    y->right = x;
    x->parent = y;
}


//Insert
void insertFixUp(RBT *T, Node *new) {
    while(isRed(new->parent)) {
        if(!side(new->parent)) {
            Node *y = new->parent->parent->right; //uncle
            if(isRed(y)) {
                new->parent->color = BLACK;
                y->color = BLACK;
                new->parent->parent->color = RED;
                new = new->parent->parent; //constinue fixUp
            } else {
                if(side(new)) {
                    new = new->parent; 
                    rotateLeft(T, new); //case 3 brings us to case 2
                }
                new->parent->color = BLACK;
                new->parent->parent->color = RED;
                rotateRight(T, new->parent->parent);
            }
        } else if(side(new->parent)) { //same thing but for other side
            Node *y = new->parent->parent->left;
            if(isRed(y)) {
                new->parent->color = BLACK;
                y->color = BLACK;
                new->parent->parent->color = RED;
                new = new->parent->parent; 
            } else {
                if(!side(new)) {
                    new = new->parent;
                    rotateRight(T, new);
                }
                new->parent->color = BLACK;
                new->parent->parent->color = RED;
                rotateLeft(T, new->parent->parent);
            }
        }
    }
    T->root->color = BLACK;
}

//Move subtrees
void transplant(RBT *T, Node *u, Node *v) {
    if(side(u) == -2) T->root = v;
    else if(!side(u)) u->parent->left = v;
    else              u->parent->right = v;

    if(v) v->parent = u->parent;
}

//Remove
void removeFixUp(RBT *T, Node *fix) {
    Node *w; //fix sibling
    while(fix != T->root && !isRed(fix)) {
        if(!side(fix)) {
            w = fix->parent->right;

            if(isRed(w)) {
                w->color = BLACK;
                fix->parent->color = RED;
                rotateLeft(T, fix->parent);
                w = fix->parent->right;
            }
            if(!isRed(w->left) && !isRed(w->right)) {
                w->color = RED;
                fix = fix->parent;
            } else {
                if(!isRed(w->right)) {
                    w->left->color = BLACK;
                    w->color = RED;
                    rotateRight(T, w);
                    w = fix->parent->right;
                }

                w->color = fix->parent->color;
                fix->parent->color = BLACK;
                w->right->color = BLACK;
                rotateLeft(T, fix->parent);
                fix = T->root;
            }
        } else {
            w = fix->parent->left;

            if(isRed(w)) {
                w->color = BLACK;
                fix->parent->color = RED;
                rotateRight(T, fix->parent);
                w = fix->parent->left;
            }
            if(!isRed(w->left) && !isRed(w->right)) {
                w->color = RED;
                fix = fix->parent;
            } else {
                if(!isRed(w->left)) {
                    w->right->color = BLACK;
                    w->color = RED;
                    rotateLeft(T, w);
                    w = fix->parent->left;
                }

                w->color = fix->parent->color;
                fix->parent->color = BLACK;
                w->left->color = BLACK;
                rotateRight(T, fix->parent);
                fix = T->root;
            }
        }
        
    }
    
    fix->color = BLACK;
}

//Print
void preOrder(Node* root) {
    if(root) {
        printf("%d, ", root->value);
        root->parent ? printf("%d, ", root->parent->value) : printf("NULL, ");
        root->left ? printf("%d, ", root->left->value) : printf("NULL, ");
        root->right ? printf("%d, ", root->right->value) : printf("NULL, ");
        isRed(root) ? printf("RED \n") : printf("BLACK \n");
        preOrder(root->left);
        preOrder(root->right);
    }
}

//Delete
void posOrder(Node** root) {
    if(*root != NULL) {
        posOrder(&(*root)->left);
        posOrder(&(*root)->right);
        free(*root);
        *root = NULL;
    }
}

int main(void) {
    RBT *t = rbCreate();
    int n;
    scanf("%d", &n);

    for(int i = 0; i < n; i++) {
        int key;
        scanf("%d", &key);
        
        rbInsert(t, key);
        rbPrint(t);
    }

    while(true) {
        int key;
        scanf("%d", &key);
        
        rbRemove(t, key);
        rbPrint(t);
    }

    rbDelete(t);

    return 0;
}