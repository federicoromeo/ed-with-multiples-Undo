#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define fp stdin
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
node* empty;

struct tree {
    node *root;   //testa
    //node *nil;
};
typedef struct tree tree;


struct list_node {
    node* root;
    struct list_node* prev;
    struct list_node* next;
};
typedef struct list_node list_node;

list_node *first;

char *line;
size_t len = 0;


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

/*void function_modify(node *iter_old_tree, tree* old_tree, int ind1, int ind2, node* my_new_root, tree*, FILE* fp);
node* function_insert(int ind1, int ind2, node *my_new_root, FILE *fp);
node* function_insert_1(int ind1, int ind2, node *my_new_root , FILE *fp);
void function_delete(node *iter_old_tree, tree *old_tree, int ind1, int ind2, node *my_new_root, tree *my_new_tree, FILE *fp);
void print_delta(node *T_root, int ind1, int ind2, FILE* out);
void print_delta2(node *curr, int ind1, int ind2, FILE *fp);*/
void function_modify(node *iter_old_tree, tree* old_tree, int ind1, int ind2, node* my_new_root, tree*);
node* function_insert(int ind1, int ind2, node *my_new_root);
node* function_insert_1(int ind1, int ind2, node *my_new_root );
void function_delete(node *iter_old_tree, tree *old_tree, int ind1, int ind2, node *my_new_root, tree *my_new_tree);
void print_delta(node *T_root, int ind1, int ind2);
void print_delta2(node *curr, int ind1, int ind2);

int count_nodes(node *root);
node *tree_search(node* x, int id);
void print2DUtil(node *root, int space);
struct node* in_order_successor(node* n);
//LINKED LIST
list_node* insert_in_list(node* tree_node_root, list_node* curr);



int main(){
    //freopen("test.txt","r",stdin);  //todo levooooooooooooooooo
    //freopen("out.txt","r",stdout);  //todo levooooooooooooooooo

    int count = 1;

    // Creation of nil node
    nil = malloc(sizeof(node));                  // sentinella
    nil->left = nil;
    nil->right = nil;
    nil->p = nil;
    nil->id = -1;
    nil->color = 'b';

    empty = malloc(sizeof(node));                  // sentinella
    empty->left = nil;
    empty->right = nil;
    empty->p = nil;
    empty->id = -2;
    empty->color = 'b';

    first = malloc(sizeof(list_node));

    // Pointer at the current node of the linked list containing the roots of the successives modified trees
    list_node* curr = malloc(sizeof(list_node));
    curr->root = empty;
    curr->next = NULL;
    curr->prev = NULL;

    //for testing
    //FILE *fp = fopen("C:/Users/feder/CLionProjects/ed-con-Undo-multipli/test.txt","r");
    //FILE* out = fopen("out.txt","w");
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

                //new_tree->nil = nil;
                if(curr->root!=empty && curr->root!=nil){
                    new_tree->root = malloc(sizeof(node));
                    new_tree->root->left = curr->root->left;
                    new_tree->root->right = curr->root->right;
                    new_tree->root->p = curr->root->p;
                    new_tree->root->id = curr->root->id;
                    //SERENA  new_tree->root->text = curr->root->text;
                    new_tree->root->text = malloc((strlen(curr->root->text)+1)*sizeof(char));
                    strcpy(new_tree->root->text, curr->root->text);
                }
                else{
                    new_tree->root = nil;
                }

                int num = count_nodes(curr->root);
                if (curr->root == nil) num--;

                //modifica
                if(tree_search(curr->root,ind1)!=nil)
                    function_modify(curr->root,old_tree,ind1,ind2,new_tree->root,new_tree);
                    //function_modify(curr->root,old_tree,ind1,ind2,new_tree->root,new_tree,fp);

                //inserimento
                else
                    if(count==1 || num==0) //empty
                        //new_tree->root = function_insert_1(ind1,ind2,new_tree->root,fp);
                        new_tree->root = function_insert_1(ind1,ind2,new_tree->root);
                    else
                        //function_modify(curr->root,old_tree,ind1,ind2,new_tree->root,new_tree,fp);
                        function_modify(curr->root,old_tree,ind1,ind2,new_tree->root,new_tree);


                count++;
                //in_order_walk(new_tree->root);
                //puts("\n");

                curr = insert_in_list(new_tree->root,curr);
                /*if(curr->next!=NULL){
                    curr = curr->next;
                }*/

                //int num = count_nodes(curr->root);
                //if (curr->root == nil) num--;
                //printf("this tree has  %d nodes\n\n", num);

                //to avoid processing of "."
                chars = getline(&line,&len,fp);
                line[strcspn(line, "\n\r")] = '\0';

                break;
            }

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

                //non deve fare niente ma deve essere considerata per le undo e redo) TODO
                if(ind1==0 && ind2==0){
                    break;
                }
                i = 1;

                if(ind1==0)
                    ind1++;

                int delta = ind2 - ind1 + 1;

                RB_delete(&new_tree->root,tree_search(new_tree->root,0));

                if(curr->root!=nil){
                    while( i!=ind1 ){
                        node* to_be_copied = tree_search(curr->root,i);
                        node* x = malloc(sizeof(node));
                        x->id = to_be_copied->id;
                        // SERENA   x->text = to_be_copied->text;
                        x->text = malloc((strlen(to_be_copied->text)+1)*sizeof(char));
                        strcpy(x->text,to_be_copied->text);
                        x->p = x->right = x->left = nil;
                        RB_insert(&new_tree->root,x);
                        i++;
                    }
                }
                i = ind2+1;
                if(curr->root!=nil){
                    while(i<=count_nodes(curr->root)){
                        node* to_be_copied = tree_search(curr->root,i);
                        if(to_be_copied == nil)
                            break;
                        node* x = malloc(sizeof(node));
                        x->id = to_be_copied->id - delta;
                        // SERENA       x->text = to_be_copied->text;
                        x->text = malloc((strlen(to_be_copied->text)+1)*sizeof(char));
                        strcpy(x->text,to_be_copied->text);
                        x->p = x->right = x->left = nil;
                        RB_insert(&new_tree->root,x);
                        i++;
                    }
                }


                //in_order_walk(new_tree->root);
                //puts("\n");

                // List handling
                curr = insert_in_list(new_tree->root,curr);

                /*if(curr->next!=NULL){
                    curr = curr->next;
                }*/

                //int num = count_nodes(curr->root);
                //if (curr->root == nil) num--;
                //printf("this tree has  %d nodes\n\n", num);

                break;
            }

            /*case 'd':{
            //while in order succ   id--

                int ind[2]={0,0}, i=0;
                char* splitted_line = strtok (line,",");
                while (splitted_line!= NULL){
                    ind[i] = (int) strtol(splitted_line, (char **)NULL, 10);
                    splitted_line = strtok (NULL, ",");
                    i++;
                }

                ind1 = ind[0];
                ind2 = ind[1];
                //int delta = ind2 - ind1 +1;

                // Creation of the new tree
                struct tree* new_tree = malloc(sizeof(struct tree*));
                if(!new_tree){
                    fprintf(stderr,"\nError new tree\n"); //todo: tolgo
                    exit(0);
                }
                // Create a tree to be passed as old tree
                struct tree* old_tree = malloc(sizeof(struct tree*));
                if(!old_tree){
                    fprintf(stderr,"\nError old tree\n"); //todo: tolgo
                    exit(0);
                }

                old_tree->root = curr->root;
                // Copy the root of the old tree in the new one
                new_tree->root = malloc(sizeof(node));
                //new_tree->nil = nil;
                if(curr->root!=nil){
                    new_tree->root->left = curr->root->left;
                    new_tree->root->right = curr->root->right;
                    new_tree->root->p = curr->root->p;
                    new_tree->root->id = curr->root->id;
                    new_tree->root->text = malloc((strlen(curr->root->text)+1)*sizeof(char));
                    strcpy(new_tree->root->text, curr->root->text);
                }
                else{
                    new_tree->root = nil;
                }

                function_delete(curr->root,old_tree,ind1,ind2,new_tree->root,new_tree,fp);


                // List handling
                curr = insert_in_list(new_tree->root,curr);

                if(curr->next!=NULL){
                    curr = curr->next;
                }
                break;

            }*/

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

                //print_delta2(curr->root,ind1,ind2,out);
                print_delta(curr->root,ind1,ind2);
                //print_delta(curr->root,ind1,ind2,out);

                break;
            }

            case 'u':{

                // Get the number of undo times by removing the last character (the command)
                line[strlen(line)-1] = '\0';
                times = (int) strtol(line, (char **)NULL, 10);                //times = atoi(line);

                    while (times > 0) {
                        if (curr->prev != NULL) {
                            curr = curr->prev;
                        } else {
                            //create a copy of first node of list
                            first->root = curr->root;
                            first->next = curr->next;
                            first->prev = curr->prev;
                            //curr->root = nil;
                            //curr->next = first;
                            //curr->prev = NULL;
                            break;
                        }
                        times--;
                    }

                break;
            }

            case 'r':{

                // Get the number of redo times by removing the last character (the command)
                line[strlen(line)-1] = '\0';
                times = (int) strtol(line, (char **)NULL, 10);                //times = atoi(line);
                //int i = 1;

                while (times > 0) {
                    //if(i==1 /*&& curr->root!=empty*/)
                    //curr = curr;
                    if (curr->next != NULL) {
                        curr = curr->next;
                    } else break;
                    times--;
                    //i=0;
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


/*void function_delete(node *iter_old_tree, tree *old_tree, int ind1, int ind2, node *my_new_root, tree *my_new_tree) {
//void function_delete(node *iter_old_tree, tree *old_tree, int ind1, int ind2, node *my_new_root, tree *my_new_tree, FILE *fp) {

    if(iter_old_tree == nil){
        return;
    }

    if(ind1 < iter_old_tree->id){
        //if not exists the node , insert
        if(iter_old_tree->left==nil){
            //todo
            while(ind1!=ind2+1){
                my_new_root = function_insert(ind1,ind2,my_new_tree->root);
                //my_new_root = function_insert(ind1,ind2,my_new_tree->root,fp);
                ind1++;
            }
        }
        else {
            //se era gia una copia
            //if(tree_search(my_new_tree->root,iter_old_tree->left->id)->is_a_copy==1){
            //    function_delete(iter_old_tree->left, old_tree, ind1, ind2, my_new_root->left,my_new_tree,fp);
            //}
            else {
                node *journey_copy = malloc(sizeof(node));
                journey_copy->left = iter_old_tree->left->left;
                journey_copy->right = iter_old_tree->left->right;
                journey_copy->id = iter_old_tree->left->id;
                journey_copy->text = malloc((strlen(iter_old_tree->left->text) + 1) * sizeof(char));
                strcpy(journey_copy->text, iter_old_tree->left->text);
                journey_copy->is_a_copy = 1;
                journey_copy->p = my_new_root;
                my_new_root->left = journey_copy;

                function_delete(iter_old_tree->left, old_tree, ind1, ind2, my_new_root->left,my_new_tree);
                //function_delete(iter_old_tree->left, old_tree, ind1, ind2, my_new_root->left,my_new_tree,fp);
            //}
        }

    }

    else if(ind1 > iter_old_tree->id){
        node* journey_copy = malloc(sizeof(node));
        //if not exists the node , insert
        if(iter_old_tree->right==nil){ //quando devo aggiungerlo sto nodo
            while(ind1!=ind2+1){
               //ion_insert(ind1,ind2,my_new_tree->root,fp);
                my_new_root = function_insert(ind1,ind2,my_new_tree->root);
                ind1++;
            }
            return;
        }
        else{
            journey_copy->left = iter_old_tree->right->left;
            journey_copy->right = iter_old_tree->right->right;
            journey_copy->id = iter_old_tree->right->id;
            journey_copy->text = malloc((strlen(iter_old_tree->right->text) + 1) * sizeof(char));
            strcpy(journey_copy->text, iter_old_tree->right->text);
            journey_copy->p = my_new_root;
            my_new_root->right = journey_copy;
        }

        function_delete(iter_old_tree->right, old_tree, ind1, ind2, my_new_root->right,my_new_tree);
        //function_delete(iter_old_tree->right, old_tree, ind1, ind2, my_new_root->right,my_new_tree,fp);
    }

    else if(ind1 == iter_old_tree->id){
        //node* next = my_new_root->right;
        node* next_one_old = in_order_successor(iter_old_tree);
        node* next_one_new = in_order_successor(my_new_root);
        RB_delete(&my_new_tree->root,my_new_root);
        ind1++;
        if(ind1<=ind2){
        //while(ind1<=ind2){


            if(tree_search(old_tree->root,my_new_tree->root->id)==my_new_tree->root){
                node* journey_copy = malloc(sizeof(node));
                journey_copy->left = my_new_tree->root->left;
                journey_copy->right = my_new_tree->root->right;
                journey_copy->id = my_new_tree->root->id;
                journey_copy->text = malloc((strlen(my_new_tree->root->text) + 1) * sizeof(char));
                strcpy(journey_copy->text, my_new_tree->root->text);
                RB_insert(&my_new_tree->root,journey_copy);
                function_delete(next_one_old,old_tree,ind1,ind2,next_one_new,my_new_tree); //my_new_root is nil, so i restart from the head
                //function_delete(next_one_old,old_tree,ind1,ind2,next_one_new,my_new_tree,fp); //my_new_root is nil, so i restart from the head
            }
            else function_delete(old_tree->root,old_tree,ind1,ind2,my_new_tree->root,my_new_tree); //my_new_root is nil, so i restart from the head
            //else function_delete(old_tree->root,old_tree,ind1,ind2,my_new_tree->root,my_new_tree,fp); //my_new_root is nil, so i restart from the head
        }
    }

}
*/

//iter_old_tree: testa albero vecchio
//my_new_root: testa albero nuovo
void function_modify(node *iter_old_tree, tree *old_tree, int ind1, int ind2, node* my_new_root, tree* my_new_tree) {
//void function_modify(node *iter_old_tree, tree *old_tree, int ind1, int ind2, node* my_new_root, tree* my_new_tree, FILE* fp) {

    if(iter_old_tree == nil){
        //my_new_root = function_insert(ind1,ind2,my_new_root->p,fp);
        return;
    }

    //target is on left, i have to copy the journey from here
    if(ind1 < iter_old_tree->id){
        //if not exists the node , insert
        if(iter_old_tree->left==nil){
            while(ind1!=ind2+1){
                //my_new_tree->root = function_insert(ind1,ind2,my_new_tree->root,fp);
                my_new_tree->root = function_insert(ind1,ind2,my_new_tree->root);
                ind1++;
            }
            return;
            /*journey_copy->left = nil;
                  journey_copy->right = nil;
                  journey_copy->id = ind1;
                  getline(&line,&len,fp);
                  line[strcspn(line,"\n\r")] = '\0';
                  journey_copy->text = malloc((strlen(line)+1) * sizeof(char));
                  strcpy(journey_copy->text,line);
                  RB_insert(&my_new_root,journey_copy);*/
        }
        else {
            node* journey_copy = malloc(sizeof(node));
            journey_copy->left = iter_old_tree->left->left;
            journey_copy->right = iter_old_tree->left->right;
            journey_copy->id = iter_old_tree->left->id;
            //SERENA        journey_copy->text = iter_old_tree->left->text;
            journey_copy->text = malloc((strlen(iter_old_tree->left->text) + 1) * sizeof(char));
            strcpy(journey_copy->text, iter_old_tree->left->text);
            journey_copy->p = my_new_root;
            my_new_root->left = journey_copy;
        }

        function_modify(iter_old_tree->left,old_tree, ind1, ind2, my_new_root->left,my_new_tree);
        //function_modify(iter_old_tree->left,old_tree, ind1, ind2, my_new_root->left,my_new_tree,fp);
    }
    //target is on right, i have to copy the journey from here
    else if(ind1 > iter_old_tree->id){
        //if not exists the node
        if(iter_old_tree->right==nil) {
            /*if(tree_search(old_tree->root,ind1)==nil){ //quando devo aggiungerlo sto nodo*/
            while (ind1 != ind2 + 1) {
                //my_new_tree->root = function_insert(ind1, ind2, my_new_tree->root, fp);
                my_new_tree->root = function_insert(ind1, ind2, my_new_tree->root);
                ind1++;
            }
            return;
            /*journey_copy->left = nil;
        journey_copy->right = nil;
        journey_copy->id = ind1;
        getline(&line, &len, fp);
        line[strcspn(line, "\n\r")] = '\0';
        journey_copy->text = malloc((strlen(line) + 1) * sizeof(char));
        strcpy(journey_copy->text, line);
        RB_insert(&my_new_root, journey_copy);*/
        }
        else{ //c'è gia in quello vecchio
         /*       if(tree_search(my_new_tree->root,ind1)==nil){ //non c'è in quello nuovo
                    while(ind1!=ind2+1){
                        my_new_root = function_insert(ind1,ind2,my_new_tree,fp);
                        ind1++;
                    }
                }
                else function_modify(tree_search(my_new_tree->root,ind1), old_tree, ind1, ind2, tree_search(old_tree->root,ind1),my_new_tree,fp);

            }
        }
        else{
            if(tree_search(old_tree->root,ind1)!=nil){  //se c'è in quello vecchio
                if(tree_search(my_new_tree->root,ind1)!=tree_search(old_tree->root,ind1)){   //se c'è (la copia) in quello nuovo, maybe ind1++
                    ind1++;
                    if(ind1<=ind2){
                        function_modify(tree_search(my_new_tree->root,ind1), old_tree, ind1, ind2, tree_search(old_tree->root,ind1),my_new_tree,fp);
                    }
                }
                else{ //se c'è in quello vecchio ma non in quello nuovo, faccio copia journey*/
                    //node* aux = tree_search(old_tree->root,ind1);
                    node* journey_copy = malloc(sizeof(node));
                    journey_copy->left = iter_old_tree->right->left;
                    journey_copy->right = iter_old_tree->right->right;
                    journey_copy->id = iter_old_tree->right->id;
                    journey_copy->text = iter_old_tree->right->text;
                    // SERENA       journey_copy->text = malloc((strlen(iter_old_tree->right->text) + 1) * sizeof(char));
                    strcpy(journey_copy->text, iter_old_tree->right->text);
                    journey_copy->p = my_new_root;
                    my_new_root->right = journey_copy;
                    //function_modify(iter_old_tree->right,old_tree, ind1, ind2, my_new_root->right,my_new_tree,fp);
                    function_modify(iter_old_tree->right,old_tree, ind1, ind2, my_new_root->right,my_new_tree);
              /*  }

            }*/
        }

    }
    //i am on the target (my_new_root);
    else if(ind1 == iter_old_tree->id){
        //free(my_new_root->text);
        getline(&line,&len,fp);
        line[strcspn(line,"\n\r")] = '\0';
        my_new_root->text = malloc((strlen(line)+1) * sizeof(char));
        strcpy(my_new_root->text,line);

        //ho trovato il primo da modificare
        ind1++;
        //giusto ma insesce nodi a caso, mettono doppi padri (fixato commentantdo rbfixup)
        if(ind1<=ind2){     //while(ind1<=ind2){
            node* next_one_old = in_order_successor(iter_old_tree);
            node* next_one_new = in_order_successor(my_new_root);

            /*if(next_one_new==next_one_old){
                //function_modify(iter_old_tree->right,old_tree,ind1,ind2,my_new_root->right,my_new_tree,fp);
            }*/

            if(next_one_old==nil && next_one_new==nil){ //ma devo inserire
                //function_modify(iter_old_tree,old_tree,ind1,ind2,my_new_root,my_new_tree,fp);
                function_modify(iter_old_tree,old_tree,ind1,ind2,my_new_root,my_new_tree);
                /*while (ind1 != ind2 + 1) {
                                   my_new_tree->root = function_insert(ind1, ind2, my_new_tree, fp);
                                   ind1++;
                               }*/
            }
            else if(next_one_old==next_one_new){ //todo here i changeddddddddddddddddddddddddddddddddd
                node* journey_copy = malloc(sizeof(node));
                journey_copy->left = next_one_old->left;
                journey_copy->right = next_one_old->right;
                journey_copy->id = next_one_old->id;
                //SERENA        journey_copy->text = next_one_old->text;
                journey_copy->text = malloc((strlen(next_one_old->text) + 1) * sizeof(char));
                strcpy(journey_copy->text, next_one_old->text);
                journey_copy->p = tree_search(my_new_tree->root,iter_old_tree->id);
                tree_search(my_new_tree->root,iter_old_tree->id)->right = journey_copy;

                next_one_new = in_order_successor(my_new_root);
                //function_modify(next_one_old,old_tree,ind1,ind2,next_one_new,my_new_tree,fp);
                function_modify(next_one_old,old_tree,ind1,ind2,next_one_new,my_new_tree);
            }
            else
                function_modify(next_one_old,old_tree,ind1,ind2,next_one_new,my_new_tree);
                //function_modify(next_one_old,old_tree,ind1,ind2,next_one_new,my_new_tree,fp);
        }
    }

}

node* function_insert_1(int ind1, int ind2, node *my_new_root) {
//node* function_insert_1(int ind1, int ind2, node *my_new_root, FILE *fp) {

    while(ind1!=ind2+1){
        node* new_node_to_insert = malloc(sizeof(node));
        new_node_to_insert->left = new_node_to_insert->right = new_node_to_insert->p = nil;
        new_node_to_insert->id = ind1;
        getline(&line,&len,fp);
        line[strcspn(line,"\n\r")] = '\0';
        new_node_to_insert->text = malloc((strlen(line)+1) * sizeof(char));
        strcpy(new_node_to_insert->text, line);
        RB_insert(&my_new_root,new_node_to_insert);
        ind1++;
    }

    return my_new_root;
}

node* function_insert(int ind1, int ind2, node *my_new_root) {
//node* function_insert(int ind1, int ind2, node *my_new_root, FILE *fp) {

    node* new_node_to_insert = malloc(sizeof(node));
    new_node_to_insert->left = new_node_to_insert->right = new_node_to_insert->p = nil;
    new_node_to_insert->id = ind1;
    getline(&line,&len,fp);
    line[strcspn(line,"\n\r")] = '\0';
    new_node_to_insert->text = malloc((strlen(line)+1) * sizeof(char));
    strcpy(new_node_to_insert->text, line);
    RB_insert(&my_new_root,new_node_to_insert);

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

    list_node *t = malloc(sizeof(list_node));
    t->root = tree_node_root;

    // if it's empty
    if (curr->root == empty) { //doubt
        curr = t;
        //curr->root = t->root;
        curr->next = NULL;
        curr->prev = NULL;
        return t;
    }

    //list_node* temp = curr;

    //dovrei fare free su tutti quelli dopo

    //WRONG
    //while (temp->next != NULL)
    //    temp = temp->next;

    curr->next = t;
    t->prev = curr;
    t->next = NULL;

    return t; //the last node
}

void print_delta(node *T_root, int ind1, int ind2){
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

void print_delta2(node *T_root, int ind1, int ind2){
//void print_delta2(node *T_root, int ind1, int ind2, FILE* out){
    node* print = tree_search(T_root, ind1);
    while (ind1 != ind2 + 1) {
        if (print != nil) {
            //printf("%s\n",tree_search(T_root, ind1)->text);
            //puts(tree_search(T_root, ind1)->text);
            fputs(print->text, out);
            fputs("\n", out);
            ind1++;
        } else {
            //puts(".");
            fputs(".\n", out);
            ind1++;
        }
        print = in_order_successor(print);
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

