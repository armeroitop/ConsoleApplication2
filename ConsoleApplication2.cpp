// ConsoleApplication2.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include "ConsoleApplication2.h"
#include <list>
#define IZQUIERDA 75
#define DERECHA 77
#define	ARRIBA 72
#define ABAJO 80
//agrego esta linea para comprobar cambios en git
//tambien agrego esta otra


using namespace std;

void gotoxy(int x, int y)
{
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD dwPos;
	dwPos.X = x;
	dwPos.Y = y;

	SetConsoleCursorPosition(hCon, dwPos);
}
void OcultarCursor() {
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 2;
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(hCon, &cci);
}
void pintar_limites_ventana() {
	for (size_t i = 2; i < 78; i++)
	{
		gotoxy(i, 3); printf("%c", 205);
		gotoxy(i, 33); printf("%c", 205);
	}
	for (size_t i = 4; i < 33; i++)
	{
		gotoxy(2, i); printf("%c", 186);
		gotoxy(77, i); printf("%c", 186);
	}
	gotoxy(2,3);   printf("%c", 201);
	gotoxy(2,33);  printf("%c", 200);
	gotoxy(77,3);  printf("%c", 187);
	gotoxy(77,33); printf("%c", 188);
}
void actualiza_puntuaciones(int _puntos) {
	gotoxy(5, 2);	printf("Puntos %d", _puntos);
}

class Nave
{
	int x, y, corazones, vidas ;
public:
	Nave(int _x, int _y,int _corazones,int _vidas):	x(_x), y(_y), corazones(_corazones),vidas(_vidas){};/*Este es el constructor en la forma mas reducida de escribirlo. A que mola*/
	int X() { return x; }
	int Y() { return y; }
	int GetVidas() { return vidas; }
	void pintar();
	void borrar();
	void borrar_explolsion();
	void mover();
	void pintar_corazones();
	void resta_corazon() { corazones--; }
	void morir();
};
void Nave::pintar() {
	gotoxy(x+2, y);   printf("%c", 30);
	gotoxy(x+1, y+1); printf("%c%c%c", 40,207,41);
	gotoxy(x, y+2);   printf("%c%c %c%c", 30,190,190,30);
}
void Nave::borrar() {
	gotoxy(x+2, y);     printf(" ");
	gotoxy(x+1, y + 1); printf("   ");
	gotoxy(x, y + 2);   printf("     ");

}
void Nave::borrar_explolsion() {
	gotoxy(x, y);     printf("        ");
	gotoxy(x, y + 1); printf("        ");
	gotoxy(x, y + 2); printf("        ");
	pintar_limites_ventana();/*refrescamos los limites de la pantalla para que no se queden tuertos*/
}
void Nave::mover() {
	if (_kbhit())
	{
		char tecla = _getch();
		Nave::borrar();
		if (tecla == IZQUIERDA && x > 3) x--;
		if (tecla == DERECHA && (x + 5) < 77) x++;
		if (tecla == ARRIBA && y > 4) y--;
		if (tecla == ABAJO && (y + 3)<33) y++;
		if (tecla == 'e')corazones--;
		pintar();/*Nos hace falta poner el "Nave::" antes de llamar al metodo??
				 la respuesta es que parece ser que no*/
		pintar_corazones();
	}
}
void Nave::pintar_corazones() {

	gotoxy(50, 2); printf("Vidas %d",vidas);
	gotoxy(64, 2); printf("Salud");
	gotoxy(70, 2); printf("     ");
	for (size_t i = 0; i < corazones; i++)
	{
		gotoxy(70+i, 2); printf("%c",3);
	}
}
void Nave::morir() {
	if (corazones == 0)
	{
		borrar_explolsion();
		gotoxy(x,y);   printf("   **   ");
		gotoxy(x,y+1); printf("  ****  ");
		gotoxy(x,y+2); printf("   **   ");

		Sleep(200);
		borrar_explolsion();
		gotoxy(x, y);     printf("* *  * *");
		gotoxy(x, y + 1); printf(" * ** * ");
		gotoxy(x, y + 2); printf("* *  * *");

		Sleep(200);
		vidas--;
		corazones = 3;
		pintar_corazones();
		borrar_explolsion();
		pintar();		
	}
}

class Asteroide
{
	int x, y;
public:
	Asteroide(int _x,int _y):x(_x),y(_y){}
	int X() { return x; }
	int Y() { return y; }
	void pintar();
	void mover();
	void choque(class Nave &N);
};
void Asteroide::pintar()
{
	gotoxy(x, y); printf("%c", 184);
}
void Asteroide::mover()
{
	gotoxy(x, y); printf(" ");
	y++;
	if (y > 32)
	{
		x = rand() % 73 + 3;
		y = 4;
	}
	pintar();
}
void Asteroide::choque(class Nave &N) {
	if (x >= N.X() && x <= (N.X() + 5) && y>=N.Y() && y<=(N.Y()+2))
	{
		N.resta_corazon();
		N.borrar_explolsion();
		N.pintar();
		N.pintar_corazones();
		x = rand() % 73 + 3;
		y = 4;
	}
}

class Bala
{
	int x, y;
public:
	Bala(int _x,int _y):x(_x),y(_y){}
	int X() { return x; }
	int Y() { return y; }
	void mover();
	bool fueraPantalla();
};
void Bala::mover() {
	gotoxy(x, y); printf(" ");
	y--;
	gotoxy(x, y); printf("^");
}
bool Bala::fueraPantalla() {
	if (y == 4)return true;
	return false;
}

int main()
{	

	int puntos=0;
	OcultarCursor();
	pintar_limites_ventana();	
	Nave N(35, 29,3,3);
	N.pintar();
	N.pintar_corazones();
	actualiza_puntuaciones(puntos);

	

	list<Asteroide*>listaAsteroides;
	list<Asteroide*>::iterator itAsteroides;
	for (size_t i = 0; i < 5; i++)
	{
		listaAsteroides.push_back(new Asteroide(rand() % 76 + 3,4));
	}

	list<Bala*>listaBalas;
	list<Bala*>::iterator itBalas;

	bool game_over = false;
	while (!game_over)
	{	
		if (_kbhit())
		{
			char tecla = _getch();
			if (tecla == 'a')
				listaBalas.push_back(new Bala(N.X() + 2, N.Y() - 1));
		}
		for (itBalas=listaBalas.begin();itBalas!=listaBalas.end();itBalas)
		{
			(*itBalas)->mover();
			if ((*itBalas)->fueraPantalla())
			{
				gotoxy((*itBalas)->X(), (*itBalas)->Y()); printf(" ");
				delete(*itBalas);
				itBalas = listaBalas.erase(itBalas);
			}
			else { itBalas++; }
		}

		N.morir(); N.mover();
		for (itAsteroides = listaAsteroides.begin(); itAsteroides != listaAsteroides.end(); itAsteroides++)
		{
			(*itAsteroides)->mover();
			(*itAsteroides)->choque(N);
			for (itBalas = listaBalas.begin(); itBalas != listaBalas.end(); itBalas)
			{
				if ((*itAsteroides)->X() == (*itBalas)->X()
					&& ((*itAsteroides)->Y() + 1 == (*itBalas)->Y()
					|| (*itAsteroides)->Y()     == (*itBalas)->Y()))
				{
					gotoxy((*itBalas)->X(), (*itBalas)->Y()); printf(" ");
					delete(*itBalas);
					itBalas = listaBalas.erase(itBalas);

					listaAsteroides.push_back(new Asteroide(rand() % 76 + 3, 4));
					gotoxy((*itAsteroides)->X(), (*itAsteroides)->Y()); printf(" ");
					delete(*itAsteroides);
					itAsteroides = listaAsteroides.erase(itAsteroides);

					puntos++;
					actualiza_puntuaciones(puntos);
				}
				else { itBalas++; }
			}
		}
		//sif (N.GetVidas() <= 0) { game_over = true; }
		
		Sleep(30);
	}
    return 0;
}

