#include <iostream>
#include <string>
#include <queue>
#include <vector>

#define tam 2
#define tamanio_slaves 3 
using namespace std;


vector<string> cola;
vector<int> slaves={
	1,  //port del slave 1
	2,  //port del slave 2
	3   //port del slave 3	
};
void rellenar_cola_nodos(string input){
	cola.clear() ;
  	if (input[1]!='R' ){
    	input=input.substr(2);
    	int t=atoi(input.substr(0,tam).c_str());
    	input=input.substr(2);
   	 	string xd=input.substr(0,t); 
    	cola.push_back(xd);
	    input=input.substr(t);
  	}
  	else{
    	input=input.substr(2);
    	while (true ){
      		int t=atoi(input.substr(0,tam).c_str());
      		if(! t){
        		break;
	      	}
      		input=input.substr(2);
      		string xd=input.substr(0,t); 
      		cola.push_back(xd);
      		input=input.substr(t);
    	}
	}
}
////////////////////////////////////////////////////
void enviar_mensaje(string m){
	for(auto i:cola){
		cout<<i[0]%tamanio_slaves<<endl;
		//SEND(M,i[0]%tamanio_slaves);
	}
}
int main(int argc, char *argv[]) {
	string t="CN05billy0104edad0218";
	string t_1="CR03ana04juan";
	rellenar_cola_nodos(t_1);
	for(auto i:cola){
		cout<<i<<endl;
	}


	return 0;
}

