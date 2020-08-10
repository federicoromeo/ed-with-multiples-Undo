#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
                }  //OK, HERE I HAVE ALL THE INPUTS FOR THE COMMAND, NOW I WANT THE TEXT LINES

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




//FUNCTIONS
