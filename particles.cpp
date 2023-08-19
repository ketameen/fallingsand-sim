#include "particles.hpp"

#include <iostream>

particleGrid::particleGrid()
{
    
}

particleGrid::particleGrid(int HEIGHT, int WIDTH)
{
    this->_HEIGHT = HEIGHT;
    this->_WIDTH  = WIDTH ;
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
        Particle->color     = sf::Color(color_value_r,190,0);

        particleGrid::add_particle(RAND_X, RAND_Y, *Particle);
    }
}

void particleGrid::initialize_cells()
{
    particle * Particle = new particle();
    particle_type type  = AIR;
    Particle->type      = type;
    Particle->color     = sf::Color(0);


    for (int i = 0; i < SIZE_X; i++)
    {
        for(int j = 0; j < SIZE_Y; j++)
        {
            this->get_cells()[i][j] = *Particle;
        }
    }
}

particle particleGrid::get_particle(int x, int y)
{
    return this->_cells[y][x];
}

void particleGrid::add_particle(int x, int y, particle  particle)
{
    this->_cells[y][x] = particle;
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

void particleGrid::update_particle_type(int x, int y, particle_type type)
{

}

int particleGrid::check_neighbors(int x, int y)
{
    auto cells = this->get_cells();

    float current_density = cells[x][y].density;


    if(x+1 < SIZE_X && cells[x+1][y].density < current_density) return 1;

    //if(x+1 < SIZE_X && cells[x+1][y].type == AIR) return 1;

    if(x+1 < SIZE_X &&
       y+1 < SIZE_Y &&
       y-1 > 0 &&
       cells[x+1][y-1].density < current_density &&
       cells[x+1][y+1].density < current_density ) return 2;

    //if(x+1 < SIZE_X && y+1 < SIZE_Y && y-1 > 0 && cells[x+1][y-1].type == AIR && cells[x+1][y+1].type == AIR ) return 2;

    if(x+1 < SIZE_X && y-1 > 0      && cells[x+1][y-1].density < current_density) return 3;

    if(x+1 < SIZE_X && y+1 < SIZE_Y && cells[x+1][y+1].density < current_density) return 4;

    if(y+1 < SIZE_Y &&
       y-1 > 0      &&
       cells[x][y].type == WATER &&
       cells[x][y+1].density < current_density &&
       cells[x][y-1].density < current_density ) return 5;

    if(y-1 > 0      && cells[x][y].type == WATER && cells[x][y-1].density < current_density ) return 6;

    if(y+1 < SIZE_Y && cells[x][y].type == WATER && cells[x][y+1].density < current_density ) return 7;

    return 0;

}

void particleGrid::swap_particle(int x0, int y0, int x1, int y1)
{
    particle Particle = this->_cells[x0][y0];

    particle_type current_type = Particle.type;
    particle_type target_type  = this->_cells[x1][y1].type;

    sf::Color current_color = Particle.color;
    sf::Color target_color  = this->_cells[x1][y1].color;

    this->_cells[x0][y0].type    = target_type;

    this->_cells[x1][y1].type    = current_type;
    this->_cells[x1][y1].color   = current_color;
    this->_cells[x1][y1].density = this->_cells[x0][y0].density;
    this->_cells[x1][y1].updated = true;

    this->_cells[x0][y0].color  = target_color;
}

void particleGrid::update_all()
{

    for(int i = 0; i < SIZE_X; i++)
    {
        for(int j = 0; j < SIZE_Y; j++ )
        {
            if(this->_cells[i][j].type != AIR && !(this->_cells[i][j].updated))
            {

                int neighbor_state = this->check_neighbors(i,j);


                switch (neighbor_state)
                {
                    case 0:
                        break;
                    case 1:
                    {
                        this->swap_particle(i, j, i+1, j);
                        break;
                    }
                    
                    case 2:
                    {
                        if(std::rand() % 3 == 0)
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
                    
                    default:
                    break;
                }
            }

        }
    }

    // reset the state of all updated particles.
    this->reset_cells();
}