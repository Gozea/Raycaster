#include <iostream>
#include <eigen3/Eigen/Dense>
#include <SFML/Graphics.hpp>

#include <math.h>

using namespace std;
using namespace Eigen;

//window property
const int width = 1024;
const int height = 640;

float fov = M_PI/2;
float nrays = 20;

const int nWalls = 25;
Vector4f walls[nWalls+4];

struct character {
	float x = width/2;
	float y = height/2;
	float angle = -M_PI/2;
	Vector2f dx = Vector2f(0, -1);
	float mvspeed = 5;
};
character hero;

void rotate(float angle) {
	hero.dx.x() = cos(hero.angle+angle);
	hero.dx.y() = sin(hero.angle+angle);
	hero.angle+=angle;
}


void initWalls() {
	for (int i = 0 ; i < nWalls ; i++) {
		//init walls
		walls[i].x() = rand()%width;
		walls[i].y() = rand()%height;
		walls[i].z() = rand()%width;
		walls[i].w() = rand()%height;
	}
	walls[nWalls].x() = 1;walls[nWalls].y() = 1;walls[nWalls].z() = 1;walls[nWalls].w() = height-1;
	walls[nWalls+1].x() = 1;walls[nWalls+1].y() = 1;walls[nWalls+1].z() = width-1;walls[nWalls+1].w() = 1;
	walls[nWalls+2].x() = width;walls[nWalls+2].y() = 0;walls[nWalls+2].z() = width;walls[nWalls+2].w() = height;
	walls[nWalls+3].x() = 0;walls[nWalls+3].y() = height;walls[nWalls+3].z() = width;walls[nWalls+3].w() = height;
}

void drawWalls(sf::RenderWindow& window) {
	for (int i  = 0 ; i < nWalls+4 ; i++) {
		//draw walls
		sf::Vertex wall[2];
		wall[0].position = sf::Vector2f(walls[i].x(), walls[i].y());
		wall[1].position = sf::Vector2f(walls[i].z(), walls[i].w());
		window.draw(wall, 2, sf::Lines);
	}
}


/* find u0 and t0 for A*t+B = C*u+D with u and t parameters in [0,1]
 * ray is unit vector starting from the hero struct
 * wall is a segment
 * in the equation, B is hero position, A unit vector C and D are deducted from the wall coordinates
 * we'll only use t to get the intersect point
 */
Vector2f intersect(Vector2f ray, Vector4f wall) {
	//define A, B, C and D
	Vector2f A = ray;
	Vector2f B = Vector2f(hero.x, hero.y);
	Vector2f C = Vector2f(wall.z() - wall.x(), wall.w() - wall.y());
	Vector2f D = Vector2f(wall.x(), wall.y());	
	//formula to get t (case with div by are if a wall is a point or ray is parallel with horizontal or vertical wall)
	float den = (-C.y()*A.x()+C.x()*A.y());
	float t = (den != 0) ? (-C.y()*(D.x()-B.x())+C.x()*(D.y()-B.y())) / den : -1;
	//we need to compute u to ensure we intersect inside of the segment
	float u = (den != 0) ? (-A.y()*(D.x()-B.x())+A.x()*(D.y()-B.y())) / den : -1;
	Vector2f res = (t>=0 && u >= 0 && u <= 1) ? Vector2f(B.x()+A.x()*t, B.y()+A.y()*t) : Vector2f(-1, -1);
	return res;
}


void rays(sf::RenderWindow& window) {
	//draw rays
	for (float i = 0 ; i < nrays ; i++) {
		sf::Vertex ray[2];
		//the direction of the ray
		Vector2f dirRay = Vector2f(cos((-fov/2+i/nrays*fov)+hero.angle), sin((-fov/2+i/nrays*fov)+hero.angle));
		cout << "dirray " << i << " " << dirRay.x() << " " << dirRay.y() << endl;
		//intersect this ray with each wall
		vector<Vector2f> intersects;
		for (int w = 0 ; w < nWalls+4 ; w++) {
			Vector2f points = intersect(dirRay, walls[w]);
			if (points.x() != -1 || points.y() != -1) intersects.push_back(points); 
		}
		//take the intersected wall for which the distance is the shortest
		Vector2f p;
		float dist = width*height;
		cout << "inter" << endl;
		for (int w = 0 ; w < intersects.size() ; w++) {
			cout << w << " " << intersects[w].x() << " " << intersects[w].y() << endl;
			float d = sqrt(pow(intersects[w].x()-hero.x, 2) + pow(intersects[w].y()-hero.y, 2));
			if (d < dist) {
				p = intersects[w]; 
				dist = d;
				cout << w << " dist " << d << endl;
			}
		}
		//draw line between hero and chosen point (we'll at least intersect the border wall)
		ray[0].position = sf::Vector2f(hero.x, hero.y);
		ray[1].position = sf::Vector2f(p.x(), p.y());
		ray[0].color = sf::Color::Red;
		ray[1].color = sf::Color::Red;

		window.draw(ray, 2, sf::Lines);
	}	

}

int main() {

	//SFML
	// create the window (remember: it's safer to create it in the main thread due to OS limitations)
        sf::RenderWindow window(sf::VideoMode(width, height), "Donut");

	//create triangle
        sf::CircleShape triangle(20.f, 3);
	triangle.setFillColor(sf::Color(100, 250, 50));
	triangle.setOrigin(20.f, 20.f);
        
	//init walls
	initWalls();

         // run the program as long as the window is open
        while (window.isOpen())
        {
		// check all the window's events that were triggered since the last iteration of the loop
                sf::Event event;
                while (window.pollEvent(event))
                {
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {rotate(-0.1); triangle.rotate(-0.1*360/(2*M_PI));}
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {rotate(0.1); triangle.rotate(0.1*360/(2*M_PI));}
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {hero.x+=hero.mvspeed*hero.dx.x();hero.y+=hero.mvspeed*hero.dx.y();};
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {hero.x-=hero.mvspeed*hero.dx.x();hero.y-=hero.mvspeed*hero.dx.y();};
                    // "close requested" event: we close the window
                    if (event.type == sf::Event::Closed) window.close();

                }

	        // clear the window with black color
                window.clear(sf::Color::Black);

		//draw walls
		drawWalls(window);

		//draw triangle
		triangle.setPosition((sf::Vector2f(hero.x, hero.y)));

		//draw rays
		rays(window);

		//window draw
		window.draw(triangle);

	        //end of current frame 
	        window.display();
	}	
}
