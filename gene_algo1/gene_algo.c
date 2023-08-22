/*
	A very simple implementation of
	genetic algorithms in C.
	No grey-coding or uintx_t types 
	or anything like that.
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_WEIGHT 30
#define NO_OF_ITEMS 8
#define POP_SIZE 128 // It's a power of 2 and large enough for our purposes

//Our genome is represented as an array of 1's and 0's 
typedef struct _genome{
	int genes[NO_OF_ITEMS];
	int score;
} Genome;

typedef struct _item{
	int weight;
	int value;
} Item;


// We could also generate this automatically...
// but for testing purposes, we'll leave as is.
// The items to be fitted into the "knapsack"
Item store[NO_OF_ITEMS] = { 
//	{weight, value}	
	{11, 12},
	{8, 16},
	{7, 4},
	{12, 20},
	{4, 8},
	{5, 5},
	{10, 15},
	{4, 3}
};

int gcmp(const void* a, const void* b) {
	/*
		Used in the qsort function
	*/
	Genome* geneA = (Genome*) a;
	Genome* geneB = (Genome*) b;
	return ( geneA->score - geneB->score );
}

void print_scores(Genome* population){
	printf("Population Scores: {");
	for(int i=0; i<POP_SIZE; i++){
		if( i%16 == 0 ){
			printf("\n");
		}
		printf("%d ", population[i].score );
	}
	printf("}\n");
}

// generation function ( summarize )
void create_population(Genome* population){
	// A pass-by-reference function
	//TODO: should allocate more than 8 bits of memory. MUST FIX
	//population = malloc(sizeof(Genome)*POP_SIZE); 
	for(int i=0; i<POP_SIZE; i++){
		// for now, we won't bother checking for repeats
		//we'll just have to assume they're unique
		for(int j=0; j<NO_OF_ITEMS; j++){
			population[i].genes[j] = (rand() % 2);
		}
	}
	return; // irrelevant, just to note
}

// fitness function ( give scores )
void give_scores(Genome* population){
	int i=0, score, total_weight;
	for(; i<POP_SIZE; i++){
		score=0; total_weight=0;
		for(int j=0; j<NO_OF_ITEMS; j++){
			if(population[i].genes[j] == 1){
				total_weight += store[j].weight;
				score += store[j].value;
			}
		}
		/*
		NOTE:
		Weight does not matter, so as long 
		as it's below 30.
		*/
		population[i].score = (total_weight >= MAX_WEIGHT) ? 0 : score;
	}
}

// cross-over function ( "breeding" )
void crossover(Genome parent1, Genome parent2, Genome* cradle1, Genome* cradle2){
	/*
	cradleN refers to the location where the child will be saved.
	*/
	int xpoint = NO_OF_ITEMS/2;
	for(int i=0; i<NO_OF_ITEMS; i++){
		cradle1->genes[i] = (i<=xpoint) ? parent1.genes[i] : parent2.genes[i] ;
		cradle2->genes[i] = (i<=xpoint) ? parent2.genes[i] : parent1.genes[i] ;
		
	}
	
	return;
}

// selection and cross-over function ( tournament )
void compete(Genome* population){
	qsort(population, POP_SIZE, sizeof(Genome), gcmp);
	/*
	NOTE:
	qsort essentially puts the lowest-scoring at the
	front of the array. Therefore, we can "eliminate"
	the weaker half without the need for 
	a more formal competition.
	*/
	int i=0, j=POP_SIZE/2+1;
	while(i<POP_SIZE/2){		
		crossover(	population[j], population[j-1], &population[i], &population[i+1] );
		j++; i++;
	}
	return;
}


// mutation function
void mutate(Genome* population){
	/*
	Selects a random genome from the population
	and mutates a random bit in it.
	*/
	int i = rand() % POP_SIZE;
	int j = rand() % NO_OF_ITEMS;
	population[i].genes[j] = (population[i].genes[j]==0) ? 1 : 0;
}

// shows avg. fitness and best fitness
void print_stats(Genome* population, int generation){
	float avg_fitness=0;
	int best_fitness=0, total_fitness=0;
	for(int i=0; i<POP_SIZE; i++){
		total_fitness += population[i].score;
	}
	best_fitness = population[POP_SIZE-1].score;
	avg_fitness = (float) total_fitness / (float) POP_SIZE ;
	printf("Generation: %d Avg. Fitness: %f, Best Fitness: %d\n", generation, avg_fitness, best_fitness);
	return;
}

int main(int argc, char** argv){
	srand(time(NULL)); // RNG-seed
	Genome population[POP_SIZE];
	create_population(population);
	int i=0;
	// itearate over a generation
	while(1){ // ad infinitum
		// evaluate fitness
		give_scores(population);
		
		
		//select best fitness
		compete(population);
		
		// mutate
		// completely optional, 
		// but we'll add it anyway
		mutate(population);
		
		if( i % 32 == 0) {
			// print statistics of population
			print_scores(population);
			print_stats(population, i);
			printf("Press Enter to Continue \n");
			getchar(); // pause for stats
		}
		i++;
	}
	
	return 0;
}
