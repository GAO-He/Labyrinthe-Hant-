#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <cstdlib>
#include <fstream>

#include "Labyrinthe.h"
#include "Chasseur.h"
#include "Gardien.h"
#define N_WALLS_MAX 1000
#define N_AFFICHAGE_MAX 500
#define N_BOX_MAX 500
#define N_GUARDS_MAX 50

using namespace std;

Sound*	Chasseur::_hunter_fire;	// bruit de l'arme du chasseur.
Sound*	Chasseur::_hunter_hit;	// cri du chasseur touché.
Sound*	Chasseur::_wall_hit;	// on a tapé un mur.

static Wall walls[N_WALLS_MAX];  	
static Wall affiche[N_AFFICHAGE_MAX];
static Box caisses[N_BOX_MAX];
static int gardien[N_GUARDS_MAX][2];

static int n_wall = 0;			// le nombre d'un mur, commence par 0
static int n_affiche = 0; 		// le nombre d'affichage
static int n_caisses = 0;		// le nombre des caisses
static int n_guards = 0;		// le nombre des guards

char ** loadMap( char * filename, int *w, int *l );
void mapCreate(char ** mapSource, int length, int width, int temp_ntex, int *_x_treasor, int *_y_treasor);

Environnement* Environnement::init (char* filename)
{
	return new Labyrinthe (filename);
}

class Vertex
{
public:
	int x;
	int y;

	Vertex(int xx = 0, int yy = 0): x(xx), y(yy) {};
	int getX() { return x; }
	int getY() { return y; }
	void output(); 				//print of (x,y) 
};

void Vertex::output()
{
	cout << "(" << x << ", " << y << ")" << endl;
}

struct node{
    Vertex point;
    node * next;
 };
 
class Queue {
private:
	node * front;		//1er pointeur de la file
	node * rear;		//dernier pointeur de la file

public:
	Queue();
	~Queue();				
	bool empty();			//vide ou pas
	void push(Vertex);		//entrer
	Vertex pop();			//sortir
};

Queue::Queue() {			//destructeur(function) pour init
	front = NULL;
	rear = NULL;
}

Queue::~Queue() {			//destructeur(function) pour detruire
	delete front;
}

void Queue::push(Vertex data) {			//prendre un vertex pour entrer dans la file
	node * tmp = new node();
	tmp->point = data;					//pointeur pour obtenir							
	tmp->next = NULL;

	if (front == NULL) {
		front = tmp;				//si file n'est pas vide, on prend le 1er
	}
	else {
		rear->next = tmp;			//sinon, on prend le prochain de dernier
	}
	rear = tmp;
}

Vertex Queue::pop() {					//sortir
	node * tmp = new node();
	Vertex value;
	
	if (front == NULL) {
		cout << "\nQueue n'a rien\n";
	}
	else {
		tmp = front;
		value = tmp->point;
		front = front->next;
		delete tmp;
	}
	return value;
}

bool Queue::empty() {					//vide
	return (front == NULL);
}


void PCC(Vertex s, int **tab)
{
    Queue Q;
    Q.push(s);						//init  et mettre s dans File
    while (!Q.empty())
    {
        Vertex v = Q.pop();			// tous les vertex sortient de File
        
        int vx = v.getX();
        int vy = v.getY();
        int dist = tab[vx][vy];
        for (int i = vx - 1; i <= vx+1; i++)			//parcours des voisins en x
        {
            for (int j = vy - 1; j <= vy+1; j++)		//parcours des voisins en y
            {
                if (tab[i][j] == -1)			// tombe à obstacle
                {
                    Vertex tmp(i, j);		
                    Q.push(tmp);				//prendre ce vertex dans File
                    tab[i][j] = dist + 1;		//Calcule de distance voisins de ce vertex dans File
		    // if( i == 80 )
		   	// cout << i <<" "<< j << " " << tab[i][j] << endl;
                }
            } 
        }
    }
    	// cout <<"BFS-PCC is ok\n"<< endl;
}

Labyrinthe::Labyrinthe (char* filename)
{
	int width, length;						// la largeur et la hauteur de Labyrinthe
	char ** map = loadMap("labyrinthe.txt", &width, &length); 	// map est une matrice[width][length] qui a les information de Labyrinthe
	// cout << "length:" << length << " width:" << width << endl;
/*
	for ( int i = 0; i < length; i ++){
		for( int j = 0; j < width; j ++){
			cout << map[i][j];
		}
		cout << endl;
	}
*/
	
	// definir la matrice de distance entre le point (x, y) and treasor
	_dist = new int * [width];			//width: x	
	for ( int i = 0; i < width; i ++)
		_dist[i] = new int [length];		//length: y   dist(x,y)

	Vertex chaser;
	Vertex treasor;
	
	// Initialisation de la matrice de distance
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < length; j++)
		{
			// cout << i <<" "<< j << " " << endl;	
			char tmp =  map[j][i];		// map(ligne, colon)
			switch (tmp) {
			case '+': _dist[i][j] = INFINI;		//si le point est inaccessible, la valeur initiale est INFINI
				break;
			case '-': _dist[i][j] = INFINI;		//INFINI ici: 10000 défini dans Labyrinthe.h
				break;
			case '|': _dist[i][j] = INFINI;
				break;
			case 'a': _dist[i][j] = INFINI;
				break;
			case 'b': _dist[i][j] = INFINI;
				break;
			case 'x': _dist[i][j] = INFINI;
				break;
			case 'T': _dist[i][j] = 0; 
				treasor.x = i;
				treasor.y = j; 
				break; 
			case 'G': _dist[i][j] = -1;		// les autres choses, initialiser a zero.
				break;
			case 'C': _dist[i][j] = -1;
				chaser.x = i;
				chaser.y = j;
				break;
			default:  _dist[i][j] = -1; 
				break;		
			}
		}
	}
/*
	for ( int i = 0; i < width; i ++){
		for( int j = 0; j < length; j ++)
		{
			cout << dist(j, i)<<" ";
		}
		cout << endl;
	}
*/

	PCC(treasor, _dist);					// on execute la function PCC, et garde les information dans la matrice _dist.
/*
	for ( int i = 0; i < width; i ++){
		for( int j = 0; j < length; j ++)
		{
			cout << dist(j, i)<<" ";
		}
		cout << endl;
	}
*/
	// cout << dist(40, 40);

	// la deuxième à une texture différente.
	char	tmp [128];
	sprintf (tmp, "%s/%s", texture_dir, "voiture.jpg");
	int temp_ntex = wall_texture (tmp);

	// pour créer la carte
	mapCreate( (char **)map, length, width, temp_ntex, &_treasor._x, &_treasor._y );

	// indiquer qu'on ne marche pas sur un  mur.
	for (int k = 0; k < n_wall; ++k){
		for( int i = 0; i < length; i ++ ) {
       			for( int j = 0; j < width; j ++ ) {
				if( (j >= walls[k]._x1 && j <= walls[k]._x2) && 
				    (i >= walls[k]._y1 && i <= walls[k]._y2) ){
					_data[j][i] = 1;
					//cout << "i :" << i << " j:" << j << endl; 
				}
			}
		}
		//cout << walls[k]._x1 << " "<< walls[k]._x2 <<" " << walls[k]._y1 << " " << walls[k]._y2 << endl;
	}
	
	// indiquer qu'on ne marche pas sur une caisse.
	for( int i = 0; i < n_caisses; i ++ ){
		_data [caisses [i]._x][caisses [i]._y] = 1;
	}
	// indiquer qu'on ne marche pas sur le trésor.
	_data[_treasor._x][_treasor._y] = 1;

	
	// les n_wall murs.
	_nwall = n_wall;
	_walls = walls;
	// n_affiche affiches.
	_npicts = n_affiche;
	_picts = affiche;

	// n_caisses caisses.
	_nboxes = n_caisses;
	_boxes = caisses;


	// le chasseur et les _nguards-1 gardiens.
	_nguards = n_guards;
	_guards = new Mover* [_nguards];
	_guards [0] = new Chasseur (this);	//_guards[0] -> _x = 10.; _guards[0] -> _y = 70.;
	
	for ( int i = 1; i < _nguards; i ++) {
		int x = i % 4;
		switch (x){
			case 0: _guards[i] = new Gardien( this, "drfreak" ); break;
			case 1: _guards[i] = new Gardien( this, "Marvin" ); break;
			case 2: _guards[i] = new Gardien( this, "Potator" ); break;
			case 3: _guards[i] = new Gardien( this, "garde" ); break;
		}

		_guards[i] -> _x = gardien[i][1]*scale;
		_guards[i] -> _y = gardien[i][0]*scale;
		_data [(int)(_guards [i] -> _x / scale)][(int)(_guards [i] -> _y / scale)] = 1;
	}
}

// pour charger la carte dynamiquement
char ** loadMap( char * filename, int *w, int *l ){

	ifstream Infile, Infile2;
	int max_length=0;
	int max_width=0; 
	//char file[1000];
	string buff, buff2;

	buff = filename;
	Infile.open(buff.c_str());					//ouvrir le fichier

	if(!Infile)									//échec de lecture
	{
  		cerr<<"Open Source File Erreur"<<endl;
	}
	
	while(!Infile.eof())			//si c'est pas vide ou on n'a pas lit à la fin du fichier
	{
		getline(Infile,buff);

  		int width=strlen(buff.c_str());
  		if( max_width<width )			//vérifier la taille dans la mémoire
  		{
   			 max_width=width;
 		}
 	   	max_length ++;
	}

	Infile.close();						//fermer le fichier
	*w = max_width;
	*l = max_length;	
	
	char ** map_matrice = new char *[max_length];		//enregistrer dans la matrice
	for(int i=0;i<max_length;i++)						//parcours de cette matrice
	{
		map_matrice[i] = new char[max_width];
  		for(int j=0;j<max_width;j++)
  		{
    			map_matrice[i][j]=' ';
  		}
	}

	buff2 = filename;
	Infile2.open(buff2.c_str());

	// int i=0;						// ligne : i 
	string line;
	for(int i = 0; getline(Infile2, line); i ++ ) {
	 	for (int j=0; j<max_width; j++)	{		// colon : j

			switch( line[j] ){                    	//transformer ASCII
		
                		case '+': map_matrice[i][j] = '+';  break; // "+"
               			case '-': map_matrice[i][j] = '-';  break; // "-"
		                case '|': map_matrice[i][j] = '|';  break; //"|"
                		case 'G': map_matrice[i][j] = 'G';  break; //"G"
		                case 'C': map_matrice[i][j] = 'C';  break; //"C"
               			case 'T': map_matrice[i][j] = 'T';  break; //"T"
		                case 'x': map_matrice[i][j] = 'x';  break; //"x"
                		case ' ': map_matrice[i][j] = ' ';  break; //"space"
		                case 'a': map_matrice[i][j] = 'a';  break; //"a"
                		case 'b': map_matrice[i][j] = 'b';  break; //"b"
                		default:  break;
           		 }
		}
	}

	/*
	for ( int i = 0; i < max_length; i ++){
		for( int j = 0; j < max_width; j ++){
			cout << map_matrice[i][j];
		}
		cout << endl;
	}
	*/
	Infile2.close();	
	return map_matrice;

}

void mapCreate(char ** mapSource, int length, int width, int temp_ntex, int *_x_treasor, int *_y_treasor) {
	// horizontalement
    for( int i = 0; i < length; i ++ ) {
        for( int j = 0; j < width; j ++ ) {
            // juger le point debutant d'un mur
            if( mapSource[i][j] == '+' ){			
                walls[n_wall]._x1 = j;
                walls[n_wall]._y1 = i;
                for ( int k = j+1; k < width; k ++ ){	
                	// si c'est le point arrive d'un mur
                    if ( mapSource[i][k] == '+' && 			
                        (mapSource[i][k+1] != '-' && mapSource[i][k+1] != '+') ){
                        walls[n_wall]._x2 = k;
                        walls[n_wall]._y2 = i;
                        j = k + 1;
                        walls[n_wall]._ntex = 0;		//numéro de texture 
                        n_wall ++;
                        break;
                    }

            		// pour déssiner les affichages 
                    if( mapSource[i][k] == 'a') {
                    	affiche[n_affiche]._x1 = k-1;
                    	affiche[n_affiche]._y1 = i;
                    	affiche[n_affiche]._x2 = k+1;
                    	affiche[n_affiche]._y2 = i;
                    	affiche[n_affiche]._ntex = 0;
                    	n_affiche ++;
                    } else if( mapSource[i][k] == 'b') {
						affiche[n_affiche]._x1 = k-1;
                    	affiche[n_affiche]._y1 = i;
                    	affiche[n_affiche]._x2 = k+1;
                    	affiche[n_affiche]._y2 = i;
                    	affiche[n_affiche]._ntex = temp_ntex;
                    	n_affiche ++;
		    		}

                } 
            }   

	    	// pour déssiner les caisses
            if (mapSource[i][j] == 'x')
            {
            	caisses[n_caisses]._x = j;
				caisses[n_caisses]._y = i;
				caisses[n_caisses]._ntex = 0;
				n_caisses ++;
            } else if( mapSource[i][j] == 'T' ){		// le trésor.
				*_x_treasor = j;
				*_y_treasor = i;
	    	} else if( mapSource[i][j] == 'G' ){
				gardien[n_guards][0] = i;			//_y
				gardien[n_guards][1] = j;			//_x
				n_guards ++;
	    	}
        }
    }


    //longitudinalement
    for ( int j = 0; j < width; j ++ ){
    	for ( int i = 0; i < length; i ++){
    		if( mapSource[i][j] == '+' ){			
			//cout << i << " " << j<< endl;
    			walls[n_wall]._x1 = j;
    			walls[n_wall]._y1 = i;
    			for ( int k = i+1; k < length; k ++ ){
                    		if ( mapSource[k][j] == '+' && 
                        	    ( mapSource[k+1][j] != '|' && mapSource[k+1][j] != '+') ){
                        		walls[n_wall]._x2 = j;
                        		walls[n_wall]._y2 = k;
                        		i = k + 1;
                        		walls[n_wall]._ntex = 0;
                        		n_wall ++;
								//cout << i << " " << j << " " << k<< endl;
                        		break;
							}

                    		if( mapSource[k][j] == 'a') {
					
                    			affiche[n_affiche]._x1 = j;
                    			affiche[n_affiche]._y1 = k-1;
                    			affiche[n_affiche]._x2 = j;
                    			affiche[n_affiche]._y2 = k+1;
                    			affiche[n_affiche]._ntex = 0;
                    			n_affiche ++;
								// cout << k << " " << j << " " << endl;
								// cout << n_affiche << endl;
                   			} else if( mapSource[i][k] == 'b') {
								affiche[n_affiche]._x1 = j;
                    			affiche[n_affiche]._y1 = k-1;
                    			affiche[n_affiche]._x2 = j;
                    			affiche[n_affiche]._y2 = k+1;
                    			affiche[n_affiche]._ntex = temp_ntex;
                    			n_affiche ++;
		 					}
            	} 
	        }
    	}
    }
}










