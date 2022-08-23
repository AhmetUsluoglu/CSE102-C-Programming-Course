#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void fix_spelling_error(char *text, char *dict, int len);
void find_best_correction(FILE *dictfile, char word[30], int len);
int isAlphabetic(char ch);

int main(void)
{
    int length = 30;
    char* text = "text (copy).txt";
    char* dictionary = "dictionary.txt";
    /*Sending the 2 file names to the function*/
    fix_spelling_error(text, dictionary, length);
    printf("\nDone!\n\n");
    return 0;
}
/*Functions reads text and dictionary files and send teh mispelled words to best correction function*/
void fix_spelling_error(char *text, char *dict, int len)
{
    char original[30] = "\0", copy[30] = "\0", check[30] = "\0"; 
    int word_count = 0, counter = 0, mispelled = 1;
    FILE* textfile = fopen(text,"r+");
    FILE* dictfile = fopen(dict, "r");
    
    if (textfile == NULL)
    {
        printf("Could not open the textfile");
        return;
    }
    if (dictfile == NULL)
    {
        printf("Could not open the dictionary");
        return;
    }
    /*GEtting the word count to end program*/
    while (fscanf(textfile, "%s", original) && !feof(textfile))
    {
        word_count++;
    }
    fseek(textfile, 0, SEEK_SET);

    /*Loop iterates over every word in textfile and sends them to best correction if it can not finds the same word in dictionary*/
    while (fscanf(textfile, "%s", original) && !feof(textfile))
    {
        mispelled = 1;
        strcpy(copy,original);
        while(fscanf(dictfile, "%s", check) && !feof(dictfile))
        {
            if(strcmp(check, original) == 0) mispelled = 0;
        }
        fseek(dictfile, 0, SEEK_SET);

        /*Sending the mispelled words to best correction then moves the pointer backwards equal to length of the word in text then prints it to it's initial place*/
        if (mispelled == 1 && counter != word_count)
        {
            counter++;
            find_best_correction(dictfile, copy, strlen(copy));
            fseek(textfile, -strlen(original), SEEK_CUR);
            fprintf(textfile, "%s", copy);
        }
    }
    fclose(textfile);
    fclose(dictfile);
  
}
/* If character between a-z or A-Z return 1. else the character is not alphabetic return 0*/
int isAlphabetic(char ch)
{
    if ((ch <= 90 && ch >= 65) || (ch <= 122 && ch >= 97)) return 1;
    else return 0;
} 
/*Functions cheks the mispelled word part by part, corrects them, unites them again*/      
void find_best_correction(FILE *dictfile, char word[30], int len)
{
    char copy[30] = "\0", check[30] = "\0";
    char word1[30] = "\0", word2[30] = "\0", punc[2] = "\0";
    int mispelled = 1, ispunc = 0, i, x, a, word1_len, word2_len;

    /*Loop checks if the character is punction mark or not*/
    for(i = 0; i < len; i++)
	{
        /*Stores punction mark in char and change the value (ispunc) which indicates if there is punction to 1*/
		if (isAlphabetic(word[i]) == 0)
        {
            ispunc = 1;
            punc[1] = word[i];
            x = i;
        }
        /*If it's not punction mark stores them in word1*/
        if(ispunc == 0)
		{
			word1[i] = word[i];
		}
        /*If punction mark spotted then stores the after letters to word2*/
        else if(ispunc == 1 && x != i)
        {
            word2[i] = word[i];
        }  
	}

    word1_len = strlen(word1);
    word2_len = strlen(word2);
   
    /*Iterates over every word in dictionary and corrects separeted words if there is one*/
    /*BOTH - 1 VALUES NEEDS TO BE CHANGED TO -2 FOR CORRECTING 2 LETTER MISTAKES*/
    while (fscanf(dictfile, "%s",check) && !feof(dictfile))
    {
        a = 0;
        if(word1_len != 1 && strlen(check) == word1_len)
        {
            for(i = 0; i < word1_len; i++)
			{
				if(word1[i] == check[i])
				{
					a++;
				}
			}
            /*IF YOU CHANGE THIS VALUE TO - 2 PROGRAM WILL CORRECT UP TO TWO MISTAKES*/
			if(a >= (word1_len - 1))
			{
				strcpy(word1,check);
			}
			a = 0;	
        }

        else if(word2_len != 1 && strlen(check) == word2_len)
        {
            for(i = 0; i < word2_len; i++)
			{
				if(word2[i] == check[i])
				{
					a++;
				}
			}
            /*IF YOU CHANGE THIS VALUE TO - 2 PROGRAM WILL CORRECT UP TO TWO MISTAKES*/
			if(a >= (word2_len - 1))
			{
				strcpy(word2,check);
			}
			a = 0;	
        }
    }
    fseek(dictfile, 0, SEEK_SET);
    /*Final part , program unites separated and corrected words into one, then changes the original word*/
    strcpy(copy,word1);
    copy[word1_len] = punc[1];
    if(word2[1] != '\0') strcat(copy,word2);
    strcpy(word,copy);
}