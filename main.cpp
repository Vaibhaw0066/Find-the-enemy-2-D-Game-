#include<iostream>
#include<string>
using namespace std;



// JSON Handeler
#include "json.hpp"
#include<fstream>
#include <iomanip>
using json = nlohmann::json;


// Data hodlers
#include<vector>

// Utilities
#include<stdlib.h>  

//Extra
class player;



json load_data(string name){
	std::ifstream i("LAST/"+name+".json");
	json j;
	i >> j;
	return j;
}

class rooms{
	
	public:
	string id,desc,e,s,w,n;
	vector<string> obj;
	
	void setData(json data){
			
	
			id=data["id"];
			desc=data["desc"];
			e=data["e"];
			s=data["s"];
			w=data["w"];
			n=data["n"];
			for(auto &e:data["obj"])	obj.push_back(e); 
		}
			void print(){
			cout<<" id -> "<<id<<endl;
			cout<<" desc -> "<<desc<<endl;
			cout<<" e -> "<<e<<endl;
			cout<<" s -> "<<s<<endl;
			cout<<" w -> "<<w<<endl;
			cout<<" n -> "<<n<<endl;
			cout<<"Object in room -> ";
			for(auto &e:obj) cout<<e<<", ";
			
			
		}
};

class enemies{
	
	public:
		string id, initialroom;
		vector<string> killedby;
		
		void setData(json data){
			id=data["id"];
			initialroom=data["initialroom"];
			for(auto &e:data["killedby"])	killedby.push_back(e);			
//			cout<<"enemy data fetched "<<endl;
		}	
		void print(){
			cout<<" id -> "<<id<<endl;
			cout<<" initial room -> "<<initialroom<<endl;
			
			for(auto &e:killedby) cout<<e<<" ";
			cout<<endl<<"Enemy completely fetched"<<endl<<endl;
			
		}

	
};


class player{
	
	public:
	string name;
	int health_point;
	rooms curr_room;
	vector<string> objects;
	
	
	 player(string n,json data){
			name =n;
			health_point=100;
			string temp= data["player"]["initialroom"];
			for(auto &r:data["rooms"])
			{
				if(r["id"]==temp) curr_room.setData(r);
			}
		}
	
	
	void move(string command,vector<rooms> room){
		
		bool flag =false;
		if(command=="east" || command =="East" and curr_room.e!="")
			for(auto &r:room)	
				if(r.id==curr_room.e) { curr_room=r; flag=true; cout<<name<<" moved to "<<r.id<<endl<<"New room "<<"Desc -> "<<r.desc<<endl; break; }
				
		if(command=="west" || command =="West" and curr_room.w!="")
			for(auto &r:room)	
				if(r.id==curr_room.w) { curr_room=r; flag=true; cout<<name<<" moved to "<<r.id<<endl<<"New room "<<"Desc -> "<<r.desc<<endl;; break; }
				
		if(command=="north" || command =="North" and curr_room.n!="")
			for(auto &r:room)	
				if(r.id==curr_room.n) { curr_room=r;flag=true; cout<<name<<" moved to "<<r.id<<endl<<"New  room "<<"Desc -> "<<r.desc<<endl;; break;  }
	
		if(command=="south" || command =="South" and curr_room.s!="")
			for(auto &r:room)	
				if(r.id==curr_room.s) { curr_room=r; flag=true; cout<<name<<" moved to "<<r.id<<endl<<"New room "<<"Desc -> "<<r.desc<<endl; break; }
		
		if(!flag) cout<<"No rooms available in this direction"<<endl;
	
				
	}
	
//	Pickup items from room
	void take(string xxx){
		bool flag=false;
		for(auto &obj:curr_room.obj) 
			if(obj==xxx) {objects.push_back(obj); flag=true;}
		if(flag) cout<<"Item taken"<<endl;
		else cout<<"Item not found !"<<endl;
	}
	
//  print all the objects player have
	
	void listItems(){
		
		cout<<"Available objects -> [";
		for(auto &o:objects) cout<<o<<", ";
		cout<<"]";
		cout<<endl;	
	}
	
//	enemy list in current room
	bool enemyPresent(vector<enemies> enemy){
		
		cout<<"\n";
		bool enemyFound=false;
		for(auto &e:enemy) {
			if(e.initialroom==curr_room.id) 
			{
				enemyFound=true;
				cout<<"Enemy found -> "<<e.id;
			}
		
		}
		
		
		return enemyFound;
		
	}
	string getEnemyInRoom(vector<enemies> enemy){
		
		cout<<"\n";
		string enemyFound="";
		for(auto &e:enemy) {
			if(e.initialroom==curr_room.id) 
			{
				enemyFound=e.id;
				return e.id;
			}
		
		}
		
		return enemyFound;
		
	}
	
	
//  kill enemy 
	
	vector<enemies> killEnemy(string xxx,vector<enemies> enemy){
		
		vector<enemies> new_enemy;
		string needed_weapon;
		
		bool flag_killed=false;
		bool flag_found=false;
		for(auto &e:enemy){
			
			if(e.id==xxx){
				flag_found=true;
				for(int i=0;i<e.killedby.size();i++){
					bool weapon_found=false;
					for(int j=0;j<objects.size();j++){
						if(e.killedby[i]==objects[j]) weapon_found=true;
					}
					if(!weapon_found){
						
						cout<<"\n";
						cout<<e.killedby[i]<<" weapon not found,"<<xxx<<" can't be killed"<<endl;
						new_enemy.clear();
						for(auto &ee:enemy) new_enemy.push_back(ee);
						return new_enemy;
						
					} 
				}
				flag_killed=true;
			}else	new_enemy.push_back(e); 
			
		}
				
				if(flag_found && flag_killed) cout<<xxx<<" found and killed!"<<endl;
				else if(flag_found && !flag_killed) cout<<xxx<<" found but weapon not available";
		
		return new_enemy;
	}
	
	
	
			
//		Enemy's attack -> true or false
		int enemyAttack(int health_point){
			
			if(rand()%2==0)	
				{
				cout<<" attacked you, your health is halved !"<<endl;
				if(health_point==50) return health_point/2;
				if(health_point==25) return 0;
				return int(health_point/2);
			}else{
				cout<<" attcked you but missed!";
				return health_point;
			}	
		}
		
	
	void info(){
		cout<<"\n---------- Player info ---------\n";
		cout<<"Name          -> "<<name<<endl;
		cout<<"Health points -> "<<health_point<<endl;
		cout<<"Current room  -> "<<curr_room.id<<endl;
		cout<<"Objects       -> ";
		for(auto &o:objects) cout<<o<<" \n";
		cout<<"\n--------------------------------\n";
		cout<<endl;
		
	}
	
};



string Lower(string s){
	
	for (int i = 0; i < s.length(); i++)
  	{
  		s[i] = tolower(s[i]);
  	}
	return s;
}

string getSecPart(string str)
{
    string word = "";
    for (auto x : str) 	
        if (x == ' ')	word = "";
        else            word = word + x;
    return word;
}

bool isSubstring(string s1, string s2)
{
    int M = s1.length();
    int N = s2.length();
    for (int i = 0; i <= N - M; i++) {
        int j;

        for (j = 0; j < M; j++)
            if (s2[i + j] != s1[j])
                break;
 
        if (j == M)   return true;
    }
 
    return false;
}



int main(){

cout<<"This is a text based adventurous game, Where you need to play \nsmartly and kill all the enemies to win the game. "; 
//cout<<"\n----------------------------------------------------------------------------------\n\n";
cout<<"\n******************* Game Rules ******************************\n\n";
cout<<" ----------->  NAVIGATION  <------------- \n";
cout<<"1. go east \n2. go west\n3. go south\n4. go north\n";
cout<<"\n******************   COMMANDS  ******************************\n";
cout<<"To pick up items         	   -> take item_name\nCheck available items 	           -> list items\nCheck whether enemy is in the room -> enemy in room\nTo kill enemy                      -> kill enemy_name\nTo get list of all alive enemy     -> list enemy\n";
cout<<"Player info                        -> info\n";
cout<<"Quit                               -> quit\n";
	
cout<<"Ready to start the game ?"<<"Y/N  ?  ";
string reply;cin>>reply;
reply=Lower(reply);
if(reply=="y" || reply=="yes"){
	cout<<"\n\n                   ----------------Starting game -------------------\n\n";
	
	cout<<"Enter your nick/first name  \n";
	string name; cin>>name;
	string new_name="";
	for(auto &e:name) if(e==' ') break; else new_name+=e; 
	name=new_name;
	cout<<"Hello, "<<name<<endl;
	
	
	cout<<"Please choose the map\n\n1. Map1\n2. Map2\n3. Map3\n";
	cin>>reply;
	reply=Lower(reply);
	
	json data;
	cout<<"\nloading...\n";
	if(reply=="1"||reply=="map1"){

//	 Fetching data
	data= load_data("map1");
	cout<<"Map 1 loaded, lets go.\n";
	 
	}else if(reply=="2"||reply=="map2"){

//	 Fetching data
	data= load_data("map2");
	cout<<"Map 2 loaded, lets go.\n";
		
	}else if(reply=="3"||reply=="map3"){
//	 Fetching data
	 data= load_data("map3");
	 cout<<"Map 3 loaded, lets go.\n";
	
	}


//	 setting up room 
	vector<rooms> room(data["rooms"].size());
	for(int i=0;i<data["rooms"].size();i++)	room[i].setData(data["rooms"][i]);

//   setting up enemies
	vector<enemies> enemy(data["enemies"].size());
	for(int i=0;i<data["enemies"].size();i++)	enemy[i].setData(data["enemies"][i]);

//	Creating player
	player p(string(name),data);
	cout<<"Hi, "<<name<<". Player ID "<<name<<" created\n";
	
	int once=0;
	while(true){
		reply="";
		if(!once) cout<<"\n"<<p.curr_room.desc<<endl;
		once++;
		cout<<"\n\nGive command :\n ->> ";
		std::getline(std::cin,reply);
		reply=Lower(reply);
		if(reply=="go east"){
			p.move("east",room);
			
		}else if(isSubstring("go west",reply)){
			p.move("west",room);
			
		}else if(isSubstring("go south",reply)){
			p.move("south",room);
			
		}else if(isSubstring("go north",reply)){
			p.move("north",room);
			
		}else if(isSubstring("take",reply)){
			string xxx= getSecPart(reply);
			p.take(xxx);
			
		}else if(isSubstring("list items",reply)){
			 p.listItems();
		}
		else if(isSubstring("enemy in room",reply)){
			bool present=p.enemyPresent(enemy);
			if(!present) cout<<"No enemy in the room";
		}else if(isSubstring("kill",reply)){
			string xxx=getSecPart(reply);
			if(p.enemyPresent(enemy))
				enemy=p.killEnemy(xxx,enemy);
		}else if(isSubstring("list enemy",reply)){
			cout<<"All alive enemy are -> ";
			for(auto &e:enemy) cout<<e.id<<", ";
			cout<<endl;
		}else if(reply=="info"){
			p.info();
		}else if(reply=="quit"){
			cout<<"Bye, have a nice day.";
			return 0;
		}else{
			cout<<"Please enter valid command";
		}
//		cout<<"Number of enemies "<<enemy.size()<<endl;
		if(enemy.size()==0) {cout<<"You won"; return 0;}
		if(reply!="info" && reply!=""&&p.enemyPresent(enemy)){
			string enem=p.getEnemyInRoom(enemy);
			if(enem!=""){
				cout<<enem;
			p.health_point=p.enemyAttack(p.health_point);
			}
		}
		if(p.health_point==0){
			cout<<p.getEnemyInRoom(enemy)<<" killed you!\n";
			cout<<"Game over";
			return  0;
			
		} 
	}
	
}else	{	cout<<"Bye, have a nice day."; }
	
	return 0;
}


