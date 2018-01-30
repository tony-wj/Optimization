#include <iostream>
#include <ctime>
using namespace std;
#define OBJ_COUNT 7
#define CAPACITY 150
#define POPULATION_SIZE 32
#define P_XOVER 0.8
#define P_MUTATION 0.15

int Weight[OBJ_COUNT] = { 35, 30, 60, 50, 40, 10, 25 };
int Value[OBJ_COUNT] = { 10, 40, 30, 50, 35, 40, 30 };
typedef struct GAType 
{
	int gene[OBJ_COUNT];
	int fitness; 
	double rf; 
	double cf; 
}GATYPE;

int EnvaluateFitness(GATYPE *pop) 
{ 
	int totalFitness = 0; 
	for( int i = 0; i < POPULATION_SIZE; i++) 
	{ 
		int tw = 0; 
		pop[ i]. fitness = 0; 
		for( int j = 0; j < OBJ_COUNT; j++) 
		{ 
			if( pop[i].gene[ j] == 1) 
			{ 
				tw += Weight[ j]; 
				pop[i].fitness += Value[ j]; 
			} 
		}
		if( tw > CAPACITY) /*penalty*/ 
		{ 
			pop[i].fitness = 1; 
		} 
		totalFitness += pop[i].fitness; 
	} 
	return totalFitness; 
}

void ExchangeOver( GATYPE *pop, int first, int second)
{ 
/*Exchange random number of genes*/ 
	int ecc = rand() % OBJ_COUNT + 1; 
	for( int i = 0; i < ecc; i++) 
	{ 
	/*The probability of exchange is the same*/ 
		int idx = rand() % OBJ_COUNT; 
		int tg = pop[ first]. gene[ idx]; 
		pop[ first]. gene[ idx] = pop[ second]. gene[ idx]; 
		pop[ second]. gene[ idx] = tg; 
	} 
}

void Crossover( GATYPE *pop) 
{ 
	int first = -1;
	// A signal that the first individual is selected
	for( int i = 0; i < POPULATION_SIZE; i++) 
	{ 
		double p = (double) rand() / RAND_MAX; 
		if( p < P_XOVER) 
		{
			if( first < 0) 
			{ 
			first = i; //Select the first individual
			} else 
			{ 
			ExchangeOver( pop, first, i); 
			first = -1;// Clear the signal
			} 
		} 
	} 
}

void ReverseGene( GATYPE *pop, int index) 
{ 
/*Reverse random number of genes*/ 
	int mcc = rand() % OBJ_COUNT + 1; 
	for( int i = 0; i < mcc; i++) 
	{ 
	/*The probability of reverse is the same*/ 
		int gi = rand() % OBJ_COUNT; 
		pop[ index]. gene[ gi] = 1 - pop[ index]. gene[ gi];
	}
}

void Mutation( GATYPE *pop) 
{ 
	for( int i = 0; i < POPULATION_SIZE; i++) 
	{ 
		double p = (double) rand() / RAND_MAX;
		if( p < P_MUTATION) 
		{ 
			ReverseGene( pop, i); 
		} 
	} 
}

void Select(int totalFitness, GATYPE *pop)
{
	GATYPE newPop[POPULATION_SIZE] = { 0 };
	double lastCf = 0.0; 
	//calculate the individual's selection frequency and cumulative frequency 
	for( int i = 0; i < POPULATION_SIZE; i++) 
	{ 
		pop[i].rf = (double) pop[i].fitness / totalFitness; 
		pop[i].cf = lastCf + pop[i].rf; 
		lastCf = pop[i].cf; 
	}
	for( int i = 0; i < POPULATION_SIZE; i++) 
	{ 
		double p = (double)rand() / RAND_MAX; 
		if( p < pop[0]. cf)
		{ 
			newPop[i] = pop[ 0]; 
		} else 
		{ 
			for( int j = 0; j < POPULATION_SIZE; j++) 
			{ 
				if(( p >= pop[j]. cf) && (p < pop[j + 1]. cf)) 
				{ 
					newPop[i] = pop[j + 1]; 
				} 
			} 
		} 
	}
	for (int i = 0; i < POPULATION_SIZE; ++i)
	{
		pop[i]=newPop[i];
	}
}

void Initialize(GATYPE *pop)
{
	//generate some random numbers
	for (int i = 0; i < POPULATION_SIZE; ++i)
	{
		for (int j = 0; j < OBJ_COUNT; ++j)
		{
			pop[i].gene[j] = rand()%2;
		}
	}

}

void Print(GATYPE *pop)
{
	int maxfit = 0;
	int maxaug = 0;
	for (int i = 0; i < POPULATION_SIZE; ++i)
	{
		if (pop[i].fitness > maxfit)
		{
			maxfit = pop[i].fitness;
			maxaug = i;
		}
	}
	for (int i = 0; i < OBJ_COUNT; ++i)
	{
		cout << pop[maxaug].gene[i] << " ";
	}
}

int main(int argc, char **argv){
	srand((unsigned)time(0));
	GATYPE population[POPULATION_SIZE] = { 0 }; 
	Initialize(population); 
	int totalFitness = EnvaluateFitness( population); 
	string str = argv[1];
	int max_generations = stoi(str);
	for( int i = 0; i < max_generations; i++) 
	{ 
		Select( totalFitness, population); 
		Crossover( population); 
		Mutation( population); 
		totalFitness = EnvaluateFitness( population); 
	}
	Print(population);
}
