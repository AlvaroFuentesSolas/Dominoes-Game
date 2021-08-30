//ANTONIO RODENAS CILLER
//ALVARO FUENTES SOLAS

// Librerías
#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <ctime>
#include <windows.h>
using namespace std;

// Prototipos de las funciones
const int MIN_JUGADORES = 2;
const int MAX_JUGADORES = 4;
const int MIN_PTOS_FICHA = 4;
const int MAX_PTOS_FICHA = 9;
const int MAX_NUM_FICHAS = (MAX_PTOS_FICHA + 1) * (MAX_PTOS_FICHA + 2) / 2;
const int ALEA = 1000;
const int REPARTO = 7;
const int FICHAS_INI_JUGADOR = 7;//El jugador comienza con 7 fichas


struct tFicha {
	int valI;
	int valD;
};

typedef tFicha tArrayF[MAX_NUM_FICHAS];

struct tListaFicha {
	tArrayF fichas;
	int cont = -1; //Inicializamos a -1 porque en ComprobarFin lo comprobamos con 0 y en la primera ronda nos daria errores
};

struct tJugador {
	tListaFicha listaFJug;
	int acuPuntos = -1;
};

typedef tJugador tArrayJugadores[MAX_JUGADORES];


struct tJuego {
	int numJug;
	int nPuntos;
	int turno = 0;
	tArrayJugadores nJugador;
	string tablero = "";
	tListaFicha pozo;
	bool ganado = false;
};

typedef string sArray[MAX_NUM_FICHAS + 70];



int MostrarMenu();
void mostrarTablero(tJuego juego, bool fOcultas);
bool ejecutarOpcion(short int opcionMenu, tJuego &juego);
bool puedePonerIzq(tJuego juego, int jugador, short int ficha);
bool puedePonerDer(tJuego juego, int jugador, short int ficha);
void ponerFichaIzq(tJuego &juego, short int ficha);
void ponerFichaDer(tJuego &juego, short int ficha);
tFicha robarFicha(tJuego &juego);
void guardarJuego(const tJuego juego);
bool comprobarJugadorPuede(tJuego juego);
void cargarPartida(tJuego &juego);
bool puedeRobar(tJuego juego);
string muestraFicha(int Izq, int Der);
int NumTableroIzq(tJuego juego);
bool jugadoresPuntosYFichas(tJuego &juego);
int NumTableroDer(tJuego juego);
int NumFichaIzq(string ficha);
int NumFichaDer(string ficha);
void generarPozo(tJuego &juego);
void eliminarFicha(tJuego &juego, short int fichaNum);
void inicializarJuego(tJuego& juego, short int jugador);
void desordenaPozo(tJuego &juego);
bool comprobarFin(tJuego &juego);
void repartir(tJuego& juego);
bool buscarDoble(tJuego &juego);
int quienEmpieza(tJuego &juego, int& indice);
void estrategia1(tJuego &juego);
void estrategia2(tJuego &juego);
void guardarListaFichas(ofstream &salida, const tListaFicha listaFichas);
void cargarListaFichas(ifstream &entrada, tListaFicha &listaFichas);
void jugarMaquinas(tJuego &juego);
int sumarPuntos(tListaFicha lista);
void actualizarPuntos(tJuego &juego);
void mostrarPuntos(tJuego juego);
bool sinSalida(const tJuego juego);

// Programa principal

int main() {

	//Definición de variables
	bool fin = false, verFichas = false;
	int OpcionMenu;
	tJuego juego;
	srand(time(NULL));
	verFichas = jugadoresPuntosYFichas(juego);

	// Bucle principal del programa
	
	while (!fin) {	
		mostrarTablero(juego, verFichas);
		if (comprobarFin(juego)) { //comprobamos si se ha acabado la partida
			fin = true;			
		}
		else {
			if (juego.turno == 0) { // turno es del jugador
				cout << endl << endl << "Turno del Jugador" << endl;
				if (juego.pozo.cont == 0 && comprobarJugadorPuede(juego)) {//comprobamos si puede robar o puede colar una de sus fichas
					cout << endl << "No puedes colocar ninguna ficha ni robar porque el pozo esta vacio. SALTAMOS TU TURNO \n"; 
					Sleep(4000);
					juego.turno = (juego.turno + 1) % juego.numJug;//no puede, saltamos turno
				}
				else {
					OpcionMenu = MostrarMenu(); //si puede, que elija opcion
					fin = ejecutarOpcion(OpcionMenu, juego);
				}
			}
			else {
				jugarMaquinas(juego); //turno de las maquinas
			}
		}
	}
	system("pause");
	return 0;
}


// Implementación de las funciones

//Preguntamos si desea ver las fichas de sus oponentes y con que fichas y con cuantos jugadores desea jugar
bool jugadoresPuntosYFichas(tJuego &juego) {
	
	string enseñarFichas;
	int numJug = 0, nPuntos = 0;
	bool continuar = false, enseñar = false;
	while (!continuar) {
		cout << endl << "Indica el numero de jugadores contra los que desea jugar (min. 2 y max. 4):  ";
		cin >> numJug;
		if ((numJug >= 2) && (numJug <= 4)) continuar = true;
	}
	continuar = false;
	while (!continuar) {
		cout << endl << "Introduce el numero de puntos de la ficha (min.6 y max.9): ";
		cin >> nPuntos;
		if ((nPuntos >= 6) && (nPuntos <= 9)) continuar = true;
	}
	continuar = false;
	while (!continuar) {
		cout << endl << "Indica si desea ver las fichas de sus oponentes (S o N): ";
		cin >> enseñarFichas;
		if ((enseñarFichas == "S") || (enseñarFichas == "N") || (enseñarFichas == "s") || (enseñarFichas == "n")) continuar = true;
	}
	juego.numJug = numJug;
	juego.nPuntos = nPuntos;
	if (enseñarFichas == "S" || enseñarFichas == "s") enseñar = true;
	
	return enseñar;
}

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
		if ((0 <= opcion) && (opcion <= 6)) {
			opcionvalida = true;
		}
	}
	return opcion;
}

//generamos el pozo con todas las fichas que correspondan segun el tamaño que hemos indicado para nuestra ficha mas grande
void generarPozo(tJuego &juego) {
	int cont = 0;
	int nPuntos = juego.nPuntos;
	for (int i = 0; i <= nPuntos; i++) {
		for (int j = i; j <= nPuntos; j++) {
			juego.pozo.fichas[cont].valI = i;
			juego.pozo.fichas[cont].valD = j;
			cont++;
		}
	}
	juego.pozo.cont = cont;


}

//Mostramos el tablero tal cual nos pide en la practica
void mostrarTablero(tJuego juego, bool verFichas) {
	int contJ;
	tFicha ficha;
	string fichaVacia = "| - |";

	cout << endl << " -------------------- " << "\n" << "|" << "    T A B L E R O    " << "|" << "\n" << " -------------------- \n";
	cout << juego.tablero << endl << endl;


	for (int k = 1; k < juego.numJug; k++) {		
		cout << "Maquina #" << k  << "  ";
		contJ = juego.nJugador[k].listaFJug.cont; 
		for(int n = 0; n < contJ; n++) {
			ficha.valI = juego.nJugador[k].listaFJug.fichas[n].valI;
			ficha.valD = juego.nJugador[k].listaFJug.fichas[n].valD;
			if (!verFichas && (k != 0))
				cout << fichaVacia << " ";
			else
				cout << muestraFicha(ficha.valI, ficha.valD) << " ";
		}
		cout << endl;
		
	}
	
	cout << endl << "Jugador     ";
	for (int n = 0; n < juego.nJugador[0].listaFJug.cont; n++) {
		ficha.valI = juego.nJugador[0].listaFJug.fichas[n].valI;
		ficha.valD = juego.nJugador[0].listaFJug.fichas[n].valD;
		cout << muestraFicha(ficha.valI, ficha.valD) << " ";
	}
	cout << endl;
}

//Comprobamos las distintas situaciones en las que habria acabado la partida
bool comprobarFin(tJuego &juego) {
	bool fin = false, ganador;
	
	if (juego.ganado) { 
		int i = 0;
		while(i < juego.numJug || !ganador){
			juego.nJugador[i].acuPuntos = sumarPuntos(juego.nJugador[i].listaFJug);
			if (juego.nJugador[i].listaFJug.cont == 0) {
				fin = true;
				ganador = true;
				if (i == 0) {
					cout << endl << "HAS GANADO LA PARTIDA. ENHORABUENA.\n";
					actualizarPuntos(juego);
					mostrarPuntos(juego);
				
				}
				else {
					cout << endl << "EL GANADOR HA SIDO LA MAQUINA #" << i << endl;
					actualizarPuntos(juego);
					mostrarPuntos(juego);
				}
			}
			i++;
		}
	}
	
	else if (sinSalida(juego)) {
		fin = true;
		cout << endl<< "Partida bloqueada, ningun jugador puede continuar. ";
		actualizarPuntos(juego);
		mostrarPuntos(juego);
	}	
	return fin;
}

//Bucle que recorre todos los jugadores sumando los puntos conseguidos
void actualizarPuntos(tJuego &juego) {
	
	for (int i = 0; i < juego.numJug; i++) {
		juego.nJugador[i].acuPuntos = sumarPuntos(juego.nJugador[i].listaFJug);
	}
}

//Con la partida acabada, muestra los puntos de cada jugador
void mostrarPuntos(tJuego juego) {
	cout << "Los puntos de cada jugador son los siguientes: \n";
	for (int i = 0; i < juego.numJug; i++) {
		if (i == 0) {
			cout << "Jugador: " << juego.nJugador[i].acuPuntos << "\n";
		}
		else {
			cout << "Maquina #" << i << ": " << juego.nJugador[i].acuPuntos << "\n";
		}
	}
}

//Indica si el pozo se ha quedado sin piezas y, además, ningún jugador puede colocar ninguna de sus piezas
bool sinSalida(const tJuego juego) {
	bool sinSalida = true;
	if (juego.pozo.cont == 0) {
		int i = 0, j = 0;
		while (i < juego.numJug && sinSalida) {
			j = 0;
			while (j < juego.nJugador[i].listaFJug.cont && sinSalida) {
				if (puedePonerDer(juego,i, j)) {
					sinSalida = false;
				}
				else if (puedePonerIzq(juego,i, j)) {
					sinSalida = false;
				}
				j++;
			}
			i++;
		}

	}
	else {
		sinSalida = false;
	}
	return sinSalida;
}

//Comprueba que el jugador o puede hacer un movimiento o puede robar, sino se le salta el turno
bool comprobarJugadorPuede(tJuego juego) {
	bool pasarSiguenteJugador = true;
	int i = 0;
	while(i < juego.nJugador[juego.turno].listaFJug.cont && pasarSiguenteJugador) {
		if (puedePonerDer(juego, juego.turno, i) || puedePonerIzq(juego, juego.turno, i)) pasarSiguenteJugador = false;
		i++;
	}
	return pasarSiguenteJugador;
}

//Ejecutamos la opcion elegida del jugador
bool ejecutarOpcion(short int opcionMenu, tJuego &juego) {

	bool salir = false; // solo es true en caso de que el usuario quiera salir del juego
	int i = 0;
	short int ficha;
	switch (opcionMenu) {

	case 1:
		
		cout << "Introduzca el numero de la ficha que quiere colocar: ";
		cin >> ficha;
		if ((0 <= ficha - 1) && (ficha - 1 < juego.nJugador[juego.turno].listaFJug.cont)) {
			if (puedePonerIzq(juego,juego.turno, ficha - 1)) {
				ponerFichaIzq(juego, ficha - 1);
			}
			else {
				cout << "No se puede colocar la ficha seleccionada. ";
				system("pause");
			}
		}
		else {

			cout << "La ficha elegida no se encuentra entre las posibles. ";
			system("pause");

		}
		break;

	case 2:
		
		cout << "Introduzca el numero de la ficha que quiere colocar: ";
		cin >> ficha;
		if ((0 <= ficha - 1) && (ficha - 1 < juego.nJugador[juego.turno].listaFJug.cont)) {
			if (puedePonerDer(juego,juego.turno, ficha - 1)) {
				ponerFichaDer(juego, ficha - 1);
			}
			else {
				cout << "No se puede colocar la ficha seleccionada. ";
				system("pause");
			}
		}
		else {

		cout << "La ficha elegida no se encuentra entre las posibles. ";
		system("pause");

		}
		break;

	case 3:
		
		if (puedeRobar(juego)) { //primero comprobamos que el jugador no puede poner en el tablero ninguna ficha que ya tenga
			tFicha nuevaFicha = robarFicha(juego);
			juego.nJugador[juego.turno].listaFJug.fichas[juego.nJugador[juego.turno].listaFJug.cont] = nuevaFicha;
			juego.nJugador[juego.turno].listaFJug.cont++;
		}
		else {
			cout << "No necesita robar, puede colocar alguna de sus fichas. ";
			system("pause");

		}
		break;

	case 4:

		inicializarJuego(juego, i);
		
		break;
	case 5:

		cargarPartida(juego);

		break;
	case 6:

		guardarJuego(juego);
		break;

	case 0:
		salir = true;
	}

	return salir;

}

//crea la configuración inicial de la partida
void inicializarJuego(tJuego& juego, short int jugador) {
	
	generarPozo(juego);
	desordenaPozo(juego);
	repartir(juego);
	while (!buscarDoble(juego)) {
		desordenaPozo(juego);
		repartir(juego);
	}
	int indice = 0;
	int jugadorInicial = quienEmpieza(juego, indice);
	if (jugadorInicial == 0) {
		cout << endl <<"Tu comienzas esta partida, tienes el doble mas alto. SUERTE" << endl;
	}
	else {
		cout << endl <<"Empieza la partida la Maquina #" << jugadorInicial << endl;
	}
	juego.turno = (juego.turno + 1) % juego.numJug;
	Sleep(3000);
}

//Busca si algun jugador tiene una ficha doble para poder comenzar la partida, si ninguno tiene se vuelve a barajear y repartir
bool buscarDoble(tJuego &juego) {
	int contF;
	int valI, valD;
	int numJug = juego.numJug;
	bool hayDoble = false;
	int k = 0;
	while (!hayDoble && k < juego.numJug) {
		contF = juego.nJugador[k].listaFJug.cont;
		int i = 0;
		while(!hayDoble && i < contF) {
			valI = juego.nJugador[k].listaFJug.fichas[i].valI;
			valD = juego.nJugador[k].listaFJug.fichas[i].valD;
			if (valI == valD) {
				hayDoble = true;
			}
			i++;
		}
		k++;
	}
	if (!hayDoble) {
		cout << "No hay ningún doble. Se reparten nuevas fichas";
		juego.pozo.cont = (juego.nPuntos + 1) * (juego.nPuntos + 2) / 2;
		bool nReparto = true;
	}
	return hayDoble;
}

//Suma los dos digito de la ficha de todas las fichas que le quedan a cada jugador una vez que la partida ha acabado
int sumarPuntos(tListaFicha lista) {

	short int suma = 0;
	int i = 0;
	while (i < lista.cont) {
		suma = suma + lista.fichas[i].valD + lista.fichas[i].valI;
		i++;
	}
	return suma;
}

//Guardamos la partida en un fichero si el usuario lo ha deseado
void guardarJuego(const tJuego juego) {

	string nombreFichero;
	ofstream fileOut;
	cout << "Introduce el nombre del fichero de tu partida: "; cin >> nombreFichero;
	fileOut.open(nombreFichero);
	if (fileOut.is_open()) {
		fileOut << juego.numJug << " " << juego.nPuntos << "\n"
			<< juego.tablero << "\n"
			<< juego.pozo.cont << "\n";
		guardarListaFichas(fileOut, juego.pozo);
		for (int i = 0; i < juego.numJug; i++) {
			fileOut << "\n" << juego.nJugador[i].listaFJug.cont << "\n";
			guardarListaFichas(fileOut, juego.nJugador[i].listaFJug);
			fileOut << "\n" << juego.nJugador[i].acuPuntos;
		}

	}

	fileOut.close();
}

//Cargamos la partida de un fichero si el usuario lo ha deseado
void cargarPartida(tJuego &juego) {

	ifstream fileIn;
	string nombreFichero;
	bool ficheroCorrecto = false;
	bool salir = false;
	cout << "Introduce el nombre del fichero con la partida que desea jugar: ";

	while (!ficheroCorrecto) {
		cin >> nombreFichero;
		fileIn.open(nombreFichero);
		if (fileIn.is_open()) {
			fileIn >> juego.numJug;
			fileIn >> juego.nPuntos;
			fileIn >> juego.tablero;
			fileIn >> juego.pozo.cont;
			cargarListaFichas(fileIn, juego.pozo);
			for (int i = 0; i < juego.numJug; i++) {
				fileIn >> juego.nJugador[i].listaFJug.cont;
				cargarListaFichas(fileIn, juego.nJugador[i].listaFJug);
				fileIn >> juego.nJugador[i].acuPuntos;
			}
			ficheroCorrecto = true;
		}
		else {
			cout << "La partida introducida no existe. ";
			system("pause");
		}
	}
}

//Mostramos o la ficha del jugador o el tablero
string muestraFicha(int izq, int der) {

	return "|" + to_string(izq) + "-" + to_string(der) + "|";

}

//Comprobamos que la opcion elegida de poner la ficha a la izquierda del tablero es posible
bool puedePonerIzq(tJuego juego, int jugador, short int ficha) {

	bool puede = false;
	//coprobamos si uno de los dos numeros de la ficha son iguales al borde de la derecha del tablero
	if (NumTableroIzq(juego) == juego.nJugador[jugador].listaFJug.fichas[ficha].valI ||
		NumTableroIzq(juego) == juego.nJugador[jugador].listaFJug.fichas[ficha].valD) {
		puede = true;
	}
	
	return puede;

}

//Comprobamos que la opcion elegida de poner la ficha a la derecha del tablero es posible
bool puedePonerDer(tJuego juego, int jugador, short int ficha) {

		bool puede = false;
		//comprobamos si uno de los dos numeros de la ficha son iguales al borde de la derecha del tablero
		if (NumTableroDer(juego) == juego.nJugador[jugador].listaFJug.fichas[ficha].valI || 
			NumTableroDer(juego) == juego.nJugador[jugador].listaFJug.fichas[ficha].valD) {
			puede = true;
		}	

	return puede;

}

//Añadimos la ficha del jugador a la izquierda del tablero por el numero que correspoda de la ficha
void ponerFichaIzq(tJuego &juego, short int ficha) {


		if (juego.nJugador[juego.turno].listaFJug.fichas[ficha].valI == NumTableroIzq(juego)) { //comprobamos cual de los dos numeros de la ficha es igual al borde del tablero por la izq

			juego.tablero = "|" + to_string(juego.nJugador[juego.turno].listaFJug.fichas[ficha].valD) + "-" +
				to_string(juego.nJugador[juego.turno].listaFJug.fichas[ficha].valI) + "|" + juego.tablero; //introducimos la ficha por el numero de la izq al tablero

		}
		else {
			juego.tablero = "|" + to_string(juego.nJugador[juego.turno].listaFJug.fichas[ficha].valI) + "-" +
				to_string(juego.nJugador[juego.turno].listaFJug.fichas[ficha].valD) + "|" + juego.tablero; //introducimos la ficha por el numero de la der al tablero

		}
		eliminarFicha(juego, ficha);//fichasJugador, fichasCont, seleccion - 1); //como el jugador ha usado su ficha, le añadimos una nueva
		if (juego.nJugador[juego.turno].listaFJug.cont == 0) juego.ganado = true;
		juego.turno = (juego.turno + 1) % juego.numJug;

}

//Añadimos la ficha del jugador a la derecha del tablero por el numero que corresponda de la ficha
void ponerFichaDer(tJuego &juego, short int ficha) {

		
		if (juego.nJugador[juego.turno].listaFJug.fichas[ficha].valI == NumTableroDer(juego)) { 
				//introducimos la ficha por el numero de la izq al tablero
			juego.tablero = juego.tablero + "|" + to_string(juego.nJugador[juego.turno].listaFJug.fichas[ficha].valI) + "-" + 
				to_string(juego.nJugador[juego.turno].listaFJug.fichas[ficha].valD) + "|";
		}
		else {
			//introducimos la ficha por el numero de la derecha al tablero
			juego.tablero = juego.tablero + "|" + to_string(juego.nJugador[juego.turno].listaFJug.fichas[ficha].valD) + "-" + 
				to_string(juego.nJugador[juego.turno].listaFJug.fichas[ficha].valI) + "|";
		}
		eliminarFicha(juego, ficha); //como el jugador ha usado su ficha, le añadimos una nueva
		if (juego.nJugador[juego.turno].listaFJug.cont == 0) juego.ganado = true;
		juego.turno = (juego.turno + 1) % juego.numJug;
	
}

//Se selecciona la ultima ficha del pozo con la finalidad de añadirla al array del jugador y se le resta uno al numero de fichas del pozo
tFicha robarFicha(tJuego &juego) {

	tFicha ficha;
	if (juego.pozo.cont > 0) {
		ficha = juego.pozo.fichas[juego.pozo.cont - 1];
		juego.pozo.cont--;
	}
	else {
		juego.turno = (juego.turno + 1) % juego.numJug;
	}
	return ficha;
}

//Comprobamos las fichas con los dos extremos del tablero para saber si alguna ficha se puede colocar o no antes de robar una ficha
bool puedeRobar(tJuego juego) {

	bool robar = true;
	int i = 0;
	while (i < juego.nJugador[juego.turno].listaFJug.cont) {
		if ((juego.nJugador[juego.turno].listaFJug.fichas[i].valD == NumTableroDer(juego)) || 
			(juego.nJugador[juego.turno].listaFJug.fichas[i].valD == NumTableroIzq(juego)) ||
			(juego.nJugador[juego.turno].listaFJug.fichas[i].valI == NumTableroDer(juego)) || 
			(juego.nJugador[juego.turno].listaFJug.fichas[i].valI == NumTableroIzq(juego))) {

			robar = false;
		}
		i++;

	}

	return robar;
}

//Devolvemos la ficha mas a la izquierda del tablero
int NumTableroIzq(tJuego juego) {
	int numero = atoi(juego.tablero.substr(1, 1).c_str());
	return numero;
}

//Devolvemos el numero mas a la derecha del tablero
int NumTableroDer(tJuego juego) {
	int numero = atoi(juego.tablero.substr(juego.tablero.size() - 2, 1).c_str());
	return numero;
}

//Devolvemos el numero de la izquierda de una ficha
int NumFichaIzq(string ficha) {
	int numero = atoi(ficha.substr(0, 1).c_str());
	return numero;
}

//Devolvemos el numero de la derecha de una ficha
int NumFichaDer(string ficha) {
	int numero = atoi(ficha.substr(2, 1).c_str());
	return numero;
}

//Cuando el jugador pone una ficha de su baraja en el tablero desplazamos todas las siguientes una posicion hacia adelante
void eliminarFicha(tJuego &juego, short int fichaNum) {

	int i = fichaNum;
	while (i < juego.nJugador[juego.turno].listaFJug.cont) {
		juego.nJugador[juego.turno].listaFJug.fichas[i] = juego.nJugador[juego.turno].listaFJug.fichas[i + 1];
		i++;
	}
	juego.nJugador[juego.turno].listaFJug.cont--;
}

//Desordenamos los pozos de manera aleatoria
void desordenaPozo(tJuego &juego) {
	int idx1, idx2;
	tFicha ficha;
	//int val1, val2;
	int nFichas = juego.pozo.cont;
	for (int i = 0; i < ALEA; i++) {
		idx1 = rand() % nFichas;
		idx2 = rand() % nFichas;
		if (idx1 != idx2) {
			ficha = juego.pozo.fichas[idx1];
			//val1 = juego.pozo.fichas[idx1].valI;
			//val2 = juego.pozo.fichas[idx1].valD;
			juego.pozo.fichas[idx1] = juego.pozo.fichas[idx2];
			//juego.pozo.fichas[idx1].valI = juego.pozo.fichas[idx2].valI;
			//juego.pozo.fichas[idx1].valD = juego.pozo.fichas[idx2].valD;
			juego.pozo.fichas[idx2] = ficha;
			//juego.pozo.fichas[idx2].valI = val1;
			//juego.pozo.fichas[idx2].valD = val2;
		}
	}
}

//Devuelve quién empieza (0: humano, >0: máquina, -1: nadie) y, en el índice, la posición de la ficha con la que empezar
int quienEmpieza(tJuego &juego, int& indice) {
	int contF = 0, puntos = 0, turno = -1;
	short int fichaNum;
	int valI, valD;
	int numJug = juego.numJug;
	int doble = -1;
	for (int k = 0; k < numJug; k++) {
		contF = juego.nJugador[k].listaFJug.cont;
		for (int i = 0; i < contF; i++) {
			valI = juego.nJugador[k].listaFJug.fichas[i].valI;
			valD = juego.nJugador[k].listaFJug.fichas[i].valD;
			if (valI == valD){
				if (valI > doble) {
					fichaNum = i;
					doble = valI;
					indice = i;
					turno = k;
				}
			}
		}
	}

	juego.tablero = "|" + to_string(doble) + "-" + to_string(doble) + "|";
	juego.turno = turno;
	eliminarFicha(juego, fichaNum);
	return turno;
}

void repartir(tJuego& juego) {

	int acuPuntos = 0;
	int contP = juego.pozo.cont - 1;
	int valI, valD;
	int numJug = juego.numJug;
	
	for (int k = 0; k < numJug; k++) {
//		if (true)//nReparto)
//   		acuPuntos = juego.nJugador[k].acuPuntos;
		juego.nJugador[k].listaFJug.cont = 0;
		juego.nJugador[k].acuPuntos = acuPuntos;
		int contJ = 0;
		for (int i = 0; i < REPARTO; i++) {
			valI = juego.pozo.fichas[contP].valI;
			valD = juego.pozo.fichas[contP].valD;
			juego.nJugador[k].listaFJug.fichas[i].valI = valI;
			juego.nJugador[k].listaFJug.fichas[i].valD = valD;
			contJ++;
			contP--;
		}
		juego.nJugador[k].listaFJug.cont = contJ;
		juego.pozo.cont = contP + 1;
	}
}

//Realiza el movimiento del jugador máquina correspondiente utilizando la estrategia 1: pondrá la primera ficha que pueda colocar
void estrategia1(tJuego &juego){
	bool fichaEncontrada = false;
	int i = 0;
	while (i < juego.nJugador[juego.turno].listaFJug.cont && !fichaEncontrada) {
		if (puedePonerIzq(juego,juego.turno, i)) {
			ponerFichaIzq(juego, i);
			fichaEncontrada = true;
		}
		else if (puedePonerDer(juego,juego.turno, i)) {
				ponerFichaDer(juego, i);
				fichaEncontrada = true;
		}
		i++;
	}
	if (!fichaEncontrada) {
		if (juego.pozo.cont != 0) {
			juego.nJugador[juego.turno].listaFJug.fichas[juego.nJugador[juego.turno].listaFJug.cont] = robarFicha(juego);
			juego.nJugador[juego.turno].listaFJug.cont++;
		}
		else {
			juego.turno = (juego.turno + 1) % juego.numJug;
		}
	}
}

//Realiza el movimiento del jugador máquina correspondiente utilizando la estrategia 2: pondrá la que sume más puntuación entre sus dos valores
void estrategia2(tJuego &juego) {
	
	bool realizoMovimiento = false;
	int suma = 0, fichaDer, fichaIzq, sumaMaxima = 0, posFicha = -1;
	for (int i = 0; i < juego.nJugador[juego.turno].listaFJug.cont; i++) {
		if (puedePonerDer(juego, juego.turno, i) || puedePonerIzq(juego, juego.turno, i)) {
			fichaDer = juego.nJugador[juego.turno].listaFJug.fichas[i].valD;
			fichaIzq = juego.nJugador[juego.turno].listaFJug.fichas[i].valI;
			suma = fichaDer + fichaIzq;
			if (suma > sumaMaxima) {
				sumaMaxima = suma;
				posFicha = i;
			}
		}
	}
	if (posFicha != -1) { // si posFicha es -1 significa que no se puede colocar ninguna ficha de las que tiene
		if (puedePonerIzq(juego, juego.turno, posFicha)) {
			ponerFichaIzq(juego, posFicha);
		}
		else if (puedePonerDer(juego, juego.turno, posFicha)) {
			ponerFichaDer(juego, posFicha);
		}
	}
	else {
		if (juego.pozo.cont > 0) {
			juego.nJugador[juego.turno].listaFJug.fichas[juego.nJugador[juego.turno].listaFJug.cont] = robarFicha(juego);
			juego.nJugador[juego.turno].listaFJug.cont++;
		}
		else {
			juego.turno = (juego.turno + 1) % juego.numJug;
		}
	}
	
}

void guardarListaFichas(ofstream &salida, const tListaFicha listaFichas) {
	
	for (int i = 0; i < listaFichas.cont; i++) {
		salida << to_string(listaFichas.fichas[i].valI) << " " << to_string(listaFichas.fichas[i].valD) << " ";
	}
}

//Carga las fichas del siguiente renglón del archivo de entrada
void cargarListaFichas(ifstream &entrada, tListaFicha &listaFichas) {

	for (int i = 0; i < listaFichas.cont; i++) {
		entrada >> listaFichas.fichas[i].valI;
		entrada >> listaFichas.fichas[i].valD;
	}
}

//Realiza el movimiento de las maquinas segun la maquina que sea y la estrategia
void jugarMaquinas(tJuego &juego) {

	if (juego.turno == 1) {
		//ESTRATEGIA 2
		cout << endl << "Turno de la Maquina #" << juego.turno << endl;
		Sleep(4000);
		estrategia2(juego);
	}
	else {
		//ESTRATEGIA 1
		cout << endl << "Turno de la Maquina #" << juego.turno << endl;
		Sleep(4000);
		estrategia1(juego);
	}
}