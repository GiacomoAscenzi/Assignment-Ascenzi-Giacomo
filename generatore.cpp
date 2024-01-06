#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>
using namespace std;

int main() {
    const int boids = 10;
    vector<double> xPos(boids);
    vector<double> yPos(boids);
    vector<double> xVel(boids);
    vector<double> yVel(boids);
    

    srand(time(0));

    for (int i = 0; i < boids; i++) {
        xPos[i] = static_cast<double>(rand()) / RAND_MAX * 100-50; 
        yPos[i] = static_cast<double>(rand()) / RAND_MAX * 100-50; 
        xVel[i] = static_cast<double>(rand()) / RAND_MAX * 10-5; 
        yVel[i] = static_cast<double>(rand()) / RAND_MAX * 10-5; 
    }

    
    ofstream file("Condizioni iniziali.txt");
    if (file.is_open()) {
        for (int i = 0; i < boids; i++) {
            file << xPos[i] << " " << yPos[i] << " " << xVel[i] << " " << yVel[i] << " " <<"\n";
        }
        file.close();
        cout << "Dati scritti correttamente nel file 'Condizioni iniziali.txt'" << endl;
    } else {
        cout << "Impossibile aprire il file!" << endl;
    }

    return 0;
}
