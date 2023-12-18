//Zach Taylor
//This Project implements a BlockBuster database of movies from the Employee Side allowing for mutliple Employees to work from a shared database
//They will have the following options:
/*
    [1] Display Movie Selection
    [2] Process a Rental Request (DVD/VHS)
    [3] Process a DVD/VHS return
    [4] Reorder Movie Selection
    [5] Terminate

*/
#include "csapp.h"
#include <stdio.h>        //Pre-Processor Directives 
#include <stdbool.h>
void print_menu();       

static void result(int result) {   //function of result() that will help aid in the warning messages that we get when doing the scanf function
    (void)result; // Use the result to avoid the result warning message 
}


int main(int argc, char *argv[])  
{
    int clientfd;  //file descriptor to communicate with the server
    char *host, *port;
    size_t n; 
    char buffer [MAXLINE]; //MAXLINE = 8192 defined in csapp.h
    bzero(buffer,MAXLINE); 
    if (argc != 3)
    {
        fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
	    exit(0);
    }
    host = argv[1];
    port = argv[2];

    //client file descriptor that will hold the host and the port 
    clientfd = Open_clientfd(host, port);
    int user_choice;
    int w_result;    //variable declarations 


    //printf("Welcome BlockBuster Employee %s\n", employee_username);
    printf("Welcome to BlockBuster's media Management Center!\n");
    printf("Please select a request down below:\n");
    print_menu();    
    w_result = scanf("%d", &user_choice);  //using the scanf function to take in user input and if it is bad then it will display a B for bad 
    if(w_result != 1){
        printf("B");
    }
    //I deciced to use a while true loop when implementing this becuase I figured that inside oof it I could have an option that would breakout when the user enters 5 to terminate the loop
    while(1){
        //Option 1:Printing the entire database of media 
        if(user_choice == 1){                      //This part is DONE for the Client side 
            printf("\n"); //New line character for printing purposes
            char choice[] = "1";
            n = write(clientfd,choice,MAXLINE);                
            if(n<0){                      
                perror("Error!!"); //Printing out an Error message to let the user know that something got lost in connecting back to the client 
            }
            printf("Printing the DVD or VHS selection below:\n");                //Displaying a message to the user that you are now displaying the input recieved from the server 
            n = read(clientfd,buffer,MAXLINE);                   //Here you are now reading in the input from the server and Displaying that to the user 
            printf("%s\n",buffer);                               //Displaying the contents of the server here 
            bzero(buffer,MAXLINE);  
        }
        else if(user_choice == 2){ //If the user selects option 2 then we can drop into here and execute code
            printf("\n");         //Printing out a new line so that when we prompt the user a question it will not be underneath the menu selection
            char Movie_name[MAXLINE];
            char Production_Company[MAXLINE];
            char choice[] = "2";           //Since we are in option 2 we can send the character array of "2" to the server 
            n = write(clientfd,choice,strlen(choice));
            printf("Enter the Name of the Movie: "); 
            result(scanf("%s",Movie_name));                        //Prompting and asking the user for the appropriate information for option 2
            getchar();                                             //Then using the scanf function to take in the user input 
            n = write(clientfd,Movie_name,strlen(Movie_name));     //Then using the write() function to send the information to the server
            printf("Enter the Name of the Production Company: ");
            result(scanf("%s",Production_Company));                         
            getchar();
            n = write(clientfd,Production_Company,strlen(Production_Company));
            if(n<0)
                perror("Error!!");
            printf("Message from the server: \n");         
            n = read(clientfd,buffer,MAXLINE);      
            printf("%s\n",buffer);          
            printf("\n");                                
            bzero(buffer,MAXLINE);                  
        }
        else if(user_choice == 3){       
            printf("\n");         //Printing out a new line so that when we prompt the user a question it will not be underneath the menu selection
            char Movie_name[MAXLINE];
            char Production_Company[MAXLINE];
            char choice[] = "3";           //Since we are in option 3 we can send the character array of "3" to the server 
            n = write(clientfd,choice,strlen(choice));
            printf("Enter the Name of the Movie: "); 
            result(scanf("%s",Movie_name));                        //Prompting and asking the user for the appropriate information for option 3
            getchar();                                             //Then using the scanf function to take in the user input 
            n = write(clientfd,Movie_name,strlen(Movie_name));     //Then using the write() function to send the information to the server
            printf("Enter the Name of the Production Company: ");
            result(scanf("%s",Production_Company));                         
            getchar();
            n = write(clientfd,Production_Company,strlen(Production_Company));
            if(n<0)
                perror("Error!!");
            printf("Message from the server: \n");         
            n = read(clientfd,buffer,MAXLINE);      
            printf("%s\n",buffer);          
            printf("\n");                                
            bzero(buffer,MAXLINE);                  
        }
         else if(user_choice == 4){
             printf("\n");
             printf("Note: BlockBuster Reordering system will order an additional 5 copies\n");
             char Movie_name[MAXLINE];
            char Production_Company[MAXLINE];
            char choice[] = "4";           //Since we are in option 4 we can send the character array of "4" to the server 
            n = write(clientfd,choice,strlen(choice));
            printf("Enter the Name of the Movie: "); 
            result(scanf("%s",Movie_name));                        //Prompting and asking the user for the appropriate information for option 4
            getchar();                                             //Then using the scanf function to take in the user input 
            n = write(clientfd,Movie_name,strlen(Movie_name));     //Then using the write() function to send the information to the server
            printf("Enter the Name of the Production Company: ");
            result(scanf("%s",Production_Company));                         
            getchar();
            n = write(clientfd,Production_Company,strlen(Production_Company));
             if(n<0)
                perror("Error!!");
            printf("Message from the server: \n");         
            n = read(clientfd,buffer,MAXLINE);      
            printf("%s\n",buffer);          
            printf("\n");                                
            bzero(buffer,MAXLINE);    
         }
        else if(user_choice == 5){   
            char choice[] = "5";       
            n = write(clientfd,choice,strlen(choice)); 
            break;
        }
        print_menu();                  
        user_choice = 0;       
        w_result = scanf("%d", &user_choice);  
        if(w_result != 1){
            printf("B");
        }
        }
    
        printf("Connection Closed!!\n"); //Once the user has selected option 5 now and is out of the while loop we can now display a message that the connection has closed between the Server and Client
        return 0; //returning zero back to the main function 
}
//function of void print_menu() defined below
void print_menu(){                  //In this function I am prompting the user of all of the available options that he or she can pick from 
    printf("[1] Display Movie Selection\n");            //Complete
    printf("[2] Process a Rental Request (DVD/VHS)\n"); //Complete
    printf("[3] Process a DVD/VHS return\n");          //Complete
    printf("[4] Reorder Movie Selection\n");        //Complete
    printf("[5] Terminate\n");                        //Complete
    printf("Select an option [1,2,3,4 or 5]: ");
}
