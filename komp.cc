#include "timer.hh"
#include <tuple>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <random>

using namespace std;


using Point = vector<double>;
using DataFrame = vector<Point>;

inline double square(double value){
	return value * value;
}

inline double squared_12_distance(const Point first,const Point second){
	double d = 0.0;
	for(size_t dim = 0; dim < first.size();dim++){
		d += square(first[dim]-second[dim]);
	}
	return d;
}

pair<DataFrame,vector<size_t>> k_means( const DataFrame& data, size_t k, size_t number_of_iterations, double ep){
	size_t dimensions = data[0].size();
	static random_device seed;
	static mt19937 random_number_generator(seed());
	uniform_int_distribution<size_t> indices(0,data.size()-1);/// change		  
	// pick centroids as random points from the dataset
	DataFrame means(k);// 
	double distanciaepsilon;
	int contador = 0;
	size_t epsilon = numeric_limits<size_t>::max();
//------------------asignacion de primeros cluster--------------------------	
	for (Point& cluster : means){ // cluster -> means
		size_t i = indices(random_number_generator);
		cluster = data[i];//data rango i nvariable tener en cuenta ultimo rango y primero		
		}
	vector<size_t> assignments(data.size());
	
//--------------------------ciclo de kmeans-------------------------------
	//#pragma omp parallel for
	for(size_t iteration = 0; iteration < number_of_iterations; ++iteration){
		// find assignements ---- con este for da mejor tiempo
		#pragma omp parallel for 
		for (size_t point = 0; point < data.size() ; ++point){
			double best_distance = numeric_limits<double>::max();// variable mejor distacia, inicializada con la maxima
			size_t best_cluster = 0; // variable mejor cluster, inicializada con 0
			for (size_t cluster = 0; cluster < k; cluster++){
				const double distance = squared_12_distance(data[point], means[cluster]);
				if(distance < best_distance){
				    best_distance = distance;
				    best_cluster = cluster;
					}
				}
			assignments[point] = best_cluster;
			}
		DataFrame new_means(k,vector<double>(dimensions,0.0));// means nuevo
		DataFrame new_meansaux(k,vector<double>(dimensions,0.0));//means actual
		vector<size_t> counts(k, 0);
	//----------------------- asigna cluster a los puntos----------------
		//#pragma omp parallel for
		for (size_t point = 0; point < data.size(); ++point){
		    const size_t cluster = assignments[point];
		    for(size_t d = 0; d < dimensions; d++){
		    	new_means[cluster][d] += data[point][d];
		    	}			
			counts[cluster] += 1;
			}
	//------------------- divide sumas por saltos para obtener centroides
		//#pragma omp parallel for
		for (size_t cluster = 0; cluster < k; ++cluster){
			const size_t count = max<size_t>(1, counts[cluster]);
			for(size_t d = 0; d < dimensions;d++){
				new_meansaux[cluster][d] = means[cluster][d];
				means[cluster][d] = new_means[cluster][d] / count;
				}
			distanciaepsilon = squared_12_distance(new_meansaux[cluster],means[cluster]);
			if(distanciaepsilon < ep){
				contador++;
				}
			if(distanciaepsilon > ep){
				contador--;
				}	
			}
		//-------------final de centroides nuevos---------------
	//-------------retorno si los centroides no cambian o el cambio es menor a epsilon
		if(contador == k ){
			return {means, assignments};
		}
		contador = 0;
	//--------------fin retorno---------------------

	}
//----------------termina iteaciones--------------------------
	return {means, assignments};
}


DataFrame readData(string File,int nVariables ){
	DataFrame data;
	ifstream input(File);
	string line;
	while(getline(input,line)){
		istringstream iss(line);
		double v;
		Point p;
		for(int i = 0;i < nVariables; i++){
			iss >> v;
			p.push_back(v);
			}
		data.push_back(p);
		}
		cout << data.size() << endl;
		return data;
}


void imprimirkameans(vector<size_t> m,DataFrame data,int k){
	vector<int> v(k);
	for(int i = 0; i < data.size(); i++) {
  		v[m[i]]++;
  		}
  	for(int x = 0; x<v.size(); x++){
  		cout << "k_means" << x << " -> "<<v[x] <<endl;
  		}
}





int main(int argc, char *argv[]){
	// main
	cout << "k_means"<< endl;
	DataFrame data = readData("arrhythmia.data",279);
	cout << data.size() << endl;
	DataFrame c;
	vector<size_t> a;
	
	int clusters = atoi(argv[1]); 
    int iteraciones = atoi(argv[2]);
    int nVariables = atoi(argv[3]);
    double epsilon = atof(argv[4]);
    
    int pruebas = atoi(argv[5]);
	
	for(int i = 0;i < pruebas; i++ ){
		ScopedTimer t;
		
		tie(c,a) = k_means(data,clusters,iteraciones,epsilon);
		cout << t.elapsed()<< endl;

	}
	
	return 0;

}
