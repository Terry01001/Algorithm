//dynamic programming
#include<iostream>
#include<stdio.h>
#include<fstream>
#include<string>
#include<vector>
#include<algorithm>
#include<iomanip>
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
float TraveledDistance(vector<int> route){//passing the route vector to the funtion
	float distance = CalDist(pts[route[0]],pts[route[route.size()-1]]);//the distance between first point and last point
	for(int i=1;i<route.size();i++){
		distance+= CalDist(pts[route[i-1]],pts[route[i]]);
	}
	return distance;
}
void DynamicProgramming(char* filepath){
	
	float **dp=new float*[pts.size()];
	for(int i=0;i<pts.size();i++){
		dp[i]=new float[int(pow(2,pts.size()-1))];
	}
	int i,j=0;
	/*//test
	for(i=0;i<pts.size();i++){
		for(j=0;j<int(pow(2,pts.size()-1));j++){
			dp[i][j]=0;
		}
	}*/
	

	//fill the array
	j=0;
	for(i=0;i<pts.size();i++){//fill the blank j=0
		dp[i][j]=CalDist(pts[i],pts[0]);
	}
	
	
	
	int flag=0;
	float mincost;
	for(j=1;j<int(pow(2,pts.size()-1));j++){
		for(i=0;i<pts.size();i++){
			if(i!=0 && j/int(pow(2,i-1))%2 == 1){//keep it blank
				
			}	
			else{//to calculate
				flag=0;
				mincost=99999999;
				while(int(pow(2,flag))<=j){
					if(j/int(pow(2,flag))%2 == 1){
						float subroute_distance=0;
						subroute_distance+=CalDist(pts[i],pts[flag+1]);//distance between current startpoint(pts[i])and the remaining point 
						subroute_distance+=dp[flag+1][j-(int(pow(2,flag)))];//check the array we calculate previously
						if(subroute_distance<mincost)
							mincost=subroute_distance;
					}
					
					flag++;
				}
				dp[i][j]=mincost;
			}
		}
	}
	cout<<"distance: "<<dp[0][int(pow(2,pts.size()-1))-1]<<endl;
	
	//find route
	vector<int> bestroute;
	i=0;j=int(pow(2,pts.size()-1))-1;
	bestroute.push_back(i);
	
	int minIndexI,minIndexJ;
	while(1){
		if(j==0){break;}
		flag=0;
		mincost=99999999;
		while(int(pow(2,flag))<=j){
			if(j/int(pow(2,flag))%2 == 1){
				float subroute_distance=0;
				subroute_distance+=CalDist(pts[i],pts[flag+1]);
				subroute_distance+=dp[flag+1][j-(int(pow(2,flag)))];
				if(subroute_distance==dp[i][j]){//check who is the mincost and record it by minindexi,minindexj
					mincost=subroute_distance;
					minIndexI=flag+1;
					minIndexJ=j-(int(pow(2,flag)));
					break;
				}
			}
			flag++;
		}
		i=minIndexI;j=minIndexJ;
		//cout<<i<<" "<<j<<endl;
		bestroute.push_back(i);
	}
	
	
	for(int k=0;k<bestroute.size();k++){
		cout<<bestroute[k]+1<<endl;
	}
	
	
	
	/*for(i=0;i<pts.size();i++){
		for(j=0;j<int(pow(2,pts.size()-1));j++){
			printf("%4.3f ",dp[i][j]);
		}
		cout<<endl;
	}*/
	
	
	//write file
	ofstream out;//write to the file
	string filename="ans_";
	filename+=filepath[2];filename+=filepath[3];filename+=filepath[4];
	filename+=".txt";
	//cout<<filename<<endl;
	out.open(filename.c_str());
	
	if(out.fail()){
		cout<<"the files can't be opened."<<endl;
		exit(0);
	}
	else{
		out<<"distance: "<<dp[0][int(pow(2,pts.size()-1))-1]<<endl;
		for(int k=0;k<bestroute.size();k++){
		out<<bestroute[k]+1<<endl;
		}
	}
	out.close();
	
}




int main(int argc,char **argv){
	//readfile
	ifstream input;
	char **file = argv;
	file++;
	char* name = *file; 
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
	
	//dynamic programming
	DynamicProgramming(name);
	
	cout<<endl;
	
	
	/*
	cout<<pts.size()<<endl;
	for(int i=0;i<pts.size();i++){
		cout<<pts[i].getx()<<" | "<<pts[i].gety()<<endl;
	}*/
	
}
