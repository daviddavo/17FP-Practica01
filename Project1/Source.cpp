#include <iostream>
#include <string>
#include <fstream>

using namespace std;

// TODO: Buscar el convenio del orden de definición de los preambulos

typedef enum{ tijera, papel, piedra, lagarto, spock } tElemento;
// typedef enum{ } tResultado; // Hacer que se ajuste al cast de quienGana

const string USERSFILE = "registro.txt";
const string RULESFILE = "reglas.txt";
const unsigned int MAX_INTENTOS = 3;

bool localizacionJugador(string apodo);
bool mostrarReglas();

tResultado quienGana(tElemento maquina, tElemento humano){

};

int main(){

	return 0;
}

bool localizacionJugador(string apodo){
	ifstream registro;
	string usr, pwd_file;
	bool ret = false;

	registro.open(USERSFILE);

	do{
		registro >> usr;
		registro >> pwd_file;
		if (usr == apodo){
			ret = true;
		}
	} while (!ret && usr != "XXX" && !registro.fail());

	if (ret){
		ret = false; // Si falla la contraseña, retorna 0
		int i = 0;
		string pwd_user;
		// Podria hacerse con for / break?
		do{
			cout << "Password: ";
			cin >> pwd_user;
			i++;
		} while (i < MAX_INTENTOS && !(pwd_user == pwd_file));

		if (i < MAX_INTENTOS) ret = true;
	}

	return ret;
}

bool mostrarReglas(){
	ifstream reglas;
	string line;

	reglas.open(RULESFILE);

	do{
		getline(reglas, line);

		if (line != "XXX") cout << line << endl;
	} while (line != "XXX" && !reglas.fail());

	// También retornará 0 si no hay centinela, por si acaso
	return !reglas.fail();
}