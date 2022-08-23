#include <stdio.h>

typedef struct customer{
    char name[20];
    int age;
    int label;
}customer;

int add_customer(customer customers[], customer temp, int index, int seq);
void print_sequence(customer customers[],int index);
void procces_customer(customer customers[]);
void fill_labels(customer customers[]);
int find_customer_index(customer customers[], int label, int seq);
int check_seq (customer customers[], int label,int seq,int start, int *end);

int main()
{
    customer customers[50], temp;
    int op = 1, seq = 0, index = 0;
    /*Initialize all labels with -1 to use easily later on*/
    fill_labels(customers);
    printf("****Banking System****\n");
    /*Loops until the operation input is 3*/
    while(1)
    {
        if(seq == 50) printf("Can not add more than 50 customers, processing automatically\n");
        printf("Current Sequence:");
        print_sequence(customers,seq);
        printf("1 - Add Customer\n2 - Process Customer\n3 - Quit\n");
        scanf("%d",&op);
        if(op == 3) 
        {
            printf("Goodbye!\n");
            return 0;
        }
        /*adds customer if it's also not he 50th customer*/
        else if(op == 1 && seq < 50)
        {
            printf("Please Enter the name of the customer:");
            scanf("%s",temp.name);
            printf("Please Enter the age of the customer:");
            scanf("%d",&temp.age);
            printf("Please Enter the label of the customer:");
            scanf("%d",&temp.label);
            /*First customer in the sequence will be assigned manually*/
            if(seq == 0)
            {
                customers[0] = temp;

            }
            /*Finds the new customers index which the customer will be in the waiting list*/
            else index = find_customer_index(customers, temp.label, seq);
            
            /*Adds cutomer to the index that returned from find_customer_index function*/
            add_customer(customers, temp, index, seq);
            seq++;
        }
        /*Process customers by selection or automatically the 50th customer*/
        else if(op == 2 || seq == 50)
        {
            if (seq == 0) printf("Please Add Customer\n");
            printf("Proceed Customer is %s\n",customer[0].name)
            procces_customer(customers);
            seq--;
        }
    }
    return 0;
}
/*Fills labels with -1*/
void fill_labels(customer customers[])
{
    int i;
    for(i = 0; i < 50; i++)
    {
        customers[i].label = -1;
    }
}
/*Prints sequence*/
void print_sequence(customer customers[],int index)
{
    int i;
    if (index == 0) printf(" ");
    else
    {
        for(i = 0; i < index;i++)
        {
            printf("%d ",customers[i].label);
        }
    }
    printf("\n");
}
/*Adds customer to the desired index*/
int add_customer(customer customers[], customer temp, int index, int seq)
{
    int i;
    for(i = seq; i >= index; i--)
    {
        customers[i + 1]= customers[i]; 
    }
    customers[index] = temp;
}
/*Process the first customer in the line*/
void procces_customer(customer customers[])
{
    if (customers[0].label == -1) return;
    customers[0].label = customers[1].label;
    procces_customer(customers + 1);
}
/*Finds customers' index*/
int find_customer_index(customer customers[], int label, int seq)
{
    int start = 0, i, end = 0, limit = 0, set = 0;
    /*Starts from 0.th index to the end */
    for(i = start; i < seq; i++)
    {
        if(label == customers[i].label) 
        {
            /*Checks if the sequence is at limit or not for given label. ex. 5 times for 0 , 3 times for 1, 2 times for 4*/
            limit = check_seq (customers, label, seq, i, &end);
            /*if limit is not reached function returns end as index, if reached functions continus to loop from the end of the limit and*/ 
            /*searches for new index for customer*/
            if (limit == 0) return end;
            else 
            {
                i = end;
                set = 1;
            }
        }
        /*Conditions for fair system for waiting, gives priorities to lower labels*/
        else if ((label > customers[i].label || label <  customers[i + 1].label) && (customers[i].label > customers[i + 1].label)) return i + 1;
        else if ((label > customers[i].label && label <  customers[i + 1].label)) return i + 1;
        else if(label < customers[i].label && set == 0) return i; 
    }
    return seq;
}
/*Gets the label and checks forward by the amount of limit value, returns 1 for reached limit 0 for not reached and returns the index to be continued*/   
int check_seq (customer customers[], int label,int seq,int start, int *end)
{
    int limit, i, j;
    if(label == 0) limit = 5;
    else if (label == 1 || label == 2) limit = 3;
    else if (label == 3 || label == 4) limit = 2;

    for(i = start; i < start + limit; i++)
    {
        if(customers[i].label != label)
        {
            *end = i;
            return 0;
        }
    }
    *end = i;
    return 1;
}