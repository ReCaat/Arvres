#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "llrbt.h"

typedef struct node_ {
    struct node_ *left;
    struct node_ *right;
    int value;
    int color;
} Node;

struct arn_ {
    Node *root;
};

//Declaração prévias de funções de suporte para aquelas disponíveis ao usuário -- explicadas posteriormente.
void preOrder(Node* root);
void posOrder(Node** root);
Node* rotationLeft(Node* x);
Node* rotationRight(Node* x);
Node* insertNode(Node *root, Node* new, bool* duplicate);
Node* removeNode(Node** root, int key, bool* success);
Node* removeMin(Node** root);
void uniNode(LLRBT *new, Node *original);
void interNode(LLRBT *inter, LLRBT *b, Node *aNode);

//Inicio de funções de usuário -- já comentados no LLRBT.h.
LLRBT *llrbCreate (void) {
    LLRBT *newT = (LLRBT*) malloc(sizeof(LLRBT));

    if(newT) 
        newT->root = NULL;
    else
        printf("Error. Insufficient space");//tratamento de erro.

    return newT;
}

bool llrbInsert (LLRBT *T, int key) {
    if(!T){printf("Error. The tree does not exist"); return false;} //tratamento de erro.

    //Se a árvore não for nula ele cria o nó.
    Node *newN = (Node*) malloc(sizeof(Node));

    if(!newN) {
        printf("Error. Insufficient space");
        return false; //Se a alocação do new nó for má sucedida.
    } else {
        newN->left = NULL; 
        newN->right = NULL;
        newN->value = key;
        newN->color = RED; //Inicializa o new nó como vermelho e com filhos nulos.
    }

    bool duplicate = false;
    //caso o número a ser inserido já estiver na árvore duplicate = true.
    T->root = insertNode(T->root, newN, &duplicate);
    T->root->color = BLACK; //Ajuste do primeiro caso.

    return !duplicate;
}

bool llrbRemove(LLRBT *T, int key) {
    if(!T){printf("Erro ao remover. Árvore não existe."); return false;} //tratamento de erro.
    bool success = false;

    if(T->root) T->root = removeNode(&T->root, key, &success);
    if(T->root) T->root->color = BLACK;
    return success;    
}

bool llrbBelong(LLRBT *T, int key) {
    if(T) {
        Node* aux = T->root;
        while(aux != NULL) {
            if(key == aux->value) return true;

            if(key < aux->value) aux = aux->left;
            else aux = aux->right;
        }
    }
    return false;
}

void llrbPrint (LLRBT *T) {
    if(T) { //tratamento de erro.
        preOrder(T->root);
        printf("\n");
    } else 
        printf("Error, the tree does not exist");
}

void llrbDelete (LLRBT *T) {
    if(T) { //tratamento de erro.
        posOrder(&T->root);

        free(T);
        T = NULL;
    }
}

bool llrbEmpty(LLRBT *T) {
    return T == NULL || T->root == NULL;
}

void llrbUnion(LLRBT* uniao, LLRBT *A, LLRBT *B){
    uniNode(uniao, A->root);
    uniNode(uniao, B->root);
}

void llrbIntersect(LLRBT* intersection, LLRBT *A, LLRBT *B){
    
    if(!A || !B) return;
    interNode(intersection, B, A->root);
}
//Fim das funções de usuário.

//Auxilia imprimir a árvore em ordem.
void preOrder(Node* root) {
    if(root) {
        printf("%d ", root->value);
        (root->color == RED) ? printf("RED\n") : printf("BLACK\n");
        preOrder(root->left);
        preOrder(root->right);
    }
}

//Auxilia apagar a árvore em pós-ordem.
void posOrder(Node** root) {
    if(*root) {
        posOrder(&(*root)->left);
        posOrder(&(*root)->right);
        free(*root);
        *root = NULL;
    }
}

//Funções auxiliares para remoção e deleção.
//Detecta se um dado nó é vermelho ou negro, lembrando que pelas regras da árvore nós nulos são negros.
bool isRed(Node* root) {
    if(!root) return false;
    return root->color == RED;
}

//Rotaciona um nó para a left e troca as colores dele com seu filho da right, para adequar com a propriedade das arestas.
Node* rotationLeft(Node* x){
    Node* y = x->right;
    x->right = y->left;
    y->left = x;
    
    int aux = x->color;
    x->color = y->color;
    y->color = aux;

    return y;
}

//Rotaciona um nó para a right e troca as colores dele com seu filho esquerdo, para adequar com a propriedade das arestas.
Node* rotationRight(Node* x){
    Node* y = x->left;
    x->left = y->right;
    y->right = x;

    int aux = x->color;
    x->color = y->color;
    y->color = aux;

    return y;
}

//Inverte a color do nó pai e de seus filhos (caso existirem).
void colorsInvertion(Node* x) {
    x->color = !x->color;
    if(x->right != NULL) x->right->color = !x->right->color;
    if(x->left != NULL) x->left->color = !x->left->color;
}

//Acha o Nó que contem a key de menor valor -- útil para trocar com o menor ítem do braço direito. Para remoção.
Node* getMin(Node* trocado) {
    Node* aux = trocado;
    while(aux->left != NULL)
        aux = aux->left;
    
    return aux;
}

//Outra função exclusiva da remoção, ela "paa a vermelhidão" na direção do nó que contenha a key que o usuário deseja remover:
//true - left;
//false - right.
Node* moveRed(Node* root, bool side) {

    colorsInvertion(root); 
    if(side && isRed(root->right->left)) {
        root->right = rotationRight(root->right);
        root = rotationLeft(root);
        colorsInvertion(root);
    } 
    
    if(!side && isRed(root->left->left)) {
        root = rotationRight(root);
        colorsInvertion(root);
    }

    return root;
}

//Na volta da recursão reorganiza a árvore e retorna o nó para devido balanceamento negro da árvore.
Node* fixUp(Node* root) {
    if(root != NULL) {
        if(isRed(root->right) && !isRed(root->left)) root = rotationLeft(root);

        if(isRed(root->left) && isRed(root->left->left)) root = rotationRight(root);
        
        if(isRed(root->right) && isRed(root->left)) colorsInvertion(root);
    }

    return root;
}

//Principal inserção
Node* insertNode(Node *root, Node* new, bool* duplicate) {
    if(root == NULL)
        return new;

    if(root->value == new->value) { //Caso houver um nó com o mesmo valor a serinserido, ele apaga o new nó, dando status de falha a inserção.
        *duplicate = true; 
        free(new); new = NULL;
        return root; //Evita valores duplicados
    }
    
    if(new->value < root->value)
        root->left = insertNode(root->left, new, duplicate);
    else
        root->right = insertNode(root->right, new, duplicate);
    
    return fixUp(root); //Rebalanceamento.
}

//Deleção principal
Node* removeMin(Node** root) {
    if(*root == NULL) return NULL;

    if((*root)->left == NULL) {
        //Não precisa se preocupar se existe um nó a right pelas propriedades da árvore.
        free(*root); *root = NULL; 
        return NULL;
    }

    if(!isRed((*root)->left) && !isRed((*root)->left->left))
        *root = moveRed(*root, true);

    (*root)->left = removeMin(&(*root)->left);

    return fixUp(*root); //Rebalanceamento.
}

Node* removeNode(Node** root, int key, bool *success) {
    if(*root == NULL) return NULL;

    if(key < (*root)->value) {
        if((*root)->left != NULL && !isRed((*root)->left) && !isRed((*root)->left->left))
            *root = moveRed(*root, true); //Mover vermelhidão.

        (*root)->left = removeNode(&(*root)->left, key, success);
    } else {

        if(isRed((*root)->left)) //Garante que não terá nós na esqueda se não houver na right e move vermelhidão.
            *root = rotationRight(*root);

        if(key == (*root)->value && (*root)->right == NULL) { //Caso o nó foi achado e não haja filhos .
            *success = true; //Redefine status da remoção como bem sucedido.
            free(*root); *root = NULL; 
            return NULL;
        }

        if((*root)->right != NULL && !isRed((*root)->right) && !isRed((*root)->right->left))
            *root = moveRed(*root, false); //Mover vermelhidão

        if(key == (*root)->value) { //Caso o nó foi achado e tenha filhos.
            *success = true; //Redefine status da remoção como bem sucedido.
            (*root)->value = getMin((*root)->right)->value;
            (*root)->right = removeMin(&(*root)->right);
        } 
        else  (*root)->right = removeNode(&(*root)->right, key, success);
    }

    return fixUp(*root); //Rebalanceamento.
}

//Ajuda a operação de unir árvores,
//Apenas tenta inserir todos os nós de uma árvore em uma nova.
void uniNode(LLRBT *new, Node *original){
    if (!original || !new) return; //Parada da recursão.
    LLRBT_inserir(new, original->value);
    uniNode(new, original->left);
    uniNode(new, original->right);
}

//Ajuda a operação de intersecção de duas árvores,
//Se o nó da árvore A pertence a árvore B ele insere na árvore intersecção.
void interNode(LLRBT *inter, LLRBT *b, Node *aNode){
    if (inter == NULL || aNode == NULL) return; //Parada da recursão.
    if (LLRBT_pertence(b, aNode->value)) LLRBT_inserir(inter, aNode->value);
    interNode(inter, b, aNode->left);
    interNode(inter, b, aNode->right);
}