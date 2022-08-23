#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*Struct for the synoyms and antonyms of the left most word at each line*/
typedef struct words
{
    char *word;
    struct words *next;
}words;

/*Struct for the left most word at each line*/
typedef struct lib
{   
    int index;
    float correct, incorrect, probability;
    char *word;
    struct lib *nextline;
    struct words *nextword;
}lib;
/*Struct for readiung and writing to binary file*/
typedef struct datas{
    unsigned short int words_index, question_type, answer;
}datas;
/*Struct for keeping users performance*/
typedef struct user_t{
    unsigned short int trues, falses, ques_count;
}user_t;

/*Pre Processing Functions */
int procces_file(char *filename, lib **list, int counter);
int process_line(char* line, int* counter, lib **list, char *buffer);
int add_node(lib **listmain, char *buffer);
void free_list(lib **list1);
void print_list(lib **list);
void reset_list_values(lib **list);
void update_list_values(lib **list);
int update_txt(lib **antlib,lib **synlib, char ant[], char syn[]);
int load_userdata(user_t *cur_user, datas *data, lib **antlib, lib **synlib, FILE *usersave);
int check_matched(lib *list, char name[]);

/*User interacted functions needs inputs*/
int start_program(char users_txt[]);
FILE* check_users(char users_txt[],char username[]);
void print_menu();
int new_question(lib *antlib,lib *synlib, datas *data);
int check_list(lib *list, char buffer[]);

/*Probability calculation functions*/
int calc_probabilty(lib *antlib,lib *synlib,datas *data, int antcount, int totalcount);
float total_probabilty(lib *list, float sum);
float find_min_max(lib *antlib, lib *synlib, float avrg, float *min, float *max);
float recursive_prob_cal(lib *antlib,lib *synlib, float avrg, int count_avrg, float totalProb, float min, float max);
float find_avrg(lib *antlib, lib *synlib, float *min, float *max,  float *total, int *avrg_count);


/*Main function sends the name of the file that stores the names of users to start function*/
int main()
{
    char users_txt[] = "users.txt";
    /*In the pdf it was not clear if we can use time.h library so I assumed we can since the questions need to be asked randomly at each run*/
    srand(time(NULL));
    /*Start function is the main function for all heavy work*/
    start_program(users_txt);
    return 0;
}

int start_program(char users_txt[])
{   
    lib *antlib = NULL, *synlib = NULL;         /*2 head pointers for 2 separete linked lists*/
    words *temp3 = NULL;
    datas data = {0,0,0}, empty_data = {0,0,0};
    user_t cur_user = {0,0,0}, empty_user = {0,0,0};
    char ant[15] = "antonyms.txt";
    char syn[15] = "synonyms.txt";
    char username[30];
    char buffer[8];
    int op = 0, question_count = 0, ant_counter = 0, syn_counter = 0, index = 0, check = 1;
    
    /*cheks if the user is in the list and .worddat file can be opened or not*/
    FILE *usersave = check_users(users_txt,username);
    if(usersave == NULL)
    {
        printf("Could not open the user.worddat file\n");
        return 1;
    }
    /*parses the txt files and creates 2 separate linked lists*/
    ant_counter = procces_file(ant,&antlib,ant_counter);
    syn_counter = procces_file(syn,&synlib,ant_counter);
    /*Loads current users data from binary file and updates the values*/
    load_userdata(&cur_user,&data,&antlib,&synlib,usersave);
    update_list_values(&antlib);
    update_list_values(&synlib);

    /*Load users info from usersave and put the cursor at the begining*/
    
    while(1)
    {
        printf("\n\t\t--- %s's Performance ---\n",username);
        printf("Questions Asked: %d    Correct Answers: %d    Incorrect Answers: %d\n",cur_user.ques_count, cur_user.trues, cur_user.falses);
        print_menu();
        printf("Select an operation:");
        scanf(" %d",&op);
        switch (op)
        {
        case 1:/*Asks a question than updates user data to binary file*/
            while(check == 1)
            {
                /*Probability calculation and finding an index and qusetion type*/
                data.words_index = calc_probabilty(antlib,synlib,&data,ant_counter,syn_counter);
                if(data.words_index <= ant_counter)     data.question_type = 0;
                else data.question_type = 1;  
                /*Ask question based on data then updates changed values*/
                check = new_question(antlib,synlib,&data);
                update_list_values(&antlib);
                update_list_values(&synlib);
                /*Save the new values to binary file for user*/
                fwrite(&data,sizeof(datas),1,usersave);
                cur_user.ques_count++;
                if(data.answer == 1) cur_user.trues++;
                else cur_user.falses++;
            }  
            check = 1;
            break;

        case 2: /*Adds new synonym to a word*/
            strcpy(buffer,"synonym");
            check_list(synlib,buffer);
            break;
        
        case 3: /*Adds new antonym to a word*/
            strcpy(buffer,"antonym");
            check_list(antlib,buffer);
            break;
            
        case 4:/*Prints The list of words*/
            printf("\t\t\t\tANTONYMS\n");
            print_list(&antlib);
            printf("\n");
            printf("\t\t\t\tSYNONYMS\n");
            print_list(&synlib);
            break;
        
        case 5:/*Change users*/
            /*Closes current users binary file and opens a new one*/
            fclose(usersave);
            usersave = check_users(users_txt,username);
            if(usersave == NULL)
            {
                printf("Could not open the user.worddat file\n");
                return 1;
            }
            /*Resets all the value came from the previous user*/
            data = empty_data;
            cur_user = empty_user;
            reset_list_values(&antlib);
            reset_list_values(&synlib);
            /*loads the new users data and updates it*/
            load_userdata(&cur_user,&data,&antlib,&synlib,usersave);
            update_list_values(&antlib);
            update_list_values(&synlib);
            break;
        
        case 6:/*Updates the text file, frees memory and halts the program*/
            update_txt(&antlib, &synlib, ant, syn);
            free_list(&antlib);
            free_list(&synlib);
            fclose(usersave);
            printf("\nGoodbye\n");
            return 0;
            break;
        
        default:
            break;
        }
    }
    fclose(usersave);
    return 0;
}

void print_menu()
{
    printf("\n\n1.New question.\n2.Add a new synonym to a word.\n3.Add a new antonym to a word.\n4.Print the words list.\n5.Change user.\n6.Quit.\n\n");
}
/*Checks the users.txt if it's already created, if not creates a new one. Also checks the new user is in the list or not*/
FILE* check_users(char users_txt[], char username[])
{
    int read = 0, matched = 0, new = 1;
    char new_user[30];
    char saved_users[30];
    FILE *userdat = NULL;
    /* Opens the users.txt file to select from saved users*/
    FILE *usertxt = fopen(users_txt,"r+");
    if(usertxt == NULL) /* If there is not a users.txt file creates one*/
    {
        usertxt = fopen(users_txt,"w+");
        if(usertxt == NULL)
        {
            printf("Could not open the users.txt\n");
            return NULL;
        }
    }

    /* Prints saved users to screen to select*/
    printf("\t*Saved Users*\n");
    while(fscanf(usertxt,"%s",saved_users) != EOF)
    {   
        read = 1;
        printf("\t    %s\n", saved_users);
    }
    /* If the users.txt file exist but no users have been recorded create a new user*/
    if(read == 0)
    {
        printf("\t=> NONE <=\n\n");
        /*Create a new user*/
        printf("Please Enter a New User:");
        scanf(" %s",new_user);
        fprintf(usertxt,"%s\n",new_user);   /*Save the user to the txt file*/
        fclose(usertxt);
        strcpy(username, new_user);
        strcat(new_user,".worddat"); /*opens a binary file with name of user*/
        userdat = fopen(new_user,"wb+");
        if(userdat == NULL) return NULL;
        return userdat;
    }
    else/*If there is existing users*/
    {
        while (1)/*Repeat until correct input is taken*/
        {
            printf("Enter the users name (enter new for new user):");
            scanf(" %s",new_user);
            /*If the input is "new"*/
            if (strcmp(new_user,"new") == 0)
            {
                /*Create a new user*/
                printf("Please Enter a New User:");
                scanf(" %s",new_user);
                fseek(usertxt,0,SEEK_SET);
                new = 1;
                while(fscanf(usertxt,"%s",saved_users) != EOF) /*Cheks if there is already a user with that name*/
                {
                    if(strcmp(new_user,saved_users) == 0)   
                    {
                        printf("This username is in use, please pick a different name!\n");
                        new = 0;
                    }    
                }
                if(new == 1)/*If the username is not taken*/
                {
                    fprintf(usertxt,"%s\n",new_user);   /*Save the user to the txt file*/
                    fclose(usertxt);
                    strcpy(username, new_user);
                    strcat(new_user,".worddat");
                    userdat = fopen(new_user,"rb"); /*opens a binary file with name of user*/
                    if(userdat == NULL) /*If there is no binary files with that name creates one*/
                    {
                        userdat = fopen(new_user,"wb+");
                        if(userdat == NULL) return NULL;
                    }
                    return userdat;
                }
            } 
            else    /*If the input is not "new"*/
            {
                fseek(usertxt,0,SEEK_SET);
                while(fscanf(usertxt,"%s",saved_users) != EOF)  /*Checks if the user exist*/
                {
                    if(strcmp(new_user,saved_users) == 0)
                    {
                        fclose(usertxt);
                        strcpy(username, saved_users);
                        strcat(saved_users,".worddat");
                        userdat = fopen(saved_users,"rb+");  /*opens a binary file with name of user*/
                        if(userdat == NULL)/*If there is no binary files with that name creates one*/
                        {
                            userdat = fopen(saved_users,"wb+");
                            if(userdat == NULL) return NULL;
                        }
                        return userdat; 
                    }     
                }
                printf("User does not exist, Try Again !\n");
            }
        }
    }
}
/*Makes txt file to ready to be read*/
int procces_file(char *filename, lib **list, int counter)
{
    lib *temp = NULL;
    words *temp2 = NULL, *temp3 = NULL;

    int MAXCHAR = 0, MAXLENGTH = 0; /*Max length of one line and the max length of one input*/
    int line_count = 0;
    char *line, *buffer;
    char ch = '\0';
    int first_line = 0, line_length = 0, count_char = 0;

    /*opens the txt files to read*/
    FILE* file = fopen(filename,"r");
    if(file == NULL)
    {
        printf("Could not open the file\n");
        return -1;
    }
    /*Gets intial information from the txt file such as longest line and longest string*/
    while(fscanf(file,"%c",&ch) != EOF)
    {
        line_length++;
        if (ch == '\n' || ch == EOF)
        {
            line_count++;
            if(line_length >= MAXLENGTH) MAXLENGTH = line_length;
            line_length= 0;
        }
        count_char++;
        if(ch == ' ' || ch == '\n' || ch == '\0')
        {
            if(count_char >= MAXCHAR) MAXCHAR = count_char;
            count_char = 0;
        }

    }
    fseek(file,0,SEEK_SET);/*back to the start of the file*/

    /*Dynamically allocating a memory to line and buffer (for words)*/
    line = calloc(MAXLENGTH + 1, sizeof(char));
    buffer = calloc(MAXCHAR + 1, sizeof(char));

    line_length = 0;
    while(fscanf(file,"%c",&ch) != EOF)
    {
        line_length++;
        if (ch == '\n' || ch == EOF)
        {
            /*Moving the cursor back to start of the line, then getting the line*/
            fseek(file,-(line_length),SEEK_CUR);
            fgets(line,line_length,file); /*fscanf(file,"%[^\n]",line);*/
            fseek(file,1,SEEK_CUR);
            /*Getting inputs from line*/
            process_line(line, &counter, list, buffer);

            line_length = 0;
            counter++;
        }
    }
    /*Free allocated memory*/
    free(buffer);
    free(line);
    fclose(file);
    return counter;
}
/*Reads infos from txt file line by line then parses it to strings and creates a linked list*/
int process_line(char* line, int* counter, lib **list, char *buffer)
{
    lib *head = *list, *temp = NULL, *new = NULL;
    words *tempword = NULL;
    /*New node for the left most word */
    new = calloc(1,sizeof(lib));
    new->index = (*counter) + 1;
    new->correct = 0;
    new->incorrect = 0;
    new->probability = 1;
    new->nextline = NULL;
    new->nextword = NULL;

    /*parses the line string by string*/
    buffer = strtok (line," ");
    new->word = calloc((strlen(buffer) + 1),sizeof(char));
    strcpy(new->word,buffer);

    buffer = strtok (NULL," ");
    while (buffer != NULL)
    {
        if(buffer[0] != '=' && buffer[0] != '<')
        {   
            /*For eaach correct string creates a new words node*/
            add_node(&new, buffer);/*create word node and connect it to new->nextword*/
        }
        buffer = strtok (NULL, " ");
    }

    /*Connects the linked list either to beginnig or to end*/
    if(head == NULL)
    {
        head = new;
        *list = head;
        return 0;
    }
    for(temp = head; temp->nextline != NULL; temp = temp->nextline);
    temp->nextline = new;
    return 0;
}
/*Creates a new words node for synoyms and antonyms of the left most word*/
int add_node(lib **listmain, char *buffer)
{
    lib *list = *listmain;
    words *new = NULL, *temp = NULL;
    int a = 0;
    /*creates new node*/
    new = calloc(1,sizeof(words));
    new->word = calloc((strlen(buffer) + 1),sizeof(char));
    strcpy(new->word, buffer);
    a = strlen(buffer)-1;
    /*handles the carriage return in the txt*/
    if(buffer[a] == 13)
    {
        new->word[a] = '\0';
    }
    new->next = NULL;
    /*Connects node either at the beginning or at the end*/
    if(list->nextword == NULL)
    {
        list->nextword = new;
        return 0;
    }
    for(temp = list->nextword; temp->next != NULL; temp = temp->next);
    temp->next = new;
    return 0;
}
/*Frees all allocated memory*/
void free_list(lib **list1)
{
    lib *temp = NULL , *list = *list1;
    words *temp2 = NULL, *temp3 = NULL;

    while (list != NULL)
    {
        temp3 = list->nextword;
        while (temp3 != NULL)
        {
            temp2 = temp3->next;
            free(temp3->word);
            free(temp3);
            temp3 = temp2;
        }
        temp = list->nextline;
        free(list->word);
        free(list);
        list = temp;
    }
}
/*Prints the information in the lists*/
void print_list(lib **list)
{
    lib *temp = NULL;
    words *temp2 = NULL;
    int i = 1;
    for(temp = *list; temp != NULL; temp = temp->nextline, i++)
    {
        printf("%3d.%-15s   ==>", i, temp->word);
        for(temp2 = temp->nextword; temp2 != NULL; temp2 = temp2->next)
        {
            printf(" %-15s",temp2->word);
        }
        printf("\n");
    }
}
/*Asks a new question based on data*/
int new_question(lib *antlib,lib *synlib, datas *data)
{
    lib *list = NULL, *temp = NULL;
    words *temp2 = NULL;
    char question[10] = "\0", answer[50] = "\0", yes[5] = "\0";

    /*Decides the linked lists to be searched*/
    if(data->question_type == 0)
    {
        list = antlib;
        strcpy(question,"antonym");
    } 
    else
    {
        list = synlib;
        strcpy(question,"synonym");
    } 

    for(temp = list; temp != NULL; temp = temp->nextline)
    {   
        /*Decides the word to be asked*/
        if(data->words_index == temp->index)
        {
            printf("\n(Enter xxx to stop)\nWhat is the %s of %s: ", question, temp->word);
            scanf(" %s",answer);
            /*If user Enters xxx it stops the new question part*/
            if(strcmp(answer,"xxx") == 0)   return 0;
            
            /*When the answer is given to the question, it searches the other list to make the questions copy to have more chance to be asked*/
            if(data->question_type == 0)   check_matched(synlib, answer);
            else  check_matched(antlib, answer);
            
            for(temp2 = temp->nextword; temp2 != NULL; temp2 = temp2->next)
            {
                if(strcmp(answer,temp2->word) == 0)
                {
                    printf("\n**Correct Answer!**\n");
                    temp->correct++;
                    data->answer = 1;
                    return 1;
                }
            }
            /*When not correct asks to add a new word or not*/
            printf("**Incorrect Answer!**\n=>Do you want to add this word as correct answer?(yes/no)\n");
            scanf(" %s",yes);
            if(strcmp(yes,"yes") == 0)
            {
                add_node(&temp,answer); 
                temp->correct++;
                data->answer = 1;
                return 1;
            } 
            data->answer = 0;
            temp->incorrect++;
            return 1;
        }
    }
}
/*Cheks the given list for the same word and updates its value*/
int check_matched(lib *list, char name[])
{
    lib *temp = NULL;
    for(temp = list; temp != NULL; temp = temp->nextline)
    {
        if(strcmp(name,temp->word) == 0)
        {
            temp->incorrect++;
            return 0;
        }
    }
    return 0;
}
/*If the user wants to add a new word it checks if the word is already in the list if not then adds it*/
int check_list(lib *list, char buffer[])
{
    lib *temp = NULL;
    words *temp2 = NULL;
    char input[50] = "\0", new_word[50] = "\0";
    printf("Enter a word from a list to add a new %s to it: ",buffer);
    scanf(" %s",input);
    for(temp = list; temp != NULL; temp = temp->nextline)
    {   
        /*Finds the word that a new synoym or antonym will be added to*/
        if(strcmp(input,temp->word) == 0)
        {
            printf("\n%d.%s ", temp->index, temp->word);
            for(temp2 = temp->nextword; temp2 != NULL; temp2 = temp2->next)
            {
                printf(" ->%s",temp2->word);
            }
            printf("\n\n");
            printf("Enter the new %s of %s: ", buffer, temp->word);
            scanf(" %s",new_word);
            /*Cheks the input if it's already in the list or not*/
            for(temp2 = temp->nextword; temp2 != NULL; temp2 = temp2->next)
            {
                if(strcmp(new_word,temp2->word) == 0)
                {
                    printf("\n***This word is already in the list!***\n");
                    return 0;                
                }
            }
            /*adds the new word to the end of the line*/
            add_node(&temp,new_word);
            return 1;
        }
    }
    printf("\n***This word is not in the txt file!***\n");
    return 1;
}
/*Overwrites the old txt files with words in the linked lists*/
int update_txt(lib **antlib,lib **synlib, char ant[], char syn[])
{
    lib *list = NULL,*temp = NULL;
    words *temp2 = NULL;
    FILE *anttxt = NULL, *syntxt = NULL;
    /*opens antonyms.txt to overwrite it*/
    anttxt = fopen(ant,"w");
    list = *antlib;
    for(temp = list; temp != NULL; temp = temp->nextline)
    {
        fprintf(anttxt,"%s <>",temp->word);
        for(temp2 = temp->nextword; temp2 != NULL; temp2 = temp2->next)
        {
            fprintf(anttxt," %s",temp2->word);
        }
        fprintf(anttxt,"\n");
    }
    fclose(anttxt);
    
    /*opens synonyms.txt to overwrite it*/
    syntxt = fopen(syn,"w");
    list = *synlib;
    for(temp = list; temp != NULL; temp = temp->nextline)
    {
        fprintf(syntxt,"%s =",temp->word);
        for(temp2 = temp->nextword; temp2 != NULL; temp2 = temp2->next)
        {
            fprintf(syntxt," %s",temp2->word);
        }
        fprintf(syntxt,"\n");
    }
    fclose(syntxt);
}
/*Loads current users data from .worddat binary file*/
int load_userdata(user_t *cur_user, datas *data, lib **antlib, lib **synlib, FILE *usersave)
{
    lib *list = NULL, *temp = NULL;
    words *temp2 = NULL;
    int a;
    /*reads binary file*/
    while(a = fread(data,sizeof(datas),1,usersave)!= 0)
    {
        cur_user->ques_count++;
        if(data->answer == 1)   cur_user->trues++;
        else cur_user->falses++;

        if(data->question_type == 0)    list = *antlib;
        else list = *synlib;
        /*finds the words index in the binary file in the list*/
        for(temp = list; temp != NULL; temp = temp->nextline)
        {
            if(data->words_index == temp->index)
            {
                if(data->answer == 1)
                {   
                    temp->correct++;
                } 
                else temp->incorrect++;

                /*Cheks the other list for the word that asked, if found it's copy increases it's chances to be asked*/
                if(data->question_type == 0)   check_matched(*synlib, temp->word);
                else  check_matched(*antlib, temp->word); 
            }
        }
    }
    return 0; 
}
/*Main fucntion for calculating the probability of the word to be asked*/
int calc_probabilty(lib *antlib,lib *synlib,datas *data, int antcount, int totalcount)
{
    float found = 0 , totalProb = 0, avrg = 0, min = 0, max = 0;
    int index = 0,  count_avrg = 1, count_found = 0;
    lib *temp = NULL;
    /*Finds initial values that is needed*/
    totalProb = total_probabilty(antlib,totalProb);
    totalProb = total_probabilty(synlib,totalProb);
    avrg = totalProb / totalcount;
    count_avrg = find_min_max(antlib, synlib, avrg, &min, &max);
    /*After the initial values controls the words randomly and recursivly, the return values is the probabilty value*/
    found = recursive_prob_cal(antlib, synlib, avrg, count_avrg , totalProb, min, max);
    /*Finds the number of words with found probabilty value*/
    for(temp = antlib; temp != NULL; temp = temp->nextline)
    {
        if(temp->probability == found) count_found++;
    }
    for(temp = synlib; temp != NULL; temp = temp->nextline)
    {
        if(temp->probability == found) count_found++;
    }
    
    /*Randomly selects from one of the values that is equal to the found*/
    index = 1 + (rand() % count_found);
    count_found = 0;
    for(temp = antlib; temp != NULL; temp = temp->nextline)
    {
        if(temp->probability == found) count_found++;
        if(count_found == index)    return temp->index;
    }
    for(temp = synlib; temp != NULL; temp = temp->nextline)
    {
        if(temp->probability == found) count_found++;
        if(count_found == index)    return temp->index;
    }
}
/*Calculates total probabilty for initial calculation*/
float total_probabilty(lib *list, float sum)
{
    lib *temp = NULL;
    for(temp = list; temp != NULL; temp = temp->nextline)
    {
        sum += temp->probability;
    }
    return sum;
}
/*Resets values in the lists*/
void reset_list_values(lib **list)
{
    lib *temp = NULL;
    for(temp = *list; temp != NULL; temp = temp->nextline)
    {
        temp->incorrect = 0;
        temp->correct = 0;
    }
}
/*Recalculates probabilty value in the lists*/
void update_list_values(lib **list)
{
    lib *temp = NULL;
    for(temp = *list; temp != NULL; temp = temp->nextline)
    {   
        /*By doing this probabilty increases with every incorrect asnwer given by user, and decreases by correct answers.*/
        /*Bottom half of the equation is also the total numbers of questions asked with that word*/
        /*simply equation is false answers divided by the false + true answers (total numbers asked).*/
        temp->probability = (temp->incorrect + 1)/(temp->incorrect + 1 + temp->correct + 1);
    }
}
/*Calculates min max and counter values for initial calculation*/
float find_min_max(lib *antlib, lib *synlib, float avrg, float *min, float *max)
{
    lib *temp = NULL;
    float counter = 0, elementCount = 0;
    *min = 9999;
    *max = 0;
    for(temp = antlib; temp != NULL; temp = temp->nextline)
    {
        if(temp->probability < *min) *min = temp->probability;
        if(temp->probability > *max) *max = temp->probability;
        if(temp->probability < avrg) counter++;
        elementCount++;
    }
    for(temp = synlib; temp != NULL; temp = temp->nextline)
    {
        if(temp->probability < *min) *min = temp->probability;
        if(temp->probability > *max) *max = temp->probability;
        if(temp->probability < avrg) counter++;
        elementCount++;
    }
    return counter;
}
/*Recursivly and randomly determinates the next question to be asked to user*/
float recursive_prob_cal(lib *antlib,lib *synlib, float avrg, int count_avrg, float totalProb, float min, float max)
{
    int index = 0;
    lib *temp = NULL;
    /*Creates a random number which the maximum is probability number and assigns it to index*/
    /*Getting a number between  0 and total probability. it's like diving rand by 100 gives us 0 - 100 numbers.*/
    if(totalProb < 1) totalProb = 1.0;
    index = rand() % (2*(int)totalProb + 1);
    /*Return part of the recursive function indicates that a word is found*/
    if(min == max || count_avrg == 1 || min == avrg || max == avrg)
    {
        if(max - avrg > avrg - min)
        {
            for(temp = antlib; temp != NULL; temp = temp->nextline)
            {
                if(min == temp->probability) return min;
            }
            for(temp = synlib; temp != NULL; temp = temp->nextline)
            {
                if(min == temp->probability) return min;
            }
            return max;  
        } 
        return max;
    }
    /*if the index value is smaller than the number of elements until the avrg value of probabilities,function makes the max value avearage of the previous iteration*/
    /*Caounting the number of elements until the average possibility gives us a certain chance that we pick a new word either in upper half or the lower half.*/
    /*Since we count elemnts until the average value, if the number of elements until average is much bigger than the values which is bigger than the average value,*/
    /*that means lower half has hihger probability than the upper half.*/
    /*Think of this part as we have 100 values and we want %60 chance so we make the if condition 60 that means random value generated betweem 0 - 100 will have 60% chance to enter the if block*/
    if(index <= count_avrg)
    {   
        /*MAX value is average*/
        max = avrg;
        avrg = find_avrg(antlib,synlib, &min, &max, &totalProb, &count_avrg);
        /*Calls itself again but since max value is average list will look the bottom half of the elements*/
        recursive_prob_cal(antlib,synlib, avrg, count_avrg, totalProb, min, max);
    }
    /*if the index value is bigger than the number of elements until the avrg value of probabilities,function makes the min value avearage of the previous iteration*/
    else
    {   
        /*MIN value is average*/
        min = avrg;
        avrg = find_avrg(antlib, synlib, &min, &max , &totalProb, &count_avrg);
        /*Calls itself again since min value is average list will look to the upper half of the elements*/
        recursive_prob_cal(antlib,synlib, avrg, count_avrg, totalProb, min, max);
    }
}
/*calculates the average, min, max, total, probability and number of elements until the avrg value*/
float find_avrg(lib *antlib, lib *synlib, float *min, float *max,  float *total, int *avrg_count)
{
    lib *temp = NULL;
    float elementCount = 0, sum = 0, avrg = 0, start = *min, end = *max, newmin = 9999, newmax = 0;
    int counter = 0;
    
    /*Calculates new max and min, and sum of probabilities*/
    for(temp = antlib; temp != NULL; temp = temp->nextline)
    {
        if(temp->probability >= start && temp->probability <= end)
        {
            sum += temp->probability;
            elementCount++;
            if(temp->probability < newmin)  newmin = temp->probability;
            if(temp->probability > newmax)  newmax = temp->probability;
        }
    }
    for(temp = synlib; temp != NULL; temp = temp->nextline)
    {
        if(temp->probability >= start && temp->probability <= end)
        {
            sum += temp->probability;
            elementCount++;
            if(temp->probability < newmin)  newmin = temp->probability;
            if(temp->probability > newmax)  newmax = temp->probability;
        }
    }

    avrg = sum / elementCount;
    *min = newmin, *max = newmax;
    
    /*Calculates the number of words with less probability than the average proabbility*/
    for(temp = antlib; temp != NULL; temp = temp->nextline)
    {
        if(temp->probability >= start && temp->probability <= end)
        {
            if(temp->probability <= avrg) counter++;
        }
    }
    for(temp = synlib; temp != NULL; temp = temp->nextline)
    {
        if(temp->probability >= start && temp->probability <= end)
        {
            if(temp->probability <= avrg) counter++;
        }
    }
    /*REturns the values*/
    *total = sum;
    *avrg_count = counter;
    return avrg;
}
/*END OF THE FUNCTIONS*/
