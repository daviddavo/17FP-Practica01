#include <iostream>
#include <string>
#include <fstream>

using namespace std;

// TODO: Buscar el convenio del orden de definición de los preambulos

/* Para cambiar el numero de elementos, tan sólo hay que modificar:
- El enum tElemento
- La constante ELEMENTOS
- La función elementoAstring
Nota: Probado con RPS-7 */
typedef enum{ tijera, papel, piedra, lagarto, spock } tElemento;
typedef enum{ ganaHumano, ganaMaquina, empate } tResultado;

const int ELEMENTOS = 5;
const string USERSFILE = "registro.txt";
const string RULESFILE = "reglas.txt";
const unsigned int MAX_INTENTOS = 3;
const string CENTINELA = "XXX";

string resultadoAstring(tResultado resultado);
tElemento eleccionHumano();
string elementoAstring(tElemento elemento);
tResultado quienGana(tElemento maquina, tElemento humano);
bool localizacionJugador(string apodo);
bool mostrarReglas();

int main(){
    string apodo;

    srand(time(NULL));
    // cout << elementoAstring(eleccionHumano()) << endl;
    // cout << quienGana(papel, piedra) << endl;
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

void menu(){
    int response;
    bool exit = false;

    do{

        cout << "1 - Jugar contra mi" << endl;
        cout << "2 - Aprende a jugar" << endl;
        cout << "0 - Huir" << endl;

        do{
            cout << "Select: ";
            cin >> response;
        }while( !(0 <= response && response <= 2) )

        switch(response){
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

    cout << "He elegido " << maquina << endl;
    cout << "Has elegido " << humano << endl;

    ganador = quienGana(maquina, humano);
    cout << endl << resultadoAstring(ganador) << endl;
    return ganador;
}

string resultadoAstring(tResultado resultado){
    string str;

    switch(resultado){
        case empate: str = "Has sabido predecir mis movimientos..."; break;
        case ganaMaquina: str = "Nunca podrás ganarme..."; break;
        case ganaHumano: str = "Es imposible... Has conseguido ganar a mi increíble IA"; break;
    }

    return str;
}

tElemento eleccionHumano(){
    int sel;

    for(unsigned int i = 0; i < ELEMENTOS; i++){
        cout << i+1 << "-" << elementoAstring(tElemento(i)) << endl;
    }

    do{
        cout << "Seleccion: ";
        cin >> sel;
        cin.sync();
    }while(! (0 < sel && sel < ELEMENTOS) );

    return tElemento(sel-1);
}

tElemento eleccionMaquina(){
    return tElemento(rand() % ELEMENTOS);
}

string elementoAstring(tElemento elemento){
    string str;

    switch (elemento){
        case piedra: str = "piedra"; break;
        case papel: str = "papel"; break;
        case tijera: str = "tijera"; break;
        case lagarto: str = "lagarto"; break;
        case spock: str = "spock"; break;
        default: str = "ERROR"; break; // Eliminar antes de entregar
    }

    return str;
}

tResultado quienGana(tElemento maquina, tElemento humano){
    return tResultado( (ELEMENTOS + maquina - humano) % 3 );
};

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

		if (line != CENTINELA) cout << line << endl;
	} while (line != CENTINELA && !reglas.fail());

	// También retornará 0 si no hay centinela, por si acaso
	return !reglas.fail();
}
