#include <iostream>
#include <string>
#include <sqlite3.h>
#include <vector>
using namespace std;
vector<string> cola;

string PadZeros(int number, int longitud){
  string num_letra = std::to_string(number);
  for (int i = num_letra.length(); i < longitud; ++i)
    num_letra = "0" + num_letra;
  
  return num_letra;
}
//----------------------------------------------------------------------
int callback(void *NotUsed, int argc, char **argv, char **azColName){
    for(int i = 0; i<argc; i++){
        if(argv[i]){
            string row_atributo(argv[i]);
            cola.push_back(row_atributo);
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
//----------------------------------------------------------------------
string create_json(vector <string> c,vector<string> t){
    string result="{";
    auto row_atributo=t.begin();
    for(auto i:c){
        if(*row_atributo == t[t.size()-1]){
            result=result+*row_atributo+":'"+i+"',";
            row_atributo=t.begin();    
            continue;
        }
        result=result+*row_atributo+":'"+i+"',";
        row_atributo++;       
    }
    result[result.size()-1]='}';
    return result;
}
//----------------------------------------------------------------------
void leer_json(string json,vector<string> row,string accion){
    cout<<json<<"\n///////////////////////////\n";
    json=json.substr(1,json.size()-1);
    string xd="";
    for (int i = 1; i < json.size(); i++){
        int pos_1=json.find(':');
        int pos_2=json.find(',');
        xd=xd+json.substr(pos_1+1, pos_2-pos_1);
        json=json.substr(pos_2+1,json.size());
        if(i%row.size()==0 ){
            string row_atributo="";
            if(xd[xd.size()-1]==',')
                xd[xd.size()-1]='\0';
            for(auto j:row){
                row_atributo=row_atributo+j+",";
            }
            row_atributo=accion+"("+row_atributo.substr(0,row_atributo.size()-1)+") values("+xd+");";
            cout<<row_atributo<<endl;//imprime todo el mensaje para ya hacer la consulta en la bd
            xd="";
            continue;
        }
        if(pos_1==-1 || pos_2==-1){
            break;
        }
    }
}
//----------------------------------------------------------------------
string create_nodo(){
    string temp;
    string msg="CN";
    bool cond=true;
    int tam=2;
    cout<<"Puede cancelar la operacion ingresando 'q'\n";
    cout<<"Ingrese el nombre del nodo\n";
    getline(cin,temp);
    temp=PadZeros(temp.size(),tam)+temp;
    msg+=temp;
    cout<<"///////////////////////////"<<endl;
    while (true){
        cout<<"Ingrese el NOMBRE de la varible \n";
        getline(cin,temp);
        if(temp=="q")
            break;
        temp=PadZeros(temp.size(),tam)+temp;
        msg+=temp;


        cout<<"Ingrese el VALOR de la varible \n";
        getline(cin,temp);
        if(temp=="q")
            break;
        temp=PadZeros(temp.size(),tam)+temp;
        msg+=temp;
        cout<<"///////////////////////////"<<endl;
    }
    return msg;
}
//----------------------------------------------------------------------
string create_relacion(){
    string temp;
    string msg="CR";
    bool cond=true;
    int tam=2;
    cout<<"Puede cancelar la operacion ingresando 'q'\n";

    cout<<"Ingrese el nombre del primer nodo\n";
    getline(cin,temp);
    temp=PadZeros(temp.size(),tam)+temp;
    msg+=temp;
    cout<<"///////////////////////////"<<endl;
    cout<<"Ingrese el nombre del segundo nodo\n";
    getline(cin,temp);
    temp=PadZeros(temp.size(),tam)+temp;
    msg+=temp;
    cout<<"///////////////////////////"<<endl;
    return msg;
}
//----------------------------------------------------------------------
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
   vector<string> row_atributo={"nombre_dato","dato"};
   string sql="select nombre_dato,dato from Atributo;";
   Base_datos bd("db1.db");
   //bd.Read("Nodo");
   bd.Consulta_directa(sql);
    string msg=create_json(cola,row_atributo);
    //---------------------------------------
    string accion="insert into Nodo";
    leer_json(msg,row_atributo,accion);

}