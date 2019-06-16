#include <algorithm>
#include <stdlib>
#include <limits>
#include <random>
#include <vector>

struct Point {
	double x{0}, y{0};	
}

using DataFrame = std::vector<Point>;

double square(double vector){
	return value * value;
}

double squared_12_distance(Point first, Point second){
	return square(first.x - second.x) + square(first.y - second.y);
}

DataFrame k_means( const DataFrame& data,
				   size_t k,
				   size_t number_of_iterations){
	static std::random_device seed;
	static std::mt19937 random_number_generator(seed());
	std::uniform_int_distribution<size_t> indices(0,data.size()-1;)		  
	
	
	// pick centroids as random points from the dataset
	DataFrame means(k);
	for (auto& cluster : means){ // cluster -> means
		cluster = data[indices(random_number_generator)];
	}				   
}


