#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#define SIZE_X 300
#define SIZE_Y 300

#define CELL_NUMBER 20000;



enum particle_type
{
    AIR  ,
    SAND ,
    WATER,
    WALL
};

typedef struct particle
{
    int x,y;

    float density {1.2};
    
    particle_type type;

    bool updated {false};
    
    sf::Color color;

} particle;

class particleGrid 
{
    private:
        int _WIDTH, _HEIGHT;
        particle _cells [SIZE_Y] [SIZE_X];
    
    public:
        particleGrid(int,int);
        particleGrid();

        auto get_cells();

        void set_dimensions(int, int);
        void initialize_cells();
        void update_particle_type(int, int, particle_type);
        void update_particle_color(int, int, sf::Color);
        void update_all();
        void add_particle(int, int, particle);
        void add_random_cells();
        void reset_cells();
        void swap_particle(int,int,int,int);
        particle get_particle(int,int);
        int check_neighbors(int,int);




};