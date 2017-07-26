//Amanoviæ Stefan ELITE-3/14, Igra Snake 

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include<ctype.h>
#include <windows.h>
#include <process.h>

#define GORE 72 
#define DOLE 80
#define LEVO 75
#define DESNO 77

int duzina;//duzina zmije
int br;//broj skretanja
int len;//sledeci segment posle skretanja
char tipka;//unos za vreme toka igre
int zivot;//broj pokusaja

void Korak();//galvna f-ja zaduzena za kretanje
void Hrana();///postavljanje hrane
void Skretanje();//f-ja za skretanje

void Dole();//f-je za kretanje u odredjenom smeru
void Levo();//
void Gore();//
void Desno();//

void mapa(int x, int y);//gotoxy f-ja za pomeranje kursora na odredjenu lokaciju kao parametre uzima standarad handle i zadate koordinate
void petlja_kas(long double);//petlja za kasnjenje
void Okvir();//postavlja okvir
void Kraj_igre();//f-ja zaduzena za detektovanja gubitka zivota
void ucitavanje();//f-ja za prikaz ekrana za ucitavanje
void Uputstva();//uputstva i pravila igre
int rezultat_ekran();//ispisuje rezultat i br. zivota na ekranu
int rezultat();//vraca rezultat kao int vrednost
void rekord();//cuva rekorde u datoteci rekordi.txt

struct koordinate {
	int x;
	int y;
	int smer;
};

typedef struct koordinate koordinate;

koordinate glava, *krivina, hrana, *telo;

int main()
{

	char tipka;
	Uputstva();
	system("cls"); //cisti ekran
	ucitavanje();
	krivina = (koordinate*)malloc((sizeof koordinate) * 500);
	telo = (koordinate*)malloc((sizeof koordinate) * 30);
	duzina = 5; //pocetna duzina,koordinate i smer zmije
	glava.x = 25;
	glava.y = 20;
	glava.smer = DESNO;

	Okvir();
	Hrana();
	zivot = 1; //broj zivota 
	krivina[0] = glava;
	Korak();

	return 0;

}

void Korak() {
	int i, a;
	do {
		Hrana();
		fflush(stdin);
		len = 0;

		petlja_kas(duzina);
		Okvir();
		if (glava.smer == DESNO)
			Desno();
		else if (glava.smer == LEVO)
			Levo();
		else if (glava.smer == DOLE)
			Dole();
		else if (glava.smer == GORE)
			Gore();
		Kraj_igre();
	} while (!kbhit());

	a = getch();//pauza
	if (a == 27)
	{
		system("cls");
		free(krivina);
		free(telo);
		exit(0);
	}

	tipka = getch();//upravljanje zmijom
	if ((tipka == DESNO&&glava.smer != LEVO&&glava.smer != DESNO) || (tipka == LEVO&&glava.smer != DESNO&&glava.smer != LEVO) || (tipka == GORE&&glava.smer != DOLE&&glava.smer != GORE) || (tipka == DOLE&&glava.smer != GORE&&glava.smer != DOLE))
	{
		br++;
		krivina[br] = glava;
		glava.smer = tipka;
		if (tipka == GORE)
			glava.y--;
		if (tipka == DOLE)
			glava.y++;
		if (tipka == DESNO)
			glava.x++;
		if (tipka == LEVO)
			glava.x--;
		Korak();
	}

	else if (tipka == 27)//ESC
	{
		system("cls");
		exit(0);
		free(krivina);
		free(telo);
	}

	else {
		printf("\a"); Korak();


	}
}

void mapa(int x, int y)
{
	COORD koordinata;
	koordinata.X = x;
	koordinata.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), koordinata);
}

void ucitavanje()
{
	int j, i;
	mapa(34, 14);
	printf("ucitavanje...");
	mapa(30, 15);
	for (j = 1; j <= 20; j++) {
		for (i = 0; i <= 100000000; i++);//da bi se sporije ispisivali
		if (kbhit())//sprecava unos strane kretanja pre pocetka igre, izalaza ili pauze prilikom "ucitavanja"
			getch();
		printf("%c", 178);
	}
}

void Gore()
{
	int i;
	for (i = 0; i <= (krivina[br].y - glava.y) && len<duzina; i++)//y koordinata skretanja se umanjuje za y koordinatu glave takodje ispis staje onog trenutka kad broj zaostalih segmenata zmije predje samu duzinu zmije kako ne bismo imali uaostale segmente ispisane po ekranu
	{
		mapa(glava.x, glava.y + i);//postavljamo pocetne koordinate za ispis 
		{
			if (len == 0)
				printf("^");//glava je uvek 0
			else
				printf("*");
		}
		telo[len].x = glava.x;//x koordinata segmenata zmije
		telo[len].y = glava.y + i;//y koordinata segmenta zmije (posto idemo gore y++)
		len++;//prelazimo na sledeci segment
	}
	Skretanje();
	if (!kbhit())//fja kbhit vraca 0 ako nije pritisnut ni jedan taster u suprotnom je 1
		glava.y--;
}
void Dole()//isto kao i za f-ju gore s tim sto se od y koord. glave oduzima koord. krivine i y se umadjuje 
{
	int i;
	for (i = 0; i <= (glava.y - krivina[br].y) && len<duzina; i++)
	{
		mapa(glava.x, glava.y - i);
		{
			if (len == 0)
				printf("v");
			else
				printf("*");
		}
		telo[len].x = glava.x;
		telo[len].y = glava.y - i;
		len++;
	}
	Skretanje();
	if (!kbhit())
		glava.y++;
}
void petlja_kas(long double broj)
{
	rezultat_ekran();
	long double i;
	for (i = 0; i <= (10000000); i++);//mozemo podesavati brzinu zmije sto bi pomoglo pri izradi kampanje
}
void Kraj_igre() {
	int i, p = 0;
	for (i = 4; i<duzina; i++)   //pocinje sa 4 jer jos treba makar 4 elementa da bi dotakla samu sebe
	{
		if (telo[0].x == telo[i].x && telo[0].y == telo[i].y)//proverava da li je zmija udarila u samu sebe
			p++;
		if (i == duzina || p != 0)
			break;
	}
	if (glava.x <= 10 || glava.x >= 70 || glava.y <= 10 || glava.y >= 30 || p != 0) //ako je glava zmije udarila u bilo koji od zidova ili je p razlicit od 0
	{
		zivot--;
		if (zivot >= 0)
		{
			glava.x = 25; //pocetne koordinate ako niste izgubili sve zivote
			glava.y = 20;
			br = 0;
			glava.smer = DESNO;
			Korak();
		}
		else
		{
			system("cls");
			mapa(15, 2);
			printf(">> Nije vam preostalo zivota, vise srece drugi put!!!\n\n\t\t>> Pritisnite bilo koje dugme za kraj igre\n");
			rekord();
			free(krivina);
			free(telo);
			exit(0);

		}
	}
}
void Hrana()
{
	if (glava.x == hrana.x&&glava.y == hrana.y)//ako je galva dosla na iste koordinate kao i hrana generise se nova hrana sa random koordinatama i duzina zmije se povecava
	{
		duzina++;
		time_t a; //random mesto postavljanja
		a = time(0);
		srand(a);
		hrana.x = rand() % 70;
		if (hrana.x <= 10)//postavljanje hrane u granici okvira
			hrana.x += 11;
		hrana.y = rand() % 30;
		if (hrana.y <= 10)
			hrana.y += 11;
	}
	else if (hrana.x == 0)//pravljenje hrane prvi put (u granici okvira)
	{
		hrana.x = rand() % 70;
		if (hrana.x <= 10)
			hrana.x += 11;
		hrana.y = rand() % 30;
		if (hrana.y <= 10)
			hrana.y += 11;
	}
}
void Levo()//levo i desno isti princip kao gore i dole samo sto se pormene obavljaju na x osi
{
	int i;
	for (i = 0; i <= (krivina[br].x - glava.x) && len<duzina; i++)
	{
		mapa((glava.x + i), glava.y);
		{
			if (len == 0)
				printf("<");
			else
				printf("*");
		}
		telo[len].x = glava.x + i;
		telo[len].y = glava.y;
		len++;
	}
	Skretanje();
	if (!kbhit())
		glava.x--;

}
void Desno()
{
	int i;
	for (i = 0; i <= (glava.x - krivina[br].x) && len<duzina; i++)
	{

		telo[len].x = glava.x - i;
		telo[len].y = glava.y;
		mapa(telo[len].x, telo[len].y);
		{
			if (len == 0)
				printf(">");
			else
				printf("*");
		}

		len++;
	}
	Skretanje();
	if (!kbhit())
		glava.x++;
}
void Skretanje()
{
	int i, j, pom;
	for (i = br; i >= 0 && len<duzina; i--)
	{
		if (krivina[i].x == krivina[i - 1].x)//pitamo da li su segment krivine i zaostali element na istoj x koordinati
		{
			pom = krivina[i].y - krivina[i - 1].y;//odredjujemo da li su zaostali segmenti sa gornje ili donje strane u zavisnosti od znaka promenljive pom, jednakost x koordinate ukazuje na to da je skretanje ili levo ili desno
			if (pom<0)
				for (j = 1; j <= (-pom); j++)
				{//postavljanje koordinata segmenta i ispisivanje istog
					telo[len].x = krivina[i].x;
					telo[len].y = krivina[i].y + j;
					mapa(telo[len].x, telo[len].y);
					printf("*");
					len++;
					if (len == duzina)
						break;
				}
			else if (pom>0)
				for (j = 1; j <= pom; j++)
				{
					telo[len].x = krivina[i].x;
					telo[len].y = krivina[i].y - j;
					mapa(telo[len].x, telo[len].y);
					printf("*");
					len++;
					if (len == duzina)
						break;
				}
		}
		else if (krivina[i].y == krivina[i - 1].y)//sada pitamo da li su segment krivine i zaostali element na istoj y koordinati
		{
			pom = krivina[i].x - krivina[i - 1].x;//odredjujemo da li su zaostali segmenti sa leve ili desne strane u zavisnosti od znaka promenljive pom, jednakost y koordinate ukazuje na to da je skretanje ili gore ili dole
			if (pom<0)
				for (j = 1; j <= (-pom) && len<duzina; j++)
				{
					//postavljanje koordinata segmenta i ispisivanje istog
					telo[len].x = krivina[i].x + j;
					telo[len].y = krivina[i].y;
					mapa(telo[len].x, telo[len].y);
					printf("*");
					len++;
					if (len == duzina)
						break;
				}
			else if (pom>0)
				for (j = 1; j <= pom&&len<duzina; j++)
				{

					telo[len].x = krivina[i].x - j;
					telo[len].y = krivina[i].y;
					mapa(telo[len].x, telo[len].y);
					printf("*");
					len++;
					if (len == duzina)
						break;
				}
		}
	}
}
void Okvir()
{
	system("cls");
	int i;
	mapa(hrana.x, hrana.y);   /*Prikaz hrane*/
	printf("%c", 254);
	for (i = 10; i<71; i++)
	{
		mapa(i, 10);
		printf("#");
		mapa(i, 30);
		printf("#");
	}
	for (i = 10; i<31; i++)
	{
		mapa(10, i);
		printf("#");
		mapa(70, i);
		printf("#");
	}
}
void Uputstva()
{

	printf("\tDobrodosli u igru Snake (pritisnite bilo sta za nastavak)\n");
	getch();
	system("cls");
	printf("\tPravila igre:\n");
	printf("\n>> Koristite strelice da pomerate zmiju.\n\n>> Hrana ce se pojavljivati na nasumicnim mestima na ekranu ('X'). \nSvaki put kada je pojedete duzina zmije ce se povecati za 1 segment,\n a samim tim i vas rezultat.\n\n>> Omogucena su vam 2 zivota, vasi zivoti ce se smanjivati ako udarite u zid ili u neki od segmenata zmije.\n\n>> Mozete pauzirati igru pritiskom na bilo koje dugme sem strelica i ESC. \nDa bi nastavili s igrom pritisnite bilo koje dugme\n\n>> Ako zelite da napustite igru pritisnite ESC \n");
	printf("\n\nPritisnite bilo koje dugme da biste igrali igru");
	if (getch() == 27)
		exit(0);
}
void rekord() {
	char ime[20], cha, c;
	int i, r;
	FILE *info;
	info = fopen("rekordi.txt", "a+");
	getch();
	system("cls");
	mapa(30, 2);//meni za unos imena igraca
	printf("Unesite svoje ime\n");
	mapa(30, 3);
	for (i = 0; i <= 16; i++)
		printf("%c", '_');
	mapa(30, 5);
	for (i = 0; i <= 16; i++)
		printf("%c", '-');
	mapa(30, 4);
	scanf("%s", ime);
	getchar();

	ime[0] = toupper(ime[0]);//Kako bi poèetno slovo imena bilo veliko
	fprintf(info, "Ime igraca: %s\n", ime);
	time_t vreme;//ispis datuma i vremena
	vreme = time(NULL);
	fprintf(info, "Vreme rezultata: %s", ctime(&vreme));
	fprintf(info, "Rezultat: %d\n", r = rezultat());//ispis rezultata

	for (i = 0; i <= 50; i++)
		fprintf(info, "%c", '_');
	fprintf(info, "\n");
	fclose(info);
	mapa(10, 2);
	printf("Ako zelite da vidite prethodne rezultate unesite 'd'/'D'\n");//meni za rezultate 
	mapa(38, 3);
	cha = getch();
	system("cls");
	if (cha == 'd' || cha == 'D') {
		info = fopen("rekordi.txt", "r");//pregled ostalih rezultata iz datoteke rezultati
		do {
			putchar(c = getc(info));
		} while (c != EOF);
	}
	fclose(info);
}
int rezultat_ekran()
{
	int rez;
	mapa(20, 8);
	rez = duzina - 5;
	printf("REZULTAT : %d", rez);
	mapa(50, 8);
	printf("ZIVOTA : %d", zivot + 1);
	return rez;
}
int rezultat()
{
	int rez = rezultat_ekran();
	system("cls");
	return rez;
}


