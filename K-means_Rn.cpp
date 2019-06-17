#include <algorithm>
#include <stdlib.h>
#include <limits>
#include <random>
#include <vector>
#include <iostream>

const RN = 2


struct Point { // puntos en una dimencion
	std::vector<size_t>	// vector de enteros
};

using DataFrame = std::vector<Point>; // vector de puntos (vector de vectores)

double square(double value){ // cuadrado de un valor
	return value * value;
}

double squared_12_distance(Point first, Point second){// vectores del mismo tamaño (troubleshooting)
	for (int i = 0; i < first.size(); i++){ // recore todo el vector para sacar el cuadrado de las distancias de 2 puntos
		aux += square(first[i] - second[i]);			
	}
	return aux;
}

// va hasta aqui el trabajo hehco
DataFrame k_means( const DataFrame& data, size_t k, size_t number_of_iterations){
					   
	static std::random_device seed;
	static std::mt19937 random_number_generator(seed());
	std::uniform_int_distribution<size_t> indices(0,data.size()-1);		  
	
	
	// pick centroids as random points from the dataset
	DataFrame means(k);
	for (auto& cluster : means){ // cluster -> means
		cluster = data[indices(random_number_generator)];
	}				   


	std::vector<size_t>assignments(data.size());
	for(size_t iteration = 0; iteration < number_of_iterations; ++iteration){
		// find assignements
		for (size_t point = 0; point < data.size() ; ++point){
			double best_distance = std::numeric_limits<double>::max();// variable mejor distacia, inicializada con la maxima
			size_t best_cluster = 0; // variable mejor cluster, inicializada con 0
			for (size_t cluster = 0; cluster < k; ++cluster){
				const double distance = squared_12_distance(data[point], means[cluster]);
				if(distance < best_distance){
				    best_distance = distance;
				    best_cluster = cluster;
				}
			}
		assignments[point] = best_cluster;
		}
		
		DataFrame new_means(k);
		std::vector<size_t> counts(k, 0);
		for (size_t point = 0; point < data.size(); ++point){
		    const auto cluster = assignments[point];
		    new_means[cluster].x += data[point].x;
		    new_means[cluster].y += data[point].y;			
			counts[cluster] += 1;
		}
		
		// divide sumas por saltos para obtener centroides
		for (size_t cluster = 0; cluster < k; ++cluster){
			const auto count = std::max<size_t>(1, counts[cluster]);
			means[cluster].x = new_means[cluster].y / count;
			means[cluster].y = new_means[cluster].y / count;
			
		}
	}	
	return means;
}

int main(){
	// main para generar un data set random (mejorar con un dataset externo)
	DataFrame SampleData(50);
	for(size_t i= 0; i < 50; i++){
		SampleData[i].x = rand() % (100);
		SampleData[i].y = rand() % (100);
		//muestra en pantalla el dataset
		std::cout << SampleData[i].x << " "  << SampleData[i].y << "\n" ;
	}
	
	DataFrame kmeans1(3);
	kmeans1 = k_means(SampleData, 3 , 4); // k means con 3 centroides y 4 iteraciones
	std::cout << "K-MEANS" << "\n";
	//muestra resutado
	for(size_t i= 0; i < 3; i++){
		
		//muestra en pantalla el kmeans
		std::cout << kmeans1[i].x << " "  << kmeans1[i].y << "\n" ;
	}
}
