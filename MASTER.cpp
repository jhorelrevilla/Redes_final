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
void function(string input){
	input=input.substr(2);
	for(int i=0;i<2;i++){
		int t=atoi(input.substr(0,tam).c_str());
		input=input.substr(2);
		string xd=input.substr(0,t);
		if(atoi(xd.c_str())){
			break;
		}
		cola.push_back(xd);
		input=input.substr(t);
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
	function(t_1);
	for(auto i:cola){
		cout<<i<<endl;
	}
	mensaje();

	return 0;
}

