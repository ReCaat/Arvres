#ifndef AVL_H
#define AVL_H
    
    #include <stdbool.h>
    typedef struct avl_ AVL;
 
    AVL *avlCreate(void);
    bool avlInsert(AVL *T, int key);
    bool avlBelong(AVL *T, int key);
    bool avlRemove(AVL *T, int key);
    bool avlEmpty(AVL *T);
    void avlPrint(AVL *T);
    void avlDelete(AVL *T);

    //void avlAnother (void);

#endif
