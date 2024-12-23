#ifndef BINARY_H
#define BINARY_H
    
    #include <stdbool.h>
    typedef struct bin_ BST;
 
    BST *binCreate(void);
    bool binInsert(BST *T, int key);
    bool binBelong(BST *T, int key);
    bool binRemove(BST *T, int key);
    void binPrint(BST *T);
    bool binEmpty(BST *T);
    void binDelete(BST *T);

    //void typeAnother (void);

#endif