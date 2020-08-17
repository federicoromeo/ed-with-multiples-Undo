#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define fp stdin
#define out stdout

struct node {
    char color;
    int id;
    char *text;
    struct node *p;
    struct node *left;
    struct node *right;
};
typedef struct node node;

node* nil;

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
void print_delta(node *T_root, int ind1, int ind2/*, FILE* out*/);
int count_nodes(node *root);
node *tree_search(node* x, int id);
void print2DUtil(node *root, int space);
struct node* in_order_successor(node* n);
//LINKED LIST
list_node* insert_in_list(node* tree_node_root, list_node* ptr_to_the_last_list_node);




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
    char* line = NULL; size_t len = 1024;
    FILE *fp = fopen("C:/Users/feder/CLionProjects/ed-con-Undo-multipli/test.txt","r");
    //FILE* out = fopen("out.txt","w");
    if(fp==NULL){
        printf("Error opening file");
        exit(0);
    }

    int ind1=0, ind2=0, times=0, delta=0, actual_id=ind1;
    char command;
    int chars = getline(&line,&len,fp);

    while(chars!=-1) {

        line[strcspn(line, "\n\r")] = '\0';
        command = line[strlen(line)-1];

        switch(command){

            case 'q':{
                exit(0);
            }

            case '.':{
                break;
            }

            case 'c':{

                int ind[2]={0,0}, i=0;
                char* splitted_line = strtok (line,",");
                while (splitted_line!= NULL){
                    ind[i]= (int) strtol(splitted_line, (char **)NULL, 10);
                    splitted_line = strtok (NULL, ",");
                    i++;
                }

                ind1 = ind[0];
                ind2 = ind[1];
                delta = ind2 - ind1 +1;
                //printf("\nqui\n\nind1: %d\nind2: %d\ncommand: %c\n\n\n",ind1,ind2,command);

                // Creation of the new tree
                struct tree* new_tree = malloc(sizeof(struct tree*));
                if(!new_tree){
                    fprintf(stderr,"\nError tree\n");
                    exit(0);
                }
                // Copy the old tree

                new_tree->root = (node*)malloc(sizeof(node));
                new_tree->root->p = curr->root->p;
                new_tree->root->left = curr->root->left;
                new_tree->root->right = curr->root->right;
                new_tree->root->id = curr->root->id;
                new_tree->root->text = curr->root->text;

                node* this_node = tree_search(new_tree->root,ind1);

                // se l'albero è vuoto E ind1=1:
                if( this_node==nil && count_nodes(curr->root)<=1 && ind1==1 ){

                    while(delta>0){

                        getline(&line,&len,fp);
                        line[strcspn(line, "\n")] = '\0';

                        node* x = (node*)malloc(sizeof(node));
                        x->text = malloc((strlen(line)+1)*sizeof(char));
                        x->id = ind1;
                        strcpy(x->text,line);
                        x->right=x->left=x->p=nil;
                        RB_insert(&new_tree->root,x);
                        delta--;
                        ind1++;
                    }
                }
                    //se ind1 è presente
                else if(this_node!=nil && ind1<=count_nodes(new_tree->root)) {
                    actual_id = ind1;
                    while (delta > 0) {

                        getline(&line,&len,fp);
                        //fgets(line, 1024, stdin);
                        line[strcspn(line, "\n")] = '\0';

                        //ottimizzaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
                        if(tree_search(new_tree->root,ind1)!=nil) {

                            node* that_node_to_modify = tree_search(new_tree->root,ind1);

                            //ottimizzaaaaaaaaaaaaaaaaaaaaaaaaaaaaa

                            // QUI
                            // copio tutti i nodi fino a arrivare a quelli da modificare partendo dalla root
                            node* x = (node*)malloc(sizeof(node));
                            x->text = (char*) malloc((strlen(line)+1)*sizeof(char));
                            x->id = that_node_to_modify->id;
                            strcpy(x->text,line);
                            x->right = that_node_to_modify->right;
                            x->left = that_node_to_modify->left;
                            x->p = that_node_to_modify->p;
                            in_order_successor(that_node_to_modify->p,x);
                            //RB_insert(&new_tree->root,x);

/*                            node* that_node_to_modify = tree_search(new_tree->root,ind1);
                            free(that_node_to_modify->text);

                            that_node_to_modify->text = (char*) malloc((strlen(line)+1)*(sizeof(char)));
                            strcpy(that_node_to_modify->text, line);
*/                       delta--;
                            ind1++;
                            actual_id = ind1;
                        }
                        else{
                            node* x = (node*)malloc(sizeof(node));
                            x->text = (char*) malloc((strlen(line)+1)*sizeof(char));
                            x->id = actual_id;
                            strcpy(x->text,line);
                            x->right=x->left=x->p=nil;
                            RB_insert(&new_tree->root,x);
                            delta--;
                            ind1++;
                            actual_id = ind1;
                        }
                    }
                }
                    //altrimenti se non c'è proprio
                else if(ind1==count_nodes(new_tree->root)+1){
                    while (delta > 0) {

                        actual_id = ind1;
                        getline(&line,&len,fp);
                        line[strcspn(line, "\n")] = '\0';

                        node *x = (node *) malloc(sizeof(node));
                        x->text = (char*)malloc((strlen(line) + 1) * (sizeof(char)));
                        x->id = actual_id;
                        strcpy(x->text, line);
                        x->right = x->left = x->p = nil;
                        RB_insert(&new_tree->root, x);
                        delta--;
                        ind1++;
                        actual_id = ind1;
                    }
                }

                // List handling
                insert_in_list(new_tree->root,curr);

                while(curr->next!=NULL){
                    curr = curr->next;
                }

                //in_order_walk(curr->root);

                break;
            }

            case 'd':{

                int ind[2]={0,0}, i=0;
                char* splitted_line = strtok (line,",");
                while (splitted_line!= NULL){
                    ind[i]= (int) strtol(splitted_line, (char **)NULL, 10);
                    splitted_line = strtok (NULL, ",");
                    i++;
                }

                ind1 = ind[0];
                ind2 = ind[1];
                int ind1_aux = ind1 + 1;
                delta = ind2 - ind1 + 1;

                // Creation of the new tree
                struct tree* new_tree = malloc(sizeof(struct tree*));
                if(!new_tree){
                    fprintf(stderr,"\nError tree\n");
                    exit(0);
                }
                // Copy the old tree
                new_tree->root = (node*)malloc(sizeof(node));
                new_tree->root->p = curr->root->p;
                new_tree->root->left = curr->root->left;
                new_tree->root->right = curr->root->right;
                new_tree->root->id = curr->root->id;
                new_tree->root->text = curr->root->text;

                //DELETE:

                //non deve fare niente ma deve essere considerata per le undo e redo)
                if(ind1==0 && ind2==0){
                    break;
                }

                node* this_node = tree_search(new_tree->root,ind1);

                if(this_node!=nil) {
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
                }

                // List handling
                insert_in_list(new_tree->root,curr);

                while(curr->next!=NULL){
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

                //PRIMA: print_delta(tree->root,ind1,ind2/*,out*/);
                print_delta(curr->root,ind1,ind2/*,out*/);

                break;
            }

            case 'u':{

                // Get the number of undo times by removing the last character (the command)
                line[strlen(line)-1] = '\0';
                times = (int) strtol(line, (char **)NULL, 10);                //times = atoi(line);

                while(times>0){
                    curr = curr->prev;
                    times--;
                }
                break;
            }

            case 'r':{

                // Get the number of redo times by removing the last character (the command)
                line[strlen(line)-1] = '\0';
                times = (int) strtol(line, (char **)NULL, 10);                //times = atoi(line);
                while(times>0){
                    curr = curr->next;
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


/*
//STACK
stack_node* new_node(char* command, char* text){
    stack_node* stackNode = (stack_node*)malloc(sizeof(stack_node));
    strcpy(stackNode->command , command);
    strcpy(stackNode->texts , text);
    stackNode->next = NULL;
    return stackNode;
}

int is_empty(stack_node* root){
    return !root;
}

stack_node* push(stack_node** root, char* command, char* text){
    stack_node * stackNode = new_node(command,text);
    stackNode->next = *root;
    *root = stackNode;
    return stackNode;
}

void pop(stack_node** root){
    if (is_empty(*root))
        return;
    stack_node* temp = *root;
    *root = (*root)->next;
    free(temp);
}

void append(stack_node* this_stack_node, char* line){
    char* aux = strcat(this_stack_node->texts,"\n");
    strcpy(this_stack_node->texts, strcat(aux,line));
}


void print_inorder_stack(stack_node* root){
    stack_node* this = root;
    while(this!=NULL){
        puts("command_line:");
        puts(root->command);
        puts("text:");
        puts(root->texts);
        puts("");
        this = this->next;
    }
}

int peek(stack_node* root){
    if (isEmpty(root))
        return -1;
    return root->data;
}*/


//IMPLEMENTATIONS

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

list_node* insert_in_list(node* tree_node_root, list_node* ptr_to_the_last_list_node) {

    list_node *t = (list_node*)malloc(sizeof(list_node));
    t->root = tree_node_root;

    // if it's empty
    if (ptr_to_the_last_list_node == NULL) { //doubt
        ptr_to_the_last_list_node = t;
        ptr_to_the_last_list_node->next = NULL;
        ptr_to_the_last_list_node->prev = NULL;
        return t;
    }

    list_node* temp = ptr_to_the_last_list_node;

    while (temp->next != NULL)
        temp = temp->next;

    temp->next = t;
    t->prev = temp;
    t->next = NULL;

    return t;
}

void print_delta(node *T_root, int ind1, int ind2/*, FILE* out*/){
    int delta = ind2 - ind1 + 1;
    while(delta>0) {
        if(tree_search(T_root, ind1)!=nil) {
            //printf("%s\n",tree_search(T_root, ind1)->text);
            //puts(tree_search(T_root, ind1)->text);
            fputs(tree_search(T_root, ind1)->text,out);
            fputs("\n",out);
            delta--;
            ind1++;
        }
        else{
            //puts(".");
            fputs(".\n",out);
            //printf(".\n");
            delta--;
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

