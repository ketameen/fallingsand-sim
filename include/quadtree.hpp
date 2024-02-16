#pragma once

#include <vector>
#include <iostream>
#include <cstdlib>
#include <array>

struct QuadPoint 
{
    float x, y;

    QuadPoint()             { this->x = 0; this->y = 0; }
    QuadPoint(int x, int y) { this->x = x; this->y = y; }

};

typedef struct QuadPoint QuadPoint;



struct Boundary
{
    QuadPoint center;
    
    float halfDimension;

    Boundary(){}

    Boundary (QuadPoint center, float halfDimension)
    {
        this->center = center;
        this->halfDimension = halfDimension;
    }

    bool contains(QuadPoint point) // check if point is within boundary
    {
        return ( std::abs(center.x - point.x) <= halfDimension )
                && 
               ( std::abs(center.y - point.y) <= halfDimension ) ;
    }

};

typedef struct Boundary Boundary;



class QuadTree 
{
    private:
        static const int _NODE_CAPACITY = 64 ;
        int _current_size               = 0 ;
        bool active                     = false;

        std::vector<QuadPoint> _points;

        

        Boundary   _boundary;

        QuadTree * _quad_NW = nullptr;
        QuadTree * _quad_NE = nullptr;
        QuadTree * _quad_SW = nullptr;
        QuadTree * _quad_SE = nullptr;


    public:
        QuadTree();
        QuadTree(Boundary);
        QuadTree(QuadPoint, float);

        bool empty();
        void setActive();
        bool isActive();

        int getCapacity();
        Boundary getBoundary();

        void setPoints(std::vector<QuadPoint>);

        QuadTree * getNW();
        QuadTree * getNE();
        QuadTree * getSW();
        QuadTree * getSE();


        std::vector<QuadPoint> queryPoints(Boundary);

        void processBoundary();
        
        std::vector<QuadTree *> getLeaves(QuadTree * root, std::vector<QuadTree*> &leaves);

        bool insert(QuadPoint);
        bool insert(QuadPoint, bool);
        void subdivide();


        ~QuadTree();


};