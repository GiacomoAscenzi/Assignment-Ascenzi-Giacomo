#include "boid.h"

#include <vector>
#include <cmath>
#define PI 3.14159265

using namespace std;


Boid::Boid(double x, double y, double vx, double vy) 
    :x_{x}, y_{y}, vx_{vx}, vy_{vy} {orientation_= atan2 (vy_,vx_);}


void Boid::updatePosition (){
    x_ += vx_;
    y_ += vy_;
    orientation_ = atan2 (vy_,vx_);
}


void Boid::cohesion (double visibleRange, double centering_factor, std::vector<Boid>& closeBoid){
    double xpos_avg = 0;
    double ypos_avg = 0;
    double neighboring_boids = 0;
    
    for (size_t i = 0; i < closeBoid.size(); ++i) {
        Boid& otherBoid = closeBoid[i]; 
        double distance = sqrt((x_ - otherBoid.get_X()) * (x_ - otherBoid.get_X()) + (y_ - otherBoid.get_Y()) * (y_ - otherBoid.get_Y()));

        if (distance < visibleRange){
            xpos_avg += otherBoid.get_X();
            ypos_avg += otherBoid.get_Y();
            neighboring_boids += 1;
        }
    }
    if (neighboring_boids > 0){
        xpos_avg /= neighboring_boids;
        ypos_avg /= neighboring_boids;
        vx_ += (xpos_avg - x_)*centering_factor;
        vy_ += (ypos_avg - y_)*centering_factor;
    }
   
};


void Boid::separation (double d_sep, double avoidfactor, std::vector<Boid>& closeBoid){
    double close_dx = 0;
    double close_dy = 0;

    for (size_t i = 0; i < closeBoid.size(); ++i) {
        Boid& otherBoid = closeBoid[i]; 
        double distance = sqrt((x_ - otherBoid.get_X()) * (x_ - otherBoid.get_X()) + (y_ - otherBoid.get_Y()) * (y_ - otherBoid.get_Y()));
        if (distance < d_sep){
            close_dx += x_ - otherBoid.get_X();
            close_dy += y_ - otherBoid.get_Y();
            
        }
        
    }
    vx_ += avoidfactor*close_dx;
    vy_ += avoidfactor*close_dy;
};

void Boid::alignment (double d_ca, double alignfactor, std::vector<Boid>& closeBoid){
    double xvel_avg = 0;
    double yvel_avg = 0;
    double close_boids = 0;

    for (size_t i = 0; i < closeBoid.size(); ++i) {
    Boid& otherBoid = closeBoid[i]; 
    double distance = sqrt((x_ - otherBoid.get_X()) * (x_ - otherBoid.get_X()) + (y_ - otherBoid.get_Y()) * (y_ - otherBoid.get_Y()));

        if (distance < d_ca){
            xvel_avg += otherBoid.get_Vx();
            yvel_avg += otherBoid.get_Vy();
            close_boids += 1;
        }
    }
    if (close_boids > 0){
        xvel_avg = xvel_avg/close_boids;
        yvel_avg = yvel_avg/close_boids;
        vx_ += (xvel_avg - vx_)*alignfactor;
        vy_ += (yvel_avg - vy_)*alignfactor;
    }
};


void Boid::speedRange (double minSpeed, double maxSpeed){
    double speed = sqrt(vx_*vx_ + vy_*vy_);
    if(speed>maxSpeed && speed>0){
    vx_ = (vx_/speed)*maxSpeed;
    vy_ = (vy_/speed)*maxSpeed;
    }
    else if(speed<minSpeed && speed>0){
    vx_ = (vx_/speed)*minSpeed;
    vy_ = (vy_/speed)*minSpeed;
    }

};
void Boid::margin (double turnfactor, double leftmargin, double rightmargin, double bottommargin, double topmargin){
    if (x_ < leftmargin + 50)
    vx_ = vx_ + turnfactor;
    if (x_ > rightmargin - 50)    
    vx_ = vx_ - turnfactor;
    if (y_ > bottommargin + 50)
    vy_ = vy_ - turnfactor;
    if (y_ < topmargin - 50)
    vy_ = vy_ + turnfactor;
    
};


