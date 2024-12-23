#ifndef RBT_H
#define RBT_H
    
    #include <stdbool.h>
    #define BLACK 0
    #define RED 1
    typedef struct bin_ RBT;
 
    RBT *rbCreate(void);
    bool rbInsert(RBT *T, int key);
    bool rbBelong(RBT *T, int key);
    bool rbRemove(RBT *T, int key);
    bool rbEmpty(RBT *T);
    void rbPrint(RBT *T);
    void rbDelete(RBT *T);

#endif

/* RULES
0 - Is a BST
1 - Every node is Red or Black
2 - The root node and NULL nodes are always Black
3 - If a node is Red, your children must be Black
4 - The number os Black nodes from to root to any NULL node are always the same for every simple path taken
*/