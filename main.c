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

struct tree {
    node *root;   //testa
};
typedef struct tree tree;

node* nil;
node* empty;


struct list_node {
    node* root;
    struct list_node* prev;
    struct list_node* next;
};
typedef struct list_node list_node;

list_node *zero;


struct list_write_only{
    int ind1,ind2;
    char* command_line;
    char* text_lines;
    char** divided_text;
    struct list_write_only* next;
    struct list_write_only* prev;
};
typedef struct list_write_only list_write_only;

list_write_only* last;


size_t len = 0;
char *line;
char** texttt;



//PROTOTYPES

// RB bst
void in_order_walk(node *T_root);
void left_rotate(node **T_root, node *x);
void right_rotate(node **T_root, node *x);
void RB_insert(node **T_root, node *z);
void RB_insert_fixup(node **T_root, node *z);
void RB_transplant(node **T_root, node *u, node *v);
node *RB_minimum(node *T_root);
void RB_delete(node **T_root, node *z);
void RB_delete_fixup(node **T_root, node *x);
int count_nodes(node *root);
node *tree_search(node* x, int id);
void print2DUtil(node *root, int space);
struct node* in_order_successor(node* n);
// Insert e Modify functions
void function_modify(node *iter_old_tree, tree* old_tree, int ind1, list_write_only* current, int ind2, node* my_new_root, tree*);
node* function_insert(int ind1, int ind2, list_write_only* current, node *my_new_root);
node* function_insert_1(int ind1, int ind2, list_write_only* current, node *my_new_root);
void print_delta(node *T_root, int ind1, int ind2);
// Linked List
list_node* insert_in_list(node* tree_node_root, list_node* curr);
list_write_only *insert_in_list_wo(list_write_only *new);


int main(){

    int count = 1;

    // Creation of nil node
    nil = malloc(sizeof(node));                  // sentinella
    nil->left = nil;
    nil->right = nil;
    nil->p = nil;
    nil->id = -1;
    nil->color = 'b';

    // Creation of empty node just for the first move
    empty = malloc(sizeof(node));
    empty->left = nil;
    empty->right = nil;
    empty->p = nil;
    empty->id = -2;
    empty->color = 'b';

    // Pointer at the current node of the linked list containing the roots of the successives modified trees
    list_node* curr = malloc(sizeof(list_node));
    curr->root = empty;
    curr->next = NULL;
    curr->prev = NULL;

    // Pointer to the list node number 0, used when i undo every action
    zero = malloc(sizeof(list_node));
    zero->next = curr;
    zero->prev = NULL;

    // Pointer to the last node in writeOnly case
    last = NULL;


    char command;
    int length = 0, max_ind = 0, prev_ind2 = 0, write_only = 1;

    int chars = getline(&line,&len,stdin);
    line[strcspn(line, "\n\r")] = '\0';

    // HERE I SAVE EVERY COMMAND GIVEN
    while(chars!=-1){

        command = line[strlen(line)-1];

        if(command=='c'){
            list_write_only* new = malloc(sizeof(list_write_only));
            new->divided_text = NULL;
            new->command_line = malloc((strlen(line)+1)*sizeof(char));
            strcpy(new->command_line,line);
            new->text_lines = malloc(sizeof(char));
            strcpy(new->text_lines,"");
            //get addresses
            int ind[2] = {0, 0}, i = 0;
            char *splitted_line = strtok(line, ",");
            while (splitted_line != NULL) {
                ind[i] = (int) strtol(splitted_line, (char **) NULL, 10);
                splitted_line = strtok(NULL, ",");
                i++;
            }
            new->ind1 = ind[0];
            new->ind2 = ind[1];
            if(prev_ind2 >= new->ind1){
                write_only = 0;
            }
            prev_ind2 = new->ind2;

            for(i=new->ind1; i<=new->ind2; i++){
                getline(&line,&len,stdin);
                length++;
                size_t first_len = strlen(new->text_lines);
                size_t second_len = strlen(line);
                new->text_lines = realloc(new->text_lines, first_len+second_len+1);
                strcpy(new->text_lines + first_len, line);
            }
            new->next = NULL;
            last = insert_in_list_wo(new);
            chars = getline(&line,&len,stdin);
        }
        else if(command=='p'){
            list_write_only* new = malloc(sizeof(list_write_only));
            new->divided_text = NULL;
            new->command_line = malloc((strlen(line)+1)*sizeof(char));
            strcpy(new->command_line,line);
            //get addresses
            int ind[2] = {0, 0}, i = 0;
            char *splitted_line = strtok(line, ",");
            while (splitted_line != NULL) {
                ind[i] = (int) strtol(splitted_line, (char **) NULL, 10);
                splitted_line = strtok(NULL, ",");
                i++;
            }
            new->ind1 = ind[0];
            new->ind2 = ind[1];
            new->text_lines = NULL;

            last = insert_in_list_wo(new);
        }
        else if(command=='d'){
            write_only = 0;
            list_write_only* new = malloc(sizeof(list_write_only));
            new->divided_text = NULL;
            new->command_line = malloc((strlen(line)+1)*sizeof(char));
            strcpy(new->command_line,line);
            //get addresses
            int ind[2] = {0, 0}, i = 0;
            char *splitted_line = strtok(line, ",");
            while (splitted_line != NULL) {
                ind[i] = (int) strtol(splitted_line, (char **) NULL, 10);
                splitted_line = strtok(NULL, ",");
                i++;
            }
            new->ind1 = ind[0];
            new->ind2 = ind[1];
            new->text_lines = NULL;

            last = insert_in_list_wo(new);
        }
        else if(command=='q') {
            list_write_only* new = malloc(sizeof(list_write_only));
            new->divided_text = NULL;
            new->command_line = malloc((strlen(line)+1)*sizeof(char));
            strcpy(new->command_line,line);

            last = insert_in_list_wo(new);
        }
        else if(command=='u' || command=='r'){
            write_only = 0;
            list_write_only* new = malloc(sizeof(list_write_only));
            new->divided_text = NULL;
            new->command_line = malloc((strlen(line)+1)*sizeof(char));
            strcpy(new->command_line,line);
            line[strlen(line)-1] = '\0';
            new->ind1 = (int) strtol(line, (char **)NULL, 10);

            last = insert_in_list_wo(new);
        }

        chars = getline(&line,&len,stdin);
        line[strcspn(line, "\n\r")] = '\0';
    }


    // HANDLING WRITE ONLY
    if(write_only==1){

        texttt = malloc((length+1)*sizeof(char*));
        list_write_only* current = NULL;
        list_write_only* temp = last;

        //current will point to the first node where i have all the commands saved
        while(temp->prev!=NULL) {
            current = temp->prev;
            temp = temp->prev;
        }
        command = current->command_line[strlen(current->command_line)-1];

        while(1){

            switch (command) {

                case 'c': {
                    int pos = current->ind1;
                    char *splitted_text = strtok(current->text_lines, "\n");
                    while (splitted_text != NULL) {
                        texttt[pos - 1] = malloc((strlen(splitted_text) + 1) * sizeof(char));
                        texttt[pos - 1] = splitted_text;
                        texttt[pos] = NULL;
                        splitted_text = strtok(NULL, "\n");
                        pos++;
                    }
                    max_ind = current->ind2;
                    break;
                }

                case 'p': {
                    if(current->ind1==0 && current->ind2==0){
                        fputs(".\n", stdout);
                        break;
                    }
                    if(current->ind1==0) current->ind1++;
                    for (int j = current->ind1; j <= current->ind2; j++) {
                        if (current->ind1>max_ind || texttt[j - 1] == NULL) {
                            while (j <= current->ind2) {
                                fputs(".\n", stdout);
                                j++;
                            }
                        }
                        else {
                            fputs(texttt[j - 1], stdout);
                            fputs("\n", stdout);
                        }
                    }
                    break;
                }

                case 'q': {
                    exit(0);
                }

                default:
                    break;
            }

            if (current->next != NULL) //redundant
                current = current->next;
            command = current->command_line[strlen(current->command_line) - 1];
        }
        exit(0); //redundant
    }

    // ALL OTHER CASES
    else{

        list_write_only* current = NULL;
        list_write_only* temp = last;
        //current will point to the first node where i have all the commands saved
        while(temp->prev!=NULL) {
            current = temp->prev;
            temp = temp->prev;
        }
        command = current->command_line[strlen(current->command_line)-1];

        while(1) {

            switch (command) {

                case 'c': {

                    // Split the lines inside current node dynamic array
                    char *splitted_text = strtok(current->text_lines, "\n");
                    int number_of_lines = current->ind2 - current->ind1 + 1;
                    int pos = 1;
                    current->divided_text = malloc((number_of_lines+1)*sizeof(char*));

                    while (splitted_text != NULL) {
                        current->divided_text[pos-1] = malloc((strlen(splitted_text) + 1) * sizeof(char));
                        current->divided_text[pos-1] = splitted_text;
                        current->divided_text[pos] = NULL;
                        splitted_text = strtok(NULL, "\n");
                        pos++;
                    }

                    struct tree* new_tree = malloc(sizeof(struct tree*));
                    if(!new_tree){
                        fprintf(stderr,"\nError tree\n"); //todo:tolgo
                        exit(0);
                    }
                    // Create a tree to be passed as old tree
                    struct tree* old_tree = malloc(sizeof(struct tree*));
                    if(!old_tree){
                        fprintf(stderr,"\nError old tree\n"); //todo: tolgo
                        exit(0);
                    }

                    old_tree->root = curr->root;

                    if(curr->root!=empty && curr->root!=nil){
                        new_tree->root = malloc(sizeof(node));
                        new_tree->root->left = curr->root->left;
                        new_tree->root->right = curr->root->right;
                        new_tree->root->p = curr->root->p;
                        new_tree->root->id = curr->root->id;
                        new_tree->root->text = curr->root->text;
                    }
                    else{
                        new_tree->root = nil;
                    }

                    int num = count_nodes(curr->root);
                    if (curr->root == nil) num--;

                    //modifica
                    if(tree_search(curr->root,current->ind1)!=nil)
                        function_modify(curr->root,old_tree,current->ind1,current,current->ind2,new_tree->root,new_tree);

                    //inserimento
                    else
                        if(count==1 || num==0) //empty
                            new_tree->root = function_insert_1(current->ind1,current->ind2,current,new_tree->root);
                        else
                            function_modify(curr->root,old_tree,current->ind1,current,current->ind2,new_tree->root,new_tree);

                    count++;

                    curr = insert_in_list(new_tree->root,curr);
                    //print2DUtil(new_tree->root,0);

                    //to avoid processing of "."
                    chars = getline(&line,&len,stdin);
                    line[strcspn(line, "\n\r")] = '\0';

                    break;
                }

                case 'd':{

                    // Creation of the new tree
                    struct tree* new_tree = malloc(sizeof(struct tree*));
                    if(!new_tree){
                        fprintf(stderr,"\nError tree\n");
                        exit(0);
                    }
                    // Copy the old tree
                    new_tree->root = malloc(sizeof(node));
                    new_tree->root->left = new_tree->root->right = new_tree->root->p = nil;
                    new_tree->root->id = 0;

                    if(current->ind1==0 && current->ind2==0)
                        break;

                    if(current->ind1==0)
                        current->ind1++;

                    int delta = current->ind2 - current->ind1 + 1;

                    RB_delete(&new_tree->root,tree_search(new_tree->root,0));

                    int i = 1;
                    if(curr->root!=nil){
                        while( i!=current->ind1 ){
                            node* to_be_copied = tree_search(curr->root,i);
                            node* x = malloc(sizeof(node));
                            x->id = to_be_copied->id;
                            x->text = to_be_copied->text;
                            x->p = x->right = x->left = nil;
                            RB_insert(&new_tree->root,x);
                            i++;
                        }
                    }
                    i = current->ind2+1;
                    if(curr->root!=nil){
                        while(i<=count_nodes(curr->root)){
                            node* to_be_copied = tree_search(curr->root,i);
                            if(to_be_copied == nil)
                                break;
                            node* x = malloc(sizeof(node));
                            x->id = to_be_copied->id - delta;
                            x->text = to_be_copied->text;
                            x->p = x->right = x->left = nil;
                            RB_insert(&new_tree->root,x);
                            i++;
                        }
                    }

                    // List handling
                    curr = insert_in_list(new_tree->root,curr);

                    break;
                }

                case 'p':{

                    if(current->ind1==0 && current->ind2==0){
                        fputs(".\n", stdout);
                        break;
                    }

                    print_delta(curr->root,current->ind1,current->ind2);
                    //puts("2D representation:\n");
                    //print2DUtil(curr->root, 0);
                    //puts("\n");

                    break;
                }

                case 'u':{

                    int times = current->ind1;

                    while (times > 0) {
                        if (curr->prev != NULL) {
                            curr = curr->prev;
                        }
                        else {
                            zero->root = nil;
                            curr->prev = zero;
                            zero->next = curr;
                            zero->prev = curr->prev;
                            curr = zero;
                            break;
                        }
                        times--;
                    }

                    break;
                }

                case 'r':{

                    int times = current->ind1;

                    while (times > 0) {
                        if (curr->next != NULL) {
                            curr = curr->next;
                        }
                        else break;
                        times--;
                    }

                    break;
                }

                case 'q':{
                    exit(0);
                }

                default:
                    break;
            }

            if (current->next != NULL) //redundant
                current = current->next;
            command = current->command_line[strlen(current->command_line) - 1];
        }
        exit(0); //redundant
    }

    return 0;
}



//IMPLEMENTATIONS

//RB bst
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
    //RB_insert_fixup(T_root, z);
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
        //free(y->text);
        //free(y);
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
void print_delta(node *T_root, int ind1, int ind2){

    node* to_print = tree_search(T_root, ind1);

    while(ind1!=ind2+1) {
        if(to_print!=nil) {
            fputs(to_print->text,stdout);
            fputs("\n",stdout);
            ind1++;
        }
        else{
            fputs(".\n",stdout);
            ind1++;
        }
        to_print = in_order_successor(to_print);
    }
}
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
        in_order_walk(T_root->right);
    }
}
node *tree_search(node* x, int id){
    if(x==nil || id==x->id ){
        return x;
    }
    else{
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
//Insert and Modify functions
void function_modify(node *iter_old_tree, tree *old_tree, int ind1, list_write_only* current, int ind2, node* my_new_root, tree* my_new_tree) {

    if(iter_old_tree == nil){
        return;
    }

    //target is on left, i have to copy the journey from here
    if(ind1 < iter_old_tree->id){
        //if not exists the node , insert
        if(iter_old_tree->left==nil){
            while(ind1!=ind2+1){
                my_new_tree->root = function_insert(ind1,ind2,current,my_new_tree->root);
                ind1++;
            }
            return;
        }
        else {
            node* journey_copy = malloc(sizeof(node));
            journey_copy->left = iter_old_tree->left->left;
            journey_copy->right = iter_old_tree->left->right;
            journey_copy->id = iter_old_tree->left->id;
            journey_copy->text = iter_old_tree->left->text;
            journey_copy->p = my_new_root;
            my_new_root->left = journey_copy;
        }

        function_modify(iter_old_tree->left,old_tree, ind1, current, ind2, my_new_root->left,my_new_tree);
    }
    //target is on right, i have to copy the journey from here
    else if(ind1 > iter_old_tree->id){
        //if not exists the node
        if(iter_old_tree->right==nil) {
            while (ind1 != ind2 + 1) {
                my_new_tree->root = function_insert(ind1, ind2, current, my_new_tree->root);
                ind1++;
            }
            return;
        }
        else{
            node* journey_copy = malloc(sizeof(node));
            journey_copy->left = iter_old_tree->right->left;
            journey_copy->right = iter_old_tree->right->right;
            journey_copy->id = iter_old_tree->right->id;
            journey_copy->text = iter_old_tree->right->text;
            journey_copy->p = my_new_root;
            my_new_root->right = journey_copy;
            function_modify(iter_old_tree->right,old_tree, ind1,current, ind2, my_new_root->right,my_new_tree);
        }

    }
    //i am on the target (my_new_root);
    else if(ind1 == iter_old_tree->id){
        my_new_root->text = current->divided_text[ind1-current->ind1];

        ind1++;
        if(ind1<=ind2){     //while(ind1<=ind2){
            node* next_one_old = in_order_successor(iter_old_tree);
            node* next_one_new = in_order_successor(my_new_root);

            if(next_one_old==nil && next_one_new==nil){
                function_modify(iter_old_tree,old_tree,ind1,current,ind2,my_new_root,my_new_tree);
            }
            else if(next_one_old==next_one_new){
                node* journey_copy = malloc(sizeof(node));
                journey_copy->left = next_one_old->left;
                journey_copy->right = next_one_old->right;
                journey_copy->id = next_one_old->id;
                journey_copy->text = next_one_old->text;
                journey_copy->p = tree_search(my_new_tree->root,iter_old_tree->id);
                tree_search(my_new_tree->root,iter_old_tree->id)->right = journey_copy;
                next_one_new = in_order_successor(my_new_root);
                function_modify(next_one_old,old_tree,ind1,current,ind2,next_one_new,my_new_tree);
            }
            else
                function_modify(next_one_old,old_tree,ind1,current,ind2,next_one_new,my_new_tree);
        }
    }

}
node* function_insert_1(int ind1, int ind2, list_write_only* current, node *my_new_root) {

    while(ind1!=ind2+1){
        node* new_node_to_insert = malloc(sizeof(node));
        new_node_to_insert->left = new_node_to_insert->right = new_node_to_insert->p = nil;
        new_node_to_insert->id = ind1;
        new_node_to_insert->text = current->divided_text[ind1-1];
        RB_insert(&my_new_root,new_node_to_insert);
        ind1++;
    }
    return my_new_root;
}
node* function_insert(int ind1, int ind2, list_write_only* current, node *my_new_root) {

    node* new_node_to_insert = malloc(sizeof(node));
    new_node_to_insert->left = new_node_to_insert->right = new_node_to_insert->p = nil;
    new_node_to_insert->id = ind1;
    new_node_to_insert->text = current->divided_text[ind1-current->ind1];
    RB_insert(&my_new_root,new_node_to_insert);

    return my_new_root;
}
//Linked List
list_write_only *insert_in_list_wo(list_write_only *new) {

    if(last==NULL){ //empty, insert as first node
        last = new;
        new->prev = NULL;
        new->next = NULL;
        return new;
    }

    while(last->next!=NULL)
        last = last->next;

    last->next = new;
    new->prev = last;
    new->next = NULL;

    return new;
}
list_node* insert_in_list(node* tree_node_root, list_node* curr) {

    list_node *t = malloc(sizeof(list_node));
    t->root = tree_node_root;

    if (curr->root == empty) {    //empty
        curr = t;
        curr->next = NULL;
        curr->prev = NULL;
        return t;
    }

    curr->next = t;
    t->prev = curr;
    t->next = NULL;

    return t; //the last node
}
