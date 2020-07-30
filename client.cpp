//Cliente UDP RDT

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
//#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include <iostream>
#include <string>

#include "RDT.h"
#include "manejo_archivos.h"

#define PORT    8080 //8080
#define MAXLINE 512
#define tam 2
using std::cout; using std::cin;
using std::string;using std::pair;

//Variables de Red
int sockfd;
char buffer[MAXLINE];
struct sockaddr_in  servaddr;
socklen_t len;
///////////////////

RDT Rdt;

void EnviarPaquete(string cadena);

void EnviarMensaje(string mensaje)
{
    int secuencia_ini = Rdt.SECUENCIA_OUT_ACTUAL; //numero de secuencia antes de creacion datagram
    Rdt.PreparacionMensaje(mensaje);
    int secuencia_fin = Rdt.SECUENCIA_OUT_ACTUAL; //Número actual luego de creacion datagram

    for (int sec= secuencia_ini; sec < secuencia_fin; ++sec)
    {
        EnviarPaquete(Rdt.VEC_SECUENCIAS_OUT->at(sec));
    }
}

void EnviarPaquete(string cadena)
{
    sendto(sockfd, cadena.c_str(), cadena.length(),
            MSG_CONFIRM, (const struct sockaddr *) &servaddr,
             sizeof(servaddr));
        cout << "\nPaquete enviado";
}

string EsperaPorMensaje()
{
    bool completo = false;
    unsigned int flujo_actual;
    int n;
    string mensaje_in = "";

    while (!completo)
    {
        bzero(buffer, MAXLINE);
        n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                                MSG_WAITALL, (struct sockaddr *) &servaddr,
                                &len);
        if (Rdt.RecepcionPaquete(string(buffer)) )
        {
            cout << "\nPaquete Recibido";
        }
        //Verificar si tenemos mensaje completo
        flujo_actual = Rdt.cola_flujos_in.front();
        if (Rdt.VEC_FLUJOS_IN->at(flujo_actual)->IsCompleto())
        {
            mensaje_in = Rdt.VEC_FLUJOS_IN->at(flujo_actual)->ExtraerMensaje();
            //Eliminar flujo
            delete Rdt.VEC_FLUJOS_IN->at(flujo_actual);
            Rdt.VEC_FLUJOS_IN->at(flujo_actual) = nullptr;
            Rdt.cola_flujos_in.pop();
            completo = true;
        }
    }
    return mensaje_in;
}
//////////////////////////////////////////////////////////////////////////////
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
int main()
{ 
    struct hostent *host;

    bool completo = false;
    unsigned int flujo_actual = -1;
    string mensaje_in = "", mensaje_out = "";

    string comando;
    
    std::map<string, int> com = {
        {"AR", 1}, //Envio de Archivo
        {"AA", 98} //ACK
    };
 
    host = (struct hostent *)gethostbyname((char *)"127.0.0.1"); //"51.15.220.108"

    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
            perror("socket creation failed");
            exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr = *((struct in_addr *)host->h_addr);
    //servaddr.sin_addr.s_addr = INADDR_ANY;

    int n;
    cout << "\n>> ";
    getline(cin, mensaje_out);

        //Enviar nombre de archivo a transmitir
        //Por ahora incluir numero de caracteres del nombre
        //ejm: 09texto.txt
        //EnviarMensaje("AR"+mensaje_out);
    conver_prot(mensaje_out);
    if(mensaje_out!=""){
        EnviarMensaje(mensaje_out);
    }
        
    
    for (;;){
        mensaje_in = EsperaPorMensaje();
    
        //Procesar mensaje payload
        //Extraccion de comando
        comando = mensaje_in.substr(0,2);
        
        switch (com[comando]){
        case 1:{ //AR 
            EnviarMensaje("OK");
            mensaje_in = EsperaPorMensaje();
            cout << "\nMensaje Recibido:"; // << mensaje_in;
            String2Txt(mensaje_out, mensaje_in);
            break;
        }
        case 98:{ //ACK
            cout << "\nACK Recibido";
            break;
        }
        default:
            break;
        }
    }

    close(sockfd);
    return 0;
}

