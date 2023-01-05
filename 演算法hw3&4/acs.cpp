//ant colony optimization
#include<iostream>
#include<cstdlib>
#include<fstream>
#include<string>
#include<vector>
#include<algorithm>
#include<iomanip>
#include<set>
#include<ctime>
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
double **pheremone;
double **dis;//dynamic allocate
double **pheremone_plus;



//calculate the distance of two points
double CalDist(point& pt1,point& pt2){//passing two point objects to function
	int x1=pt1.getx();
	int x2=pt2.getx();
	int y1=pt1.gety();
	int y2=pt2.gety();
	
	double distx = (x1-x2)*(x1-x2);
	double disty = (y1-y2)*(y1-y2);
	double distance = sqrt(distx+disty);
	return distance;
}
//calculate the traveled distance of the route
double TraveledDistance(vector<int> route){//passing the route vvector to the funtion
	double distance = CalDist(pts[route[0]],pts[route[route.size()-1]]);//the distance between first point and last point
	for(int i=1;i<route.size();i++){
		distance+= CalDist(pts[route[i-1]],pts[route[i]]);
	}
	return distance;
}

class Ant{
	public:
		set<int> available;//remain city
		vector<int> trail;//path of ant
		int start_point;//random allocate
		double alpha,beta;//relative importance of pheremone versus distnace
		
		Ant(double a,double b){
			alpha=a;
			beta=b;
			
			start_point=rand()%(pts.size());
			trail.push_back(start_point);
			for(int i=0;i<pts.size();i++){
				if(i==start_point)
					continue;
				else 
					available.insert(i);
			}
		}
		void reset(){
			vector<int> re;
			start_point=rand()%(pts.size());
			re.push_back(start_point);
			trail=re;
			for(int i=0;i<pts.size();i++){
				if(i==start_point)
					continue;
				else 
					available.insert(i);
			}
		}
		void deposit(){
			double tourcost=TraveledDistance(trail);
			//int Q=100;
			double DepositAmount = 1/tourcost;
			pheremone_plus[trail[0]][trail[trail.size()-1]]+=0.1*DepositAmount;//pheremone decay parameter:0.1
			pheremone_plus[trail[trail.size()-1]][trail[0]]+=0.1*DepositAmount;
			for(int i=0;i<trail.size()-1;i++){
				pheremone_plus[trail[i]][trail[i+1]]+=0.1*DepositAmount;
				pheremone_plus[trail[i+1]][trail[i]]+=0.1*DepositAmount;
			}
		}
		
		void setsolution(){//build path
			if(trail.size()==pts.size()){return;}
			
			int currentCity = trail[trail.size()-1];
			int randnum=rand()%11;
			double q=randnum/10.0;
			const double q0=0.9;
			if(q<q0&&q==q0){//if q<=q0,exploitation
				double p;
				double highestProb=0;
				int cityHighest=-1;
				for(set<int>::iterator i=available.begin();i!=available.end();i++){
					p=(pow(pheremone[currentCity][*i],1))*(pow(1/dis[currentCity][*i],beta));
					if(p>highestProb){
						cityHighest=*i;
						highestProb=p;
					}
				}
				trail.push_back(cityHighest);
				available.erase(cityHighest);
			}
			else{//otherwise,biased exploration
				double norm = probabilityNorm(currentCity);
			
				//bool moved=false;
				double p;
				double highestProb=0;
				int cityHighest=-1;
				for(set<int>::iterator i=available.begin();i!=available.end();i++){
					p=(pow(pheremone[currentCity][*i],alpha))*(pow(1/dis[currentCity][*i],beta)) /norm;//probability distribution
					if(p>highestProb){//choose highest probability
						cityHighest=*i;
						highestProb=p;
					}
				}
				//cout<<start_point<<"|"<<cityHighest<<endl;
				trail.push_back(cityHighest);
				available.erase(cityHighest);
			}
				
			setsolution();
		
		}
		double probabilityNorm(int currentCity){
			int size=available.size();
			double norm=0.0;
			for(set<int>::iterator i = available.begin();i!=available.end();i++){
				norm+=(pow(pheremone[currentCity][*i],alpha))*(pow(1/dis[currentCity][*i],beta));
			}
			return norm;
		}
		
		
};

class ACO{
	public://N:cities M:no. of ants
		int N,M;
		vector<Ant> ant;
		double evaporation,alpha,beta;
		int run,iteration;
		
		ACO(double a,double b,double e,int r,int iter){
			alpha=a;beta=b;evaporation=e;
			run=r;iteration=iter;
			N=pts.size();
			M=10;//no. of ants
			
			for(int i=0;i<M;i++){
				Ant a(alpha,beta);
				ant.push_back(a);
			}
		}
		
		void simulate(char* filepath){
			/*in iteration
			construct ant solution(all ants)
			update
			*/
			vector<int> path;
			double mintour;
			vector<double> all_pathdistance;
			double all_min=99999999;//to remember the shortest path
			
			for(int run_index=0;run_index<run;run_index++){//run
				//reset each run
				for(int i=0;i<pts.size();i++){//initialize the 2d-array of pheremone
					for(int j=0;j<pts.size();j++){
						if(i==j){pheremone[i][j]=0;}
						else
							pheremone[i][j]=0.000167;//0.0016   (0.000167?)
					}
				}
				mintour=99999999;
				/*for(int x_index=0;x_index<pts.size();x_index++){
					for(int y_index=0;y_index<pts.size();y_index++){
						pheremone_plus[x_index][y_index]=0;
					}
				}*/
				
				for(int j=0;j<iteration;j++){//iteration
				
					for(int x_index=0;x_index<pts.size();x_index++){//reset pheremone plus
						for(int y_index=0;y_index<pts.size();y_index++){
							pheremone_plus[x_index][y_index]=0;
						}
					}
					
					int globalbesttour=-1;//record for global updating
					int globalmininkant=99999999;
					
					for(int k=0;k<M;k++){//run all ants solution
						ant[k].reset();
						ant[k].setsolution();
						if(TraveledDistance(ant[k].trail)<globalmininkant){
							globalbesttour=k;
							globalmininkant=TraveledDistance(ant[k].trail);
						}
						if(TraveledDistance(ant[k].trail)<mintour){
							mintour=TraveledDistance(ant[k].trail);
							if(mintour<all_min || mintour==all_min)//to record all_min's path
								path=ant[k].trail; 
						}
						//ant[k].deposit();
						//local uodate pheremone
						for(int x_index=0;x_index<pts.size();x_index++){
							for(int y_index=0;y_index<pts.size();y_index++){
								pheremone[x_index][y_index]*=(1-evaporation);
								pheremone[x_index][y_index]+=evaporation*0.000167;//plus (evaporation*initialized-pheremone-value)
							}
						}
						
					}
					ant[globalbesttour].deposit(); 
					
					//global update pheremone
					for(int x_index=0;x_index<pts.size();x_index++){
						for(int y_index=0;y_index<pts.size();y_index++){
							pheremone[x_index][y_index]*=(1-evaporation);
							pheremone[x_index][y_index]+=pheremone_plus[x_index][y_index];
						}
					}
					
					//cout<<i<<"run,j:"<<j<<"|"<<mintour<<endl;
				
				}
				//cout<<run_index+1<<"run|"<<mintour<<endl; 
				all_pathdistance.push_back(mintour);
			}
			//cal average
			double average=0;
			for(int i=0;i<all_pathdistance.size();i++){
				average+=all_pathdistance[i];
			}
			cout<<"distance: "<<average/all_pathdistance.size()<<endl;
			for(int i=0;i<path.size();i++){
				cout<<path[i]+1<<endl;
			}
			
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
				out<<"distance: "<<average/all_pathdistance.size()<<endl;
				for(int i=0;i<path.size();i++){
				out<<path[i]+1<<endl;
				}
			}
			out.close();

 		}
};

int main(int argc,char **argv){
	srand(time(NULL));
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
	
	int run=atoi(argv[2]);
	int iteration=atoi(argv[3]);
	//distance
	dis=new double*[pts.size()];
	for(int i=0;i<pts.size();i++)
		dis[i]=new double[pts.size()];
	
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
	
	//pheremone
	pheremone=new double*[pts.size()];
	for(int i=0;i<pts.size();i++)
		pheremone[i]=new double[pts.size()];
	
	//pheremone_plus
	pheremone_plus=new double*[pts.size()];
	for(int i=0;i<pts.size();i++)
		pheremone_plus[i]=new double[pts.size()];
	
	
	
	
	//Ant a(1.0,2.0);
	
	//a.setsolution();
	//cout<<endl; 
	/*
	//test
	for(int i=0;i<pts.size();i++){
		for(int j=0;j<pts.size();j++){
			cout<<pheremone[i][j]<<"|";
		}
		cout<<endl;
	}
	*/
	
	
	
	ACO colony(1,2,0.1,run,iteration);
	
	colony.simulate(name);
	
	/*//test
	for(int i=0;i<pts.size();i++){
		for(int j=0;j<pts.size();j++){
			cout<<pheremone[i][j]<<"|";
		}
		cout<<endl;
	}
	
	*/
	return 0;



}
