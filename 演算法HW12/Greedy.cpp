//greedy
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

void ReadFile(){//read file
	ifstream input;
	input.open("point.txt");//open file
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
			point p(xi,yi,i);
			pts.push_back(p);
		}while(input>>i);
	}
	input.close();
}

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
float TraveledDistance(vector<int> route){//passing the route vector to the funtion
	float distance = CalDist(pts[route[0]],pts[route[route.size()-1]]);//the distance between first point and last point
	for(int i=1;i<route.size();i++){
		distance+= CalDist(pts[route[i-1]],pts[route[i]]);
	}
	return distance;
}

void Greedy(float **dis){
	
	vector<float> ROUTE_DISTANCE;//store the route distance
	int allroute[pts.size()][pts.size()]={0};//an 2d-array to store all the route
	
	
	for(int i=0;i<pts.size();i++){//set pts[i] be the start point ,then start finding the route(using greedy)
		int visited[pts.size()]={0};//an array represent visited point
		vector<int> route;
		
		
		route.push_back(pts[i].getindex());
		visited[i]=1;//visited 設為1
		
		
		while(route.size() != pts.size()){//把route找完 
			float min; 
			int z;
			for(int j=0;j<pts.size();j++){
				if(visited[j]!=1){//設定min值 
					min=dis[route.at(route.size()-1)][j];//j與當前vector裡的點的距離先設為最小值，之後再進行比較 
					z=j;
				}
			}
			
			for(int j=0;j<pts.size();j++){//每次找一個最近的點走 
				if(dis[route.at(route.size()-1)][j] < min && visited[j]!=1 ){//若有更短距離，記錄index值 
					min=dis[route.at(route.size()-1)][j];
					z=j;
				}
			}
			route.push_back(z);//put into vetor 
			visited[z]=1;//設為拜訪過的點 
		}
		
		ROUTE_DISTANCE.push_back(TraveledDistance(route));//把此route的距離放進vector 
		
		for(int k=0;k<pts.size();k++){//save route into 2d-array
			allroute[i][k]=route[k];
		}
	}
	
	/*
	//test
	for(int k=0;k<ROUTE_DISTANCE.size();k++){
		cout<<ROUTE_DISTANCE[k]<<"|"; 
	}
	cout<<endl; 
	*/
	
	
	
	float minValue = ROUTE_DISTANCE[0];//比較根據greedy走出來的所有route的最短距離即為答案 
	int minIndex = 0;
	for(int i=1;i<ROUTE_DISTANCE.size();i++){
		if(ROUTE_DISTANCE[i] < minValue){
			minValue=ROUTE_DISTANCE[i];
			minIndex=i;
		}
	}
	
	
	
	cout<<"distance: "<<ROUTE_DISTANCE[minIndex]<<endl;//把distance跟route印出來 
	for(int k=0;k<pts.size();k++){
		cout<<allroute[minIndex][k]+1<<endl;
	}
	
	ofstream out;//write to file
	out.open("ans-greedy.txt");
	if(out.fail()){
		cout<<"The file can't be opened."<<endl;
		exit(0);
	}
	else{
		out<<"distance: "<<ROUTE_DISTANCE[minIndex]<<endl;
		for(int k=0;k<pts.size();k++){
			out<<allroute[minIndex][k]+1<<endl;
		}
	}
	
	
	/*
	//reroute
	vector<int> route;
	int visited[pts.size()]={0};
	
	route.push_back(minIndex);
	cout<<"my minIndex："<< minIndex<<endl;
	visited[minIndex]=1;
	
	while(route.size() != pts.size()){//把route找完 
		float min; 
		int z;
		for(int j=0;j<pts.size();j++){
			if(visited[j]!=1){//設定min值 
				min=dis[route.at(route.size()-1)][j];
				z=j;
			}
		}
			
		for(int j=0;j<pts.size();j++){//每次找一個最近的點走 
			if(dis[route.at(route.size()-1)][j] < min && visited[j]!=1 ){//若有更短距離，記錄index值 
				z=j;
			}
		}
		route.push_back(z);
		visited[z]=1;
	}
	
	cout<<"distance: "<<minValue<<endl;
	
	for(int i=0;i<route.size();i++){
		cout<<route[i]+1<<endl;
	}
		
	
	*/
	
	
	
	
	
}


int main(){
	ReadFile();//readfile 
	
	float **dis;//dynamic allocate
	dis=new float*[pts.size()];
	for(int i=0;i<pts.size();i++)
		dis[i]=new float[pts.size()];
	
	for(int i=0;i<pts.size();i++){//initialize the 2d-array of distance
		for(int j=0;j<pts.size();j++){
			if(i==j){dis[i][j]=-1;}
			else if(i<j){
				dis[i][j]=CalDist(pts[i],pts[j]);
			}
			else{
				dis[i][j]=dis[j][i];
			}
		}
	}
	
	
	
	Greedy(dis);//greedy 
	
	
	
	cout<<endl;
	
	/*
	//test
	cout<<pts.size()<<endl;
	for(int i=0;i<pts.size();i++){
		for(int j=0;j<pts.size();j++){
			printf("%10f",dis[i][j]);
		}
		cout<<endl;
	}
	*/
}
