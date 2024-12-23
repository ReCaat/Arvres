#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "avl.h"

typedef struct node_ {
    int value;
    int hight;
    struct node_* left;
    struct node_* right;
} Node;

struct avl_ {
    Node *root;
};

int nodeHight(Node *x);
int balanceFactor(Node *x);
Node *createNode(int key);
Node *insertNode(Node *root, Node *new, bool *duplicate);
Node *removeNode(Node *root, int key, bool *success);
void preOrder(Node* root);
void posOrder(Node** root);


AVL *avlCreate(void) {
    AVL* newT = (AVL*) malloc(sizeof(AVL));
    
    if (newT) newT->root = NULL;
    else printf("Error on create");
    
    return newT;
}

bool avlInsert(AVL *T, int key) {
    if(!T) return false;
    Node *newN = createNode(key);
    if(!newN) return false;

    bool duplicate = false;
    T->root = insertNode(T->root, newN, &duplicate);

    return !duplicate;
}

bool avlBelong(AVL *T, int key) {
    if(!T) return false;

    Node *aux = T->root;
    while(!aux) {
        if(aux->value == key) return true;
        (key < aux->value) ? (aux = aux->left) : (aux = aux->right);    
    }

    return false;
}

bool avlRemove(AVL *T, int key) {
    if(!T) return false;

    bool success = false;
    T->root = removeNode(T->root, key, &success);

    return success;
}

void avlPrint(AVL *T) {
    if(T) {
        preOrder(T->root);
        printf("\n");
    } else 
        printf("Error, the Tree does not exist.");
}

bool avlEmpty(AVL *T) {
    return T == NULL || T->root == NULL;
}

void avlDelete(AVL *T) {
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
        newN->hight = 0;
        newN->left = NULL;
        newN->right = NULL;
    }
    return newN;
}

int max(int a, int b) {
    return ((a > b) ? a : b);
}

//Rotations
Node *rotateLeft(Node *x) {

    Node *y = x->right;
    x->right = y->left;
    y->left = x;

    x->hight = 1 + max(nodeHight(x->left), nodeHight(x->right));
    y->hight = 1 + max(nodeHight(x), nodeHight(y->right));

    return y;
}

Node *rotateRight(Node *x) {

    Node *y = x->left;
    x->left = y->right;
    y->right = x;

    x->hight = 1 + max(nodeHight(x->left), nodeHight(x->right));
    y->hight = 1 + max(nodeHight(y->left), nodeHight(x));

    return y;
}

int nodeHight(Node *x) {
    if(!x) return -1;
    return x->hight;
}

int balanceFactor(Node *x) {
    if(!x) return 0;
    return (nodeHight(x->left) - nodeHight(x->right));
}

//Insert
Node *insertNode(Node *root, Node *new, bool *duplicate) {
    if(!root) return new;

    if(new->value == root->value) {
        *duplicate = true;
        free(new);
        return root;
    }

    if(new->value < root->value) root->left = insertNode(root->left, new, duplicate);
    else                         root->right = insertNode(root->right, new, duplicate);

    root->hight = 1 + max(nodeHight(root->left), nodeHight(root->right));

    if(balanceFactor(root) >= 2) {
        if(balanceFactor(root->left) < 0) 
            root->left = rotateLeft(root->left);
        root = rotateRight(root);
    } else if(balanceFactor(root) <= -2) {
        if(balanceFactor(root->right) > 0) 
            root->right = rotateRight(root->right);
        root = rotateLeft(root);
    }

    return root;
}

//Remove
int getMin(Node *root) {
    Node *aux = root;
    while(aux->left)
        aux = aux->left;

    return aux->value;
}

Node *removeNode(Node *root, int key, bool *success) {
    if(!root) return NULL;

    if(key < root->value) 
        root->left = removeNode(root->left, key, success);
    else if(key > root->value) 
        root->right = removeNode(root->right, key, success);
    else {
        *success = true;
        if(!root->left || !root->right) {
            Node *aux;
            if(!root->left) aux = root->right; 
            if(!root->right) aux = root->left;
            free(root);
            return aux;
        }

        root->value = getMin(root->right);
        root->right = removeNode(root->right, root->value, success);
    }

    root->hight = 1 + max(nodeHight(root->left), nodeHight(root->right));

    if(balanceFactor(root) >= 2) {
        if(balanceFactor(root->left) < 0) 
            root->left = rotateLeft(root->left);
        root = rotateRight(root);
    } else if(balanceFactor(root) <= -2) {
        if(balanceFactor(root->right) > 0) 
            root->right = rotateRight(root->right);
        root = rotateLeft(root);
    }

    return root;
}

//Print
void preOrder(Node* root) {
    if(root) {
        printf("%d, ", root->value);
        printf("%d, ", root->hight);
        printf("%d\n", balanceFactor(root));
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