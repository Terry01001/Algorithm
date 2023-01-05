//exhaustive
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<algorithm>
#include"math.h"
using namespace std;

class point{
	public:
		point(int a,int b,int ind){//constructor
			x=a;y=b;index=ind-1;
		}
		int getx(){return x;}
		int gety(){return y;}
		int getindex(){return index;}
		
	private:
		int x;
		int y;
		int index;
		
};
vector<point> pts;//vector store class point


//calculate the distance of two points
float CalDist(point& pt1,point& pt2){//passing two point objects to function
	int x1=pt1.getx();
	int x2=pt2.getx();
	int y1=pt1.gety();
	int y2=pt2.gety();
	
	float distx = (x1-x2)*(x1-x2);
	float disty = (y1-y2)*(y1-y2);
	float distance = sqrt(distx+disty);
	return distance;
}
//calculate the traveled distance of the route
float TraveledDistance(vector<int> route){//passing the route vvector to the funtion
	float distance = CalDist(pts[route[0]],pts[route[route.size()-1]]);//the distance between first point and last point
	for(int i=1;i<route.size();i++){
		distance+= CalDist(pts[route[i-1]],pts[route[i]]);
	}
	return distance;
}
void ExhaustiveSearch(){
	vector<int> route;//a vector store the route
	for(int i=0;i<pts.size();i++)
		route.push_back(pts[i].getindex());
	
	route.push_back(pts[0].getindex());
	
	vector<int> BestRoute=route;
	
	float traveled_distance=TraveledDistance(route);
	
	while(next_permutation(route.begin()+1,route.end()-1)){//try all the possible route way//using the 'algorithm' library
		float temp = TraveledDistance(route);
		if(temp<traveled_distance){
			traveled_distance = temp;
			BestRoute = route;
			
		}
		
	}
	
	cout<<"distance: "<<traveled_distance<<endl;
	
	for(int i=0;i<BestRoute.size()-1;i++){
		cout<<BestRoute[i]+1<<endl;
	}
	
	ofstream out;//write to the file
	out.open("ans-my.txt");
	if(out.fail()){
		cout<<"the files can't be opened."<<endl;
		exit(0);
	}
	else{
		out<<"distance: "<<traveled_distance<<endl;
		for(int i=0;i<BestRoute.size()-1;i++){
			out<<BestRoute[i]+1<<endl;
		}
	}
	out.close();
	
}




int main(int argc,char **argv){
	//readfile
	ifstream input;
	char **file = argv;
	file++;
	const char* name = *file; 
	//cout<<name<<endl;
	
	input.open(name);//open file 
	if(input.fail()){
		cout<<"The file can't not be opened."<<endl;
		exit(0);
	}
	else{
		int i,xi,yi;
		input >> i;
		do{
			input>>xi;
			input>>yi;
			point p(xi,yi,i);//create point
			pts.push_back(p);//push into vector
		}while(input>>i);
	}
	input.close();
	
	//exhaustive serach
	ExhaustiveSearch();
	
	cout<<endl;
	
	
	/*
	cout<<pts.size()<<endl;
	for(int i=0;i<pts.size();i++){
		cout<<pts[i].getx()<<" | "<<pts[i].gety()<<endl;
	}*/
	
}
