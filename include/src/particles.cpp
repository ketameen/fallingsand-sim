

#include "particles.hpp"
#include <iostream>
#include <cmath>

particleGrid::particleGrid(){};

particleGrid::particleGrid(int HEIGHT, int WIDTH)
{
    this->_HEIGHT = HEIGHT;
    this->_WIDTH  =  WIDTH;
}

sf::Vector2f particleGrid::getScale()
{
    return this->_scale;
}

void particleGrid::setScale(sf::Vector2f scale)
{
    this->_scale = scale;
}

auto particleGrid::get_cells()
{
    return this->_cells;
}

void particleGrid::set_dimensions(int HEIGHT, int WIDTH)
{
    this->_HEIGHT = HEIGHT;
    this->_WIDTH  = WIDTH ;
}

void particleGrid::add_random_cells()
{
    int counter = CELL_NUMBER;

    

    while(counter--)
    {
        int RAND_X = std::rand() % SIZE_X;
        int RAND_Y = std::rand() % SIZE_Y;
        
        int base = 150;
        int color_value_r =  base + std::rand() % (255-base);

        particle * Particle = new particle();
        particle_type type  = SAND;
        Particle->type      = type;
        Particle->density   = color_value_r;
        Particle->color     = sf::Color(color_value_r,190,0);

        particleGrid::add_particle(RAND_X, RAND_Y, *Particle);
    }
}

// treating each empty cell as an AIR cell is obviously very bad.
void particleGrid::initialize_cells()
{
    particle * Particle = new particle();
    particle_type type  = AIR;
    Particle->type      = type;
    Particle->density   = getDensityByType(type);
    Particle->color     = sf::Color(0);

    for (int i = 0; i < SIZE_X; i++)
    {
        for(int j = 0; j < SIZE_Y; j++)
        {
            this->get_cells()[i][j] = *Particle;
        }
    }
}

int particleGrid::getBrushSize()
{
    return this->brush_size;
}

void particleGrid::initialize_quadTree()
{
    this->quadTree = QuadTree();
}

particle particleGrid::get_particle(int x, int y)
{
    return this->_cells[y][x];
}

uint8_t particleGrid::getDensityByType(particle_type type)
{
    switch(type)
    {
        case SAND:
            return 3;
            break;

        case WATER:
            return 2;
            break;
        
        case SMOKE:
            return 0;
            break;
        //case STONE:
        //    return 5;
        //    break;
        default:
            return     1;
    }
}

bool particleGrid::getStateByType(particle_type type)
{
    switch(type)
    {
        case SAND:
            return false;
            break;

        case WATER:
            return false;
            break;
        
        case SMOKE:
            return false;
            break;
        case STONE:
            return true;
            break;
        default:
            return false;
    }
}

sf::Color particleGrid::generateColor(particle_type type)
{
    
    int base, color_value_r, color_value_g, color_value_b;
    sf::Color color(0, 0, 0);
    switch (type)
    {
        case AIR:
            return color;
            break;

        case SAND:
            base            = 120;
            color_value_r   = base + std::rand() % (255 - base);
            color = sf::Color(color_value_r, 90, 0);
            return color;
            break;

        case WATER:
            base            = 190;
            color_value_b   =  base + std::rand() % (255-base);
            color = sf::Color(120,120,color_value_b);
            return color;
            break;

        case STONE:
            base           =  175;
            color_value_r  =  base + std::rand() % (190-base);
            color_value_g  =  base + std::rand() % (190-base);
            color = sf::Color(color_value_r, base, base);
            return color;
            break;
        case SMOKE:
            base           =  120;
            color = sf::Color(base, base, base);
            return color;
            break;
        default:
            return color;
            break;
        
    }
}

void particleGrid::add_particle(int x, int y, particle  particle)
{
    this->_cells[y][x] = particle;
}

void particleGrid::setBrushSize(int size)
{
    this->brush_size = size;
}

void particleGrid::add_bucket(int mouseX, int mouseY, particle_type type)
{

    for (int i = -this->brush_size; i < this->brush_size; i++)
        {
            for (int j = -this->brush_size; j < this->brush_size; j++)
            {
                int cell_x = (mouseX + i) / getScale().x;
                int cell_y = (mouseY + j) / getScale().y;

                int base            = 120;
                int color_value_r   = base + std::rand() % (255 - base);

                particle * Particle = new particle();
                Particle->type      = type;
                Particle->density   = getDensityByType(type);
                Particle->dynamic   = getDynamicStateByType(type);
                Particle->stable    = getStateByType(type);
                Particle->color     = generateColor(type);
                Particle->state     = getMatterStateByType(type);

                this->add_particle(cell_x, cell_y, *Particle);
            }
        }
}

matter_state particleGrid::getMatterStateByType(particle_type type)
{
    switch (type)
    {
        case STONE:
            return SOLID;
            break;
        case WATER:
            return FLUID;
            break;
        case SAND:
            return SOLID;
            break;
        case SMOKE:
            return GAS;
            break;
        default:
            return GAS;
            break;
    }
}

bool particleGrid::getDynamicStateByType(particle_type type)
{
    switch (type)
    {
        case STONE:
            return false;
            break;
        case WATER:
            return true;
            break;
        case SAND:
            return true;
            break;
        case SMOKE:
            return true;
            break;
        default:
            return true;
            break;
    }
}


void particleGrid::reset_cells()
{
    for(int i = 0; i < SIZE_X; i++)
    {
        for(int j = 0; j < SIZE_Y; j++ )
        {
            this->_cells[i][j].updated = false;
        }
    }
}

bool particleGrid::inbound(int x, int y)
{
    if (x > SIZE_X || y > SIZE_Y || x < 0 || y < 0) return false;
    return true;
}

void particleGrid::update_particle_state(int x, int y, bool state)
{
    this->_cells[x][y].stable = state;
}

void particleGrid::update_particle_type(int x, int y, particle_type type)
{
    this->_cells[x][y].type = type;
}

/*
 *
 We need to check for the change in neighbors and make the cell not
 stable in that case.
 *
*/

uint8_t particleGrid::check_neighbors(int x, int y)
{
    auto cells = this->get_cells();

    float current_density = cells[x][y].density;

    if (inbound(x, y))
    {
        // check if there is a cell below the current cell
        if(cells[x+1][y].density < current_density) return 1;

        // check lower right & lower left    
        if(cells[x+1][y-1].density < current_density &&
           cells[x+1][y+1].density < current_density ) return 2;
        
        // check left
        if(cells[x+1][y-1].density < current_density) return 3;
        
        // check right
        if(cells[x+1][y+1].density < current_density) return 4;
        
        // check left & right (water) to refactor
        if(
        cells[x][y].type == WATER               &&
        cells[x][y+1].density < current_density &&
        cells[x][y-1].density < current_density ) return 5;
        
        // check left (water) to refactor
        if(cells[x][y].type == WATER &&
           cells[x][y-1].density < current_density ) return 6;
        
        // check right (water) to refactor
        if(cells[x][y].type == WATER &&
           cells[x][y+1].density < current_density ) return 7;

        if(cells[x][y].type != WATER) return 8;
        
        return 9;
    }
    return 0;

}

void particleGrid::swap_particle(int x0, int y0, int x1, int y1)
{
    particle Particle = this->_cells[x0][y0];

    this->_cells[x0 - 1][y0].stable = false;

    particle_type current_type   = Particle.type;
    particle_type target_type    = this->_cells[x1][y1].type;

    sf::Color current_color      = Particle.color;
    sf::Color target_color       = this->_cells[x1][y1].color;

    uint8_t target_density       = this->_cells[x1][y1].density;

    this->_cells[x0][y0].type    = target_type;

    this->_cells[x1][y1].type    = current_type;
    this->_cells[x1][y1].color   = current_color;
    this->_cells[x1][y1].density = this->_cells[x0][y0].density;
    this->_cells[x1][y1].updated = true;

    this->_cells[x0][y0].color   = target_color;
    this->_cells[x0][y0].density = target_density;
}

bool particleGrid::update_particle(int i, int j)
{

    if(!inbound(i+1, j+1) || !inbound(i-1, j-1))
    {
        return 0;
    }

    // to remove AIR condition.
    if(
       this->_cells[i][j].type != AIR &&
     !(this->_cells[i][j].updated)    &&
     !(this->_cells[i][j].stable)
     )
            {

                uint8_t neighbor_state = this->check_neighbors(i,j);


                switch (neighbor_state)
                {
                        
                    case 1:
                    {
                        this->swap_particle(i, j, i+1, j);
                        break;
                    }
                    case 2:
                    {
                        if(std::rand() % 10 == 0)
                        this->swap_particle(i, j, i+1, j - 1 );
                        else 
                        this->swap_particle(i, j, i+1, j + 1);
                        break;
                    }
                    
                    case 3:
                    {
                        this->swap_particle(i, j, i+1, j-1);
                        break;
                    }
                    case 4:
                    {
                        this->swap_particle(i, j, i+1, j+1);
                        break;
                    }
                    case 5:
                    {
                        if(std::rand() % 3 == 0)
                        this->swap_particle(i, j, i, j - 1 );
                        else 
                        this->swap_particle(i, j, i, j + 1);
                        break;
                    }
                    case 6:
                    {
                        this->swap_particle(i, j, i, j-1);
                        break;
                    }
                    case 7:
                    {
                        this->swap_particle(i, j, i, j+1);
                        break;
                    }
                    case 8:
                    {
                        update_particle_state(i, j, true);
                        break;
                    }
                    
                    default:
                    break;
                }
            }

            return 0;
}

void particleGrid::update_all()
{

    //std::cout << "starting update" << std::endl;
    for(int i = 0; i < SIZE_X; i++)
    {
        for(int j = 0; j < SIZE_Y; j++ )
        {
            //std::cout << "updating particle" << std::endl;
            update_particle(i, j);
            //if (get_particle(i, j).type != AIR) this->quadTree.insert(QuadPoint(i, j));
        }
    }

    // reset the state of all updated particles.
    this->reset_cells();
}

void particleGrid::processByQuadTree()
{
    // meh
    std::vector<QuadTree> leaves = this->quadTree.getLeaves(this->quadTree);

    for (QuadTree leaf: leaves)
    {
        update_region(leaf.getBoundary());
    }
}

void particleGrid::update_region(Boundary boundary)
{
    int start_x = boundary.center.x - boundary.halfDimension ;
    int   end_x = boundary.center.x + boundary.halfDimension ;

    int start_y = boundary.center.y - boundary.halfDimension ;
    int   end_y = boundary.center.y + boundary.halfDimension ;

    for (int x = start_x; x < end_x; x ++)
    {
        for (int y = start_y; y < end_y; y++)
        {
            update_particle(x, y);
        }
    }

}

void particleGrid::renderRegion(sf::RenderWindow& window, Boundary boundary)
{

    sf::RectangleShape rectangle(sf::Vector2f(boundary.center.x - boundary.halfDimension,
                                              boundary.center.y - boundary.halfDimension));

    rectangle.setSize(sf::Vector2f(2 * boundary.halfDimension,
                                   2 * boundary.halfDimension));

    rectangle.setFillColor(sf::Color(125,125,125));

    window.draw(rectangle);
}


void particleGrid::renderQuadRegions(sf::RenderWindow& window)
{
    std::vector<QuadTree> leaves = this->quadTree.getLeaves(this->quadTree);

    for (QuadTree leaf: leaves)
    {
        renderRegion(window, leaf.getBoundary());
    }

} 

void particleGrid::render(sf::RenderWindow& window)
{
    //std::cout << "rendering start" << std::endl;
    for(int x = 0; x < SIZE_X; x++)
        {
            for(int y = 0; y < SIZE_Y; y++)
            {
                //std::cout << "scaling cell position" << std::endl;
                int cell_x = floor(x * getScale().x);
                int cell_y = floor(y * getScale().y);

                //std::cout << "cell_x: "<< cell_x << " " << "cell_y: " << cell_y << std::endl; 

                //std::cout << "getting particle info" << std::endl;
                particle Particle = get_particle(x, y); 


                //std::cout << "checking against AIR" << std::endl;
                if(Particle.type != AIR)
                {
                    //std::cout << "Drawing rectangle" << std::endl;
                    sf::RectangleShape rectangle(sf::Vector2f(getScale().x, getScale().y));

                    rectangle.setFillColor(Particle.color);

                    rectangle.setPosition(cell_x, cell_y);

                    window.draw(rectangle);
                }
            }
        }
}