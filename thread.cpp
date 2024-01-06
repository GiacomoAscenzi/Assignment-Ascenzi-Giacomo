#include "boid.h"

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <thread>
#include <mutex>
#include <chrono> 
#include <condition_variable>

using namespace std;



std::mutex Boidmtx;
std::condition_variable savedBoid;
std::condition_variable updateBoid;
bool saveRequested = false;

void saveAllBoidsPositions(const std::vector<Boid>& boids, std::ofstream& outputFile) {
    for (size_t j = 0; j < boids.size(); ++j) {
        const Boid& currentBoid = boids[j];
        outputFile << currentBoid.get_X() << " " << currentBoid.get_Y() << " " << currentBoid.get_Orientation() << " ";
    }
    
}

void updateBoidPosition(Boid& boid,const vector<Boid>& allBoids, int iterations, double d_sep, double separationFactor, double d_ca, 
double alignmentFactor, double visibleRange, double cohesionFactor, double minSpeed, double maxSpeed, double leftMargin, 
double rightMargin, double bottomMargin, double topMargin, double turnFactor) {

    for(int i = 0; i < iterations; ++i) {
        std::vector<Boid> closeBoids;
        for (size_t k = 0; k < allBoids.size(); ++k) {
            const Boid& otherBoid = allBoids[k];
            if (&boid != &otherBoid) { 
                double distance = sqrt(pow(boid.get_X() - otherBoid.get_X(), 2) + pow(boid.get_Y() - otherBoid.get_Y(), 2));
                if (distance < visibleRange) {
                closeBoids.push_back(otherBoid);
                }
            }
        }
        
        {
            
            std::unique_lock<std::mutex> lock(Boidmtx);
            while (!saveRequested){            
                boid.separation(d_sep, separationFactor, closeBoids);
                boid.alignment(d_ca, alignmentFactor, closeBoids); 
                boid.cohesion(visibleRange, cohesionFactor, closeBoids);   
                boid.margin(turnFactor, leftMargin, rightMargin, bottomMargin, topMargin);    
                boid.speedRange(minSpeed, maxSpeed);  
                boid.updatePosition();
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            lock.unlock();
            savedBoid.notify_one(); 
        }
               
    }
}


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

    }

    double cohesionFactor = 0.005;
    double separationFactor = 0.005;
    double alignmentFactor = 0.001;
    double d_ca = 110.0;
    double d_sep = 150.0;
    double visibleRange = 200.0;
    double minSpeed = 0.7;
    double maxSpeed = 4.0;
    double leftMargin = -500;
    double rightMargin = 500;
    double bottomMargin = -500;
    double topMargin = 500;
    double neighborhoodVisible = 400.0;
    double turnFactor = 1;
    
    
    std::ofstream outputFile("coordinates.txt");
    if (!outputFile.is_open()) {
        std::cerr << "Impossibile aprire il file per salvare le posizioni dei boids." << std::endl;
        return 1;
    }

    vector<std::thread> boidThreads;
    for (int i = 0; i < boids.size(); ++i) {
        boidThreads.emplace_back([&boids, i, d_sep, separationFactor, d_ca, alignmentFactor, visibleRange, cohesionFactor, minSpeed, maxSpeed, leftMargin, rightMargin, bottomMargin, topMargin, turnFactor]() {
        updateBoidPosition(boids[i], boids, 1000, d_sep, separationFactor, d_ca, alignmentFactor, visibleRange, cohesionFactor, minSpeed, maxSpeed, leftMargin, rightMargin, bottomMargin, topMargin, turnFactor);
        });
    }
    

    std::thread savePositionsThread([&]() {
        for (int saveCount = 0; saveCount < 30; ++saveCount) {
            while (saveRequested) {
                std::unique_lock<std::mutex> lock(Boidmtx);
                savedBoid.wait(lock);
                for (size_t i = 0; i < boids.size(); ++i) {
                    const Boid& boid = boids[i];
                    saveAllBoidsPositions(boids, outputFile);
                }
                outputFile << "\n";
                saveRequested = false;
                lock.unlock();            
            }
           
            updateBoid.notify_all();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            saveRequested = true;
        }
        outputFile.close(); 
        
    });

    for (int i = 0; i < boidThreads.size(); ++i) {
        boidThreads[i].join();
    } 
    
    

    savePositionsThread.join();

    

    return 0;
}

        


