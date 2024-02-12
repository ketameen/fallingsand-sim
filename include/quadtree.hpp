#pragma once

#include <vector>
#include <iostream>

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
        return ( point.x - center.x > halfDimension )
                && 
               ( point.y - center.y > halfDimension ) ;
    }

};

typedef struct Boundary Boundary;



class QuadTree 
{
    private:
        static const int _NODE_CAPACITY = 8 ;
        int _current_size               = 0 ;

        QuadPoint  _points [_NODE_CAPACITY];

        Boundary   _boundary;

        QuadTree * _quad_NW;
        QuadTree * _quad_NE;
        QuadTree * _quad_SW;
        QuadTree * _quad_SE;


    public:
        QuadTree();
        QuadTree(Boundary);
        QuadTree(QuadPoint, float);
        
        int getCapacity();
        Boundary getBoundary();

        QuadTree * getNW();
        QuadTree * getNE();
        QuadTree * getSW();
        QuadTree * getSE();


        QuadPoint * queryPoints(Boundary);

        void processBoundary();
        
        std::vector<QuadTree> getLeaves(QuadTree& root);

        bool insert(QuadPoint);
        void subdivide();



};