#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Movie{
    double budget;
    int genre;
    char *name;
    double score;
    int year;
}Movie;


/*Copy of movies struct but every element is string, purpose is hold the infos read from lines*/
typedef struct Movie_temp{
    char *budget, *genre, *name, *score, *year;
}Movie_temp;

/*These Global Variables is implemented dynamically later on*/
Movie *movies;
Movie_temp movies_buffer;
char** genres;
int genre_count = 1;
int line_count = 0;
int MAXCHAR = 0;

/*Processing functions runs before menu and selection*/
int procces_file(char* movief);
int process_line(char* line, int* counter);
int new_genre();
int new_movie(int counter);
char** resize_array(char**array, int counter);
/*Selection and menu functions, rest of the functions called from start_program acoording to inputs*/
int start_program(char* movief);
void print_menu();
void average_imdb(int limit);
void print_all_info(int limit);
void list_genres(int limit);
/*Sorting Functions*/
void sort_score(double score, int until, int limit);
void min_max_score(double* maxi, double* mini, int limit);
void sort_year(int year, int until, int limit);
void min_max_year(double* maxi, double* mini, int limit);
void sort_by(int sort, int start, int end, int limit);
void sort_budget(int limit);
void sort_genre(int limit);
void sort_name(int limit);
void print_sorted(int start, int end);


int main()
{
    char* movief = "Movies.txt";
    start_program(movief);
    return 0;
}

int start_program(char* movief)
{
    int op1 = 0, op2 = 0, movie_count = 0, i = 0, year = 0, until = 0, start = 0, end = 0, sort = 0, isless = 0;
    double maxi = 0.0, mini = 9999.0, score = 0.0;

    /*Before Calling the Menu necessary processes of file reading and line parsing is done*/
    movie_count = procces_file(movief);

    /*GEtting the operations repedately*/
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
                        scanf(" %d",&start);
                        end = start;
                        if(start > 0 && end > 0)
                        {
                            sort_by(sort, start-1, end-1 , movie_count);
                        }
                        else  printf("\nInvaild Input\n");
                        
                    }
                    else if(op2 == 2)
                    {
                        printf("\nEnter Start Value:");
                        scanf(" %d",&start);
                        printf("\nEnter End Value:");
                        scanf(" %d",&end);
                        if(start > 0 && end > 0)
                        {
                            sort_by(sort, start-1, end-1, movie_count);
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
            for(i = 0; i < genre_count -1; i++)    printf("%s\n",genres[i]);
            break;
        /*Getting the min max years first than getting the inputs and checking the inputs and sending it to function*/
        case 3:
            min_max_year(&maxi, &mini, movie_count);
            do
            {
                printf("Enter a year:");
                scanf(" %d",&year);
                printf("Until (0) or Since (1)  %d:",year);
                scanf(" %d",&until);
                if(year > maxi || year < mini) printf("\nInvalid Input\n");
                else sort_year(year, until, movie_count);
            }while(year > maxi || year < mini);
            mini = 9999.0, maxi = 0.0;
            break;
        /*Getting the min max scores first than getting the inputs and checking the inputs and sending it to function*/
        case 4:
            min_max_score(&maxi, &mini, movie_count);
            do{
                printf("Enter a score:");
                scanf(" %lf",&score);
                printf("Lesser (0) or Greater (1)  %.1f:",score);
                scanf(" %d",&isless);
                if(score > maxi || score < mini) printf("\nInvalid Input\n");
                else sort_score(score, isless, movie_count);
            }while(score > maxi || score < mini);
            mini = 9999.0, maxi = 0.0;
            break;

        case 5:
            print_all_info(movie_count);
            break;

        case 6:
            average_imdb(movie_count);
            break;

        case 7:
            list_genres(movie_count);
            break;

        case 8:
            printf("Goodbye\n");
            for(i = 0; i < movie_count;i++)     free(movies[i].name);
            for(i = 0; i < genre_count - 1; i++)        free(genres[i]);
            free(genres);
            free(movies);
            return 0;
            break;

        default:
            printf("\nTRY AGAIN\n");
            break;
        }
    }
    return 0;
}

int procces_file(char*movief)
{
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
        if (ch == '\n' || ch == EOF)    line_count++;
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

    /*Dynamically allocte memory to movies by the amaount of movies(lines in file)*/
    movies = calloc(line_count, sizeof(Movie));

    /*Dynamically allocte memory for buffers*/
    movies_buffer.budget = calloc(MAXCHAR + 1,sizeof(char));
    movies_buffer.genre = calloc(MAXCHAR + 1,sizeof(char));
    movies_buffer.name = calloc(MAXCHAR + 1,sizeof(char));
    movies_buffer.score = calloc(MAXCHAR + 1,sizeof(char));
    movies_buffer.year = calloc(MAXCHAR + 1,sizeof(char));

    while(fscanf(file,"%c",&ch) != EOF)
    {
        line_length++;
        if (ch == '\n' || ch == EOF)
        {
            /*Dynamically allocating a memory to line*/
            line = calloc(line_length, sizeof(char));

            /*Moving the cursor back to start of the line, then getting the line*/
            fseek(file,-(line_length),SEEK_CUR);
            fgets(line,line_length,file); /*fscanf(file,"%[^\n]",line);*/
            fseek(file,1,SEEK_CUR);
            /*Getting inputs from line*/
            process_line(line, &counter);
            free(line);
            line_length = 0;
            counter++;
        }
    }

    free(movies_buffer.budget);
    free(movies_buffer.genre);
    free(movies_buffer.name);
    free(movies_buffer.score);
    free(movies_buffer.year);

    fclose(file);
    return counter;
}

int process_line(char* line, int* counter)
{
    int i = *counter , j = 0, k = 0;

    sscanf(line,"%[^,],%[^,],%[^,],%[^,],%[^,]",movies_buffer.budget, movies_buffer.genre, movies_buffer.name, movies_buffer.score, movies_buffer.year);

    /*If the current movies is not a new movie change i to index of the movie that found*/
    j = new_movie(*counter);
    if(j != 1)
    {
        i = j;
        free(movies[i].name);
        *counter = *counter-1;
    }

    /*Assign name*/
    movies[i].name = (char*)calloc((strlen(movies_buffer.name) + 1),sizeof(char));
    strcpy(movies[i].name,movies_buffer.name);

    /*Assign budget*/
    if(movies_buffer.budget[0] == 'u') movies[i].budget = -1;
    else movies[i].budget = atof(movies_buffer.budget);

    /*Assign score*/
    movies[i].score = atof(movies_buffer.score);

    /*Assign year*/
    movies[i].year = atoi(movies_buffer.year);

    /*Assign genre*/
    if(genre_count == 1 || new_genre() == 1)
    {
        genres = resize_array(genres, genre_count);
        genres[genre_count - 1] = calloc(strlen(movies_buffer.genre)+1,sizeof(char));
        strcpy(genres[genre_count - 1], movies_buffer.genre);
        /*printf("Genre Count %d\n",genre_count);*/
        genre_count++;
    }
    for(k = 0; k < genre_count - 1; k++)    if(strcmp(movies_buffer.genre, genres[k]) == 0)     movies[i].genre = k;

    return 0;
}

char** resize_array(char**array, int counter)
{
    int i;
    char** temp;
    /*Holding a pointer to the main array */
    temp = array;
    /*Allocating a new memory to the main array with a new size*/
    array = (char**)calloc(counter,(sizeof (char *)));
    /*Allocating memory for cahrs to strings and freeing the memory temp was holding*/
    for(i = 0; i < counter -1 ; i++)
    {
        array[i] = (char*)calloc(strlen(temp[i]) + 1,sizeof(char));
        strcpy(array[i],temp[i]);
        free(temp[i]);
    }
    /*Freeing the previosly allocated memory for main array by freeing temp, then returning the array*/
    free(temp);
    return array;
}

/*Checks if there is new genre in the array, if found returns the index else returns 1*/
int new_genre()
{
    int i = 0;
    for(i = 0; i < genre_count - 1; i++)
    {
        if(strcmp(genres[i],movies_buffer.genre) == 0) return 0;
    }
    return 1;
}
/*Checks if there is new movie in the array, if found returns the index else returns 1*/
int new_movie(int counter)
{
    int i = 0;
    for(i = 0; i < counter; i++)
    {
        if(strcmp(movies[i].name, movies_buffer.name) == 0) return i;
    }
    return 1;
}
/*Just Printing MENU :)*/
void print_menu()
{
    printf("\n1. List of the Sorted Data\n2. List of the Genres\n3. List of the Movie Through the Years\n");
    printf("4. List of the Movie Through the Scores\n5. All Informations of a Single Movie\n");
    printf("6. Average of the IMDB Scores\n7. Frequence of the Genres\n8. Exit\n\nPlease Select an operation:");
}
/*Calculating and printing the averages of imdb scores*/
void average_imdb(int limit)
{
    double sum = 0.0;
    int i = 0;
    for(i = 0; i < limit; i++) sum += movies[i].score;
    printf("\nAverage: %f\n",sum/limit);
}
/*Chenking the movie array if founds the movie prints it else prints warning*/
void print_all_info(int limit)
{
    char *name;
    int i = 0;
    name = calloc(MAXCHAR + 1, sizeof(char));
    while(1)
    {
        printf("\nPlease Enter The Name of the Movie:");
        scanf(" %[^\n]s ",name);
        for(i = 0; i < limit; i++)
        {
            if(strcmp(movies[i].name, name) == 0)
            {
                if(movies[i].budget == -1) printf("Budget : Unknown\n");
                else printf("Budget : %.0f\n",movies[i].budget);
                printf("Genre  : %s\n",genres[movies[i].genre]);
                printf("Name   : %s\n",movies[i].name);
                printf("Score  : %f\n",movies[i].score);
                printf("Year   : %d\n",movies[i].year);
                free(name);
                return;
            }
        }
    printf("\nCould not find the name\n");
    }
    free(name);
}
/*Prints the frequency of genres*/
void list_genres(int limit)
{
    int i = 0, j = 0, sum = 0;
    for(i = 0; i < genre_count - 1; i++)
    {
        for(j = 0; j < limit; j++)
        {
            if(i == movies[j].genre) sum++;
        }
        printf("%-10s    %5d\n", genres[i], sum);
        sum = 0;
    }
}
/*Getting the min max values*/
void min_max_year(double* maxi, double* mini, int limit)
{
    int i = 0;
    double max = 0.0, min = 9999.0;
    for(i = 0; i < limit; i++)
    {
        if(movies[i].year > max)   max = movies[i].year;
        if(movies[i].year < min)   min = movies[i].year;
    }
    printf("Min:%.0f  Max:%.0f\n", min, max);
    *maxi = max, *mini = min;
}

void sort_year(int year, int until, int limit)
{
    /*creating a temp movie struct to save infos temporarily*/
    Movie temp;
    int i = 0, j = 0, min = 0;

    /*At each loop look from the right of the current year to the all movies and find the max value*/
    /*at every turn moves max to the current index until the end of elements*/
    /*basically cheks every other index finds the current minimum in the array and swaps the indexes and continues from the sorted index*/
    for (i = 0; i < limit - 1; i++)
    {
        min = i;
        for (j = i + 1; j < limit; j++)
        {
            if (movies[j].year <= movies[min].year)  min = j;
        }
        temp = movies[min];
        movies[min] = movies[i];
        movies[i] = temp;
    }
    /*If the call comes from part1 this part will not print anything else it prints the names of movies in given range*/
    if(year != 0)
    {
        printf("\n");
        for (i = 0; i < limit - 1; i++)
        {
            if(until == 1)  if(movies[i].year >= year)   printf("%-80s\n",movies[i].name);
            if(until == 0)  if(movies[i].year <= year)   printf("%-80s\n",movies[i].name);
        }
    }
}
/*Getting the min max values*/
void min_max_score(double* maxi, double* mini, int limit)
{
    int i = 0;
    double max = 0.0, min = 10.0;
    for(i = 0; i < limit; i++)
    {
        if(movies[i].score > max)   max = movies[i].score;
        if(movies[i].score < min)   min = movies[i].score;
    }
    printf("Min:%.1f  Max:%.1f\n", min, max);
    *maxi = max, *mini = min;
}

void sort_score(double score, int isless, int limit)
{
    /*creating a temp movie struct to save infos temporarily*/
    Movie temp;
    int i = 0, j = 0, min = 0;

    /*At each loop look from the right of the current score to the all movies and find the max value*/
    /*at every turn moves max to the current index until the end of elements*/
    /*basically cheks every other index finds the current minimum in the array and swaps the indexes and continues from the sorted index*/
    for (i = 0; i < limit - 1; i++)
    {
        min = i;
        for (j = i + 1; j < limit; j++)
        {
            if (movies[j].score <= movies[min].score)  min = j;
        }
        temp = movies[min];
        movies[min] = movies[i];
        movies[i] = temp;
    }

/*If the call comes from part1 this part will not print anything else it prints the names of movies in given range*/
    if(score != 0)
    {
        printf("\n");
        for (i = 0; i < limit; i++)
        {
            if(isless == 1)     if(movies[i].score > score)     printf("%-80s\n",movies[i].name);
            if(isless == 0)     if(movies[i].score < score)     printf("%-80s\n",movies[i].name);
        }
    }
}

void sort_budget(int limit)
{
    /*creating a temp movie struct to save infos temporarily*/
    Movie temp;
    int i = 0, j = 0, min = 0;

    /*At each loop look from the right of the current budget to the all movies and find the max value*/
    /*at every turn moves max to the current index until the end of elements*/
    /*basically cheks every other index finds the current minimum in the array and swaps the indexes and continues from the sorted index*/
    for (i = 0; i < limit - 1; i++)
    {
        min = i;
        for (j = i + 1; j < limit; j++)
        {
            if (movies[j].budget <= movies[min].budget)  min = j;
        }
        temp = movies[min];
        movies[min] = movies[i];
        movies[i] = temp;
    }
}

void sort_genre(int limit)
{
    /*creating a temp movie struct to save infos temporarily*/
    Movie temp;
    int i = 0, j = 0, min = 0;

    /*At each loop look from the right of the current genre to the all movies and find the max value*/
    /*at every turn moves max to the current index until the end of elements*/
    /*basically cheks every other index finds the current minimum in the array and swaps the indexes and continues from the sorted index*/
    for (i = 0; i < limit - 1; i++)
    {
        min = i;
        for (j = i + 1; j < limit; j++)
        {
            if (strcmp(genres[movies[j].genre], genres[movies[min].genre]) < 0) min = j;
        }
        temp = movies[min];
        movies[min] = movies[i];
        movies[i] = temp;
    }
}

void sort_name(int limit)
{
    /*creating a temp movie struct to save infos temporarily*/
    Movie temp;
    int i = 0, j = 0, min = 0;

    /*At each loop look from the right of the current name to the all movies and find the max value*/
    /*at every turn moves max to the current index until the end of elements*/
    /*basically cheks every other index finds the current minimum in the array and swaps the indexes and continues from the sorted index*/
    for (i = 0; i < limit - 1; i++)
    {
        min = i;
        for (j = i + 1; j < limit; j++)
        {
            if (strcmp(movies[j].name, movies[min].name) < 0) min = j;
        }
        temp = movies[min];
        movies[min] = movies[i];
        movies[i] = temp;
    }
}

void sort_by(int sort, int start, int end, int limit)
{
    int i = 0, j = 0;

    /*Calling the sort function by the value of sort came from main*/
    /*Calling each sort function first then printing the results*/
    switch (sort)
    {
    case 1:
        /*Sorting the budget*/
        sort_budget(limit);
        /*Printing the budget*/
        for(i = 0, j = 0 ;j<limit, i <= end; j++)
        {
            if(movies[j].budget != -1 && i >= start && i <= end && j<limit)
            {
                printf("%2d.    " ,i+1);
                if(movies[j].budget == -1)  printf("Unkown      ");
                else printf("%-10.0f  ", movies[j].budget);
                printf("%-10s    %-80s    %-.1f  %-5d\n", genres[movies[j].genre], movies[j].name, movies[j].score, movies[j].year);
                i++;
            }
            if(movies[j].budget != -1) i++;            
        }
        if(j > limit) printf("\nInvalid Input due to unknown amount of budgets\n")
        break;

    case 2:
        sort_genre(limit);
        print_sorted(start, end);
        break;

    case 3:
        sort_name(limit);
        print_sorted(start, end);
        break;

    case 4:
        sort_score(0, 0, limit);
        print_sorted(start, end);
        break;

    case 5:
        sort_year(0, 0, limit);
        print_sorted(start, end);
        break;
    }
}

/*Prints movie infos between range*/
void print_sorted(int start, int end)
{
    int i = 0;
    for(i = start; i <= end; i++)
    {
        printf("%2d.    " ,i+1);
        if(movies[i].budget == -1)  printf("Unkown      ");
        else printf("%-10.0f  ", movies[i].budget);
        printf("%-10s    %-80s    %-.1f  %-5d\n", genres[movies[i].genre], movies[i].name, movies[i].score, movies[i].year);

    }
}

