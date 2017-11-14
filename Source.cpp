// RPSLS David Davó 2017
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

/* Para cambiar el numero de elementos, tan sólo hay que modificar:
- El enum tElemento
- La constante ELEMENTOS
- La función elementoAstring
Esto no sería necesario con otra estructura, la verdad
Nota: Probado con RPS-7 */
typedef enum{ tijera, papel, piedra, lagarto, spock } tElemento;
typedef enum{ ganaHumano, ganaMaquina, empate } tResultado;

const int ELEMENTOS = 5;
const string USERSFILE = "registro.txt";
const string RULESFILE = "reglas.txt";
const unsigned int MAX_INTENTOS = 3;
const string CENTINELA = "XXX";

int seleccion(int, int);
void menu();
tResultado play();
tElemento eleccionHumano();
tElemento eleccionMaquina();
tResultado quienGana(tElemento, tElemento);
string elementoAstring(tElemento);
string resultadoAstring(tResultado);
bool localizacionJugador(string);
bool mostrarReglas();

int main(){
    string apodo;

    srand(time(NULL));

    cout << "Introduce tu apodo: ";
    cin >> apodo;
    if(localizacionJugador(apodo)){
        cout << "Puedes jugar" << endl;
        menu();
    }else{
        cout << "Lo siento, " << apodo << ", no puedes jugar :(" << endl;
    }

	return 0;
}

int seleccion(int minimo, int maximo){
    char response;

    do{
        cout << "Select: ";

        cin >> response;
        cin.sync();

        if(isdigit(response)){
            response = response - '0';
        }
    }while( !(minimo <= response && response <= maximo) );

    return response;
}

void menu(){
    int e = 0, g = 0, p = 0;
    bool exit = false;

    do{

        cout << "1 - Jugar contra mi" << endl;
        cout << "2 - Aprende a jugar" << endl;
        cout << "0 - Huir" << endl;

        switch(seleccion(0, 2)){
            case 0: exit = true; break;
            case 1:
                switch(play()){
                    case empate: e++; break;
                    case ganaMaquina: p++; break;
                    case ganaHumano: g++; break;
                } break;
            case 2: mostrarReglas(); break;
        }

    }while(!exit);

    cout << "Hemos jugado " << g+e+p << " partida(s)" << endl;
    cout << "Has ganado: " << g << endl;
    cout << "He ganado: " << p << endl;
    cout << "Hemos empatado: " << e << endl;
}

tResultado play(){
    tElemento maquina = eleccionMaquina(), humano = eleccionHumano();
    tResultado ganador;

    cout << "Has elegido " << elementoAstring(humano) << endl;
    cout << "He elegido " << elementoAstring(maquina) << endl;

    ganador = quienGana(maquina, humano);
    cout << endl << resultadoAstring(ganador) << endl;
    return ganador;
}

tElemento eleccionHumano(){
    for(unsigned int i = 0; i < ELEMENTOS; i++){
        cout << i+1 << "-" << elementoAstring(tElemento(i)) << endl;
    }

    return tElemento(seleccion(1, ELEMENTOS) - 1);
}

tElemento eleccionMaquina(){
    return tElemento(rand() % ELEMENTOS);
}

tResultado quienGana(tElemento maquina, tElemento humano){
    return tResultado( (ELEMENTOS + maquina - humano) % 3 );
};

string elementoAstring(tElemento elemento){
    string str;

    switch (elemento){
        case piedra: str = "piedra"; break;
        case papel: str = "papel"; break;
        case tijera: str = "tijera"; break;
        case lagarto: str = "lagarto"; break;
        case spock: str = "spock"; break;
    }

    return str;
}

string resultadoAstring(tResultado resultado){
    string str;
    int r = rand() % 2;

    // Esto se podría hacer mejor con arrays, la verdad

    switch(resultado){
        case empate:
            switch(r){
                case 0: str = "Has sabido predecir mis movimientos..."; break;
                case 1: str = "No he perdido, pero tampoco me has ganado..."; break;
                case 2: str = "Eres igual de inteligente que yo"; break;
            }break;
        case ganaMaquina:
            switch(r){
                case 0: str = "Nunca podras ganarme..."; break;
                case 1: str = "Por si no lo sabias, te he dejado ganar"; break;
                case 2: str = "¿En serio creias que ibas a ganar?"; break;
            }break;
        case ganaHumano:
            switch(r){
                case 0: str = "Es imposible... Has conseguido ganar a mi increible IA"; break;
                case 1: str = "Debe haber sido algun error del programador, no puedo perder"; break;
                case 2: str = "Los humanos sois muy dificiles de predecir..."; break;
            }break;
    }

    return str;
}

bool localizacionJugador(string apodo){
	ifstream registro;
	string usr, pwd_file;
	bool ret = false;

	registro.open(USERSFILE);

	do{
		registro >> usr >> pwd_file;
		if (usr == apodo){
			ret = true;
		}
	} while (!ret && usr != CENTINELA && !registro.fail());

	if (ret){
		ret = false; // Si falla la contraseña, retorna 0
		unsigned int i = 0;
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

		if (line != CENTINELA) cout << line << endl;
	} while (line != CENTINELA && !reglas.fail());

	// También retornará 0 si no hay centinela, por si acaso
	return !reglas.fail();
}
