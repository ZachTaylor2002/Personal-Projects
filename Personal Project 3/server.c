//Zach Taylor
//server.c  
//Personal Project
//This server.c will take in the information passed in from the client.c and perform the actions and then write back to the mutiple clients.
#include "csapp.h"
//Declaration of the struct of Arguments
struct Arguments {  //Declaring a struct of "Arguments" that will hold the file descriptor, the client hostname, and the client port 
    int connfd;     //Decided to make this structure becuase the pthread_create function only takes in four arguments and needs this as way to pass in more.  
    char client_hostname[MAXLINE];       
    char client_port[MAXLINE];
};

//Declaration of a mutex and setting it equal to default values 
int readcnt=0;
sem_t mutex,w;
//Declaration of the struct of Movies_Info
typedef struct {   //Declaration of the struct of Movies_info that holds the data members of the movie
  char MovieType[MAX_LENGTH];
  char Movie_Title[MAX_LENGTH];
  char Movie_Rating[MAX_LENGTH];
  char Production[MAX_LENGTH];
  int year_released;
  int Number_Of_Copies;
} Movies_Info;

//definition of the void server function that will take in the connection file descriptor as well as the character arrays of the hostname and port
void serverFunction(int connfd){
    FILE *file_object;                        //Declaring a file_object so that operations can be done with the movies.csv
    Movies_Info Movies[MAX_SIZE];           //Declaring an array of Movies of type 
    char line[MAXLINE];                       //character array of line that will be used to send the information back to the client
    int count = 0;                           //counter variable to keep up with the total amount of people in the employee array
    file_object = fopen("movies.csv", "r");   //Opening the file for reading 
    if (file_object == NULL) {
        printf("Unable to open the file.\n");  //Print out an error message if it doesn't open
    }

    //Adding the record from movies.csv to the movies structure
    while (fgets(line, sizeof(line), file_object)) {
    sscanf(line, "%[^,],%[^,],%[^,],%[^,],%d,%d", Movies[count].MovieType,
           Movies[count].Movie_Title, Movies[count].Movie_Rating,
           Movies[count].Production, &Movies[count].year_released,
           &Movies[count].Number_Of_Copies);
    count++;
  }

    fclose(file_object);//closing the file 
    char buffer[MAXLINE]; //MAXLINE = 8192 defined in csapp.h 
    size_t n;
    if(n<0){
        perror("Error!!");
    }
    //While loop to continue until we have reached option 5 (which will break out)
    while(1){
         bzero(buffer,MAXLINE);                     //before we do any option we will clean out the buffer and read in the first option
         n = read(connfd, buffer, MAXLINE);        //we will then store the character answer in the option character array and use the atoi to convert to 
        int action_option;                         //string to integer
        char option[MAXLINE];
        sscanf(buffer, "%[^\n]", option);        //Here I am using the sscanf function that will stop until we get to the end of a new line
        action_option = atoi(option);
        bzero(buffer,MAXLINE);
    if(action_option == 1){     //Option 1 will send back to the client the entire database of the movies 
        P(&mutex);
        readcnt++;
        if (readcnt == 1) 
            P(&w);
        V(&mutex);
        char  movie_format[MAXLINE];
        file_object = fopen("movies.csv", "r"); //Opening the file for reading
        count=0;
        while (fgets(line, sizeof(line), file_object)) {  //Storing the contents into the array of structure 
    sscanf(line, "%[^,],%[^,],%[^,],%[^,],%d,%d", Movies[count].MovieType,
           Movies[count].Movie_Title, Movies[count].Movie_Rating,
           Movies[count].Production, &Movies[count].year_released,
           &Movies[count].Number_Of_Copies);
    count++;
  }
            fclose(file_object);
        bzero(buffer, MAXLINE);
        bzero(line, MAXLINE);
        P(&mutex);
        readcnt--;
        if (readcnt == 0) /* Last out */
            V(&w);
        V(&mutex);
            for(int index = 0; index < count; index++){ //For loop that will iterate throught the struct and writing back to the client   
                strcat(line, Movies[index].MovieType);
                strcat(line, ",");                
                strcat(line, Movies[index].Movie_Title);
                strcat(line, ",");
                strcat(line, Movies[index].Movie_Rating);
                strcat(line, ",");
                strcat(line, Movies[index].Production);
                strcat(line, ",");
                char year[MAX_LENGTH];
                snprintf(year, sizeof(year), "%d", Movies[index].year_released);
                strcat(line, year);
                strcat(line, ",");
                char copies[MAX_LENGTH];
                snprintf(copies, sizeof(copies), "%d", Movies[index].Number_Of_Copies);
                strcat(line, copies);
                strcat(line, "\n");
            }
            n = write(connfd, line, MAXLINE);//Writing back the information back to the client 
            bzero(line, MAXLINE);
            bzero(buffer,MAXLINE);
    }
    else if(action_option == 2){
        char  movie_title[MAXLINE];
        char  production[MAXLINE];
        char rental_message [] = "Rental Request Granted!!\n";
        char denied_message [] = "Not Enough Copies left:(Limited Stock!!)\n";
        char not_found [] = "Invalid Movie Request\n";
        n = read(connfd, buffer, MAXLINE);
        sscanf(buffer, "%[^\n]", movie_title);
        bzero(buffer,MAXLINE);
        n = read(connfd, buffer, MAXLINE);
        sscanf(buffer, "%[^\n]", production);
        bzero(buffer,MAXLINE);
        int match_found = 0; // Flag variable to track if a match is found
        for (int index = 0; index < count; index++) { //For loop  to iterate through the array of movies struct and doing a comparison to see if the movie and production are the same as what the client has passed in.
            if ((strcmp(Movies[index].Movie_Title, movie_title) == 0) && (strcmp(Movies[index].Production, production) == 0)) {
        if (Movies[index].Number_Of_Copies > 0) { //Checking to see if the number of copies is greater than zero
            Movies[index].Number_Of_Copies--;  //Decreasing the number of copies by one since this is a rental request
            n = write(connfd, rental_message, MAXLINE); //Writing back to the client the rental granted message 
            match_found = 1; // Set flag indicating a match is found
            break; // Exit loop once a match is found and processed
            }
        else if(Movies[index].Number_Of_Copies == 0){
            n = write(connfd,denied_message, MAXLINE); //If the number of copies is equal to zero then write back saying not enough copies left to rent out 
            match_found = 1; // Set flag indicating a match
            break;
        }
        }
    }
        if (!match_found) { //If the movie or the production company wasnt found then write back to client that the movie is an invalid request 
            n = write(connfd,not_found, MAXLINE);
        }
        P(&w);
        file_object = fopen("movies.csv", "w");//Writing back the information in the database
        if (file_object == NULL) {
            printf("Error opening file.\n");
        return 1;
    }
    for (int index = 0; index < count; index++) {
        fprintf(file_object, "%s,%s,%s,%s,%d,%d\n", Movies[index].MovieType,
                Movies[index].Movie_Title, Movies[index].Movie_Rating,
                Movies[index].Production, Movies[index].year_released,
                Movies[index].Number_Of_Copies);
    }
    fclose(file_object);
    V(&w);
    bzero(line, MAXLINE);
    bzero(buffer,MAXLINE);
    }
    else if(action_option == 3){
        char rental_message [] = "Rental Return Accepted!!\n";
        char not_found [] = "Invalid Movie Request\n";
        char Movie_Name[MAXLINE];
        char PRoduction_Company[MAXLINE];
        n = read(connfd, buffer, MAXLINE);
        sscanf(buffer, "%[^\n]", Movie_Name);
        bzero(buffer,MAXLINE);
        n = read(connfd, buffer, MAXLINE);
        sscanf(buffer, "%[^\n]", PRoduction_Company);
        bzero(buffer,MAXLINE);                      
        bzero(buffer,MAXLINE);
        int match_found = 0; 
        for (int index = 0; index < count; index++) {//For loop  to iterate through the array of movies struct and doing a comparison to see if the movie and production are the same as what the client has passed in.
            if ((strcmp(Movies[index].Movie_Title, Movie_Name) == 0) && (strcmp(Movies[index].Production,PRoduction_Company) == 0)) {
                Movies[index].Number_Of_Copies++;//Incrementing the number of copies by once since this is a rental acceptance 
                n = write(connfd, rental_message, MAXLINE);//Writing back to the client the Rental was Accepted
                match_found = 1; // Set flag indicating a match is found
                break; // Exit loop once a match is found and processed
            }
        }
        if (!match_found) { //Write back the invalid movie request
            n = write(connfd,not_found, MAXLINE);
        }
        P(&w);
        file_object = fopen("movies.csv", "w");//Writing back the informatio back to the database
        if (file_object == NULL) {
            printf("Error opening file.\n");
        return 1;
    }
    for (int index = 0; index < count; index++) {
        fprintf(file_object, "%s,%s,%s,%s,%d,%d\n", Movies[index].MovieType,
                Movies[index].Movie_Title, Movies[index].Movie_Rating,
                Movies[index].Production, Movies[index].year_released,
                Movies[index].Number_Of_Copies);
    }
    fclose(file_object);
    V(&w);
    bzero(line, MAXLINE);
    bzero(buffer,MAXLINE);
    }
    else if(action_option == 4){
        char rental_message [] = "BlockBuster Reorder Successful!!\n";
        char not_found [] = "Invalid Movie Request\n";
        char movie_title [MAXLINE];
        char production [MAXLINE];
        n = read(connfd, buffer, MAXLINE);
        sscanf(buffer, "%[^\n]", movie_title);
        bzero(buffer,MAXLINE);
        n = read(connfd, buffer, MAXLINE);
        sscanf(buffer, "%[^\n]", production);
        bzero(buffer,MAXLINE);
        int match_found = 0; 
        for (int index = 0; index < count; index++) {//For loop  to iterate through the array of movies struct and doing a comparison to see if the movie and production are the same as what the client has passed in.
            if ((strcmp(Movies[index].Movie_Title, movie_title) == 0) && (strcmp(Movies[index].Production,production) == 0)) {
                Movies[index].Number_Of_Copies+=5;//Since this is a restock add/order an additonal 5 copies
                n = write(connfd, rental_message, MAXLINE);
                match_found = 1; // Set flag indicating a match is found
                break; // Exit loop once a match is found and processed
            }
        }
        if (!match_found) {
            n = write(connfd,not_found, MAXLINE);
        }
        P(&w);
        file_object = fopen("movies.csv", "w");
        if (file_object == NULL) {
            printf("Error opening file.\n");
        return 1;
    }
    for (int index = 0; index < count; index++) {
        fprintf(file_object, "%s,%s,%s,%s,%d,%d\n", Movies[index].MovieType,
                Movies[index].Movie_Title, Movies[index].Movie_Rating,
                Movies[index].Production, Movies[index].year_released,
                Movies[index].Number_Of_Copies);
    }
    fclose(file_object);
    V(&w);
    bzero(line, MAXLINE);
    bzero(buffer,MAXLINE);
    }
    else if(action_option == 5){  //Option 5, breakout of the while true loop
         break;
    }
}
}
void *thread(void *vargp){ //thread function 
    struct Arguments *arguments = (struct Arguments *)vargp;
    int connfd = (*arguments).connfd;
    char *client_hostname = (*arguments).client_hostname;
    char *client_port = (*arguments).client_port;
    pthread_detach(pthread_self());
    free(vargp);
    serverFunction(connfd);
    Close(connfd);
    printf("(%s,%s)disconnected\n", client_hostname, client_port);
    return NULL;
}


int main(int argc, char *argv[])
{
   int listenfd;
   int *connfd; //file descriptor to communicate with the client
   socklen_t clientlen;
   struct sockaddr_storage clientaddr;  //Enough space for any address 
   char client_hostname[MAXLINE], client_port[MAXLINE];
   pthread_t tid;
    sem_init(&mutex,0,1);
    sem_init(&w,0,1);
    if (argc != 2) {
	fprintf(stderr, "usage: %s <port>\n", argv[0]);
	exit(0);
    }

    listenfd = Open_listenfd(argv[1]);

    //Server runs in the infinite loop. 
    //To stop the server process, it needs to be killed using the Ctrl+C key. 
    while (1) {               
    	clientlen = sizeof(struct sockaddr_storage);
        // wait for the connection from the client.
        connfd = malloc(sizeof(int));
    	*connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, 
                                   MAXLINE,client_port, MAXLINE, 0);

        printf("Connected to (%s, %s)\n", client_hostname, client_port);
        struct Arguments *arguments = malloc(sizeof(struct Arguments)); //declaring a argument data type of struct to take in the additional information
        (*arguments).connfd = *connfd;             //Copying over the data to the argument structure 
        strcpy(arguments->client_hostname, client_hostname);
        strcpy(arguments->client_port, client_port);
        pthread_create(&tid, NULL, thread, arguments); //calling the pthread create function
        //function to interact with the client's information
        //calling the server function to do the operations 
    }
        
    exit(0);
}
