// Nishaan Makim (n-makim)

#include "xcode_redirect.hpp"
#include <algorithm>
#include <utility>
#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <iomanip>
#include <cmath> //sqrt
#include <limits> //infinity

class MST {
public:
	struct Point {
		Point(double distance = std::numeric_limits<double>::infinity(), int xCoor = 0,
			  int yCoor = 0, int previous = -1, bool visited = false, char terrain = 'L'):
		distance(distance), xCoor(xCoor), yCoor(yCoor), previous(previous),
		visited(visited), terrain(terrain) {}
		double distance = std::numeric_limits<double>::infinity();
		int xCoor = 0;
		int yCoor = 0;
		int previous = -1;
		bool visited = false;
		char terrain = 'L'; //can either be L, C, or W
	};
	
	bool findDist(Point* lhs, Point* rhs);
	
	void readMap();
	
	void createMST();
	
	void printMST();
	
	char terrain(int x, int y);
	
	~MST();
	
private:
	std::vector<Point*> map;
	
	double MSTtotal = 0;
	
	bool hasLand = false, hasCoast = false, hasWater = false;
};

MST::~MST(){
	for(std::size_t i = 0; i < map.size(); ++i){
		delete map[i];
	}
}

void MST::readMap(){
	std::size_t vecSize;
	std::cin >> vecSize;
	map.resize(vecSize);
	
	int xC = 0, yC = 0;
	for(std::size_t i = 0; i < map.size(); ++i){
		std::cin >> xC >> yC;
		Point* add = new Point;
		add->xCoor = xC;
		add->yCoor = yC;
		add->terrain = terrain(xC, yC);
		map[i] = add;
	}
	if(hasLand && hasWater && !hasCoast){
		std::cerr << "Cannot construct MST";
		exit(1);
	}
}

char MST::terrain(int x, int y){
	if(y > 0 || x > 0){
		hasLand = true;
		return 'L';
	}
	if(y < 0 && x < 0){
		hasWater = true;
		return 'W';
	}
	hasCoast = true;
	return 'C';
}

bool MST::findDist(Point* lhs, Point* rhs){
	if((lhs->terrain == 'L' && rhs->terrain == 'W') ||
	   (lhs->terrain == 'W' && rhs->terrain == 'L')){
		return false;
	}
	double diff1 = (lhs->xCoor - rhs->xCoor);
	double diff2 = (lhs->yCoor - rhs->yCoor);
	double result = sqrt(diff1 * diff1 + diff2 * diff2);
	if(result < rhs->distance){
		rhs->distance = result;
		return true;
	}
	else {
		return false;
	}
}

void MST::createMST(){
	map[0]->distance = 0;
	map[0]->previous = 0;
	
	for(std::size_t i = 0; i < map.size(); ++i){
		
		std::size_t current = 0;
		double currDist = std::numeric_limits<double>::infinity();
		for(std::size_t j = 0; j < map.size(); j++){
			if((!map[j]->visited) && (map[j]->distance < currDist)){
				current = j;
				currDist = map[current]->distance;
			}
		}
		
		map[current]->visited = true;
		
		for(std::size_t j = 0; j < map.size(); j++){
			if(!map[j]->visited && findDist(map[current], map[j])){
				map[j]->previous = int(current);
			}
		}
	} //overall for
} //createMST

void MST::printMST(){
	for(std::size_t i = 0; i < map.size(); ++i){
		MSTtotal += map[i]->distance;
	}
	
	std::cout << MSTtotal << "\n";
	for(int i = 1; i < int(map.size()); ++i){
		std::cout << std::min(i, map[std::size_t(i)]->previous) << " " <<
		std::max(i, map[std::size_t(i)]->previous) << "\n";
	}
}


int main(int argc, char** argv){
	std::ios_base::sync_with_stdio(false);
	
	std::cout << std::setprecision(2);
	std::cout << std::fixed;
	
	xcode_redirect(argc, argv);
	
	MST mst;
	mst.readMap();
	mst.createMST();
	mst.printMST();
	
	return 0;
}
