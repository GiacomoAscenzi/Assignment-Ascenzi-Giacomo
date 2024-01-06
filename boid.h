#ifndef Boid_h
#define Boid_h

#include <iostream>
#include <vector>
#include <fstream>
class Boid {
    public:
        Boid(double x, double y, double vx, double vy);

        double get_X() const { return x_; }
        double get_Y() const { return y_; }
        double get_Vx() const { return vx_; }
        double get_Vy() const { return vy_; }
        double get_Orientation() const { return orientation_;}
        void updatePosition ();
        void cohesion (double visibleRange, double centering_factor, std::vector<Boid>& closeBoid);
        void separation (double visibleRange, double centering_factor, std::vector<Boid>& closeBoid);
        void alignment (double visibleRange, double centering_factor, std::vector<Boid>& closeBoid);
        void speedRange (double minSpeed, double maxSpeed);
        void margin (double turnfactor, double leftmargin, double rightmargin, double bottommargin, double topmargin);
        void savePosition(std::ofstream& file) const { file << x_ << " " << y_ << " " << orientation_ << " "; }

    private:
        double x_;
        double y_;
        double vx_;
        double vy_;
        double orientation_;
        

        
};

#endif