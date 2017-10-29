using namespace std;

template <class T>
struct Key{
private:
	T key;
public:

	void set_key(T s){
		key = s;
	}

	T get_key(){
		return key;
	}
};