#include <iostream>
#include <random>
#include <ctime>
#include "chromosome.h"
#include "key.h"



//Keeping Key as general as can be, in this program only string keys are used
typedef Key<string> string_key;

using namespace std;

int KEY_SIZE, POPULATION_SIZE = 100;
const double CROSSOVER_POINT = 0.5;

const int CHROMOSOME_SIZE = 15;

const double MIN_PROB = 0.1, MAX_PROB = 1;

const double GRADED_RETAIN_PERCENT = 0.3; //percentage retained of best-fitting individuals
const double NONGRADED_RETAIN_PERCENT = 0.2;	//percentage retained of remaining individuals

const bool DEBUGGING = true;


const unsigned int seed = time(0);
mt19937_64 rng(seed);
uniform_int_distribution<int> unii(32,126);
uniform_int_distribution<int> loc(0, CHROMOSOME_SIZE);
uniform_int_distribution<int> rand_parent(0, POPULATION_SIZE*(GRADED_RETAIN_PERCENT+NONGRADED_RETAIN_PERCENT));
uniform_real_distribution<double> mutation_probability(MIN_PROB, MAX_PROB);
uniform_real_distribution<double> unid(0, 100);

char get_letter(){
    
	return unii(rng);	//Ascii code chosen bounds
}

Chromosome create_chromosome(int size){
	string s = "";
	for(int i = 0; i < size; i++){
		s += get_letter();
	}
	return Chromosome(s);
}

void mutateChromosome(Chromosome &x){
	
	int n = x.s.size();
	
	//mutate 1 gene randomly
	x.s[loc(rng)] = get_letter();
}

void mutate(vector<Chromosome> &chromosomes){

	for(int i = 0; i < chromosomes.size(); i++){
		
		//Choose probability of mutation for each child
		double p = mutation_probability(rng);

		//Decide whether or not this child will mutate according to mutation probability selected
		double x = unid(rng);
		if(x <= p){	//mutate child
			mutateChromosome(chromosomes[i]);
		}	//else do nothing to child
	}
}


double get_fitness(string x, string s){

	double fitness = 0.0;

	if(x.size() != s.size()){
		cerr<<"ERROR: string_key size does not match chromosome size."<<endl;
		return -1.0;
	}

	for(int i = 0; i < s.size(); i++){
		if(x[i] == s[i]) fitness++;
	}
	return (fitness/s.size());
}

/**
 * @brief      DESCRRIPTYN
 *
 * @param[in]  c     { parameter_description }
 */
void printChromosomes(vector<Chromosome> c){
	for(int i = 0; i <c.size(); i++){
		cout<<"Chromosome: "<<c[i].s<<"  -  Fitness: "<<c[i].fitness<<endl;
	}
}
void printChromosomes(vector<Chromosome> c, int p){
	for(int i = p; i <c.size(); i++){
		cout<<"Chromosome: "<<c[i].s<<"  -  Fitness: "<<c[i].fitness<<endl;
	}
}

vector<Chromosome> selection(vector<Chromosome> chromosomes_list){

	//Sort individuals by fitting score (DESCENDING ORDER)
	vector<Chromosome> sorted_chromosomes(chromosomes_list);
	sort(sorted_chromosomes.begin(), sorted_chromosomes.end(), greater<Chromosome>());
	//if(DEBUGGING) printChromosomes(sorted_chromosomes);

	//Select GRADED_RETAIN_PERCENT of best individuals
	vector<Chromosome> survivors;
	int n = sorted_chromosomes.size();
	for(int i = 0; i < n*GRADED_RETAIN_PERCENT; i++){
		survivors.push_back(sorted_chromosomes[i]);
	}
	if(DEBUGGING) {
		//cout<<"Best survivors: "<<endl;
		// printChromosomes(survivors);
		cout<<"Chromosome: "<<survivors[0].s<<"  -  Fitness: "<<survivors[0].fitness<<endl;
	}

	uniform_int_distribution<int> index_picker(n*GRADED_RETAIN_PERCENT, n-1);
	
	for(int i = n*GRADED_RETAIN_PERCENT; i < n*GRADED_RETAIN_PERCENT + n*NONGRADED_RETAIN_PERCENT; i++){
		int random_index = index_picker(rng);
		survivors.push_back(sorted_chromosomes[random_index]);
	}
	// if(DEBUGGING){
	// 	cout<<"Remaining chosen: "<<endl;
	// 	printChromosomes(survivors, n*GRADED_RETAIN_PERCENT);
	// }

	return survivors;


}

vector<Chromosome> create_base_population(int population_size, int chromosome_size){
	vector<Chromosome> chromosomes_list;
	for(int i = 0; i < population_size; i++){
		Chromosome c = create_chromosome(chromosome_size);
		chromosomes_list.push_back(c);
	}
	return chromosomes_list;
}



pair<bool, Chromosome> checkPopulation(string key, vector<Chromosome> chromosomes){
	for(int i = 0; i < chromosomes.size(); i++){
		if(chromosomes[i].s == key) return make_pair(true, chromosomes[i]);
	}

	//doesnt matter which chromosome we return if false
	return make_pair(false, chromosomes[0]);
}

void evaluate_fitness(string key, vector<Chromosome> &chromosomes){
	for (int i = 0; i < chromosomes.size(); i++)
	{
		chromosomes[i].fitness = get_fitness(key, chromosomes[i].s);
	}
}		


void crossover(vector<Chromosome> &chromosomes, int population_size, double crossover_point){

	vector<Chromosome> children;
	for(int i = 0; i < population_size - chromosomes.size(); i++){

		//select 2 parents at random
		int p1 = rand_parent(rng), p2 = rand_parent(rng);
		while(p1 == p2) p2 = rand_parent(rng); //Making sure that parent doesnt reproduce with itself

		Chromosome child = Chromosome::single_crossover(chromosomes[p1], chromosomes[p2], crossover_point);
		children.push_back(child);

	}

	//cout<<"Children size: "<<children.size()<<endl;
	//cout<<"remaining population size: "<<population_size - chromosomes.size();

	//Mutate children
	mutate(children);

	//Append children to parents to form new population
	chromosomes.insert(chromosomes.end(), children.begin(), children.end());
}





pair<bool, Chromosome> genetic_algorithm(string_key *key){


	string answer = key->get_key();

	vector<Chromosome> chromosomes = create_base_population(POPULATION_SIZE, CHROMOSOME_SIZE);

	while(!checkPopulation(answer, chromosomes).first){
		
		//Evaluate fitness
		evaluate_fitness(answer, chromosomes);

		//Select chromosomes
		chromosomes = selection(chromosomes);

		//Reproduction: children from parent chromosomes
		////Mutation step: without this fitness gets stuck: EMBEDDED INSIDE CROSSOVER
		//This is in order to only mutate the children and not the parents
		crossover(chromosomes, POPULATION_SIZE, CROSSOVER_POINT);
		
		
		

	}

	return checkPopulation(answer, chromosomes);

}







int main()
{

	string_key *key = new string_key();
	key->set_key("My name is rami");
	KEY_SIZE = key->get_key().size();


	pair<bool, Chromosome> t = genetic_algorithm(key);
	if(t.first){
		cout<<"Chromosome found: "<<t.second.s<<endl;
	}else{
		cout<<"Algorithm failed. Key was not found."<<endl;
	}

	

	// chromosomes_list = selection(chromosomes_list);

	// Chromosome t = Chromosome::single_crossover(chromosomes_list[0], chromosomes_list[1], 0.5);
	


}
















