//ANTONIO RODENAS CILLER
//ALVARO FUENTES SOLAS

// Librerías
#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <ctime>
#include <stdlib.h>

using namespace std;

// Prototipos de las funciones
const int MAXIMO_PUNTOS = 6; //indicamos que ficha queremos que sea la mas grande, en este caso |4 - 4|
const int NUM_FICHAS = ((MAXIMO_PUNTOS + 1) * (MAXIMO_PUNTOS + 2)) / 2; // segun la ficha mas grande, calculamos cuantas fichas va a haber.
const int FICHAS_INI_JUGADOR = 7;//El jugador comienza con 7 fichas
typedef string fichasJug[NUM_FICHAS];//Array con las fichas que tiene el jugador
typedef string Pozo[NUM_FICHAS];//Array con todas las fichas posibles
int MostrarMenu();

void mostrarTablero(fichasJug fichasJug, short int fichasCont, string tablero, short int numColocadas, short int numRobadas);
bool ejecutarOpcion(short int opcionMenu, fichasJug &fichaJugador, string &tablero, Pozo &pozo, int &maxPuntos,
	short int &numColocadas, short int &numRobadas, short int &fichasCont, short int &pozoCont);
bool puedePonerIzq(string fichaJugador, string tablero);
bool puedePonerDer(string fichaJugador, string tablero);
void ponerFichaIzq(fichasJug &fichaJugador, string &tablero, short int &numColocadas, short int &fichasCont);
void ponerFichaDer(fichasJug &fichaJugador, string &tablero, short int &numColocadas, short int &fichasCont);
string robarFicha(Pozo &pozo, short int &pozoCont);
void guardarPartida(string tablero,int maxPuntos, int numColocadas, int numRobadas, fichasJug fichaJugador, Pozo pozo, short int pozoCont, short int fichasCont);
void cargarPartida(string &tablero,int &maxPuntos, short int &numColocadas, short int &numRobadas,
	fichasJug &fichaJugador, Pozo &pozo, short int &pozoCont, short int &fichasCont);
bool puedeRobar(string tablero, fichasJug fichas, short int fichasCont);
short int sumaPuntos(fichasJug fichas, short int fichasCont);
string muestraFicha(string ficha);
string NumTableroIzq(string tablero);
string NumTableroDer(string tablero);
string NumFichaIzq(string ficha);
string NumFichaDer(string ficha);
void generarPozo(Pozo &pozo, int maxPuntos);
void eliminaFicha(fichasJug &fichasJug, short int &fichasCont, short int fichaNum);
void inicializar(string &tablero, fichasJug &fichas, short int &numColocadas, short int &numRobadas,
	short int &fichasCont, short int &pozoCont, int &maxPuntos, Pozo &pozo);
void desordenaPozo(Pozo &pozo);
bool finalPartida(fichasJug fichaJugador, string tablero, short int fichasCont, short int pozoCont, short int numRobadas, short int numColocadas);

// Programa principal

int main() {

	//Definición de variables

	bool fin;
	fichasJug fichaJugador;
	Pozo pozo;
	string tablero;
	short int numColocadas, numRobadas, fichasCont, pozoCont;
	int OpcionMenu;
	int maxPuntos = MAXIMO_PUNTOS + 1; //añadimos uno a la fic

	//Inicializamos las variables
	numColocadas = 0;
	numRobadas = 0;
	fichasCont = -1; // Inicializamos a -1 porque con 0 indicariamos que ha ganado el jugador en nuestra primera comprobacion en "finalPartida"
	pozoCont = -1;
	fin = false;


	// Bucle principal del programa


	while (!fin) {
		//Primero comprobamos si se ha acabado la partida porque el jugador ha ganado o ha perdido la partida
		if (finalPartida(fichaJugador, tablero, fichasCont, pozoCont, numColocadas, numRobadas)) { 
			fin = true;
		}
		else {
			system("cls");
			mostrarTablero(fichaJugador, fichasCont, tablero, numColocadas, numRobadas);
			OpcionMenu = MostrarMenu();
			//Segundo comprobamos si la partida acaba porque el jugador quiere sino ejecutamos la accion deseada
			fin = ejecutarOpcion(OpcionMenu, fichaJugador, tablero, pozo, maxPuntos, numColocadas, numRobadas, fichasCont, pozoCont);
		}
	}
	system("pause");
	return 0;
}



// Implementación de las funciones


//Mostramos las opciones que tiene el jugador y hasta que no introduzca una opcion correcta le preguntamos de nuevo
int MostrarMenu() {

	int opcion;
	bool opcionvalida = false;
	cout << " ------------------------- " << "\n" << "|" << "    MENU DE OPCIONES     " << "|" << "\n" << " ------------------------- " << "\n";
	cout << "1. Poner ficha por la izquierda\n";
	cout << "2. Poner ficha por la derecha\n";
	cout << "3. Robar ficha nueva\n";
	cout << "4. Inicializar\n";
	cout << "5. Cargar\n";
	cout << "6. Guardar\n";
	cout << "0. Salir\n";
	cout << "\n" << "Elija una opcion: ";  cin >> opcion;
	if ((0 <= opcion) && (opcion < 7)) {
		opcionvalida = true;
	}
	while (!opcionvalida) {  //en caso de error, comprobamos hasta que sea valida
		cout << "Opcion no valida, Elija de nuevo: "; cin >> opcion;
		if ((0 < opcion) && (opcion < 4)) {
			opcionvalida = true;
		}
	}
	return opcion;
}

//generamos el pozo con todas las fichas que correspondan segun el tamaño que hemos indicado para nuestra ficha mas grande
void generarPozo(Pozo &pozo, int maxPuntos) {

	string ficha = "XXX";
	int fig = 48;	//if (NUM_DIGITOS > 10) fig = 65; else fig = 48;
	int k = 0;
	while (k < NUM_FICHAS) {
		for (int i = 0; i < maxPuntos; i++) {
			for (int j = i; j < maxPuntos; j++) {
				ficha[0] = char(fig + i);
				ficha[1] = '-';
				ficha[2] = char(fig + j);
				pozo[k] = ficha;
				k++;
			}
		}
	}

}

//Mostramos el tablero tal cual nos pide en la practica
void mostrarTablero(fichasJug fichasJug, short int fichasCont, string tablero, short int numColocadas, short int numRobadas) {

	cout << " --------------- " << "\n" << "|" << "    TABLERO    " << "|" << "\n" << " --------------- \n";
	cout << tablero << "\n";
	cout << "Fichas colocadas:" << numColocadas << " - " << "Fichas robadas: " << numRobadas << "\n";
	cout << "Fichas jugador: ";
	int i = 0;
	while (i < fichasCont) {
		cout << "(" << i + 1 << ")" << muestraFicha(fichasJug[i]) << " ";
		i++;
	}
	cout << "\n";

}

bool finalPartida(fichasJug fichaJugador, string tablero, short int fichasCont, short int pozoCont, short int numColocadas, short int numRobadas) {
	if (fichasCont == 0) { // si el jugador ha puesto todas sus fichas, gana
		system("cls");
		mostrarTablero(fichaJugador, fichasCont, tablero, numColocadas, numRobadas);
		cout << "\nHas ganado esta partida, has colocado todas tus fichas, ENHORABUENA\n";
		return true;
	}
	else if (pozoCont == 0 && puedeRobar(tablero, fichaJugador, fichasCont)) { 
		//si el pozo esta vacio y puedeRobar es true, es decir, no puede colocar ninguna de las fichas que tiene en el tablero, pierde.
		system("cls");
		mostrarTablero(fichaJugador, fichasCont, tablero, numColocadas, numRobadas);
		cout << "\nHas perdido esta partida, no puedes ni robar ni colocar fichas, pero tus puntos han sido: " << sumaPuntos(fichaJugador, fichasCont) << "\n";
		return true;
	}
	else return false;

}

//Ejecutamos la opcion elegida del jugador
bool ejecutarOpcion(short int opcionMenu, fichasJug &fichaJugador, string &tablero, Pozo &pozo, int &maxPuntos,
	short int &numColocadas, short int &numRobadas, short int &fichasCont, short int &pozoCont) {

	bool salir = false; // solo es true en caso de que el usuario quiera salir del juego

	switch (opcionMenu) {

	case 1:
		ponerFichaIzq(fichaJugador, tablero, numColocadas, fichasCont);
		break;
	case 2:
		ponerFichaDer(fichaJugador, tablero, numColocadas, fichasCont);
		break;
	case 3:

		if (puedeRobar(tablero, fichaJugador, fichasCont)) { //primero comprobamos que el jugador no puede poner en el tablero ninguna ficha que ya tenga
			fichaJugador[fichasCont] = robarFicha(pozo, pozoCont);
			fichasCont++;
			numRobadas++;
		}
		else {
			cout << "No necesita robar, puede colocar alguna de sus fichas. ";
			system("pause");

		}

		break;
	case 4:
		inicializar(tablero, fichaJugador, numColocadas, numRobadas, fichasCont, pozoCont, maxPuntos, pozo);
		break;
	case 5:
		cargarPartida(tablero,maxPuntos, numColocadas, numRobadas, fichaJugador, pozo, pozoCont, fichasCont);
		break;
	case 6:
		guardarPartida(tablero, maxPuntos, numColocadas, numRobadas, fichaJugador, pozo, pozoCont, fichasCont);
		break;
	case 0:
		salir = true;
	}

	return salir;

}

//inicializamos todos los valores
void inicializar(string &tablero, fichasJug &fichas, short int &numColocadas, short int &numRobadas,
	short int &fichasCont, short int &pozoCont, int &maxPuntos, Pozo &pozo) {


	generarPozo(pozo, maxPuntos);
	desordenaPozo(pozo);
	tablero = "|" + pozo[NUM_FICHAS - 1] + "|";
	int i = 0;
	fichasCont = 0;
	numColocadas = 1; //La pieza con la que se empieza a jugar
	numRobadas = 0;
	pozoCont = NUM_FICHAS - 1;

	while (i < FICHAS_INI_JUGADOR) {
		fichas[i] = pozo[NUM_FICHAS - 2 - i];
		pozoCont--;
		fichasCont++;
		i++;
	}
}

//cuando el jugador pierde, suma los puntos de las fichas que le quedan por poner
short int sumaPuntos(fichasJug fichas, short int NumColocadas) {

	short int suma = 0;
	int i = 0;
	while (i < NumColocadas) { 
		suma = suma + atoi(fichas[i].substr(0, 1).c_str()) + atoi(fichas[i].substr(2, 1).c_str());
		i++;
	}
	return suma;
}

//Guardamos la partida en un fichero si el usuario lo ha deseado
void guardarPartida(string tablero, int maxPuntos, int numColocadas, int numRobadas, fichasJug fichaJugador, Pozo pozo, short int pozoCont, short int fichasCont) {

	string nombreFichero;
	ofstream fileOut;
	cout << "Introduce el nombre del fichero de tu partida: "; cin >> nombreFichero;
	fileOut.open(nombreFichero);
	if (fileOut.is_open()) {
		fileOut << maxPuntos<< "\n" << tablero << "\n" << numColocadas << "\n" << numRobadas << "\n" << fichasCont << "\n";
		int i = 0;
		while (i < fichasCont) {
			fileOut << fichaJugador[i] << " ";
			i++;
		}
		fileOut << "\n" << pozoCont << "\n";
		i = 0;
		while (i < pozoCont) {
			fileOut << pozo[i] << " ";
			i++;
		}

	}

	fileOut.close();
}

//Cargamos la partida de un fichero si el usuario lo ha deseado
void cargarPartida(string &tablero, int &maxPuntos, short int &numColocadas, short int &numRobadas,
	fichasJug &fichaJugador, Pozo &pozo, short int &pozoCont, short int &fichasCont) {

	ifstream fileIn;
	string nombreFichero;
	bool ficheroCorrecto = false;
	bool salir = false;
	int opc;
	cout << "Introduce el nombre del fichero con la partida que desea jugar: ";

	while (!ficheroCorrecto) {
		cin >> nombreFichero;
		fileIn.open(nombreFichero);
		if (fileIn.is_open()) {
			fileIn >> maxPuntos;
			fileIn >> tablero;
			fileIn >> numColocadas;
			fileIn >> numRobadas;
			fileIn >> fichasCont;
			int i = 0;
			while (i < fichasCont) {
				fileIn >> fichaJugador[i];
				i++;
			}
			fileIn >> pozoCont;
			i = 0;
			while (i < pozoCont) {
				fileIn >> pozo[i];
				i++;
			}
			ficheroCorrecto = true;
		}
		else {
			cout << "El la partida introducida no existe, indique que desea hacer: \n";
			cout << "1. SALIR de la opcion CARGAR PARTIDA y volver al menu\n";
			cout << "2. Introducir otro nombre de una partida \n";
			salir = false;
			while (!salir) {
				cin >> opc;
				if (opc == 1) {
					ficheroCorrecto = true;
					salir = true;
				}
				if (opc == 2) {
					cout << "Nombre del fichero: ";
					salir = true;
				}
				else {
					cout << "ERROR, solo puede seleccionar la opcion 1 o la opcion 2, intentelo de nuevo: ";
				}
			}
		}
	}
}

//Mostramos o la ficha del jugador o el tablero
string muestraFicha(string ficha) {

	return "|" + ficha + "|";

}

//Comprobamos que la opcion elegida de poner la ficha a la izquierda del tablero es posible
bool puedePonerIzq(string fichaJugador, string tablero) {

	//coprobamos si uno de los dos numeros de la ficha son iguales al borde de la derecha del tablero
	if (NumTableroIzq(tablero) == NumFichaIzq(fichaJugador) || NumTableroIzq(tablero) == NumFichaDer(fichaJugador)) {
		return true;
	}

	return false;

}

//Comprobamos que la opcion elegida de poner la ficha a la derecha del tablero es posible
bool puedePonerDer(string fichaJugador, string tablero) {


	//comprobamos si uno de los dos numeros de la ficha son iguales al borde de la derecha del tablero
	if (NumTableroDer(tablero) == NumFichaIzq(fichaJugador) || NumTableroDer(tablero) == NumFichaDer(fichaJugador)) {
		return true;
	}

	return false;


}

//Añadimos la ficha del jugador a la izquierda del tablero por el numero que correspoda de la ficha
void ponerFichaIzq(fichasJug &fichasJugador, string &tablero, short int &numColocadas, short int &fichasCont) {

	string ficha;
	int seleccion, i;
	bool opcionValida = false;
	bool salir = false;
	int opc;
	cout << "Introduzca el numero de la ficha que quiere colocar: ";
	while (!opcionValida) {

		cin >> seleccion;

		if ((0 < seleccion) && (seleccion <= fichasCont)) {

			ficha = fichasJugador[seleccion - 1];


			if (puedePonerIzq(ficha, tablero)) {
				if (NumFichaDer(ficha) == NumTableroIzq(tablero)) { //comprobamos cual de los dos numeros de la ficha es igual al borde del tablero por la izq

					tablero = "|" + NumFichaIzq(ficha) + "-" + NumFichaDer(ficha) + "|" + tablero; //introducimos la ficha por el numero de la izq al tablero

				}
				else {

					tablero = "|" + NumFichaDer(ficha) + "-" + NumFichaIzq(ficha) + "|" + tablero; //introducimos la ficha por el numero de la der al tablero

				}
				eliminaFicha(fichasJugador, fichasCont, seleccion - 1); //como el jugador ha usado su ficha, le añadimos una nueva
				numColocadas++;	//sumamos una al numero de fichas colocadas
				opcionValida = true;
			}

			else {

				cout << "La ficha elegida no se puede colocar en el tablero, indique que desea hacer:  \n";
				cout << "1. SALIR de la opcion COLOCAR A LA DERECHA y volver al menu\n";
				cout << "2. Volver a intentarlo \n";
				salir = false;
				while (!salir) {
					cin >> opc;
					if (opc == 1) {
						opcionValida = true;
						salir = true;
					}
					else if (opc == 2) {
						cout << "Introduzca el numero de la ficha: ";
						salir = true;
					}
					else {
						cout << "ERROR, solo puede seleccionar la opcion 1 o la opcion 2, intentelo de nuevo: ";
					}
				}
			}
		}

		else {
			cout << "La opcion elegida no se encuentra disponible entre las posibles, indique que desea hacer: \n";
			cout << "1. SALIR de la opcion COLOCAR A LA DERECHA y volver al menu\n";
			cout << "2. Volver a intentarlo \n";
			salir = false;
			while (!salir) {
				cin >> opc;
				if (opc == 1) {
					opcionValida = true;
					salir = true;
				}
				else if (opc == 2) {
					cout << "Introduzca el numero de la ficha: ";
					salir = true;
				}
				else {
					cout << "ERROR, solo puede seleccionar la opcion 1 o la opcion 2, intentelo de nuevo: ";
				}
			}
		}
	}
}

//Añadimos la ficha del jugador a la derecha del tablero por el numero que corresponda de la ficha
void ponerFichaDer(fichasJug &fichasJugador, string &tablero, short int &numColocadas, short int &fichasCont) {

	string ficha;
	int seleccion, i, opc;
	bool opcionValida = false;
	bool salir = false;
	cout << "Introduzca el numero de la ficha que quiere colocar: ";
	while (!opcionValida) {

		cin >> seleccion;

		if ((0 < seleccion) && (seleccion <= fichasCont)) {

			ficha = fichasJugador[seleccion - 1];

			if (puedePonerDer(ficha, tablero)) {
				if (NumFichaIzq(ficha) == NumTableroDer(tablero)) { //comprobamos cual de los dos numeros de la ficha es igual al borde del tablero por la izq

					//introducimos la ficha por el numero de la izq al tablero
					tablero = tablero + "|" + NumFichaIzq(ficha) + "-" + NumFichaDer(ficha) + "|";

				}
				else {
					//introducimos la ficha por el numero de la derecha al tablero
					tablero = tablero + "|" + NumFichaDer(ficha) + "-" + NumFichaIzq(ficha) + "|";

				}
				eliminaFicha(fichasJugador, fichasCont, seleccion - 1); //como el jugador ha usado su ficha, le añadimos una nueva
				numColocadas++;	//sumamos una al numero de fichas colocadas
				opcionValida = true;
			}
			else {

				cout << "La ficha elegida no se puede colocar en el tablero, indique que desea hacer:  \n";
				cout << "1. SALIR de la opcion COLOCAR A LA DERECHA y volver al menu\n";
				cout << "2. Volver a intentarlo \n";
				salir = false;
				while (!salir) {
					cin >> opc;
					if (opc == 1) {
						opcionValida = true;
						salir = true;
					}
					else if (opc == 2) {
						salir = true;
						cout << "Introduzca el numero de la ficha: ";
					}
					else {
						cout << "ERROR, solo puede seleccionar la opcion 1 o la opcion 2, intentelo de nuevo: ";
					}
				}
			}
		}

		else {
			cout << "La opcion elegida no se encuentra disponible entre las posibles, indique que desea hacer: \n";
			cout << "1. SALIR de la opcion COLOCAR A LA IZQUIERDA y volver al menu\n";
			cout << "2. Volver a intentarlo \n";
			salir = false;
			while (!salir) {
				cin >> opc;
				if (opc == 1) {
					opcionValida = true;
					salir = true;
				}
				if (opc == 2) {
					cout << "Introduzca la ficha que quiere colocar: ";
					salir = true;
				}
				else {
					cout << "ERROR, solo puede seleccionar la opcion 1 o la opcion 2, intentelo de nuevo: ";
				}
			}
		}
	}
}

//Se selecciona la ultima ficha del pozo con la finalidad de añadirla al array del jugador y se le resta uno al numero de fichas del pozo
string robarFicha(Pozo &pozo, short int &pozoCont) {
	string ficha = pozo[pozoCont - 1];
	pozoCont--;
	return ficha;
}

//Comprobamos todas las fichas del jugador con los dos extremos del tablero para saber si alguna ficha se puede colocar o no, antes de robar o para saber 
//si el jugador ha perdido
bool puedeRobar(string tablero, fichasJug fichas, short int fichasCont) {

	int i = 0;
	while (i < fichasCont) {
		if ((NumFichaDer(fichas[i]) == NumTableroDer(tablero)) || (NumFichaDer(fichas[i]) == NumTableroIzq(tablero)) ||
			(NumFichaIzq(fichas[i]) == NumTableroDer(tablero)) || (NumFichaIzq(fichas[i]) == NumTableroIzq(tablero))) {

			return false;
		}
		i++;

	}

	return true;
}

//Devolvemos la ficha mas a la izquierda del tablero
string NumTableroIzq(string tablero) {
	string numero = tablero.substr(1, 1);
	return numero;
}

//Devolvemos el numero mas a la derecha del tablero
string NumTableroDer(string tablero) {
	string numero = tablero.substr(tablero.size() - 2, 1);
	return numero;

}

//Devolvemos el numero de la izquierda de una ficha
string NumFichaIzq(string ficha) {
	string numero = ficha.substr(0, 1);
	return numero;
}

//Devolvemos el numero de la derecha de una ficha
string NumFichaDer(string ficha) {
	string numero = ficha.substr(2, 1);
	return numero;

}

//Cuando el jugador pone una ficha de su baraja en el tablero desplazamos todas las siguientes una posicion hacia adelante
void eliminaFicha(fichasJug &fichasJug, short int &fichasCont, short int fichaNum) {

	int i = fichaNum;
	while (i < fichasCont) {
		fichasJug[i] = fichasJug[i + 1];
		i++;
	}
	fichasCont--;
}

//Desordenamos el pozo de manera aleatoria
void desordenaPozo(Pozo &pozo) {
	int idx1, idx2;
	string tmp;
	for (int i = 0; i < 1000; i++) {
		idx1 = rand() % NUM_FICHAS;
		idx2 = rand() % NUM_FICHAS;
		if (idx1 != idx2) {
			tmp = pozo[idx1];
			pozo[idx1] = pozo[idx2];
			pozo[idx2] = tmp;
		}
	}
}