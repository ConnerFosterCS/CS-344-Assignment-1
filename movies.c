/*#define _GNU_SOURCE*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Prototypes.
struct movie;
//void userOptions(struct movie* movies);
/* Function to swap data of two nodes a and b*/
void swap(struct movie* a, struct movie* b, struct movie* movies);

//Movie data that is being stored.
struct movie
{
	char* title;
	char* year;
	char* languages;
	char* rating;
	struct movie* next;
	struct movie* prev;
};

// Parse the current line which is space delimited and create a movie struct with the data in this line.
struct movie* createMovie(char* currLine)
{
	struct movie* currMovie = malloc(sizeof(struct movie));
	char* saveptr; // For use with strtok_r

	// The first token is the title
	char* token = strtok_r(currLine, "[],", &saveptr);
	currMovie->title = calloc(strlen(token) + 1, sizeof(char));
	strcpy(currMovie->title, token);

	// The next token is the year
	token = strtok_r(NULL, "[],", &saveptr);
	currMovie->year = calloc(strlen(token) + 1, sizeof(char));
	strcpy(currMovie->year, token);

	// The next token is the languages
	token = strtok_r(NULL, "[],", &saveptr);
	currMovie->languages = calloc(strlen(token) + 1, sizeof(char));
	strcpy(currMovie->languages, token);

	// The last token is the rating
	token = strtok_r(NULL, "\n", &saveptr);
	currMovie->rating = calloc(strlen(token) + 1, sizeof(char));
	strcpy(currMovie->rating, token);

	currMovie->next = NULL; // Set the next node to NULL in the newly created movie entry.
	currMovie->prev = NULL;
	return currMovie;
}

/*
* Return a linked list of movies by parsing data from
* each line of the specified file.
*/
struct movie* processFile(char* filePath)
{
	FILE* movieFile = fopen(filePath, "r");	// Open the specified file for reading only
	char* currLine = NULL;
	size_t len = 0;
	ssize_t nread;
	struct movie* head = NULL; 	// The head of the linked list
	struct movie* tail = NULL; 	// The tail of the linked list

	getline(&currLine, &len, movieFile); //Skip first line (headers) from csv file.

	while ((nread = getline(&currLine, &len, movieFile)) != -1) // Read the file line by line
	{
		struct movie* newNode = createMovie(currLine); // Get a new movies node corresponding to the current line

		if (head == NULL) // Set the head and the tail to this node
		{
			head = newNode;
			tail = newNode;
		}
		else // Add this node to the list and advance the tail
		{
			newNode->prev = tail;
			tail->next = newNode;
			tail = newNode;
		}
	}
	free(currLine);
	fclose(movieFile);
	return head;
}

//Finds the movies buy user's inputed year of choice.
void findYear(struct movie* movies) {
	int yearChoice = 0; //user inputed variable
	int counter = 0; //Count how many matches were found.
	struct movie* start = movies; //Save start of linked list to recall.

	//Get user input for year they desire.
	printf("Enter the year for which you want to see movies: ");
	scanf("%i", &yearChoice);

	while (movies != NULL) //Loop parses through movies untill NULL(end).
	{
		int currYear = atoi(movies->year); //Convert struct value from char into an integer.
		if(currYear == yearChoice) //Compare the values from the list and the user input.
		{
			counter++; //count how many movies were found
			printf("%s\n", movies->title); //print movie title
		}
		movies = movies->next; //Continue loop by going to next movie
	}
	if(counter == 0) //If no movies were found...
		printf("No data about movies released in the year %i\n", yearChoice);
	
	movies = start; //Jump back to start of list
	//userOptions(movies);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
/*
** The objective of implementing this function was to create a helper function that would sort the Movie list by year from smallest to largest.
** The purpose of these functions are to assist my program on finding the highest rating from each year. By grouping the years through sorting, it made it much
** easier to find the correct information. I got the code from a K-State Polytechnic online reading/review found here,
** ("http://faculty.salina.k-state.edu/tim/CMST302/study_guide/topic7/bubble.html"). I chose this particular source because other source's methods
** of bubble sorting were not working with my code and were difficult to work around. This method was simply easier to implement into my pre-exsisting work.
** The only changes made were variable name changes to make sure the variables work with my exsisting variables.
*/

//This function swap the current and next movie positions in the list for bubble sort
struct movie* list_switch(struct movie* l1, struct movie* l2)
{
	l1->next = l2->next;
	l2->next = l1;
	return l2;
}

struct movie* sortList(struct movie* start)
{
	struct movie* p, * q, * top;
	int changed = 1; //Records if swap was preformed.

	top = (struct movie*)malloc(sizeof(struct movie)); //Save start of list.

	top->next = start; //Move to next node in list.
	if (start != NULL && start->next != NULL) { //Checks to make sure you're not at end of list
		while (changed) {
			changed = 0;
			q = top;
			p = top->next;
			while (p->next != NULL) {
				int i = atoi(p->year);
				int j = atoi(p->next->year);
				//push bigger items down
					if (i > j) {
						q->next = list_switch(p, p->next); //Perform swap and record it.
						changed = 1;
					}
				q = p;
				if (p->next != NULL)
					p = p->next;
			}
		}
	}
	p = top->next;
	free(top);
	return p;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------

//Finds the highest rating in each year of movie data.
void findRating(struct movie* movies)
{
	struct movie* start = movies; //Save start of linked list to recall.
	movies = sortList(movies); //call sort function to order lists by year.
	
	int currYear, nextYear, prevYear;
	double currRating, nextRating;
	char* ptr;
	struct movie* topRatingYear = movies;
	//Loops untill end of list.
	while (movies != NULL) 
	{
		if (movies->prev != NULL) //If the pervious movie node is not blank, set prevYear variable.
		{
			prevYear = atoi(movies->prev->year);
		}
		//Set current year and rating values from start.
		currYear = atoi(movies->year);
		currRating = strtod(movies->rating, &ptr);
		if (movies->next != NULL) //if the next movie node is not blank, set nerRating and nextYear values.
		{
			nextRating = strtod(movies->next->rating, &ptr);
			nextYear = atoi(movies->next->year);
		}
		else //If at end of list, print found values.
		{
			printf("%s %s%s\n", movies->year, movies->title, movies->rating);
		}
	
		if (currYear == nextYear && currRating >= nextRating) //If the years match and the currant value is higher.
		{
			if (topRatingYear == NULL) //If there is no current top rating.
			{
				topRatingYear = movies; //Set top rating for that year.
			}
		}
		else if (currYear == nextYear && currRating < nextRating) //If the years match and the next value is higher.
		{
			if (topRatingYear == NULL) //If there is no current top rating.
			{
				topRatingYear = movies->next; //Set top rating for that year.
			}
		}
		else if (currYear != nextYear && prevYear == currYear) //If at end of list, print found values and erase saved values for new inpts.
		{
			printf("%s %s%s\n", movies->year, movies->title, movies->rating);
			topRatingYear = NULL;
		}
		else 
		{
			printf("%s %s%s\n", movies->year, movies->title, movies->rating);
			topRatingYear = NULL;
		}
		movies = movies->next; //Move to next movie in sorted list.
	}
	movies = start; //Jump back to start of list
}

//Find the movies by user inputed language of choice.
void findLanguage(struct movie* movies) {
	char* languageChoice = malloc(sizeof(char)* 20); //Assuming that the name of the language is not more than 20 charecters.
	struct movie* start = movies; //Save start of linked list to recall.
	int counter = 0;  //Counts how many matches were found.

	//Get user input for year they desire.
	printf("Enter the language for which you want to see movies: ");
	scanf("%s", languageChoice);

	while (movies != NULL) //Loop parses through movies untill NULL(end).
	{
		char* tempPtr;
		char tempStr[256];
		strcpy(tempStr, movies->languages); //copy data into the temp string

		//Utalizing tokens, parse through movie language data by finding the ";" delimiters. Runs 5 times for a max of 5 languages.
		char* token = strtok_r(tempStr, ";", &tempPtr);
		if (token != NULL && strcmp(languageChoice, token) == 0) {
			printf ("%s %s\n", movies->year, movies->title);
			counter++;
		}

		for(int i=0; i<4; ++i) {
			token = strtok_r(NULL, ";", &tempPtr);
			if (token != NULL && strcmp(languageChoice, token) == 0) {
				printf ("%s %s\n", movies->year, movies->title);
				counter++;
			}
		}
		movies = movies->next; //Continue loop by going to next movie.
	}
	if(counter == 0) //If no movies were found...
		printf("No data about movies in  %s\n", languageChoice);

	movies = start; //Jump back to start of list
	//userOptions(movies);
}

//Quits out of program when user decides to.
void quit() {
	exit(0);
}

//Print out options for use interface.
void userOptions(struct movie* movies, char* argv[])
{
	int options = 0; //Keep track of users choices.
	while (options != 4) {
		
		struct movie* movies = processFile(argv[1]);
		//Promt user.
		printf("\n1. Show movies released in the specified year\n");
		printf("2. Show highest rated movie for each year\n");
		printf("3. Show the title and year of release of all movies in a specific language\n");
		printf("4. Exit from the program\n\n");
		printf("Enter a choice from 1 to 4: ");
		//Get user's input.
		scanf("%i", &options);
		if (options == 1)
		{
			findYear(movies);
		}
		else if (options == 2)
		{
			findRating(movies);
		}
		else if (options == 3)
		{
			findLanguage(movies);
		}
		else if (options == 4)
		{
			quit();
		}
		else
		{
			printf("\nYou must enter 1, 2, 3, or 4.\n");
			//userOptions(movies);
		}
	}
}

/*
*   Process the file provided as an argument to the program to
*   create a linked list of movie structs and print out the info.
*/
int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		printf("You must provide the name of the file to process\n");
		printf("Example usage: ./students student_info1.txt\n");
		return EXIT_FAILURE;
	}
	struct movie* movies = processFile(argv[1]);
	
	int numMovies = 0;
	while (movies != NULL) {
		numMovies++; //parse though movies and count how many are found.
		movies = movies->next;
	}
	printf("Processed file %s and parsed data for %i movies\n", argv[1], numMovies);
	
	userOptions(movies, argv);
}