#include<stdio.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>
#include<errno.h>

#include "add_record.h"
#include "modify_book.h"
#include "layout.h"
#include "database_lookup.h"
#include "manager_menu.h"

time_t current;
struct tm* pLocal;
int choice;

int isbn_validation(char search_term[],int search_field){

        /**
        * The function search the isbn_no in the csv file and validates it if present or not.
        * @param[in] search_term 	Term to match in database.
        * @param[in] search_field 	A number indicating which field of the table to search
        * @param[out] isFound	Acknowledge to the calling function if isbn_no is present or not.
        */

        char buff[255];
        int isFound = 0;
        FILE *returnFile = lookup(search_term,1,search_field);

        if (returnFile == NULL){
            switch(errno){
                case ENOENT: printf ("The file doesn't exist\n");
                             break;
                    default: printf("The error number is %d\n", errno);
            }
        }

        while (fgets(buff, 1024, returnFile)){

            char *field = strtok(buff, "\",\"");

            field = strtok(NULL, "\",\"");
            field = strtok(NULL, "\",\"");

            if(strcmp(field,search_term)==0){
                isFound = 1;
                break;
            }
        }

        return isFound;
}

int increment_id(){

       /**
       * The function search the last book_id present in csvfile auto-increment it and sent back to the calling function
       * @param[out] increment Incremented book_id sent back to the calling function
       */

       struct books *a = malloc(sizeof(struct books));
       char buf[255];
       char *field;
       int increment;

       FILE *mainFile =fopen("data/bookdetails.csv","r");

       if (mainFile == NULL){
            switch(errno){
                case ENOENT: printf ("The file doesn't exist\n");
                             break;
                    default: printf("The error number is %d\n", errno);
            }
       }

       while (fgets(buf, 1024, mainFile)){
         field = strtok(buf, "\",\"");
       }

       field = strtok(buf, "\",\"");
       increment = atoi(field)+1;

    return increment;
}

void add_book()
{
        /**
        * The function adds the book details to the csv file.Validation for duplicate
          isbn_no if already present in the csv file and auto-increment for the book_id as
          going on adding new books has also been implemented.
        */

        struct books *a = malloc(sizeof(struct books));

        system(CLEAR);
        header_layout("ADD BOOK");

        gotoxy(40,11);printf("ENTER BELOW DETAILS:");

         //fields for the adding book
        gotoxy(40,14);printf("Book ID:");
            int increment = increment_id();
            //converting int into char array
            snprintf(a->book_id, sizeof(20), "%d", increment);
        gotoxy(59,14);printf("%s",a->book_id);

        gotoxy(40,16);printf("Book Name:");
    new_book:
        gotoxy(59,16);scanf("%[^\n]%*c",a->book_title);
        gotoxy(40,18);printf("Author:");
        gotoxy(59,18);scanf("%[^\n]%*c",a->author_name);
        gotoxy(40,20);printf("Quantity:");
        gotoxy(59,20);scanf("%[^\n]%*c",a->quantity);

        gotoxy(40,22);printf("ISBN No:");
   isbn_repeat:
        gotoxy(59,22);scanf("%[^\n]%*c",a->isbn_no);
        fflush(stdout);

        //duplicate isbn validation check from the database table
        if(isbn_validation(a->isbn_no,3)==1){
            gotoxy(45,24);printf("The isbn number already exists\n");
            gotoxy(45,25);printf("Please choose one of the filter to proceed");
            gotoxy(45,26);printf("1. Move to modify page to change isbn number ");
            gotoxy(45,27);printf("2. Enter new ISBN Number ");

            gotoxy(45,28);printf("Select the filter : ");
            gotoxy(69,28);scanf("%d",&choice);

            if(choice == 1){
                //move to modify book to change the book details of the entered isbn_no
                modify_book();
            }
            else if(choice == 2){
                gotoxy(59,22);printf("                                                                           ");
                gotoxy(45,24);printf("                                                                           ");
                gotoxy(45,25);printf("                                                                           ");
                gotoxy(45,26);printf("                                                                           ");
                gotoxy(45,27);printf("                                                                           ");
                gotoxy(45,28);printf("                                                                           ");
                gotoxy(59,28);printf("                                                                           ");
                getchar();
                goto isbn_repeat;
            }
            else{
                gotoxy(45,30);printf("Invalid filter.Please try again");
                getchar();
                gotoxy(59,16);printf("                                                                           ");
                gotoxy(45,30);printf("                                                                           ");
                gotoxy(40,18);printf("                                                                           ");
                gotoxy(59,18);printf("                                                                           ");
                gotoxy(40,20);printf("                                                                           ");
                gotoxy(69,20);printf("                                                                           ");
                gotoxy(40,22);printf("                                                                           ");
                gotoxy(59,22);printf("                                                                           ");
                gotoxy(45,24);printf("                                                                           ");
                gotoxy(45,25);printf("                                                                           ");
                gotoxy(45,26);printf("                                                                           ");
                gotoxy(45,27);printf("                                                                           ");
                gotoxy(45,28);printf("                                                                           ");
                gotoxy(59,28);printf("                                                                           ");
                goto new_book;
            }
        }

        gotoxy(40,24);printf("Category:");
        gotoxy(59,24);scanf("%[^\n]%*c",a->category);
        gotoxy(40,26);printf("Language:");
        gotoxy(59,26);scanf("%[^\n]%*c",a->language);

        gotoxy(40,28);printf("Date of Publication (DD-MM-YYYY format):");
        gotoxy(82,28);scanf("%s",a->date_of_pub);

        gotoxy(40,30);printf("Entry Date : ");

            time(&current);
            pLocal = localtime(&current);
            strftime(a->entry_date, sizeof(a->entry_date), "%d-%m-%Y", pLocal);

        gotoxy(59,30);puts(a->entry_date);

        gotoxy(40,32);printf("Status:");
        gotoxy(59,32);scanf("%s",a->status);

        void *struct_data = a;

        if(add_record(struct_data,1) == 1){
             getchar();
             gotoxy(40,34);printf("Book added successfully");
             gotoxy(40,36);printf("Press any key to continue to main menu...");
             getchar();
             manager_menu();
        }
        else{
            printf("Error in saving details");
        }
}

