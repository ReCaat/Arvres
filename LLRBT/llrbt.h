#ifndef LLRBT_H
#define LLRBT_H
    
    #include <stdbool.h>
    #define BLACK 0
    #define RED 1
    typedef struct arn_ LLRBT;
 
    LLRBT *llrbCreate(void);
    bool llrbInsert(LLRBT *T, int key);
    bool llrbBelong(LLRBT *T, int key);
    bool llrbRemove(LLRBT *T, int key);
    bool llrbEmpty(LLRBT *T);
    void llrbPrint(LLRBT *T);
    void llrbDelete(LLRBT *T);

    void llrbUnion (LLRBT *Result, LLRBT *A, LLRBT *B);
    void llrbIntersect (LLRBT *Result, LLRBT *A, LLRBT *B);

#endif

/*Rules - Variation os RBT
0 - Is a BST
1 - Every edge is Red or Balck
2 - The edges leaving to the root node and NULL nodes are always Black
3 - The Red edge must go to the left son
4 - A node can't have more than one red edge
5 - The number of black edges from the root node to the NULL leafs is always the same for every path taken
*/