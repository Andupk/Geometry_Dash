#include "Framework/DrawingWindow.h"
#include "Framework/Visual2D.h"
#include "Framework/Transform2D.h"
#include "Framework/Transform3D.h"
#include "Framework/Line2D.h"
#include "Framework/Rectangle2D.h"
#include "Framework/Circle2D.h"
#include "Framework/Polygon2D.h"
#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <string>
#include <string.h>

#define PI 3.14159265358979323846
#define inf 1000000
#define LP 50
using namespace std;

Rectangle2D *ps,*e1, *e2, *m;
Visual2D *scor, *game, *bottom;
Circle2D *circle;
Rectangle2D *obst, *flag_stick,*podea;
Polygon2D *tri,  *flag_tri;
Text *Lifes, *Flags, *Points, *GameWon, *GameLost;

int moveX = 0, pas = 0, moveY = 0, r = -1, nr_flags = 0;
float rot = 0;
float n = 1;
bool jump = false;
int HP = 3, ok;
bool endGame = false;
int ob_prev = -1;

//functia care permite adaugarea de obiecte
void DrawingWindow::init()
{

	// Partea upper scor + remaining lifes
	scor = new Visual2D(0,0, 600,600, 0, DrawingWindow::height / 6, DrawingWindow::width,0);
	scor->tipTran(true);
	addVisual2D(scor);
	Lifes = new Text("Lifes Remaining: 3", Point2D(50, 500), Color(1, 0, 0), BITMAP_HELVETICA_18);
	Points = new Text("Points: 0", Point2D(250, 500), Color(1, 0, 0), BITMAP_HELVETICA_18);
	Flags = new Text("Flags Captured: 0", Point2D(400, 500), Color(1, 0, 0), BITMAP_HELVETICA_18);
	addText_to_Visual2D(Lifes, scor);
	addText_to_Visual2D(Points, scor);
	addText_to_Visual2D(Flags, scor);

	// center patratelul miscator
	game = new Visual2D(0,0, 600,600, 0,5 * DrawingWindow::height / 6, DrawingWindow::width, DrawingWindow::height / 6);
	game->tipTran(true);
	addVisual2D(game);

	//bottom 
	bottom = new Visual2D(0,0, 600, 600,  0,DrawingWindow::height, DrawingWindow::width, 5*DrawingWindow::height/6);
	bottom->tipTran(true);
	addVisual2D(bottom);	
	podea = new Rectangle2D(Point2D(0, 0), 600, 100, Color(0, 0, 0), true);
	addObject2D_to_Visual2D(podea, bottom);

	//body
	ps = new Rectangle2D(Point2D(100, 100), LP, LP, Color(1, 0, 0), false);
	addObject2D_to_Visual2D(ps, game);
	
	//eyes
	e1 = new Rectangle2D(Point2D(DrawingWindow::width / 6 + 5 , DrawingWindow::height / 6+30), 10, 10, Color(1, 0, 1), true);
	e2 = new Rectangle2D(Point2D(DrawingWindow::width / 6 + 25, DrawingWindow::height / 6+30), 10, 10, Color(1, 0, 1), true);
	addObject2D_to_Visual2D(e1, game);
	addObject2D_to_Visual2D(e2, game);

	//mouth
	m = new Rectangle2D(Point2D(DrawingWindow::width / 6 + 5 , DrawingWindow::height / 6 + 5), 40, 10, Color(0, 1, 1), true);
	addObject2D_to_Visual2D(m, game);


}


//functia care permite animatia
void DrawingWindow::onIdle()
{

	if (endGame == false){
		string puncte = to_string(moveX);
		removeText_from_Visual2D(Points, scor);
		Points = new Text("Points: " + puncte, Point2D(250, 500), Color(1, 0, 0), BITMAP_HELVETICA_18);
		addText_to_Visual2D(Points, scor);

		moveX += 5;
		game->fereastra(0 + moveX, 0, 600 + moveX, 600);
		//bottom->fereastra(0 + moveX, 0,600 + moveX, 600);
		//scor->fereastra(0 + moveX, 0,600 + moveX, 600);
		Transform2D::loadIdentityMatrix();

		// Coliziuni
		if (moveX % 500 == 0){
			if ( ( r == 0 || r == 1 || r == 2 ) && (moveY < 20) ) {
				/*
				Debug
				obst = new Rectangle2D(Point2D(moveX, 100), 20, 100,Color(1,0,0), true);
				addObject2D_to_Visual2D(obst, game);
				cout <<"inaltime: "<< moveY << endl;
				*/
				HP--;
				string lr = to_string(HP);
				removeText_from_Visual2D(Lifes, scor);
				Lifes = new Text("Lifes Remaining:" + lr, Point2D(50, 500), Color(1, 0, 0), BITMAP_HELVETICA_18);
				addText_to_Visual2D(Lifes, scor);
			}
		}
		if ( moveX % 500 == 0){
			if (r == 3){
				nr_flags++;
				string fl = to_string(nr_flags);
				removeText_from_Visual2D(Flags, scor);
				Flags = new Text("Flags Captured: " + fl, Point2D(400, 500), Color(1, 0, 0), BITMAP_HELVETICA_18);
				addText_to_Visual2D(Flags, scor);
			}
		}


		//Saritura/Mersul cubului cubului
		//Partea de urcare
		if (ok == 0 && jump == true){
			rot += 0.12;

			if (rot < 2 * PI){
				Transform2D::translateMatrix(-125, -125);
				Transform2D::rotateMatrix(-rot);
				moveY += 10;
				Transform2D::translateMatrix(moveX + 125, moveY + 125);

			}
			else{
				moveY += 10;
				Transform2D::translateMatrix(moveX, moveY);
			}
			if (moveY == 250)
				ok = 1;
		}
		//Partea de coborare
		else if (ok == 1 && jump == true){
			rot += 0.12;
			if (rot < 2 * PI){
				Transform2D::translateMatrix(-125, -125);
				Transform2D::rotateMatrix(-rot);
				moveY -= 10;
				Transform2D::translateMatrix(moveX + 125, moveY + 125);

			}
			else{
				moveY -= 10;
				Transform2D::translateMatrix(moveX, moveY);
			}
			if (moveY == 0)
				jump = false;
		}
		//mers normal
		else{
			Transform2D::translateMatrix(moveX, 0);
		}

		// La fiecare 500px cream ori un obstacol
		// ori un steag 
		if (moveX % 500 == 0){
			ob_prev = r;
			r = rand() % 4;
			cout << r << endl;
			//cerc
			if (r == 0){
				//free(circle);
				circle = new Circle2D(Point2D(moveX + 500, 150), 50);
				addObject2D_to_Visual2D(circle, game);

			}
			//triunghi
			if (r == 1){
				//free(tri);
				tri = new Polygon2D(Color(1, 0, 1), true);
				tri->addPoint(Point2D(moveX + 500, 100));
				tri->addPoint(Point2D(moveX + 600, 100));
				tri->addPoint(Point2D(moveX + 550, 200));
				addObject2D_to_Visual2D(tri, game);
			}
			//dreptunghi
			if (r == 2){
				//free(obst);
				obst = new Rectangle2D(Point2D(moveX + 500, 100), 20, 100);
				addObject2D_to_Visual2D(obst, game);

			}
			//Point/checkpoint
			if (r == 3){
				//free(flag_stick);
				//free(flag_tri);
				flag_stick = new Rectangle2D(Point2D(moveX + 500, 100), 20, 250);
				flag_tri = new Polygon2D(Color(1, 1, 0), true);
				flag_tri->addPoint(Point2D(moveX + 500, 350));
				flag_tri->addPoint(Point2D(moveX + 500, 300));
				flag_tri->addPoint(Point2D(moveX + 550, 325));
				addObject2D_to_Visual2D(flag_stick, game);
				addObject2D_to_Visual2D(flag_tri, game);
			}
		}

		//Daca nu mai avem vieti
		if (HP == 0 && endGame == false){
			endGame = true;
			//moveX = 0;
			removeText_from_Visual2D(Flags, scor);
			removeText_from_Visual2D(Points, scor);
			removeText_from_Visual2D(Lifes, scor);
			GameLost = new Text("YOU LOST !!! --- PRESS SPACE TO PLAY AGAIN ", Point2D(50, 500), Color(1, 0, 0), BITMAP_HELVETICA_18);
			addText_to_Visual2D(GameLost, scor);
		}

		// Daca am cules 5 steaguri
		if (nr_flags == 5 && endGame == false){
			endGame = true;
			//moveX = 0;
			removeText_from_Visual2D(Flags, scor);
			removeText_from_Visual2D(Points, scor);
			removeText_from_Visual2D(Lifes, scor);
			GameWon = new Text("YOU WON !!! --- PRESS SPACE TO PLAY AGAIN", Point2D(50, 500), Color(1, 0, 0), BITMAP_HELVETICA_18);
			addText_to_Visual2D(GameWon, scor);
		}

		//Aplicam transformarile pe cub
		Transform2D::applyTransform(ps);
		Transform2D::applyTransform(e1);
		Transform2D::applyTransform(e2);
		Transform2D::applyTransform(m);
	}
}

//functia care se apeleaza la redimensionarea ferestrei
void DrawingWindow::onReshape(int width,int height)
{ 
	
}

//functia care defineste ce se intampla cand se apasa pe tastatura
void DrawingWindow::onKey(unsigned char key)
{
	switch(key)
	{
		case 27 : exit(0);
		
		case ' ': 
			if (jump == false){
				ok = 0;
				rot = 0.0;
				moveY = 0;
				jump = true;

			}
			if (endGame == true){
				endGame = false;
				HP = 3;
				nr_flags = 0;
				//moveX = 0;
				removeText_from_Visual2D(GameWon, scor);
				removeText_from_Visual2D(GameLost, scor);
				Lifes = new Text("Lifes Remaining: 3", Point2D(50, 500), Color(1, 0, 0), BITMAP_HELVETICA_18);
				Points = new Text("Points: 0", Point2D(250, 500), Color(1, 0, 0), BITMAP_HELVETICA_18);
				Flags = new Text("Flags Captured: 0", Point2D(400, 500), Color(1, 0, 0), BITMAP_HELVETICA_18);
				addText_to_Visual2D(Lifes, scor);
				addText_to_Visual2D(Points, scor);
				addText_to_Visual2D(Flags, scor);
			}
			break;
	}
}

//functia care defineste ce se intampla cand se da click pe mouse
void DrawingWindow::onMouse(int button,int state,int x, int y)
{

}


int main(int argc, char** argv)
{
	//creare fereastra
	DrawingWindow dw(argc, argv, 600, 600, 200, 50, "Laborator EGC");
	
	//se apeleaza functia init() - in care s-au adaugat obiecte
	dw.init();

	//se intra in bucla principala de desenare - care face posibila desenarea, animatia si procesarea evenimentelor
	dw.run();
	return 0;

}