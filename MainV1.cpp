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
int MostrarMenu();
bool ejecutarOpcion(short int opcionMenu, string &fichaJugador, string &tablero, short int &numColocadas, short int &numRobadas);
void MostrarTablero(string fichaJugador, string tablero, short int numColocadas, short int numRobadas);
bool puedePonerIzq(string fichaJugador, string tablero);
bool puedePonerDer(string fichaJugador, string tablero);
void ponerFichaIzq(string &fichaJugador, string &tablero, short int &numColocadas);
void ponerFichaDer(string &fichaJugador, string &tablero, short int &numColocadas);
void robarFicha(string &fichaJugador, string tablero, short int &numRobadas);
void guardarPartida(string tablero, int numColocadas, int numRobadas, string fichaJugador);
int elegirPartida();
bool cargarPartida(string &tablero, short int &numColocadas, short int &numRobadas, string &fichaJugador);
string generarFicha();
string muestraFicha(string ficha);
string NumTableroIzq(string tablero);
string NumTableroDer(string tablero);
string NumFichaIzq(string ficha);
string NumFichaDer(string ficha);


// Programa principal

int main() {

	//Definición de variables
	bool fin;
	string fichaJugador;
	string tablero;
	short int numColocadas, numRobadas;
	int OpcionMenu;

	//Preguntamos al usuario si quiere una partida nueva o cargar una partida anterior
	int quePartida = elegirPartida();
	bool partidaCargada = false;

	//Inicializamos las variables segun la eleccion del jugador
	if (quePartida == 2) { //Si quiere partida nueva, generamos todo desde cero
		fichaJugador = generarFicha();
		tablero = generarFicha();
		numColocadas = 0;
		numRobadas = 0;
	}
	else { // si por lo contrario, desea seguir con la partida anterior, inicializamos con los valores del fichero
		if (!cargarPartida(tablero, numColocadas, numRobadas, fichaJugador)) {// si no hay una partida anterior, generamos una nueva
			fichaJugador = generarFicha();
			tablero = generarFicha();
			numColocadas = 0;
			numRobadas = 0;
		}

	}
	fin = false;

	// Bucle principal del programa

	while (!fin)
	{
		system("cls");
		MostrarTablero(fichaJugador, tablero, numColocadas, numRobadas);
		OpcionMenu = MostrarMenu();
		fin = ejecutarOpcion(OpcionMenu, fichaJugador, tablero, numColocadas, numRobadas);
	}

	return 0;
}



// Implementación de las funciones

//Preguntamos al usuario si quiere jugar una partida nueva o desea cargar una partida guardada anteriormente
int elegirPartida() {
	bool opcionValida = false;
	int opcion;
	cout << "Que partida desea jugar:\n" << "1. Cargar partida anterior (si no existe, se creara una partida nueva)\n" << "2. Jugar partida nueva\n";
	cin >> opcion;
	while (!opcionValida) {
		if (opcion == 1) {
			return 1;
		}
		else if (opcion == 2) {
			return 2;
		}
		else {
			cout << "Opcion no correcta, elija de nuevo: ";
			cin >> opcion;
		}
	}
}

//Mostramos las opciones que tiene el jugador y hasta que no introduzca una opcion correcta le preguntamos de nuevo
int MostrarMenu() {

	int opcion;
	bool opcionvalida = false;
	cout << " ------------------------- " << "\n" << "|" << "    MENU DE OPCIONES     " << "|" << "\n" << " ------------------------- " << "\n";
	cout << "1. Poner ficha por la izquierda\n";
	cout << "2. Poner ficha por la derecha\n";
	cout << "3. Robar ficha nueva\n";
	cout << "0. Salir\n";
	cout << "\n" << "Elija una opcion: ";  cin >> opcion;
	if ((0 <= opcion) && (opcion < 4)) {
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

//Mostramos el tablero tal cual nos pide en la practica
void MostrarTablero(string fichaJugador, string tablero, short int numColocadas, short int numRobadas) {

	cout << " --------------- " << "\n" << "|" << "    TABLERO    " << "|" << "\n" << " --------------- \n";
	cout << muestraFicha(tablero) << "\n";
	cout << "Fichas colocadas:" << numColocadas << " - " << "Fichas robadas: " << numRobadas << "\n";
	cout << "Ficha jugador: " << muestraFicha(fichaJugador) << "\n" << "\n";

}

//Formamos una ficha con dos numeros enteros aleatorios
string generarFicha() {

	string ficha;
	int num1 = (rand() % 6);
	srand(clock());
	int num2 = (rand() % 6);

	ficha = "|" + to_string(num1) + "-" + to_string(num2) + "|"; // creamos la ficha con dos numeros aleatorios

	return ficha;
}

//Ejecutamos la opcion elegida del jugador
bool ejecutarOpcion(short int opcionMenu, string &fichaJugador, string &tablero, short int &numColocadas, short int &numRobadas) {

	bool salir = false; // solo es true en caso de que el usuario quiera salir del juego
	bool opcionCorrecta = false; //si la opcion no se puede ejecutar no sale del bucle y vuelve a preguntar
	bool SePuedeIzq = puedePonerIzq(fichaJugador, tablero); //comprobamos por la izq y der para saber si robar seria una opcion correcta
	bool SePuedeDer = puedePonerDer(fichaJugador, tablero);

	while (!opcionCorrecta) {
		switch (opcionMenu) {

		case 1:
			if (SePuedeIzq) { //comprobamos
				ponerFichaIzq(fichaJugador, tablero, numColocadas); //ejecutamos
				opcionCorrecta = true;
			}
			else {
				cout << "No se puede poner la ficha en la izquierda, vuelva a intentarlo: "; cin >> opcionMenu;
			}
			break;
		case 2:
			if (SePuedeDer) { //comprobamos
				ponerFichaDer(fichaJugador, tablero, numColocadas); //ejecutamos
				opcionCorrecta = true;
			}
			else {
				cout << "No se puede poner la ficha en la derecha, vuelva a intentarlo: "; cin >> opcionMenu;
			}
			break;
		case 3:
			if (!SePuedeDer && !SePuedeIzq) { //como no se puede ni por la izq ni por la der, se roba
				robarFicha(fichaJugador, tablero, numRobadas);
				opcionCorrecta = true;
			}
			else {
				cout << "No necesita robar, puede colocar la ficha, vuelva a intentarlo: "; cin >> opcionMenu;
			}
			break;
		case 0:
			bool esCorrecto = false;
			int eleccion = 0;
			int s = 168; // La ? en código ASCII
			char interrogacion = char(s); //Lo convertimos en un char
			cout << interrogacion << "Desea guardar la partida actual?\n" << "1. Si\n" << "2. No\n";
			cin >> eleccion;
			while (!esCorrecto) {
				if (eleccion == 1) { //guardamos partida antes de acabar
					guardarPartida(tablero, numColocadas, numRobadas, fichaJugador);
					esCorrecto = true;
					opcionCorrecta = true;
				}
				else if (eleccion == 2) { //no hacemos ninguna accion
					esCorrecto = true;
					opcionCorrecta = true;
				}
				else {
					cout << "Opcion no valida, vuelva a intentarlo: ";
					cin >> eleccion;
				}

			}
			salir = true;
			break;
		}
	}
	return salir;

}

//Guardamos la partida en un fichero si el usuario lo ha deseado
void guardarPartida(string tablero, int numColocadas, int numRobadas, string fichaJugador) {

	ofstream fileOut;
	fileOut.open("PartidaDomino.txt");
	if (fileOut.is_open()) {
		fileOut << tablero << "\n" << numColocadas << "\n" << numRobadas << "\n" << fichaJugador;
	}
	fileOut.close();
}

//Cargamos la partida de un fichero si el usuario lo ha deseado
bool cargarPartida(string &tablero, short int &numColocadas, short int &numRobadas, string &fichaJugador) {
	ifstream fileIn;
	fileIn.open("PartidaDomino.txt");
	if (fileIn.is_open()) {
		fileIn >> tablero;
		fileIn >> numColocadas;
		fileIn >> numRobadas;
		fileIn >> fichaJugador;
		return true;
	}
	else {
		return false;
	}
}

//Mostramos o la ficha del jugador o el tablero
string muestraFicha(string ficha) {
	return ficha;
}

//Comprobamos que la opcion elegida de poner la ficha a la izquierda del tablero es posible
bool puedePonerIzq(string fichaJugador, string tablero) {

	bool sePuede = false;

	//coprobamos si uno de los dos numeros de la ficha son iguales al borde de la derecha del tablero
	if (NumTableroIzq(tablero) == NumFichaIzq(fichaJugador) || NumTableroIzq(tablero) == NumFichaDer(fichaJugador)) {
		sePuede = true;
	}

	return sePuede;

}

//Comprobamos que la opcion elegida de poner la ficha a la derecha del tablero es posible
bool puedePonerDer(string fichaJugador, string tablero) {

	bool sePuede = false;

	//comprobamos si uno de los dos numeros de la ficha son iguales al borde de la derecha del tablero
	if (NumTableroDer(tablero) == NumFichaIzq(fichaJugador) || NumTableroDer(tablero) == NumFichaDer(fichaJugador)) {
		sePuede = true;
	}

	return sePuede;


}

//Añadimos la ficha del jugador a la izquierda del tablero por el numero que correspoda de la ficha
void ponerFichaIzq(string &fichaJugador, string &tablero, short int &numColocadas) {

	if (NumFichaDer(fichaJugador) == NumTableroIzq(tablero)) { //comprobamos cual de los dos numeros de la ficha es igual al borde del tablero por la izq

		tablero = "|" + NumFichaIzq(fichaJugador) + "-" + NumFichaDer(fichaJugador) + "|" + tablero; //introducimos la ficha por el numero de la izq al tablero

	}
	else {

		tablero = "|" + NumFichaDer(fichaJugador) + "-" + NumFichaIzq(fichaJugador) + "|" + tablero; //introducimos la ficha por el numero de la der al tablero

	}
	fichaJugador = generarFicha(); //como el jugador ha usado su ficha, le añadimos una nueva
	numColocadas++;	//sumamos una al numero de fichas colocadas
}

//Añadimos la ficha del jugador a la derecha del tablero por el numero que corresponda de la ficha
void ponerFichaDer(string &fichaJugador, string &tablero, short int &numColocadas) {

	if (NumFichaIzq(fichaJugador) == NumTableroDer(tablero)) { //comprobamos cual de los dos numeros de la ficha es igual al borde del tablero por la izq

		//introducimos la ficha por el numero de la izq al tablero
		tablero = tablero + "|" + NumFichaIzq(fichaJugador) + "-" + NumFichaDer(fichaJugador) + "|";

	}
	else {
		//introducimos la ficha por el numero de la derecha al tablero
		tablero = tablero + "|" + NumFichaDer(fichaJugador) + "-" + NumFichaIzq(fichaJugador) + "|";

	}
	fichaJugador = generarFicha(); // sumamos una al numero de fichas colocadas
	numColocadas++; //sumamos una al numero de fichas colocadas

}

//Generamos una nueva ficha aleatoria y se lo añadimos al jugador
void robarFicha(string &fichaJugador, string tablero, short int &numRobadas) {

	fichaJugador = generarFicha(); // se genera una nueva ficha 
	numRobadas++; // sumamos el numero de fichas robadas

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
	string numero = ficha.substr(1, 1);
	return numero;
}

//Devolvemos el numero de la derecha de una ficha
string NumFichaDer(string ficha) {
	string numero = ficha.substr(3, 1);
	return numero;

}

