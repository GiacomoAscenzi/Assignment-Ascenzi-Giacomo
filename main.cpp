#include "boid.h"

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

using namespace std;

int main() {

    vector<Boid> boids;

    ifstream file("Condizioni iniziali.txt");
    if (file.is_open()) {
        double x, y, vx, vy;

        while (file >> x >> y >> vx >> vy) {
            Boid boid(x, y, vx, vy);
            boids.push_back(boid);
        }
        file.close();

        double cohesionFactor = 0.005;
        double separationFactor = 0.02;
        double alignmentFactor = 0.005;
        double d_ca = 80.0;
        double d_sep = 50.0;
        double visibleRange = 100.0;
        double minSpeed = 0.7;
        double maxSpeed = 5.0;
        double leftMargin = -500;
        double rightMargin = 500;
        double bottomMargin = -500;
        double topMargin = 500;
        double neighborhoodVisible = 200.0;
        double turnFactor = 0.7;

        ofstream outputFile("coordinates.txt");
        if (outputFile.is_open()) {
            outputFile << leftMargin << " " << rightMargin << " " << bottomMargin << " " << topMargin << "\n";
            vector<Boid> closeBoids; 
    
            int iterations = 3000;
            for (size_t i = 0; i < iterations; ++i) {
            
                for (size_t j = 0; j < boids.size(); ++j) {
                    Boid& boid = boids[j];

                vector<Boid> closeBoids; 
                closeBoids.clear(); 

                    for (size_t k = 0; k < boids.size(); ++k) {
                        const Boid& otherBoid = boids[k];
                        double distance = sqrt(pow(boid.get_X() - otherBoid.get_X(), 2) + pow(boid.get_Y() - otherBoid.get_Y(), 2));
                        if (j != k && distance < neighborhoodVisible) {
                            closeBoids.push_back(otherBoid);
                        }
                    }
                
                    boid.separation(d_sep, separationFactor, closeBoids);
                    boid.alignment(d_ca, alignmentFactor, closeBoids); 
                    boid.cohesion(visibleRange, cohesionFactor, closeBoids);   
                
                    boid.margin(turnFactor, leftMargin, rightMargin, bottomMargin, topMargin);    
                    boid.speedRange(minSpeed, maxSpeed);           

                
                    boid.updatePosition();
                
                    boid.savePosition(outputFile);
                }
                outputFile << "\n"; 
            }
    
        outputFile.close();
        std::cout << "Posizioni salvate correttamente nel file 'coordinates.txt'.\n";
        } else {
        std::cerr << "Impossibile aprire il file per salvare le posizioni!\n";
        }  

    } else {
        cout << "Impossibile aprire il file!" << endl;
        return 1; 
    }

    return 0;
    
}