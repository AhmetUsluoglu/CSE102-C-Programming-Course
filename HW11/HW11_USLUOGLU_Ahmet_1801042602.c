/*The txt file from moodle has a empty last line so my program works according the provided movies file*/
/*If the program will be checked in the Valgrind it will need to be waited arrounf 10-20 seconds, does not need wait if compiled normally*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Copy of movies struct but every element is string, purpose is hold the infos read from lines*/
typedef struct Movie_temp{
    char *budget, *genre, *name, *score, *year;
}Movie_temp;

/*Movie budget for creating the list contains name year budget list*/
typedef struct Movie_Budget
{   
    char *name;
    int year;
    double budget;
    struct Movie_Budget *next;
} Movie_Budget;

/*Movie budget for creating the list contains name year budget list*/
typedef struct Movie_Name
{
    char *name;
    char *genre;
    double score;
    struct Movie_Name *next;
} Movie_Name;

/*Processing functions runs before menu and selection*/
int procces_file(char* movief,Movie_Budget **list_budget, Movie_Name **list_name, Movie_temp *movies_buffer);
int process_line(char* line, int* counter,Movie_Budget **list_budget, Movie_Name **list_name, Movie_temp *movies_buffer);
void insert_year(Movie_Budget **list_budget, Movie_temp *movies_buffer);
Movie_Budget *find_location(Movie_Budget *head, int year, double budget);
int new_movie(int counter, Movie_temp *movies_buffer, Movie_Budget *list_budget, Movie_Name *list_name);

/*Selection and menu functions, rest of the functions called from start_program acoording to inputs*/
int start_program(char* movief);
void print_menu();
void average_imdb(Movie_Name *list_name);
void print_all_info(Movie_Budget *list_budget, Movie_Name *list_name);

/*Sorting Functions*/
void min_max_score(double* maxi, double* mini, Movie_Name *list_name);
void min_max_year(double* maxi, double* mini, Movie_Budget *list_budget);
void sort_by(int sort, double start, double end, Movie_Budget *list_budget, Movie_Name *list_name, int limit, int operation);
void sort_score(Movie_Name *list_name);
void sort_year(Movie_Budget *list_budget);
void sort_budget(Movie_Budget *list_budget);
void sort_genre(Movie_Name *list_name);
void sort_name(Movie_Name *list_name);
void print_sorted(double start, double end, Movie_Budget *list_budget, Movie_Name *list_name);


/*Main function calls start program and sends the name of the file*/
int main()
{
    char* movief = "Movies.txt";
    start_program(movief);
    return 0;
}

int start_program(char* movief)
{
    int op1 = 0, op2 = 0, movie_count = 0, i = 0, year = 0, until = 0, sort = 0, isless = 0;
    double maxi = 0.0, mini = 9999.0, score = 0.0, start = 0.0, end = 0.0;
    Movie_temp movies_buffer;
    Movie_Budget *list_budget = NULL, *temp =NULL;
    Movie_Name *list_name = NULL, *temp_name = NULL;
    
    /*Before Calling the Menu necessary processes of file reading and line parsing is done*/
    movie_count = procces_file(movief,&list_budget,&list_name, &movies_buffer);
    if(movie_count == -1)   return -1;
    /*Getting the operations repedately*/
    while(1)
    {
        print_menu();
        scanf(" %d",&op1);
        printf("\n");
        switch (op1)
        {
        /*Selecting operations with switch*/
        /*Part1 programs takes 2 operatior inputs and decides the operation and sends the inputs to sort by function*/    
        case 1: 
            while(sort < 1 || sort > 5)
            {
                printf("\n1.Budget\n2.Genre\n3.Name\n4.Score\n5.Year\n\nPlease Select An Operation:");
                scanf(" %d",&sort);
                
                while((op2 != 1 && op2 != 2) &&(sort >= 1 || sort <= 5))
                {
                    printf("\n1.Single Selection\n2.Multiple Selection\n\nPlease Select An Operation:");
                    scanf(" %d",&op2);
                    /*If single selection is selected program sends the end value same as the start value*/
                    if(op2 == 1)
                    {
                        printf("\nEnter Value:");
                        scanf(" %lf",&start);
                        end = start;
                        if(start > 0 && end > 0 && end <= movie_count && start <= movie_count)
                        {
                            sort_by(sort, start, end , list_budget, list_name, movie_count,op2);
                        }
                        else  printf("\nInvaild Input\n");
                        
                    }
                    else if(op2 == 2)
                    {
                        printf("\nEnter Start Value:");
                        scanf(" %lf",&start);
                        printf("\nEnter End Value:");
                        scanf(" %lf",&end);
                        if(start > 0 && end > 0)
                        {
                            /*If the inputs is not in range when name or genre is selected gives warning*/
                            if((sort == 2 || sort == 3) && (start > movie_count || end > movie_count))
                            {
                                printf("Invalid Input\n");
                            }
                            else sort_by(sort, start, end, list_budget, list_name, movie_count,op2);
                        }
                        else  printf("\nInvaild Input\n");
                        
                    }
                    else printf("\nINVALID INPUT\n");
                }
                if (sort < 1 || sort > 5)   printf("\nINVALID INPUT\n");
            }
            sort = 0, op2 = 0;
            break;
        
        /*printing list of genres*/
        case 2:
            /*sorting the list first*/
            sort_genre(list_name);
            /*Printing the genres one by one if the later genre is different than the previous one*/
            for (temp_name = list_name; temp_name->next != NULL; temp_name = temp_name->next)
            {
                if(strcmp(temp_name->genre,temp_name->next->genre)!= 0)     printf("%-15s\n",temp_name->genre);
            }
            printf("%-15s\n",temp_name->genre);
            break;
        
        /*Getting the min max years first than getting the inputs and checking the inputs and sending it to function*/
        case 3:
            /*finding the min max values than sorting the list first*/
            min_max_year(&maxi, &mini, list_budget);
            sort_year(list_budget);
            do
            {
                printf("Enter a year:");
                scanf(" %d",&year);
                printf("Until (0) or Since (1)  %d:",year);
                scanf(" %d",&until);
                if(year > maxi || year < mini) printf("\nInvalid Input\n");
                else 
                {   
                    for (temp = list_budget; temp != NULL; temp = temp->next)
                    {
                        if(until == 1)  if(temp->year >= year)   printf("%-80s  \n",temp->name);
                        if(until == 0)  if(temp->year <  year)   printf("%-80s  \n",temp->name);
                    }
                }
            }while(year > maxi || year < mini);
            mini = 9999.0, maxi = 0.0;
            break;
        
        /*Getting the min max scores first than getting the inputs and checking the inputs and sending it to function*/
        case 4:
            /*finding the min max values than sorting the list first*/
            min_max_score(&maxi, &mini, list_name);
            sort_score(list_name);
            do{
                printf("Enter a score:");
                scanf(" %lf",&score);
                printf("Lesser (0) or Greater (1)  %.1f:",score);
                scanf(" %d",&isless);
                if(score > maxi || score < mini) printf("\nInvalid Input\n");
                else 
                {
                    for (temp_name = list_name; temp_name != NULL; temp_name = temp_name->next)
                    {
                        if(isless == 1)  if(temp_name->score >= score)   printf("%-80s  \n",temp_name->name);
                        if(isless == 0)  if(temp_name->score <=  score)   printf("%-80s  \n",temp_name->name);
                    }
                }
            }while(score > maxi || score < mini);
            mini = 9999.0, maxi = 0.0;
            break;

        case 5:
            print_all_info(list_budget, list_name);
            break;

        case 6:
            average_imdb(list_name);
            break;

        case 7:
            /*sorting the list first*/
            sort_genre(list_name);
            /*Printing the genres one by one if the later genre is different than the previous one*/
            for (i = 0, temp_name = list_name; temp_name->next != NULL; temp_name = temp_name->next)
            {
                i++;
                if(strcmp(temp_name->genre,temp_name->next->genre)!= 0)
                {
                    printf("%-15s   %-5d\n",temp_name->genre, i);
                    i = 0;
                }
            }
            printf("%-15s   %-5d\n",temp_name->genre, ++i); 
            break;

        case 8:
            while (list_budget!= NULL)
            {
                temp = list_budget->next;
                free(list_budget->name);
                free(list_budget);
                list_budget= temp;
            }
            while (list_name!= NULL)
            {
                temp_name = list_name->next;
                free(list_name->name);
                free(list_name->genre);
                free(list_name);
                list_name = temp_name;
            }
            printf("Goodbye\n");
            return 0;
            break;   
        
        default:
            printf("\nTRY AGAIN\n");
            break;
        }
    }
    return 0;
}

int procces_file(char*movief,Movie_Budget **list_budget, Movie_Name **list_name, Movie_temp *movies_buffer)
{
    int MAXCHAR = 0, MAXLENGTH = 0; /*Max length of one line and the max length of one input*/
    int line_count = 0;
    char *line;
    char ch = '\0';
    int first_line = 0, line_length = 0, counter = 0, count_char = 0;
    /*opening and checking the file*/
    FILE* file = fopen(movief,"r");
    if(file == NULL)
    {
        printf("Could not open the file\n");
        return -1;
    }

    /*Counts number of lines by counting the "\n" characters, then goes back to star of the file, also counting the largest input between commas nad the length of the first line*/
    while(fscanf(file,"%c",&ch) != EOF)
    {
        line_length++;
        if (ch == '\n' || ch == EOF)
        {
            line_count++;
            if(line_length >= MAXLENGTH) MAXLENGTH = line_length;
            line_length= 0;
        }    
        if (line_count == 0)  first_line++;
        count_char++;
        if(ch == ',')
        {
            if(count_char >= MAXCHAR) MAXCHAR = count_char;
            count_char = 0;
        }
    }
    fseek(file,0,SEEK_SET);
    fseek(file,first_line +1,SEEK_CUR);

    /*Dynamically allocating a memory to line*/
    line = calloc(MAXLENGTH + 1, sizeof(char));

    /*Dynamically allocte memory for buffers*/
    movies_buffer->budget = calloc(MAXCHAR + 1,sizeof(char));
    movies_buffer->genre  = calloc(MAXCHAR + 1,sizeof(char));
    movies_buffer->name   = calloc(MAXCHAR + 1,sizeof(char));
    movies_buffer->score  = calloc(MAXCHAR + 1,sizeof(char));
    movies_buffer->year   = calloc(MAXCHAR + 1,sizeof(char));

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
            process_line(line, &counter, list_budget, list_name, movies_buffer);

            line_length = 0;
            counter++;
        }
    }

    /*Freeing the buffers*/
    free(movies_buffer->budget);
    free(movies_buffer->genre);
    free(movies_buffer->name);
    free(movies_buffer->score);
    free(movies_buffer->year);

    free(line);
    fclose(file);
    return counter;
}

int process_line(char* line, int* counter, Movie_Budget **list_budget, Movie_Name **list_name, Movie_temp *movies_buffer)
{
    int i = *counter , j = 0, k = 0;
    Movie_Name *head = *list_name, *temp = *list_name;
    Movie_Name *new;
    /*Parsing the line and saving the inputs to dynamic string buffers*/
    sscanf(line,"%[^,],%[^,],%[^,],%[^,],%[^,]",movies_buffer->budget, movies_buffer->genre, movies_buffer->name, movies_buffer->score, movies_buffer->year);

     /*If the current movies is not a new movie j is 0, if it's j is 1*/
    j = new_movie(*counter, movies_buffer,*list_budget, *list_name);
    if(j == 0)
    {
        /*Allocating new memory for new Movie Name node and assigning the values*/
        new = calloc(1,sizeof(Movie_Name));
        new->name = (char*)calloc((strlen(movies_buffer->name) + 1),sizeof(char));
        strcpy(new->name, movies_buffer->name);
        new->genre = (char*)calloc((strlen(movies_buffer->genre) + 1),sizeof(char));
        strcpy(new->genre, movies_buffer->genre);
        new->score = atof(movies_buffer->score);
        new -> next = NULL;
        /*Adding the new node at the begining of the list ---- this list is not descending order because this is movie name list*/
        new->next = temp;
        head = new;
        *list_name = head;
    }
    else    /*If the movie already exist*/
    {
        for(temp = *list_name; temp != NULL; temp = temp->next)
        {   
            /*Finding the movie in the list name and changing the values*/
            if(strcmp(temp->name, movies_buffer->name) == 0)
            {   
                temp->score = atof(movies_buffer->score);
                free(temp->genre);
                temp->genre = (char*)calloc((strlen(movies_buffer->genre) + 1),sizeof(char));
                strcpy(temp->genre, movies_buffer->genre);
            }
        }
        *counter = *counter-1;
    }
    /*Insert new node to the list budget descendingly*/
    insert_year(list_budget, movies_buffer);
    return 0;
}
/*******************************************************************************/

/*Insert new node to the list budget descenging by year and budget*/
void insert_year(Movie_Budget **list_budget, Movie_temp *movies_buffer)
{
    Movie_Budget **head = list_budget;
    Movie_Budget* insert_here = *head;
    Movie_Budget* temp = *head;
    int year = atoi(movies_buffer->year);
    double budget = 0.0;

    /*Creating the new node and assigning the values*/    
    Movie_Budget *new = calloc(1,sizeof(Movie_Budget));
    new->name = (char*)calloc((strlen(movies_buffer->name) + 1),sizeof(char));
    strcpy(new->name, movies_buffer->name);
    new -> year = year;
    if(movies_buffer->budget[0] == 'u') budget = -1;
    else budget = atof(movies_buffer->budget);
    new->budget = budget;
    new -> next = NULL;
    
    /*If its the first element insert it at the beginning*/
    if(*head == NULL) 
    {   
        *head = new;
        return; 
    }

    /*If the new nodes year lesser than the already existing the node find a correct place to insert it*/
    if(temp->year >= year)
    {   
        /*Finds location descendingly by year and budget*/
        insert_here = find_location(temp,year,budget);
        /*If the return location is pointing the head node and year valu is equal to it, check budget value if it's greater insert it at the beggining*/
        if(insert_here == *head && insert_here->year == year && insert_here->budget < budget)
        {
            new->next = insert_here;
            *head = new;
            return;
        }
        /*If next is available insert it there*/
        if(insert_here->next != NULL)
        {
            new->next = insert_here->next;
            insert_here->next = new;
        }
        else insert_here->next = new; 
    }   
    else /*If the new nodes year is bigger than the head node just insert it at the begining*/
    {
        new->next = insert_here;
        *head = new;
    }
}
/*Finds correct location descendingly by year and budget*/
Movie_Budget *find_location(Movie_Budget *head, int year, double budget)
{
    Movie_Budget *temp = head;
    Movie_Budget *temp_prev = temp;

    /*new year is lesser than the current node*/
    while(temp->year >= year)
    {
        if(temp->year == year)/*If its equal check the budgets*/
        {
            if(temp->budget <= budget)/*return the previous node's pointer if the new budget is bigger*/
            {
                return temp_prev;
            }
        }
        /*new year is lesser than the next 2 nodes and current node is not NULL*/
        if(temp->next != NULL && temp->next->year >= year)
        {
            /*Move one node forward and hold the previous nodes pointer*/
            temp_prev = temp;
            temp = temp->next;
        }    
        else return temp; /*Returns the current node*/
    }
    return temp;    /*Returns the current node*/
}

/*Checks if there is new movie in the array, if found returns the index else returns 1*/
int new_movie(int counter, Movie_temp *movies_buffer, Movie_Budget *list_budget, Movie_Name *list_name)
{
    Movie_Budget *temp = NULL, *prev = NULL, *rmv =NULL;
    Movie_Name  *temp_n = NULL, *prev_n = NULL;
    int i = 0;
    for(temp = list_budget; temp != NULL; prev = temp, temp = temp->next)
    {
        if(strcmp(temp->name, movies_buffer->name) == 0)
        {
            /*When existing movie is found hold its adress, then connect previous node to next node, then free the found node*/
            rmv = temp;
            prev->next = temp->next;
            free(temp->name);
            free(temp);
            return 1;
        }
    }
    return 0;
}
/*Just Printing MENU :)*/
void print_menu()
{
    printf("\n1. List of the Sorted Data\n2. List of the Genres\n3. List of the Movie Through the Years\n");
    printf("4. List of the Movie Through the Scores\n5. All Informations of a Single Movie\n");
    printf("6. Average of the IMDB Scores\n7. Frequence of the Genres\n8. Exit\n\nPlease Select an operation:");
}
/*Calculating and printing the averages of imdb scores by traversing the list*/
void average_imdb(Movie_Name *list_name)
{
    double sum = 0.0, div = 0.0;
    Movie_Name *temp = NULL;
    for(temp = list_name; temp!=NULL; temp = temp->next, div++)     sum += temp->score;
    printf("\nAverage: %f\n",sum/div);
}
/*Chenking the movie lists and if founds the movie prints it else prints warning*/
void print_all_info(Movie_Budget *list_budget, Movie_Name *list_name)
{
    Movie_Name *temp_name;
    Movie_Budget *temp_budget;
    char *name;
    int i = 0;
    /*Allocating a memory for input to compare it to the movies' names*/
    name = calloc(100, sizeof(char));
    while(1)
    {
        printf("\nPlease Enter The Name of the Movie:");
        scanf(" %[^\n]s ",name);
        for(temp_budget = list_budget; temp_budget != NULL; temp_budget = temp_budget->next)
        {
            if(strcmp(temp_budget->name, name) == 0)
            {   
                for(temp_name = list_name; temp_name != NULL; temp_name = temp_name->next)
                {
                    
                    if(strcmp(temp_name->name,name) == 0)
                    {
                        if(temp_budget->budget == -1) printf("Budget : Unknown\n");
                        else printf("Budget : %.0f\n",temp_budget->budget);
                        printf("Genre  : %s\n",temp_name->genre);
                        printf("Name   : %s\n",temp_name->name);
                        printf("Score  : %.1f\n",temp_name->score);
                        printf("Year   : %d\n",temp_budget->year);
                        free(name);
                        return;
                    }
                }
            }
        }
        printf("\nCould not find the name\n");
    }
    free(name);
}

/*Getting the min max values in the list budget by traversing it*/
void min_max_year(double* maxi, double* mini, Movie_Budget *list_budget)
{   
    Movie_Budget *temp = NULL;
    int i = 0;
    double max = 0.0, min = 9999.0;
    for(temp = list_budget; temp!=NULL; temp=temp->next)
    {
        if(temp->year > max)   max = temp->year;
        if(temp->year < min)   min = temp->year;
    }
    printf("Min:%.0f  Max:%.0f\n", min, max);
    *maxi = max, *mini = min;
}

/*Sorts year ascendingly by using a BUBBLE SORT algorithm*/
void sort_year(Movie_Budget *list_budget)
{
    Movie_Budget *head = NULL, *temp_val = NULL, *temp = NULL;
    /*creating a temp movie struct to save infos temporarily*/
    temp_val = calloc(1,sizeof(Movie_Budget));
    head = list_budget; /*Head starts from the begining*/
    /*This loop starts from beggining and moves every value which is out of order to right until it finds value bigger than itself*/
    /*At each iteration it moves the biggest number that out of place to the end of the list until all of them is listed*/
    for(head = list_budget; head != NULL; head = head->next)    /*Moves to the next node until the end of the list and starts new loop to check all rest of the nodes*/
    {
        for(temp = head->next; temp != NULL; temp = temp->next)
        {
           if(head->year > temp->year)  /*If the head node is bigger than the next node swap the values*/
            {
              temp_val->year = head->year;
              temp_val->budget = head->budget;
              temp_val->name = head->name;

              head->year = temp->year;
              head->budget = temp->budget;
              head->name = temp->name;

              temp->year = temp_val->year;
              temp->budget = temp_val->budget;
              temp->name = temp_val->name;
            }
        }
    }
    free(temp_val);/*freein the temporary value*/
}
/*Getting the min max values in the list name by traversing it*/
void min_max_score(double* maxi, double* mini, Movie_Name *list_name)
{
    Movie_Name *temp = NULL;
    int i = 0;
    double max = 0.0, min = 9999.0;
    for(temp = list_name; temp!=NULL; temp=temp->next)
    {
        if(temp->score > max)   max = temp->score;
        if(temp->score < min)   min = temp->score;
    }
    printf("Min:%.1f  Max:%.1f\n", min, max);
    *maxi = max, *mini = min;
}

/*Sorts score ascendingly by using a BUBBLE SORT algorithm*/
void sort_score(Movie_Name *list_name)
{
    Movie_Name *head = NULL, *temp_val = NULL, *temp = NULL;
    /*creating a temp movie struct to save infos temporarily*/
    temp_val = calloc(1,sizeof(Movie_Budget));
    /*Head starts from the begining*/
    head = list_name;
    /*This loop starts from beggining and moves every value which is out of order to right until it finds value bigger than itself*/
    /*At each iteration it moves the biggest number that out of place to the end of the list until all of them is listed*/
    for(head = list_name; head != NULL; head = head->next)  /*Moves to the next node until the end of the list and starts new loop to check all rest of the nodes*/
    {
        for(temp = head->next; temp != NULL; temp = temp->next)
        {
            if(head->score > temp->score)    /*If the head node is bigger than the next node swap the values*/
            {
              temp_val->score = head->score;
              temp_val->genre = head->genre;
              temp_val->name = head->name;

              head->score = temp->score;
              head->genre = temp->genre;
              head->name = temp->name;

              temp->score = temp_val->score;
              temp->genre = temp_val->genre;
              temp->name = temp_val->name;
            }
        }
    }
    free(temp_val);
}
/*Sorts budget ascendingly by using a BUBBLE SORT algorithm*/
void sort_budget(Movie_Budget *list_budget)
{
    Movie_Budget *head = NULL, *temp_val = NULL, *temp = NULL;
    /*creating a temp movie struct to save infos temporarily*/
    temp_val = calloc(1,sizeof(Movie_Budget));
    
    head = list_budget;
    /*This loop starts from beggining and moves every value which is out of order to right until it finds value bigger than itself*/
    /*At each iteration it moves the biggest number that out of place to the end of the list until all of them is listed*/
    for(head = list_budget; head != NULL; head = head->next)    /*Moves to the next node until the end of the list and starts new loop to check all rest of the nodes*/
    {
        for(temp = head->next; temp != NULL; temp = temp->next)
        {
           if(head->budget > temp->budget)  /*If the head node is bigger than the next node swap the values*/
            {
              temp_val->year = head->year;
              temp_val->budget = head->budget;
              temp_val->name = head->name;

              head->year = temp->year;
              head->budget = temp->budget;
              head->name = temp->name;

              temp->year = temp_val->year;
              temp->budget = temp_val->budget;
              temp->name = temp_val->name;
            }
        }
    }
    free(temp_val);
}

/*Sorts genre ascendingly by using a BUBBLE SORT algorithm*/
void sort_genre(Movie_Name *list_name)
{
    Movie_Name *head = NULL, *temp_val = NULL, *temp = NULL;
    /*creating a temp movie struct to save infos temporarily*/
    temp_val = calloc(1,sizeof(Movie_Budget));
    
    head = list_name;
    /*This loop starts from beggining and moves every value which is out of order to right until it finds value bigger than itself*/
    /*At each iteration it moves the biggest number that out of place to the end of the list until all of them is listed*/
    for(head = list_name; head != NULL; head = head->next)  /*Moves to the next node until the end of the list and starts new loop to check all rest of the nodes*/
    {
        for(temp = head->next; temp != NULL; temp = temp->next)
        {
           if(strcmp(head->genre, temp->genre) > 0) /*If the head node is bigger than the next node swap the values*/
            {
              temp_val->score = head->score;
              temp_val->genre = head->genre;
              temp_val->name = head->name;

              head->score = temp->score;
              head->genre = temp->genre;
              head->name = temp->name;

              temp->score = temp_val->score;
              temp->genre = temp_val->genre;
              temp->name = temp_val->name;
            }   
        }
    }
    free(temp_val);
}
/*Sorts name ascendingly by using a BUBBLE SORT algorithm*/
void sort_name(Movie_Name *list_name)
{
    Movie_Name *head = NULL, *temp_val = NULL, *temp = NULL;
    /*creating a temp movie struct to save infos temporarily*/
    temp_val = calloc(1,sizeof(Movie_Budget));
    
    head = list_name;
    /*This loop starts from beggining and moves every value which is out of order to right until it finds value bigger than itself*/
    /*At each iteration it moves the biggest number that out of place to the end of the list until all of them is listed*/
    for(head = list_name; head != NULL; head = head->next)  /*Moves to the next node until the end of the list and starts new loop to check all rest of the nodes*/
    {
        for(temp = head->next; temp != NULL; temp = temp->next)
        {
           if(strcmp(head->name, temp->name) > 0) /*If the head node is bigger than the next node swap the values*/
            {
              temp_val->score = head->score;
              temp_val->genre = head->genre;
              temp_val->name = head->name;

              head->score = temp->score;
              head->genre = temp->genre;
              head->name = temp->name;

              temp->score = temp_val->score;
              temp->genre = temp_val->genre;
              temp->name = temp_val->name;
            }   
        }
    }
    free(temp_val);
}

/*Calls other sorting functions via checking the operation flags*/
void sort_by(int sort, double start, double end, Movie_Budget *list_budget, Movie_Name *list_name, int limit, int operation)
{
    int i = 0, j = 0;
    Movie_Budget *temp = NULL;
    Movie_Name *temp_name = NULL;

    /*Calling the sort function by the value of sort came from main*/
    /*Calling each sort function first then printing the results*/
    /*Operation == 1 means that it's a single choice so prints accordingly*/

    /*All the printings works as: first find the movie in a list that it's sorted according to, 
    then finds the same name in the other list which is not sorted by that type,then print the infos*/
    switch (sort)
    {
    case 1:/*While printing the budget i did not include the unknown budget values as it was in HW10, but the unknown values are also sorted but not printed*/
        /*Sorting the budget*/
        sort_budget(list_budget);
        /*Printing the budget*/
        if(operation == 1)
        {
            for(i = 0, temp = list_budget; temp != NULL, i <= end, j < limit; temp = temp->next, ++j)
            {   
                if(j == limit - 1) printf("Invalid Input\n");
                if(temp->budget != -1)  i++;
                if(i >= start && i <= end)
                {
                    for(temp_name = list_name; temp_name != NULL; temp_name = temp_name->next)
                    {
                        if(strcmp(temp_name->name,temp->name) == 0)
                        {   
                            printf("%5d.    " ,i);
                            if(temp->budget == -1) printf("Unknown         ");
                            else printf("%-16.0f",temp->budget);
                            printf("%-10s   %-80s    %-5.1f  %-6d\n",temp_name->genre, temp->name, temp_name->score, temp->year);
                        }
                    }
                }
            }
        }
        else
        {
            for(i = 0, temp = list_budget; temp != NULL; temp = temp->next)
            {
                if(temp->budget >= start && temp->budget <= end)
                {
                    for(temp_name = list_name; temp_name != NULL; temp_name = temp_name->next)
                    {
                        if(strcmp(temp_name->name,temp->name) == 0)
                        {   
                            printf("%5d.    " ,++i);
                            if(temp->budget == -1) printf("Unknown         ");
                            else printf("%-16.0f",temp->budget);
                            printf("%-10s   %-80s    %-5.1f  %-6d\n",temp_name->genre, temp->name, temp_name->score, temp->year);
                        }
                    }
                }
            }
        }
        break;

    case 2:
        /*Sorting the genre*/
        sort_genre(list_name);
        /*printing the genre*/
        print_sorted(start, end, list_budget, list_name);
        break;

    case 3:
        /*Sorting the name*/
        sort_name(list_name);
        /*printing the name*/
        print_sorted(start, end, list_budget, list_name);
        break;

    case 4:
        /*Sorting the score*/
        sort_score(list_name);
        /*Printing the  scores*/
        if(operation == 1)    print_sorted(start, end, list_budget, list_name);
        else
        {
            for(i = 0, temp_name = list_name; temp_name != NULL; temp_name = temp_name->next)
            {
                if(temp_name->score >= start &&  temp_name->score <= end)
                {
                    for(temp = list_budget; temp != NULL; temp = temp->next)
                    {
                        if(strcmp(temp_name->name,temp->name) == 0)
                        {   
                            printf("%5d.    " ,++i);
                            if(temp->budget == -1) printf("Unknown         ");
                            else printf("%-16.0f",temp->budget);
                            printf("%-10s   %-80s    %-5.1f  %-6d\n",temp_name->genre, temp->name, temp_name->score, temp->year);
                        }
                    }
                }
            }
        }
        break;

    case 5:
        /*Sorting the years*/
        sort_year(list_budget);
        /*Printing the years*/
        if(operation == 1)
        {
            for(i = 1, temp = list_budget; temp != NULL, i <= end; temp = temp->next, i++)
            {
                if(i >= start && i <= end)
                {
                    for(temp_name = list_name; temp_name != NULL; temp_name = temp_name->next)
                    {
                        if(strcmp(temp_name->name,temp->name) == 0)
                        {   
                            printf("%5d.    " ,i);
                            if(temp->budget == -1) printf("Unknown         ");
                            else printf("%-16.0f",temp->budget);
                            printf("%-10s   %-80s    %-5.1f  %-6d\n",temp_name->genre, temp->name, temp_name->score, temp->year);
                        }
                    }
                }
            }
        }
        else
        {
            for(i = 0, temp = list_budget; temp != NULL; temp = temp->next)
            {
                if(temp->year >= start && temp->year <= end)
                {
                    for(temp_name = list_name; temp_name != NULL; temp_name = temp_name->next)
                    {
                        if(strcmp(temp_name->name,temp->name) == 0)
                        {   
                            printf("%5d.    " ,++i);
                            if(temp->budget == -1) printf("Unknown         ");
                            else printf("%-16.0f",temp->budget);
                            printf("%-10s   %-80s    %-5.1f  %-6d\n",temp_name->genre, temp->name, temp_name->score, temp->year);
                        }
                    }
                }
            }
        }
        break;
    }
}

/*Prints movie infos between range*/
void print_sorted(double start, double end, Movie_Budget *list_budget, Movie_Name *list_name)
{
    int i = 1;
    Movie_Budget *temp = NULL;
    Movie_Name *temp_name = NULL;
    for(i = 1, temp_name = list_name; temp_name != NULL, i <= end; temp_name = temp_name->next, i++)
    {
        if(i >= start && i <= end)
        {
            for(temp = list_budget; temp != NULL; temp = temp->next)
            {
                if(strcmp(temp_name->name,temp->name) == 0)
                {   
                    printf("%5d.    " ,i);
                    if(temp->budget == -1) printf("Unknown         ");
                    else printf("%-16.0f",temp->budget);
                    printf("%-10s   %-80s    %-5.1f  %-6d\n",temp_name->genre, temp->name, temp_name->score, temp->year);
                }
            }
        }
    }
}
