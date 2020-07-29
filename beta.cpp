#include <iostream>
#include <string>
#include <vector>
#include <ctype.h>
#define tam 2
using namespace std;


bool buscar_error(string t){
	vector<string> diccionario={"nodo","relacion","atributo","-","="};
	for(auto w:diccionario)
		if(t==w)
			return true;
	return false;
}
//////////////////////////////////////////////////////////////////////////////
string PadZeros(int number, int longitud){
	string num_letra = std::to_string(number);
	for (int i = num_letra.length(); i < longitud; ++i)
		num_letra = "0" + num_letra;
	return num_letra;
}
//////////////////////////////////////////////////////////////////////////////
string formar_palabra(string t){
	return PadZeros(t.size(),tam)+t;
}
//////////////////////////////////////////////////////////////////////////////
pair<string,string> separar(string temp,string cond){
	pair<string,string> out;
	out.first=temp.substr(0,temp.find(cond));
	out.second=temp.substr(temp.find(cond)+1);
	return out;
}
//////////////////////////////////////////////////////////////////////////////
void sep_comas(string palab,string cond, vector<string> &izq,vector<string> &der){
	izq.clear();der.clear();
	pair<string,string> temp;
	while(true){
		int t=palab.find(",");
		string t_1=palab.substr(0,t);
		temp=separar(t_1,cond);
		if(t==-1 ){
			izq.push_back(temp.first);
			der.push_back(temp.second);
			break;
		}
		izq.push_back(temp.first);
		der.push_back(temp.second);
		palab=palab.substr(t+1);
	}
}
//////////////////////////////////////////////////////////////////////////////
string crear(string temp){
	string msg="";
	vector<string> atributo;
	vector<string> valor;
	vector<string> partes;
	while(true){
		int t=temp.find(" ");
		if(t==-1 ){
			partes.push_back(temp);
			break;
		}
		string t_1=temp.substr(0,t);
		partes.push_back(t_1);
		temp=temp.substr(t+1);
	}
	if(buscar_error(partes[1]))
		return "";
	if(partes[0]=="nodo"){
		msg="CN";
		msg+=formar_palabra(partes[1]);
		if(partes.size()>3 && partes[2]=="atributo"){
			sep_comas(partes[3],"=",atributo,valor);
			msg+=PadZeros(atributo.size(),tam);
			for(int i=0;i<atributo.size();i++){
				msg+=formar_palabra(atributo[i]);
				msg+=formar_palabra(valor[i]);
			}
		}
	}
	if(partes[0]=="relacion"){
		msg="CR";
		sep_comas(partes[1],"-",atributo,valor);
		msg+=formar_palabra(atributo[0]);
		msg+=formar_palabra(valor[0]);
	}
	return msg;
}
//////////////////////////////////////////////////////////////////////////////
string actualizar(string temp){
	string msg="";
	vector<string> atributo;
	vector<string> valor;
	vector<string> partes;
	while(true){
		int t=temp.find(" ");
		if(t==-1 ){
			partes.push_back(temp);
			break;
		}
		string t_1=temp.substr(0,t);
		partes.push_back(t_1);
		temp=temp.substr(t+1);
	}
	if(partes[1]=="atributo"){
		msg="UA";
		msg+=formar_palabra(partes[0]);
		sep_comas(partes[2],"=",atributo,valor);
		msg+=formar_palabra(atributo[0]);
		msg+=formar_palabra(valor[0]);
	}
	if(partes[1]=="relacion"){
		msg="UR";
		msg+=formar_palabra(partes[0]);
		sep_comas(partes[2],"=",atributo,valor);
		msg+=formar_palabra(atributo[0]);
		msg+=formar_palabra(valor[0]);
	}
	return msg;
}
//////////////////////////////////////////////////////////////////////////////
string borrar(string temp){
	string msg="";
	vector<string> atributo;
	vector<string> valor;
	vector<string> partes;
	while(true){
		int t=temp.find(" ");
		if(t==-1 ){
			partes.push_back(temp);
			break;
		}
		string t_1=temp.substr(0,t);
		partes.push_back(t_1);
		temp=temp.substr(t+1);
	}
	if(partes[0]=="nodo"){
		msg="DN";
		msg+=formar_palabra(partes[1]);
	}
	if(partes[0]=="atributo"){
		msg="DA";
		msg=msg+formar_palabra(partes[1])+formar_palabra(partes[2]);
	}
	if(partes[0]=="relacion"){
		msg="DR";
		sep_comas(partes[1],"-",atributo,valor);
		msg+=formar_palabra(atributo[0]);
		msg+=formar_palabra(valor[0]);
	}
	return msg;
}
string mostrar(string temp){
	string msg="";
	vector<string> atributo;
	vector<string> valor;
	vector<string> partes;
	while(true){
		int t=temp.find(" ");
		if(t==-1 ){
			partes.push_back(temp);
			break;
		}
		string t_1=temp.substr(0,t);
		partes.push_back(t_1);
		temp=temp.substr(t+1);
	}
	if(partes[0]=="nodo"){
		msg="SN";
		msg+=formar_palabra(partes[1]);
	}
	if(partes[0]=="atributo"){
		msg="SA";
		msg+=formar_palabra(partes[1]);
	}
	if(partes[0]=="relacion"){
		msg="SR";
		msg+=formar_palabra(partes[1]);
		if(partes.size()>2){
			msg+=formar_palabra(partes[2]);
		}
	}
	return msg;
}
//////////////////////////////////////////////////////////////////////////////
string conver_prot(string t){
	int i=t.find(" ");
	string cond=t.substr(0,i);
	t=t.substr(i+1,t.size()-i);
	if(cond=="crear"){
		return crear(t);
	}
	if(cond=="actualizar"){
		return actualizar(t);
	}
	if(cond=="borrar"){
		return borrar(t);
	}
	if(cond=="mostrar"){
		return mostrar(t);
	}
	return "error";
}
//////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[]) {
	
	string caso="";
	while(true){
		cout<<"\n------EJEMPLOS-------\n";
		cout<< "crear nodo ana atributo edad=18"<<endl;
		cout<< "crear relacion ana-juan"<<endl;
		cout<<"///\n";
		cout<< "actualizar ana atributo edad=16"<<endl;
		cout<< "actualizar ana relacion billy=pedro"<<endl;
		cout<<"///\n";
		cout<< "borrar nodo ana"<<endl;
		cout<< "borrar atributo ana edad"<<endl;
		cout<< "borrar relacion ana-juan"<<endl;
		cout<<"///\n";
		cout<< "mostrar nodo ana"<<endl;
		cout<< "mostrar atributo ana"<<endl;
		cout<< "mostrar relacion ana"<<endl;
		cout<<"----------------------\n";
		caso.clear();
		cout<<">> ";
		getline(cin,caso);
		if(caso=="q")
			break;
		cout<<conver_prot(caso)<<endl;
		cin;
	}
	
	return 0;
}

