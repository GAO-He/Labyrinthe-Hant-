#ifndef LABYRINTHE_H
#define LABYRINTHE_H

#include "Environnement.h"

#define INFINI 10000
#define	LAB_WIDTH	100
#define	LAB_HEIGHT	65

class Labyrinthe : public Environnement {
private:
	char	_data [LAB_WIDTH][LAB_HEIGHT];
	int 	** _dist;

public:

	Labyrinthe (char*);
	int width () { return LAB_WIDTH;}	// retourne la largeur du labyrinthe.
	int height () { return LAB_HEIGHT;}	// retourne la longueur du labyrinthe.
	char data (int i, int j)
	{
		return _data [i][j];
	}	// retourne la case (i, j).

	void set_data( int i, int j, int ch ){
		_data[i][j] = ch;
	}
	
	int dist (int i, int j){
		return _dist [i][j];		// la distance entre treasor et le point(i, j)
	}
};

#endif
