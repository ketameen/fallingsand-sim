


#include "quadtree.hpp"

QuadTree::QuadTree(){}

QuadTree::QuadTree(Boundary boundary) {this->_boundary = boundary;}

QuadTree::QuadTree(QuadPoint center, float halfDimension) {this->_boundary = Boundary(center, halfDimension);}

Boundary QuadTree::getBoundary() { return this->_boundary; }

QuadTree * QuadTree::getNW() {return this->_quad_NW;}

QuadTree * QuadTree::getNE() {return this->_quad_NE;}

QuadTree * QuadTree::getSW() {return this->_quad_SW;}

QuadTree * QuadTree::getSE() {return this->_quad_SE;}

bool QuadTree::insert(QuadPoint point)
{
    /*
    if(this->_current_size < this->_NODE_CAPACITY)
    {
        this->points[this->_current_size] = point ;

        this->_current_size ++ ;
    }
    */
    //____________________________________________________

        std::cout << "inserting point" << std::endl;

        if (!this->_boundary.contains(point))
        {
            std::cout << "does not contain point" << std::endl;
            return false; 
        }

        std::cout << "checking" << std::endl;

        if (this->_current_size < this->_NODE_CAPACITY && this->_quad_NW == nullptr)
        {
            std::cout << "adding point" << std::endl;

            this->_points[this->_current_size] = point ;

            this->_current_size ++ ;

            return true;
        }

        if (this->_quad_NW == nullptr)
        {
            std::cout << "subdividing" << std::endl;
            this->subdivide();
        }
            
        
        std::cout << "inserting to other regions" << std::endl;

        if (this->getNW()->insert(point)) {std::cout << "inserting to NW" << std::endl; return true;}
        if (this->getNE()->insert(point)) {std::cout << "inserting to NE" << std::endl; return true;}
        if (this->getSW()->insert(point)) {std::cout << "inserting to SW" << std::endl; return true;}
        if (this->getSE()->insert(point)) {std::cout << "inserting to SE" << std::endl; return true;}

        std::cout << "done" << std::endl;
    
        return false;

    
}

void QuadTree::subdivide()
{
    QuadTree * NW = this->getNW();
    QuadTree * NE = this->getNE();
    QuadTree * SW = this->getSW();
    QuadTree * SE = this->getSE();

    Boundary current_boundary = this->getBoundary();
    QuadPoint center          = current_boundary.center;
    float halfDimension       = current_boundary.halfDimension;

    std::cout << "affecting boundaries" << std::endl;

    //_______________________________________________________________

    Boundary NW_boundary 
    = 
    Boundary(
             QuadPoint(center.x - halfDimension / 2,
                       center.y + halfDimension / 2)
                       ,
                       current_boundary.halfDimension / 2
                       );
    NW = new QuadTree(NW_boundary);


    //_______________________________________________________________

    Boundary NE_boundary = 
    Boundary(
            QuadPoint(center.x + halfDimension / 2,
                      center.y + halfDimension / 2)
                      ,
                      current_boundary.halfDimension / 2 
            );
    
    NE = new QuadTree(NE_boundary);


    ///_______________________________________________________________

    Boundary SW_boundary = 
    Boundary(
            QuadPoint(center.x - halfDimension / 2,
                      center.y - halfDimension / 2)
                      ,
                      current_boundary.halfDimension / 2 );
    
    SW = new QuadTree(SW_boundary);


    //_______________________________________________________________

    Boundary SE_boundary = 
    Boundary(
            QuadPoint(center.x + halfDimension / 2,
                      center.y - halfDimension / 2)
                      ,
                     current_boundary.halfDimension / 2 );
    
    SE = new QuadTree(SE_boundary);

    std::cout << "boundaries affected" << std::endl;
}

// retrieving leaf nodes to know which boundaries to process.
std::vector<QuadTree> QuadTree::getLeaves(QuadTree & root)
{
    //std::vector<QuadTree> leaves;

    //if(this->getNW() == nullptr) leaves.push_back();
    static std::vector<QuadTree> Qtree;

    std::cout << "getting leaves" << std::endl;

    if (root.getNW() == nullptr) 
    {
        
        std::cout << "only root" << std::endl;
        Qtree.push_back(root);
    }
    

    else
    {
        std::cout << "dividing" << std::endl;
        getLeaves(*root.getNW());
        getLeaves(*root.getNE());
        getLeaves(*root.getSW());
        getLeaves(*root.getSE());
    }

    return Qtree;

}
