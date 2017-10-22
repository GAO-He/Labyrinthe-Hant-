#include <math.h>
#include <iostream>
#include <time.h>
#include <cstdlib>
#include <vector>
#include "Labyrinthe.h"

#include "Mover.h"

#ifndef GARDIEN_H
#define GARDIEN_H

#define PI 3.14159265359
#define N 9999

using namespace std;

class Labyrinthe;

class Gardien : public Mover {
private:
	bool move_aux (double dx, double dy);


public:
	Gardien (Labyrinthe* l, const char* modele) : Mover (120, 80, l, modele)
	{	

		//_l -> data ((int)((_x + dx) / Environnement::scale), (int)((_y + dy) / Environnement::scale))

		//point = get_point_def();
		//dx = point[0];
		//dy = point[1];
		//cout << ((Labyrinthe*)_l)->dist(_x/Environnement::scale, _y/ Environnement::scale);
		//cout << dx << " " << dy << endl;
	}
			
	int level = 0;							// le niveau de surveillance, 0-2, de faible a eleve.
	int cpt = 0;
	int duree = rand() % 71 + 70;	
	double rand_x = (rand() % (N+1) / float(5000)-0.1) * 2;		// pour produire un random chiffre : -0.2~0.2;				
	double rand_y = (rand() % (N+1) / float(5000)-0.1) * 2;		// pour produire un random chiffre : -0.2~0.2;	

	// mon gardien pense très mal!
		
	//Vertex defender;

	double dx;
	double dy;
	double *point;

	double * get_point_def(void);

	void update (void);
	// et ne bouge pas!
	bool move (double dx, double dy) { 
		return move_aux (dx, dy) || move_aux (dx, 0.0) || move_aux (0.0, dy);
	}
	// ne sait pas tirer sur un ennemi.
	void fire (int angle_vertical) {}
	// quand a faire bouger la boule de feu...
	bool process_fireball (float dx, float dy) { return false; }

};

#endif
