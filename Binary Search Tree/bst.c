#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "bst.h"

typedef struct node_ {
    int value;
    struct node_* left;
    struct node_* right;
    struct node_* parent;
} Node;

struct bin_ {
    Node *root;
};

Node *createNode(int key);
Node *getMin(Node *root);
void transplant(BST *T, Node *u, Node *v);
void preOrder(Node* root);
void posOrder(Node** root);


BST *binCreate(void) {
    BST* newT = (BST*) malloc(sizeof(BST));
    
    if (newT) newT->root = NULL;
    else printf("Error on create");
    
    return newT;
}

bool binInsert(BST *T, int key) {
    if(!T) return false;
    Node *newN = createNode(key);
    if(!newN) return false;

    Node* x = T->root;
    Node* y = NULL;

    while(x) {
        y = x;

        if(key == x->value) {
            free(newN);   
            return false;
        }
        if(key < x->value)  x = x->left;
        else                x = x->right;
    }
    newN->parent = y;

    if(!y)                  T->root = newN;
    else if(key < y->value) y->left = newN;
    else                    y->right = newN;

    return true;
}

bool binBelong(BST *T, int key) {
    if(!T) return false;

    Node *aux = T->root;
    while(!aux) {
        if(aux->value == key) return true;
        (key < aux->value) ? (aux = aux->left) : (aux = aux->right);    
    }

    return false;
}

bool binRemove(BST *T, int key) {
    if(!T) return false;

    Node *del = T->root;
    while(del) {
        if(del->value == key) break;
        (key < del->value) ? (del = del->left) : (del = del->right);    
    }
    if(!del) return false;
        
    
    if(!del->left)
        transplant(T, del, del->right);
    else if(!del->right)
        transplant(T, del, del->left);
    else {
        Node*sub = getMin(del->right);
        if(sub->parent != del) {
            transplant(T, sub, sub->right);
            sub->right = del->right;
            sub->right->parent = sub;
        }

        transplant(T, del, sub);
        sub->left = del->left;
        sub->left->parent = sub;
    }


    del->parent = NULL;
    del->left = NULL;
    del->right = NULL;
    free(del);

    return true;
}

void binPrint(BST *T) {
    if(T) {
        preOrder(T->root);
        printf("\n");
    } else 
        printf("Error, the Tree does not exist.");
}

bool binEmpty(BST *T) {
    return T == NULL || T->root == NULL;
}

void binDelete(BST *T) {
    if(T) {
        posOrder(&T->root);

        free(T);
        T = NULL;
    }
}

//Create
Node *createNode(int key) {
    Node *newN = (Node*) malloc(sizeof(Node));
    if(newN) {
        newN->value = key;
        newN->left = NULL;
        newN->right = NULL;
    }
    return newN;
}

//Remove
Node *getMin(Node *root) {
    Node *aux = root;
    while(aux->left)
        aux = aux->left;

    return aux;
}

void transplant(BST *T, Node *u, Node *v) {
    if(!u->parent)               T->root = v;
    else if(u == u->parent->left) u->parent->left = v;
    else                         u->parent->right = v;

    if(v) v->parent = u->parent;
}

//Print
void preOrder(Node *root) {
    if(root) {
        printf("%d, ", root->value);
        root->parent ? printf("%d\n", root->parent->value) : printf("NULL\n");
        preOrder(root->left);
        preOrder(root->right);
    }
}

//Delete
void posOrder(Node **root) {
    if(*root != NULL) {
        posOrder(&(*root)->left);
        posOrder(&(*root)->right);
        free(*root);
        *root = NULL;
    }
}