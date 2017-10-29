using namespace std;

struct Chromosome{
public:
	
	string s;
	double fitness;

	Chromosome(string x){
		s = x;
		fitness = 0.0;
	}

	Chromosome(){
		s = "";
		fitness = 0.0;
	}
	
	bool operator < (const Chromosome& c) const{
		return (fitness < c.fitness);
	}
	bool operator > (const Chromosome& c) const{
		return (fitness > c.fitness);
	}


	static Chromosome single_crossover(Chromosome p1, Chromosome p2, double p){
		
		//Select p percent and 1-p percent of parent genetic material
		string h1(p1.s.begin(), p1.s.begin()+p*p1.s.size());
		string h2(p2.s.begin()+p*p1.s.size(), p2.s.end());

		return Chromosome(h1+h2);
	}


};