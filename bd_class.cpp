#include <iostream>
#include <string>
#include <sqlite3.h>
#include <vector>
using namespace std;
vector<string> cola;


int callback(void *NotUsed, int argc, char **argv, char **azColName){
    for(int i = 0; i<argc; i++){
        if(argv[i]){
            string temp(argv[i]);
            cola.push_back(temp);
        }
    }
    return 0;
}
class Base_datos{
public:
    char *zErrMsg=0;
    sqlite3 *database;
    int conexion; 
    Base_datos(string nm){
        conexion=sqlite3_open(nm.c_str(),&database);
        if(conexion)
            cout<<"No se puede conectar a la base de datos "<<nm<<endl;
        cout<<"Se conecto con la base de datos "<<nm<<endl;
    }
    string Consulta_directa(string sql){
        conexion=sqlite3_exec(database, sql.c_str(),callback,0,&zErrMsg);
    }
    ~Base_datos(){
        sqlite3_close(database);
    }
};
string create_json(vector <string> c,vector<string> t){
    string result="{";
    auto temp=t.begin();
    for(auto i:c){
        if(*temp == t[t.size()-1]){
            result=result+*temp+":'"+i+"',";
            temp=t.begin();    
            continue;
        }
        result=result+*temp+":'"+i+"',";
        temp++;       
    }
    result[result.size()-1]='}';
    return result;
}
int main(){    
    //ROWID es el id de cada elemento
    /*                        NODO
    nombre string(20)
    /////////////////////////////////////////////////////////////
                            Relacion                          //relacion nodo  a nodo
    id_1 int      | id_2 int   
    /////////////////////////////////////////////////////////////
                            Atributo  
                           
    nodo int          | nombre_dato string(20)      | dato string(20) //nodo es del nodo que pertenece ese atributo
    /////////////////////////////////////////////////////////////
    */
   //string sql="insert into Relacion(id_1,id_2) values(1,2);";
   vector<string> temp={"nombre_dato","dato"};
   string sql="select nombre_dato,dato from Atributo;";
   Base_datos bd("db1.db");
   //bd.Read("Nodo");
   bd.Consulta_directa(sql);
   cout<<create_json(cola,temp);
}