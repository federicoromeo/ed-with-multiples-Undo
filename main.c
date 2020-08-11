#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct node {
    char color;
    int id;
    char *text;
    struct node *p;
    struct node *left;
    struct node *right;
};

typedef struct node node;

struct tree{
    node *root;   //testa
    node *nil;
};

typedef struct tree tree;

node* nil;
int max_index = -1;

//PROTOTYPES

void in_order_walk(node *T_root);
node *node_search(node *T_root, char *k);
void left_rotate(node **T_root, node *x);
void right_rotate(node **T_root, node *x);
void RB_insert(node **T_root, node *z);
void RB_insert_fixup(node **T_root, node *z);
void RB_transplant(node **T_root, node *u, node *v);
node *RB_minimum(node *T_root);
void RB_delete(node **T_root, node *z);
void RB_delete_fixup(node **T_root, node *x);
void empty_tree(node **T_root);

void print_delta(node *T_root, int ind1, int ind2);
int count_nodes(node *root);
node *tree_search(node* x, int id);
void print2DUtil(node *root, int space);


int main(){

    // Creation of nil node
    nil = (node*) malloc(sizeof(node));                  // sentinella
    nil->left = nil;
    nil->right = nil;
    nil->p = nil;
    nil->color = 'b';

    // Creation of the editor tree
    tree* tree = malloc(sizeof(tree));
    if(!tree){
        fprintf(stderr,"\nError\n");
        exit(-1);
    }
    tree->root = nil;

    //for testing
    char line[1024];
    FILE *fp = fopen("C:/Users/feder/CLionProjects/ed-con-Undo-multipli/test.txt","r");
    if(fp==NULL){
        printf("Error opening file");
        exit(-1);
    }

    int ind1=0, ind2=0, times=0, delta=0, actual_id=ind1;
    char command;

    while(!feof(fp)) {
        fgets(line, 1024, fp);
        line[strcspn(line, "\n\r")] = '\0';

        //fscanf(fp, "%s", line);
        //printf("\n");

        command = line[strlen(line)-1];
        switch(command){

            case 'q':{
                exit(0);
            }

            case '.':{
                break;
            }

            case 'c':{
                char* splitted_line = strtok(line, ",");
                while( splitted_line != NULL ) {
                    int j = 1;
                    for (int k=0; k<=strlen(splitted_line); k++) {
                        if(j==1){
                            // Get the first address
                            ind1 = (int) strtol(splitted_line, (char **)NULL, 10);
                            //ind1 = atoi(splitted_line);
                            splitted_line = strtok(NULL, ",");
                        }
                        else if(j==2){
                            // Get the second address by removing the last character (the command)
                            splitted_line[strlen(splitted_line)-1] = '\0';
                            ind2 = (int) strtol(splitted_line, (char **)NULL, 10);
                            //ind2 = atoi(splitted_line);

                            splitted_line = strtok(NULL, ",");
                            break;
                        }
                        j++;
                    }
                }

                delta = ind2 - ind1 +1;

                //CHANGE:

                /*
                 *** se l'albero è vuoto E ind1=1:
                 *      RB_insert
                 *** else se ind1 è presente (cioe ind1<=count_nodes(tree))
                 *      change_text fino a che sono dentro e RB_insert quando non esiste piu il nodo
                 *
                 *** else se ind1==count_node(tree)+1
                 *       RB_insert normale
                 */



                // se l'albero è vuoto E ind1=1:
                if( tree_search(tree->root,ind1)==nil && count_nodes(tree->root)<=1 && ind1==1 ){
                    while(delta>0){

                        fgets(line, 1024, fp);
                        line[strcspn(line, "\n\r")] = '\0';

                        //fscanf(fp, "%s", line);
                        node* x = (node*)malloc((strlen(line)+1)*sizeof(node));
                        x->text = malloc((strlen(line)+1)*(sizeof(char*)));
                        x->id = ind1;
                        strcpy(x->text,line);
                        x->right=x->left=x->p=nil;
                        RB_insert(&tree->root,x);
                        delta--;
                        ind1++;
                    }
                }
                //se ind1 è presente
                else if(tree_search(tree->root,ind1)!=nil && ind1<=count_nodes(tree->root)) {
                    actual_id = ind1;
                    while (delta > 0) {

                        fgets(line, 1024, fp);
                        line[strcspn(line, "\n\r")] = '\0';
                        //fscanf(fp, "%s", line);

                        if(tree_search(tree->root,ind1)!=nil) {
                            free(tree_search(tree->root, ind1)->text);
                            tree_search(tree->root, ind1)->text = malloc((strlen(line)+1)*(sizeof(char*)));
                            strcpy(tree_search(tree->root, ind1)->text, line);
                            delta--;
                            ind1++;
                            actual_id = ind1;
                        }
                        else{
                            node* x = (node*)malloc((strlen(line)+1)*sizeof(node));
                            x->text = malloc((strlen(line)+1)*(sizeof(char*)));
                            x->id = actual_id;
                            strcpy(x->text,line);
                            x->right=x->left=x->p=nil;
                            RB_insert(&tree->root,x);
                            delta--;
                            ind1++;
                            actual_id = ind1;
                        }
                    }
                }
                else if(ind1==count_nodes(tree->root)+1){
                    while (delta > 0) {

                        fgets(line, 1024, fp);
                        line[strcspn(line, "\n\r")] = '\0';
                        //fscanf(fp, "%s", line);

                        node *x = (node *) malloc((strlen(line) + 1) * sizeof(node));
                        x->text = malloc((strlen(line) + 1) * (sizeof(char *)));
                        x->id = actual_id;
                        strcpy(x->text, line);
                        x->right = x->left = x->p = nil;
                        RB_insert(&tree->root, x);
                        delta--;
                        ind1++;
                        actual_id = ind1;
                    }
                }

                break;
            }
            case 'd':{
                char* splitted_line = strtok(line, ",");
                while( splitted_line != NULL ) {
                    int j = 1;
                    for (int k=0; k<=strlen(splitted_line); k++) {
                        if(j==1){
                            // Get the first address
                            ind1 = (int) strtol(splitted_line, (char **)NULL, 10);
                            //ind1 = atoi(splitted_line);
                            splitted_line = strtok(NULL, ",");
                        }
                        if(j==2){
                            // Get the second address by removing the last character (the command)
                            splitted_line[strlen(splitted_line)-1] = '\0';
                            ind2 = (int) strtol(splitted_line, (char **)NULL, 10);
                            //ind2 = atoi(splitted_line);

                            splitted_line = strtok(NULL, ",");
                            break;
                        }
                        j++;
                    }
                }

                delta = ind2 - ind1 +1;

                while(delta>0){
                    fscanf(fp, "%s", line);
                    printf("NEW LINE: ");
                    puts(line);
                    delta--;
                }

                //DELETE:


                break;
            }
            case 'p':{
                char* splitted_line = strtok(line, ",");
                while( splitted_line != NULL ) {
                    int j = 1;
                    for (int k=0; k<=strlen(splitted_line); k++) {
                        if(j==1){
                            // Get the first address
                            ind1 = (int) strtol(splitted_line, (char **)NULL, 10);
                            //ind1 = atoi(splitted_line);
                            splitted_line = strtok(NULL, ",");
                        }
                        if(j==2){
                            // Get the second address by removing the last character (the command)
                            splitted_line[strlen(splitted_line)-1] = '\0';
                            ind2 = (int) strtol(splitted_line, (char **)NULL, 10);
                            //ind2 = atoi(splitted_line);

                            splitted_line = strtok(NULL, ",");
                            break;
                        }
                        j++;
                    }
                }

                if(ind1==0 && ind2==0){
                    puts(".");
                    break;
                }
                // GRAPHIC
                    //puts("2D representation:\n");
                    //print2DUtil(tree->root, 0);
                    //puts("\n");

                // PRINT
                //puts("inorder print:\n");
                print_delta(tree->root,ind1,ind2);

                break;
            }

            case 'u':
            case 'r':{
                // Get the number of redo/undo times by removing the last character (the command)
                line[strlen(line)-1] = '\0';
                times = (int) strtol(line, (char **)NULL, 10);
                //times = atoi(line);
                printf("\ntimes= %d", times);
                break;
            }

            default: break;

        }

    }
    fclose(fp);
    return 0;
}



//IMPLEMENTATIONS
void print_delta(node *T_root, int ind1, int ind2){
    int delta = ind2 - ind1 + 1;
    while(delta>0) {
        if(tree_search(T_root, ind1)!=nil) {
            puts(tree_search(T_root, ind1)->text);
            delta--;
            ind1++;
        }
        else break;
    }
}

// Function to print binary tree in 2D
// It does reverse inorder traversal
void print2DUtil(node *root, int space) {
    // Base case
    if (root == nil)
        return;

    // Increase distance between levels
    space += 10;

    // Process right child first
    print2DUtil(root->right, space);

    // Print current node after space
    // count
    printf("\n");
    for (int i = 10; i < space; i++)
        printf(" ");
    printf("%d\n", root->id);

    // Process left child
    print2DUtil(root->left, space);
}

void in_order_walk(node *T_root) {
    if (T_root != nil) {
        in_order_walk(T_root->left);
        printf("%d",T_root->id);
        printf(" - ");
        printf("%s",T_root->text);
        printf("\n");
        //fputs((const char *) T_root->id, stdout);
        //fputs(" ", stdout);
        //fputs((const char *) T_root->text, stdout);
        //fputs(" ", stdout);
        in_order_walk(T_root->right);
    }
}

// Search a node with that id
node *tree_search(node* x, int id){
    if(x==nil || id==x->id ){      //se non c'e oppure � proprio lui
        return x;
    }
    else{ //mod
        if(id < x->id){
            return tree_search(x->left,id);
        }
        else {
            return tree_search(x->right,id);
        }
    }
}



int count_nodes(node *root) {
    int count = 1;
    if (root->left != nil) {
        count += count_nodes(root->left);
    }
    if (root->right != nil) {
        count += count_nodes(root->right);
    }
    return count;
}

/*
node *node_search(node *T_root, char *k) {
    while (T_root != &nil) {
        int cmp = strcmp(k, T_root->key);
        if (cmp == 0) {
            break;
        }
        if (cmp < 0) {
            T_root = T_root->left;
        } else {
            T_root = T_root->right;
        }
    }
    if (T_root == &nil) {
        return &nil;
    } else {
        return T_root;
    }
}*/

void left_rotate(node **T_root, node *x) {
    if ((**T_root).p == nil && x->right != nil) {
        node *y = x->right;
        x->right = y->left;
        if (y->left != nil ) {
            y->left->p = x;
        }
        y->p = x->p;
        if (x->p == nil) {
            *T_root = y;
        }
        else if (x == x->p->left) {
            x->p->left = y;
        }
        else {
            x->p->right = y;
        }
        y->left = x;
        x->p = y;
    }
}

void right_rotate(node **T_root, node *x) {
    if ((**T_root).p == nil && x->left != nil) {
        node *y = x->left;
        x->left = y->right;
        if (y->right != nil ) {
            y->right->p = x;
        }
        y->p = x->p;
        if (x->p == nil ) {
            *T_root = y;
        }
        else if (x == x->p->right) {
            x->p->right = y;
        }
        else {
            x->p->left = y;
        }
        y->right = x;
        x->p = y;
    }
}

void RB_insert(node **T_root, node *z) {
    node *y = nil;
    node *x = *T_root;
    while (x != nil) {
        y = x;
        if (z->id < x->id) {
            x = x->left;
        }
        else {
            x = x->right;
        }
    }
    z->p = y;
    if (y == nil) {
        *T_root = z;
    }
    else if (z->id < y->id) {
        y->left = z;
    }
    else {
        y->right = z;
    }
    z->left = nil;
    z->right = nil;
    z->color = 'r';
    RB_insert_fixup(T_root, z);
}

void RB_insert_fixup(node **T_root, node *z) {
    while (z->p->color == 'r') {
        if (z->p == z->p->p->left) {
            node *y = z->p->p->right;
            if (y->color == 'r') {
                z->p->color = 'b';
                y->color = 'b';
                z->p->p->color = 'r';
                z = z->p->p;
            }
            else {
                if (z == z->p->right) {
                    z = z->p;
                    left_rotate(T_root, z);
                }
                z->p->color = 'b';
                z->p->p->color = 'r';
                right_rotate(T_root, z->p->p);
            }
        }
        else {
            node *y = z->p->p->left;
            if (y->color == 'r') {
                z->p->color = 'b';
                y->color = 'b';
                z->p->p->color = 'r';
                z = z->p->p;
            }
            else {
                if (z == z->p->left) {
                    z = z->p;
                    right_rotate(T_root, z);
                }
                z->p->color = 'b';
                z->p->p->color = 'r';
                left_rotate(T_root, z->p->p);
            }
        }
    }
    (*T_root)->color = 'b';
}

void RB_transplant(node **T_root, node *u, node *v) {
    if (u->p == nil) {
        *T_root = v;
    }
    else if (u == u->p->left) {
        u->p->left = v;
    }
    else {
        u->p->right = v;
    }
    v->p = u->p;
}

node *RB_minimum(node *T_root) {
    while(T_root->left != nil) {
        T_root = T_root->left;
    }
    return T_root;
}

void RB_delete(node **T_root, node *z) {
    if (*T_root != nil) {
        node *x;
        node *y = z;
        char y_original_color = y->color;
        if (z->left == nil) {
            x = z->right;
            RB_transplant(T_root, z, z->right);
        } else if (z->right == nil) {
            x = z->left;
            RB_transplant(T_root, z, z->left);
        } else {
            y = RB_minimum(z->right);
            y_original_color = y->color;
            x = y->right;
            if (y->p == z) {
                x->p = y;
            } else {
                RB_transplant(T_root, y, y->right);
                y->right = z->right;
                y->right->p = y;
            }
            RB_transplant(T_root, z, y);
            y->left = z->left;
            y->left->p = y;
            y->color = z->color;
        }
        if (y_original_color == 'b') {
            RB_delete_fixup(T_root, x);
        }
        if (y != z) {
            y = z;
        }
        free(y->id);
        free(y);
    }
}

void RB_delete_fixup(node **T_root, node *x) {
    while (x != *T_root && x->color == 'b') {
        if (x == x->p->left) {
            node *w = x->p->right;
            if (w->color == 'r') {
                w->color = 'b';
                x->p->color = 'r';
                left_rotate(T_root, x->p);
                w = x->p->right;
            }
            if (w->left->color == 'b' && w->right->color == 'b') {
                w->color = 'r';
                x = x->p;
            }
            else {
                if (w->right->color == 'b') {
                    w->left->color = 'b';
                    w->color = 'r';
                    right_rotate(T_root, w);
                    w = x->p->right;
                }
                w->color = x->p->color;
                x->p->color = 'b';
                w->right->color = 'b';
                left_rotate(T_root, x->p);
                x = *T_root;
            }
        }
        else {
            node *w = x->p->left;
            if (w->color == 'r') {
                w->color = 'b';
                x->p->color = 'r';
                right_rotate(T_root, x->p);
                w = x->p->left;
            }
            if (w->right->color == 'b' && w->left->color == 'b') {
                w->color = 'r';
                x = x->p;
            }
            else {
                if (w->left->color == 'b') {
                    w->right->color = 'b';
                    w->color = 'r';
                    left_rotate(T_root, w);
                    w = x->p->left;
                }
                w->color = x->p->color;
                x->p->color = 'b';
                w->left->color = 'b';
                right_rotate(T_root, x->p);
                x = *T_root;
            }
        }
    }
    x->color = 'b';
}

void empty_tree(node **T_root) {
    while (*T_root != nil) {
        RB_delete(T_root, *T_root);
    }
}

