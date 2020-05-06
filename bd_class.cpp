#include <iostream>
#include <string>
#include <sqlite3.h>
using namespace std;

int callback(void *NotUsed, int argc, char **argv, char **azColName){
    int i;
    for(i = 0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
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
    string Read(string tabla){
        string sql="SELECT * FROM ";
        sql+=tabla+";";
        conexion=sqlite3_exec(database, sql.c_str(),callback,0,&zErrMsg);
    }
    string Create(string tabla,string at1,string at2,string at3=""){
        string sql="INSERT INTO "+tabla;
        switch (tabla[0]){
        case 'N':
            sql+=" (nombre,apellido,escuela) VALUES (";
            sql+="'"+at1+"','"+at2+"','"+at3+"');";
            conexion=sqlite3_exec(database, sql.c_str(),callback,0,&zErrMsg);
            break;
        case 'R':
            sql+=" (id_nodo1,id_nodo2,id_atrib_relacion) VALUES (";
            sql+=at1+", "+at2+", "+at3+");";
            conexion=sqlite3_exec(database, sql.c_str(),callback,0,&zErrMsg);
            break;
        case 'A':
            sql+=" (nombre_relacion,peso) VALUES (";
            sql+="'"+at1+"','"+at2+"');";
            conexion=sqlite3_exec(database, sql.c_str(),callback,0,&zErrMsg);
            break;
        default:
            cout<<"Error Create"<<endl;
            break;
        }
    }
    ~Base_datos(){
        sqlite3_close(database);
    }
};
int main(){    
    //Crear Read
    /*                        NODO
    nombre string(20) | apellido string(20) | escuela string(20)
    /////////////////////////////////////////////////////////////
                            Relacion
    id_nodo1 int      | id_nodo2 int        | id_atrib_relacion int
    /////////////////////////////////////////////////////////////*/
   string sql="SELECT * FROM Nodo;";
   Base_datos bd("Redes.db");
   //bd.Consulta_directa(sql);
   //bd.Create("Relacion","1","2","3");
}