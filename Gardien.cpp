#include "Gardien.h"

/*
 *	Tente un déplacement.
 */


bool Gardien::move_aux (double dx, double dy) {						// scale: 10
	if( EMPTY == _l -> data ((int)((_x + dx*10) / Environnement::scale),		// si il est vide
							 (int)((_y + dy*10) / Environnement::scale)) )		
		{

			_x += dx;
			_y += dy;

			return true;
		}				
		return false;
}

void Gardien::update (void) {

	// how to determine level of guard.
	// levelDetermine() {}
	// donc la distance : random_x * duree, random_y * duree
	if( level == 0 ) {			// le statut discursif
		double dx = rand_x;
		double dy = rand_y;

		double alpha = dy / dx;
		if( dx > 0){
			_angle = atan(alpha)*180/PI-90;
		} else {
			_angle = atan(alpha)*180/PI+90;
		}
		( (Labyrinthe*)_l)->set_data( (int)(_x / Environnement::scale), (int)(_y / Environnement::scale), EMPTY );
		move(dx, dy);
		( (Labyrinthe*)_l)->set_data( (int)((_x) / Environnement::scale), (int)((_y) / Environnement::scale), 1 );
		//cout << "dx:" << dx << " dy:" << dy << endl;

	
		if( cpt++ >= duree ){	

 			rand_x = (rand() % (N+1) / float(50000)-0.1) * 2;	// pour produire un random chiffre : -0.2~0.2;
			rand_y = (rand() % (N+1) / float(50000)-0.1) * 2;	
			duree = rand() % 71 + 70;		
			cpt = 0;	
			//cout << ((Labyrinthe*)_l)->dist(40/Environnement::scale, 40/Environnement::scale) << endl;
		} 
	}


	if( level == 1 ){			// le statut defensif

		double alpha = dy / dx;
		//cout << dx << " " << dy << endl;

		//cout << "test" << endl;
		if( dx > 0){
			_angle = atan(alpha)*180/PI-90;
		} else {
			_angle = atan(alpha)*180/PI+90;
		}
		( (Labyrinthe*)_l)->set_data( (int)(_x / Environnement::scale), (int)(_y / Environnement::scale), EMPTY );
		move(dx, dy);
		( (Labyrinthe*)_l)->set_data( (int)(_x / Environnement::scale), (int)(_y / Environnement::scale), 1 );
		//cout << "dx:" << dx << " dy:" << dy << endl;

		if( cpt++ >= duree ){	

 			point = get_point_def();
			double dx = point[0];
			double dy = point[1];	
			duree = rand() % 71 + 70;		
			cpt = 0;
			//cout << cpt << endl;
			//cout << ((Labyrinthe*)_l)->dist(40/Environnement::scale, 40/Environnement::scale) << endl;
		} 

	}

	if( level == 2 ){			// le statut attaquant

	}

}

double * Gardien::get_point_def(){
	vector <double> v_petit;		// if distance < dis[guard_x][guard_y]
	vector <double> v_equal;		// if distance = dis[guard_x][guard_y]
	vector <double> v_grand;		// if distance > dis[guard_x][guard_y]

	double speed = 0.2;
	double alpha = PI/180;
	double * point = new double[2];
	//cout << point[0] << " " << point[1] << endl;

	for( int i = _x - 1; i < (_x+2); i ++ ){
		for( int j = _y - 1; j < (_y+2); j ++ ){
			if( ((Labyrinthe*)_l)->dist(i/Environnement::scale, j/Environnement::scale) < INFINI ){
				// cout << ((Labyrinthe*)_l)->dist(i/Environnement::scale, j/Environnement::scale);
				if( ((Labyrinthe*)_l)->dist(i/Environnement::scale, j/Environnement::scale) <
							 ((Labyrinthe*)_l)->dist(_x/Environnement::scale, _y/Environnement::scale) ){
					if( rand()%(N+1) < 5000 ){

						v_petit.push_back(i);
						v_petit.push_back(i);
						// cout << rand()%(N+1) << endl;
						point[0] = speed*(_x-i);
						point[1] = speed*(_y-j);
						break;
					}
				} else if ( ((Labyrinthe*)_l)->dist(i/Environnement::scale, j/Environnement::scale) ==
							 ((Labyrinthe*)_l)->dist(_x/Environnement::scale, _y/Environnement::scale) ){
					if( rand()%(N+1) < 3000 ){
						point[0] += speed*(i-_x);
						point[1] += speed*(j-_y);
					}
				} else if ( ((Labyrinthe*)_l)->dist(i/Environnement::scale, j/Environnement::scale) > 
							 ((Labyrinthe*)_l)->dist(_x/Environnement::scale, _y/Environnement::scale) ){
					if( rand()%(N+1) < 2000){				
						point[0] += speed*(i-_x);
						point[1] += speed*(j-_y);
					}
				}	
				//cout << point[0] << " " << point[1] << endl;

			}
		}
	}

	return point;
}


