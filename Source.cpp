// RPSLS David Davó 2017
// https://github.com/daviddavo/17FP-Practica01
// Nota: No se han usado clear ni pause para evitar incompatibilidades
#include <iostream>
#include <string>
#include <fstream>
#include <time.h> // No necesario con g++

using namespace std;

/* Para cambiar el numero de elementos, tan sólo hay que modificar:
- El enum tElemento, tal que el elemento n gana al n+(2k+1) donde k != 0 y k != ELEMENTOS
- La constante ELEMENTOS
- La función elementoAstring
Esto no sería necesario con otra estructura, la verdad
Nota: Probado antes de entregarlo con RPS-7 http://www.umop.com/rps7.htm */
typedef enum{ tijera, papel, piedra, lagarto, spock } tElemento;
typedef enum{ ganaHumano, ganaMaquina, empate } tResultado;

const int ELEMENTOS = 5; // Numero de elementos de tElemento
const string USERSFILE = "registro.txt";
const string RULESFILE = "reglas.txt";
const unsigned int MAX_INTENTOS = 3; // Maximo de intentos para la contraseña
const string CENTINELA = "XXX";

int seleccion(int, int); // Usada por los menus para no repetir codigo
void menu(); // El menu inicial (Jugar, Relgas, Salir)
tResultado play(); // Cuando se pulsa "1- Jugar"
tElemento eleccionHumano(); // Retorna la eleccion del humano
tElemento eleccionMaquina(); // Retorna la eleccion de la maquina
tResultado quienGana(tElemento, tElemento); // Retorna quien gana dados maquina y humano
string elementoAstring(tElemento); // Convierte un elemento a string
string resultadoAstring(tResultado); // Convierte un resultado a string
bool localizacionJugador(string); // Retorna True si el jugador puede jugar
bool mostrarReglas(); // Muestra las reglas

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
	// Cada vez que se debe hacer la selección en un menú
    char response;

    do{
        cout << "Select: ";

		cin >> response;
        cin.sync();

        if(isdigit(response)){
            response = response - '0'; // ASCII
        }
    }while( !(minimo <= response && response <= maximo) );

    return response;
}

void menu(){
	// El menu principal
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
	// La función llamada cada vez que se pulsa "Jugar" en el menú
    tElemento maquina = eleccionMaquina(), humano = eleccionHumano();
    tResultado ganador;

    cout << "Has elegido " << elementoAstring(humano) << endl;
    cout << "He elegido " << elementoAstring(maquina) << endl;

    ganador = quienGana(maquina, humano);
    cout << endl << resultadoAstring(ganador) << endl;
    return ganador;
}

tElemento eleccionHumano(){
	// La elección del humano
    for(unsigned int i = 0; i < ELEMENTOS; i++){
        cout << i+1 << "-" << elementoAstring(tElemento(i)) << endl;
    }

    return tElemento(seleccion(1, ELEMENTOS) - 1);
}

tElemento eleccionMaquina(){
	// La elección de la máquina
    return tElemento(rand() % ELEMENTOS);
}

tResultado quienGana(tElemento maquina, tElemento humano){
	/*
	Esta función es difícil de explicar...
	Si ordenamos el enum tElemento de tal manera que el elemento n gane al n+1,
	también ganará al n+(2k+1) (n + impar) y perderá contra los n+2k (n+par)
	Podemos despejar que n-m dará -par si gana n, y -impar si gana m
	En el caso de empate n-m será igual a 0 (que, formalmente, no es par ni impar)
	Para mantener la resta n-m > 0, le sumamos ELEMENTOS, y para saber cuando es par, impar o 0, dividimos entre 3
	Si ordenamos el enum tResultado, podemos hacer un cast directamente
	*/
    return tResultado( (ELEMENTOS + maquina - humano) % 3 );
};

string elementoAstring(tElemento elemento){
	// Convierte un elemento a string
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
	// Hay unas cuantas frases por darle variedad al juego
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
	// Retorna 1 si el usuario puede jugar
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

	if (ret){ // No se ejecuta esta parte del codigo si ha fallado la lectura del archivo
		ret = false; // Si falla la contraseña, retorna 0
		unsigned int i = 0;
		string pwd_user;
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
	// Muestra las reglas
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
