#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define fp stdin
//#define out stdout

struct node {
    char color;
    int id;
    //char text[1024];
    char *text;
    struct node *p;
    struct node *left;
    struct node *right;
};
typedef struct node node;

node* nil;
node* util;

struct tree {
    node *root;   //testa
    node *nil;
};
typedef struct tree tree;


struct list_node {
    node* root;
    struct list_node* prev;
    struct list_node* next;
};
typedef struct list_node list_node;

char* line; size_t len = 0;


//PROTOTYPES

void in_order_walk(node *T_root);
node *node_search(node *T_root, char *k);
void left_rotate(node **T_root, node *x);
void right_rotate(node **T_root, node *x);
void RB_insert(node **T_root, node *z);
void RB_insert_fixup(node **T_root, node *z);
void RB_transplant(node **T_root, node *u, node *v);
node *RB_minimum(node *T_root);
node *RB_maximum(node *T_root);
void RB_delete(node **T_root, node *z);
void RB_delete_fixup(node **T_root, node *x);
void empty_tree(node **T_root);
//TREE mine
void function_modify(node *iter_old_tree, int ind1, int ind2, node* my_new_root, FILE* fp);
node* function_insert(int ind1, int ind2, node *my_new_root, FILE *fp);
void print_delta(node *T_root, int ind1, int ind2, FILE* out);
int count_nodes(node *root);
node *tree_search(node* x, int id);
void print2DUtil(node *root, int space);
struct node* in_order_successor(node* n);
//LINKED LIST
list_node* insert_in_list(node* tree_node_root, list_node* curr);

int main(){

    // Creation of nil node
    nil = (node*) malloc(sizeof(node));                  // sentinella
    nil->left = nil;
    nil->right = nil;
    nil->p = nil;
    nil->id = -1;
    nil->color = 'b';

    // Pointer at the current node of the linked list containing the roots of the successives modified trees
    list_node* curr = (list_node*)malloc(sizeof(list_node));
    curr->root = nil;
    curr->next = NULL;
    curr->prev = NULL;

    //for testing
    FILE *fp = fopen("C:/Users/feder/CLionProjects/ed-con-Undo-multipli/test.txt","r");
    FILE* out = fopen("out.txt","w");
    if(fp==NULL){
        printf("Error opening file");
        exit(0);
    }

    int ind1=0, ind2=0, times=0;
    char command;
    int chars = getline(&line,&len,fp);


    /*fseek(stdin, 0, SEEK_END);      // seek to end of file
    int file_size = ftell(stdin);   // get current file pointer
    fseek(stdin, 0, SEEK_SET);      // seek back to beginning of file
    // proceed with allocating memory and reading the file
    memcpy()*/

    //line = fgets(line,1024,fp);

    //line = malloc(1024);
    while(chars!=-1){
    //while(!feof(fp)) {
    //while(fgets(line,1024,fp)){
        //fgets(line,1024,fp);
        //la ho gia     line = strtok(line,"\n");
        line[strcspn(line, "\n\r")] = '\0';
        command = line[strlen(line)-1];

        switch(command){

            case 'q':{
                exit(0);
            }

            case '.':{
                break;
            }

            case 'c': {

                int ind[2] = {0, 0}, i = 0;
                char *splitted_line = strtok(line, ",");
                while (splitted_line != NULL) {
                    ind[i] = (int) strtol(splitted_line, (char **) NULL, 10);
                    splitted_line = strtok(NULL, ",");
                    i++;
                }

                ind1 = ind[0];
                ind2 = ind[1];


                // Creation of the new tree
                struct tree* new_tree = malloc(sizeof(struct tree*));
                if(!new_tree){
                    fprintf(stderr,"\nError tree\n");
                    exit(0);
                }
                new_tree->root = malloc(sizeof(node));
                new_tree->nil = nil;
                if(curr->root!=nil){
                    new_tree->root->left = curr->root->left;
                    new_tree->root->right = curr->root->right;
                    new_tree->root->p = curr->root->p;
                    new_tree->root->id = curr->root->id;
                    new_tree->root->text = malloc((strlen(line)+1)*sizeof(char));
                    strcpy(new_tree->root->text, curr->root->text);
                }
                else{
                    new_tree->root = nil;
                }

                //modifica
                if(tree_search(curr->root,ind1)!=nil)
                    function_modify(curr->root,ind1,ind2,new_tree->root,fp);

                //inserimento
                else
                    new_tree->root = function_insert(ind1,ind2,new_tree->root,fp);

                in_order_walk(new_tree->root);

                curr = insert_in_list(new_tree->root,curr);
                if(curr->next!=NULL){
                    curr = curr->next;
                }

                //to avoid processing of "."
                chars = getline(&line,&len,fp);
                line[strcspn(line, "\n\r")] = '\0';

                break;
            }

            /*case 'c':{

                int ind[2]={0,0}, i=0;
                char* splitted_line = strtok(line,",");
                while (splitted_line!= NULL){
                    ind[i]= (int) strtol(splitted_line, (char **)NULL, 10);
                    splitted_line = strtok (NULL, ",");
                    i++;
                }

                ind1 = ind[0];
                ind2 = ind[1];
                //printf("\nqui\n\nind1: %d\nind2: %d\ncommand: %c\n\n\n",ind1,ind2,command);

                // Creation of the new tree
                struct tree* new_tree = malloc(sizeof(struct tree*));
                if(!new_tree){
                    fprintf(stderr,"\nError tree\n");
                    exit(0);
                }

                // Copy the old root
                new_tree->root = malloc(sizeof(node));
                new_tree->root->left = new_tree->root->right = new_tree->root->p = nil;
                new_tree->root->id = 0;

                i = 1;
                if(curr->root!=nil){
                    //node* x = malloc(sizeof(node));
                    while( i!=ind1 ){
                        node* to_be_copied = tree_search(curr->root,i);
                        node* x = malloc(sizeof(node));
                        x->id = to_be_copied->id;
                        x->text = malloc((strlen(to_be_copied->text)+1)*sizeof(char));
                        strcpy(x->text,to_be_copied->text);
                        x->p = x->right = x->left = nil;
                        RB_insert(&new_tree->root,x);
                        free(x);
                        i++;
                    }
                }

                int dont_re_add = 0;

                while(ind1!=ind2+1){

                    fgets(line,1024,fp);
                    //getline(&line,&len,fp);
                    line[strcspn(line, "\n\r")] = '\0';

                    // se non c'è, devo inserirlo
                    if(tree_search(curr->root,ind1)==nil){
                        dont_re_add = 1;
                        node* x = malloc(sizeof(node));
                        x->p = x->right = x->left = nil;
                        x->id = ind1;
                        x->text = (char*)malloc((strlen(line)+1)*sizeof(char));
                        strcpy(x->text,line);
                        RB_insert(&new_tree->root,x);
                        //free(x);
                    }
                    // se c'è, lo modifico
                    else{
                        node* to_be_modified = tree_search(curr->root,ind1);
                        node* x = (node*)malloc(sizeof(node));
                        x->p = x->right = x->left = nil;
                        x->id = ind1;
                        x->text = (char*)malloc((strlen(line)+1)*sizeof(char));
                        strcpy(x->text,line);
                        RB_insert(&new_tree->root,x);
                    }

                    ind1++;
                }

                // riaggiungo i nodi mancanti
                if(dont_re_add==0){
                    i = ind2+1;
                    if(curr->root!=nil){
                        while(i!=count_nodes(curr->root) ){//-1
                            node* to_be_copied = tree_search(curr->root,i);
                            node* x = (node*)malloc(sizeof(node));
                            x->id = to_be_copied->id;
                            x->text = (char*)malloc((strlen(to_be_copied->text)+1)*sizeof(char));
                            strcpy(x->text,to_be_copied->text);
                            x->p = x->right = x->left = nil;
                            RB_insert(&new_tree->root,x);
                            i++;
                        }
                    }
                }
                //list_node* new_list_node = (list_node*)malloc(sizeof(list_node));
                //new_list_node->root = new_tree->root;

                insert_in_list(new_tree->root,curr);

                if(curr->next!=NULL){
                    curr = curr->next;
                }

                break;
            }*/

            case 'd':{

                int ind[2]={0,0}, i=0;
                char* splitted_line = strtok (line,",");
                while (splitted_line!= NULL){
                    ind[i] = (int) strtol(splitted_line, (char **)NULL, 10);
                    splitted_line = strtok (NULL, ",");
                    i++;
                }

                ind1 = ind[0];
                ind2 = ind[1];
                int delta = ind2 - ind1 +1;

                // Creation of the new tree
                struct tree* new_tree = malloc(sizeof(struct tree*));
                if(!new_tree){
                    fprintf(stderr,"\nError tree\n");
                    exit(0);
                }
                // Copy the old tree
                new_tree->root = (node*)malloc(sizeof(node));
                new_tree->root->left = new_tree->root->right = new_tree->root->p = nil;
                new_tree->root->id = 0;

                //non deve fare niente ma deve essere considerata per le undo e redo)
                if(ind1==0 && ind2==0){
                    break;
                }

                i = 1;
                if(curr->root!=nil){
                    while( i!=ind1 ){
                        node* to_be_copied = tree_search(curr->root,i);
                        node* x = (node*)malloc(sizeof(node));
                        x->id = to_be_copied->id;
                        x->text = (char*)malloc((strlen(to_be_copied->text)+1)*sizeof(char));
                        strcpy(x->text,to_be_copied->text);
                        x->p = x->right = x->left = nil;
                        RB_insert(&new_tree->root,x);
                        i++;
                    }
                }

                i = ind2+1;
                if(curr->root!=nil){
                    while(i<=count_nodes(curr->root)-1){
                        node* to_be_copied = tree_search(curr->root,i);
                        node* x = (node*)malloc(sizeof(node));
                        x->id = to_be_copied->id - delta;
                        x->text = (char*)malloc((strlen(to_be_copied->text)+1)*sizeof(char));
                        strcpy(x->text,to_be_copied->text);
                        x->p = x->right = x->left = nil;
                        RB_insert(&new_tree->root,x);
                        i++;
                    }
                }

                /*if(this_node!=nil) {
                    while (delta > 0) {
                        this_node = tree_search(new_tree->root,ind1);
                        RB_delete(&new_tree->root,this_node);
                        delta--;
                        ind1_aux = ind1+1;
                        node* node_to_decrement = tree_search(new_tree->root,ind1_aux);
                        while(node_to_decrement!=nil){
                            node_to_decrement->id--;
                            ind1_aux++;
                            node_to_decrement = tree_search(new_tree->root,ind1_aux);  //oppure cerco il next
                        }
                        //ind1++;
                    }
                }*/

                // List handling
                insert_in_list(new_tree->root,curr);

                if(curr->next!=NULL){
                    curr = curr->next;
                }
                break;
            }

            case 'p':{

                int ind[2]={0,0}, i=0;
                char* splitted_line = strtok (line,",");
                while (splitted_line!= NULL){
                    ind[i]= (int) strtol(splitted_line, (char **)NULL, 10);
                    splitted_line = strtok (NULL, ",");
                    i++;
                }
                ind1 = ind[0];
                ind2 = ind[1];

                if(ind1==0 && ind2==0){
                    fputs(".\n",out);
                    break;
                }
                // GRAPHIC
                //puts("2D representation:\n");
                //print2DUtil(curr->root, 0);
                //puts("\n");

                // PRINT
                //puts("\ninorder print:");
                //in_order_walk(tree->root);

                print_delta(curr->root,ind1,ind2,out);
                //print_delta(curr->root,ind1,ind2,out);

                break;
            }

            case 'u':{

                // Get the number of undo times by removing the last character (the command)
                line[strlen(line)-1] = '\0';
                times = (int) strtol(line, (char **)NULL, 10);                //times = atoi(line);

                while(times>0){
                    if(curr->prev!=NULL){
                        curr = curr->prev;
                    }
                    else break;
                    times--;
                }
                break;
            }

            case 'r':{

                // Get the number of redo times by removing the last character (the command)
                line[strlen(line)-1] = '\0';
                times = (int) strtol(line, (char **)NULL, 10);                //times = atoi(line);
                while(times>0){
                    if(curr->next!=NULL){
                        curr = curr->next;
                    }
                    else break;
                    times--;
                }
                break;
            }

            default: break;

        }

        chars = getline(&line,&len,fp);
    }
    fclose(fp);
    free(line);
    return 0;
}

//IMPLEMENTATIONS



//iter_old_tree: testa albero vecchio
//my_new_root: testa albero nuovo
void function_modify(node *iter_old_tree, int ind1, int ind2, node* my_new_root, FILE* fp) {

    if(iter_old_tree == nil){
        my_new_root = function_insert(ind1,ind2,my_new_root,fp);
        return;
    }

    //target is on left, i have to copy the journey from here
    if(ind1 < iter_old_tree->id){
        node* journey_copy = malloc(sizeof(node));
        //   no    journey_copy->p = iter_old_tree->left->p;
        if(iter_old_tree->left==nil){
            journey_copy->left = nil;
            journey_copy->right = nil;
            journey_copy->id = ind1;
            //journey_copy->text = malloc((strlen("aaa") + 1) * sizeof(char));
            //strcpy(journey_copy->text, "temp");
        }
        else {
            journey_copy->left = iter_old_tree->left->left;
            journey_copy->right = iter_old_tree->left->right;
            journey_copy->id = iter_old_tree->left->id;
            journey_copy->text = malloc((strlen(iter_old_tree->left->text) + 1) * sizeof(char));
            strcpy(journey_copy->text, iter_old_tree->left->text);
        }
        journey_copy->p = my_new_root;
        my_new_root->left = journey_copy;

        function_modify(iter_old_tree->left, ind1, ind2, my_new_root->left,fp);
        return;
    }
    //target is on right, i have to copy the journey from here
    else if(ind1 > iter_old_tree->id){
        node* journey_copy = malloc(sizeof(node));
        //if not exists the node , insert
        if(iter_old_tree->right==nil){
            journey_copy->left = nil;
            journey_copy->right = nil;
            journey_copy->id = ind1;
            //journey_copy->text = malloc((strlen("aaa") + 1) * sizeof(char));
            //strcpy(journey_copy->text, "temp");
        }
        else{
            journey_copy->left = iter_old_tree->right->left;
            journey_copy->right = iter_old_tree->right->right;
            journey_copy->id = iter_old_tree->right->id;
            journey_copy->text = malloc((strlen(iter_old_tree->right->text) + 1) * sizeof(char));
            strcpy(journey_copy->text, iter_old_tree->right->text);
        }
        journey_copy->p = my_new_root;
        my_new_root->right = journey_copy;

        function_modify(iter_old_tree->right, ind1, ind2, my_new_root->right,fp);
        return;
    }
    //i am on the target (my_new_root);
    else if(ind1 == iter_old_tree->id){
        free(my_new_root->text);
        getline(&line,&len,fp);
        line[strcspn(line,"\n\r")] = '\0';
        my_new_root->text = malloc((strlen(line)+1) * sizeof(char));
        strcpy(my_new_root->text,line);

        ind1++;
        if(ind1<=ind2){
        //while(ind1<=ind2){
            function_modify(iter_old_tree,ind1,ind2,my_new_root,fp);
        }
        else return;
    }

}


node* function_insert(int ind1, int ind2, node *my_new_root, FILE *fp) {

    while(ind1!=ind2+1){
        node* new_node_to_insert = malloc(sizeof(node));
        new_node_to_insert->left = new_node_to_insert->right = new_node_to_insert->p = nil;
        new_node_to_insert->id = ind1;
        getline(&line,&len,fp);
        line[strcspn(line,"\n\r")] = '\0';
        //free(my_new_root->text);
        new_node_to_insert->text = malloc((strlen(line)+1) * sizeof(char));
        strcpy(new_node_to_insert->text, line);
        RB_insert(&my_new_root,new_node_to_insert);
        ind1++;
    }

    return my_new_root;
}


struct node* in_order_successor(node* n){
    if (n->right != nil)
        return RB_minimum(n->right);

    node* p = n->p;
    while (p != nil && n == p->right) {
        n = p;
        p = p->p;
    }
    return p;
}

struct node* in_order_predecessor(node* n){
    // ?
    if (n->left != nil)
        return RB_maximum(n->left);

    node* p = n->p;
    while (p != nil && n == p->right) {
        n = p;
        p = p->p;
    }
    return p;
}

//return the last list_node node
list_node* insert_in_list(node* tree_node_root, list_node* curr) {

    list_node *t = (list_node*)malloc(sizeof(list_node));
    t->root = tree_node_root;

    // if it's empty
    if (curr->root == nil) { //doubt
        curr = t;
        curr->next = NULL;
        curr->prev = NULL;
        return t;
    }

    list_node* temp = curr;

    while (temp->next != NULL)
        temp = temp->next;

    temp->next = t;
    t->prev = temp;
    t->next = NULL;

    return t;
}

void print_delta(node *T_root, int ind1, int ind2, FILE* out){
//void print_delta(node *T_root, int ind1, int ind2, FILE* out){
    while(ind1!=ind2+1) {
        if(tree_search(T_root, ind1)!=nil) {
            //printf("%s\n",tree_search(T_root, ind1)->text);
            //puts(tree_search(T_root, ind1)->text);
            fputs(tree_search(T_root, ind1)->text,out);
            fputs("\n",out);
            ind1++;
        }
        else{
            //puts(".");
            fputs(".\n",out);
            ind1++;
        }
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
    printf("%d - %s\n", root->id, root->text);

    // Process left child
    print2DUtil(root->left, space);
}

void in_order_walk(node *T_root) {
    if (T_root != nil && T_root->id>0) {
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
        count += count_nodes(root->left );
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

node *RB_maximum(node *T_root) {
    while(T_root->right != nil) {
        T_root = T_root->right;
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
        //free(y->id);
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

