#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct node {
    char color;
    char *key;
    char *text;
    struct node *p;
    struct node *left;
    struct node *right;
} node;


typedef struct tree{
    node *root;   //testa
    node *nil;
}tree;


node T_nil;
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


int main(){

    //for testing
    char line[1024];
    FILE *fp = fopen("C:/Users/feder/CLionProjects/ed-con-Undo-multipli/test.txt","r");
    if(fp==NULL){
        printf("Error opening file");
        exit(0);
    }

    int ind1, ind2, times, delta;
    char command;

    while(!feof(fp)) {
        fscanf(fp, "%s", line);
        printf("\n");

        switch(line[strlen(line)-1]){

            case 'q':{
                exit(0);
            }

            case '.':{
                break;
            }

            case 'c':
            case 'd':
            case 'p':{
                char* splitted_line = strtok(line, ",");
                while( splitted_line != NULL ) {
                    int j = 1;
                    for (int k=0; k<=strlen(splitted_line); k++) {
                        if(j==1){
                            // Get the first address
                            ind1 = (int) strtol(splitted_line, (char **)NULL, 10);
                            //ind1 = atoi(splitted_line);
                            printf("\nind1= %d", ind1);
                            splitted_line = strtok(NULL, ",");
                        }
                        if(j==2){
                            // Get the command
                            command = splitted_line[strlen(splitted_line)-1];
                            printf("\ncommand = %c", command);
                            // Get the second address by removing the last character (the command)
                            splitted_line[strlen(splitted_line)-1] = '\0';
                            ind2 = (int) strtol(splitted_line, (char **)NULL, 10);
                            //ind2 = atoi(splitted_line);
                            printf("\nind2= %d\n", ind2);

                            splitted_line = strtok(NULL, ",");
                            break;
                        }
                        j++;
                    }
                }
                //OK, HERE I HAVE ALL THE INPUTS FOR THE COMMAND, NOW I WANT THE TEXT LINES

                delta = ind2 - ind1 +1;

                while(delta>0){
                    fscanf(fp, "%s", line);
                    printf("NEW LINE: ");
                    puts(line);
                    delta--;
                }

                break;
            }

            case 'u':
            case 'r':{
                // Get the command
                command = line[strlen(line)-1];
                printf("\ncommand = %c", command);
                // Get the number of redo/undo times by removing the last character (the command)
                line[strlen(line)-1] = '\0';
                times = (int) strtol(line, (char **)NULL, 10);
                //times = atoi(line);
                printf("\ntimes= %d", times);
                break;
            }

        }

    }
    fclose(fp);
    return 0;
}




//IMPLEMENTATIONS

void in_order_walk(node *T_root) {
    if (T_root != &T_nil) {
        in_order_walk(T_root->left);
        fputs(T_root->key, stdout);
        fputs(" ", stdout);
        in_order_walk(T_root->right);
    }
}

node *node_search(node *T_root, char *k) {
    while (T_root != &T_nil) {
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
    if (T_root == &T_nil) {
        return &T_nil;
    } else {
        return T_root;
    }
}

void left_rotate(node **T_root, node *x) {
    if ((**T_root).p == &T_nil && x->right != &T_nil) {
        node *y = x->right;
        x->right = y->left;
        if (y->left != &T_nil ) {
            y->left->p = x;
        }
        y->p = x->p;
        if (x->p == &T_nil) {
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
    if ((**T_root).p == &T_nil && x->left != &T_nil) {
        node *y = x->left;
        x->left = y->right;
        if (y->right != &T_nil ) {
            y->right->p = x;
        }
        y->p = x->p;
        if (x->p == &T_nil ) {
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
    node *y = &T_nil;
    node *x = *T_root;
    while (x != &T_nil) {
        y = x;
        if (strcmp(z->key , x->key) < 0) {
            x = x->left;
        }
        else {
            x = x->right;
        }
    }
    z->p = y;
    if (y == &T_nil) {
        *T_root = z;
    }
    else if (strcmp(z->key,y->key) < 0) {
        y->left = z;
    }
    else {
        y->right = z;
    }
    z->left = &T_nil;
    z->right = &T_nil;
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
    if (u->p == &T_nil) {
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
    while(T_root->left != &T_nil) {
        T_root = T_root->left;
    }
    return T_root;
}

void RB_delete(node **T_root, node *z) {
    if (*T_root != &T_nil) {
        node *x;
        node *y = z;
        char y_original_color = y->color;
        if (z->left == &T_nil) {
            x = z->right;
            RB_transplant(T_root, z, z->right);
        } else if (z->right == &T_nil) {
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
        free(y->key);
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
    while (*T_root != &T_nil) {
        RB_delete(T_root, *T_root);
    }
}

