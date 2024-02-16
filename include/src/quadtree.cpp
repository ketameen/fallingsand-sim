


#include "quadtree.hpp"

QuadTree::QuadTree(){}

QuadTree::QuadTree(Boundary boundary) {this->_boundary = boundary;}

QuadTree::QuadTree(QuadPoint center, float halfDimension) {this->_boundary = Boundary(center, halfDimension);}

Boundary QuadTree::getBoundary() { return this->_boundary; }

QuadTree * QuadTree::getNW() {return this->_quad_NW;}

QuadTree * QuadTree::getNE() {return this->_quad_NE;}

QuadTree * QuadTree::getSW() {return this->_quad_SW;}

QuadTree * QuadTree::getSE() {return this->_quad_SE;}

int QuadTree::getCapacity(){return this->_NODE_CAPACITY;}

bool QuadTree::empty()
{
    return this->_current_size == 0;
}

void QuadTree::setActive()
{
    this->active = true;
}

bool QuadTree::isActive()
{
    return this->active;
}

/*
bool QuadTree::insert(QuadPoint point, bool particleActive)
{
    if (particleActive) setActive();
    insert(point);
    return true;
}
*/

bool QuadTree::insert(QuadPoint point, bool particleActive)
{

        if (!this->_boundary.contains(point))
        {
            /*
            std::cout << "does not contain point. Boundary centerX: " 
                      << this->_boundary.center.x 
                      << ", "
                      << this->_boundary.center.y
                      << " | half dimension: "
                      << this->_boundary.halfDimension
                      << std::endl;
            */
            return false; 
        }

        //std::cout << "checking" << std::endl;

        if (this->_points.size() < this->_NODE_CAPACITY )
        {
            //std::cout << "adding point" << std::endl;

            this->_points.push_back(point) ;

            //std::cout << "point added" << std::endl;

            if (particleActive) this->setActive();
            
            //std::cout << "current size: "<<  this->_current_size << std::endl;

            return true;
        }

        if (this->_quad_NW == nullptr)
        {
            //std::cout << "subdividing" << std::endl;
            this->subdivide();
            this->_points.clear();
        }
            
        
        //std::cout << "inserting to other regions" << std::endl;

        if (this->getNW()->insert(point, particleActive)) {
            //std::cout << "inserting to NW" << std::endl;
             return true;}
        if (this->getNE()->insert(point, particleActive)) {
            //std::cout << "inserting to NE" << std::endl;
             return true;}
        if (this->getSW()->insert(point, particleActive)) {
            //std::cout << "inserting to SW" << std::endl;
             return true;}
        if (this->getSE()->insert(point, particleActive)) {
            //std::cout << "inserting to SE" << std::endl;
            return true;}

        //std::cout << "done" << std::endl;
    
        return false;

    
}

std::vector<QuadPoint> QuadTree::queryPoints(Boundary boundary)
{
    std::vector<QuadPoint> points;

    for (int i = 0; i < this->_points.size(); i++)
    {
        QuadPoint point = this->_points[i];
        if (boundary.contains(point))
        {
            points.push_back(point);
        }
    }

    return points;
}

void QuadTree::setPoints(std::vector<QuadPoint> points)
{
    this->_points = points;
}

void QuadTree::subdivide()
{
    Boundary current_boundary = this->getBoundary();
    QuadPoint center          = current_boundary.center;
    float halfDimension       = current_boundary.halfDimension;

    //std::cout << "affecting boundaries" << std::endl;

    //_______________________________________________________________

    Boundary NW_boundary 
    = 
    Boundary(
             QuadPoint(center.x - halfDimension / 2,
                       center.y + halfDimension / 2)
                       ,
                       current_boundary.halfDimension / 2
                       );

    this->_quad_NW = new QuadTree(NW_boundary);

    std::vector<QuadPoint> pointsNW = this->queryPoints(NW_boundary);

    if(!pointsNW.empty())
    {

        this->_quad_NW->setPoints(pointsNW);
    }

    //_______________________________________________________________

    Boundary NE_boundary = 
    Boundary(
            QuadPoint(center.x + halfDimension / 2,
                      center.y + halfDimension / 2)
                      ,
                      current_boundary.halfDimension / 2 
            );
    
    this->_quad_NE = new QuadTree(NE_boundary);

    std::vector<QuadPoint> pointsNE = this->queryPoints(NE_boundary);

    if(!pointsNE.empty())
    {

        this->_quad_NE->setPoints(pointsNE);
    }

    ///_______________________________________________________________

    Boundary SW_boundary = 
    Boundary(
            QuadPoint(center.x - halfDimension / 2,
                      center.y - halfDimension / 2)
                      ,
                      current_boundary.halfDimension / 2 );
    
    this->_quad_SW = new QuadTree(SW_boundary);

    std::vector<QuadPoint> pointsSW = this->queryPoints(SW_boundary);

    if(!pointsSW.empty())
    {

        this->_quad_SW->setPoints(pointsSW);
    }
    //_______________________________________________________________

    Boundary SE_boundary = 
    Boundary(
            QuadPoint(center.x + halfDimension / 2,
                      center.y - halfDimension / 2)
                      ,
                     current_boundary.halfDimension / 2 );
    
    this->_quad_SE = new QuadTree(SE_boundary);

    std::vector<QuadPoint> pointsSE = this->queryPoints(SE_boundary);

    if(!pointsSE.empty())
    {

        this->_quad_SE->setPoints(pointsSE);
    }
    //std::cout << "boundaries affected" << std::endl;
}

// retrieving leaf nodes to identify which boundaries to process.
std::vector<QuadTree *> QuadTree::getLeaves(QuadTree * root, std::vector<QuadTree*> &leaves)
{

    if (root->getNW() == nullptr) 
    {
        
        //std::cout << "only root" << std::endl;
        leaves.push_back(root);
    }
    

    else
    {
        //std::cout << "getting NW children" << std::endl;

        getLeaves(root->getNW(), leaves);

        //std::cout << "getting NE children" << std::endl;
        getLeaves(root->getNE(), leaves);

        //std::cout << "getting SW children" << std::endl;
        getLeaves(root->getSW(), leaves);

        //std::cout << "getting SE children" << std::endl;
        getLeaves(root->getSE(), leaves);

        //std::cout << "got children" << std::endl;
    }

    return leaves;

}


QuadTree::~QuadTree(){}
