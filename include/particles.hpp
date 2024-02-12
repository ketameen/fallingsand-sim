#pragma once

#include <quadtree.hpp>

#include <vector>
#include <cstdint>

#include <SFML/Graphics.hpp>

#define SIZE_X 200
#define SIZE_Y 200

#define MIN_BUCKET_SIZE 3
#define MAX_BUCKET_SIZE 12

#define CELL_NUMBER 20000



enum particle_type
{
    AIR  ,
    SAND ,
    WATER,
    SMOKE,
    STONE
};

enum matter_state
{
    SOLID,
    FLUID,
    GAS  ,
};

typedef struct particle
{
    int x, y;

    particle_type type;

    matter_state state;
    
    uint8_t  density {1};

    bool    updated {false};

    bool    stable  {false};
    
    bool    dynamic {true};

    sf::Color color;

} particle;

class particleGrid 
{
    private:
        QuadTree quadTree;
        int _WIDTH, _HEIGHT;
        particle _cells [SIZE_Y] [SIZE_X];
        sf::Vector2f _scale;
        
    
    public:
        int brush_size = MIN_BUCKET_SIZE;
        particleGrid(int,int);
        particleGrid();

        void setBrushSize(int size);
        int getBrushSize();

        auto get_cells();
        void set_dimensions(int, int);

        void initialize_cells();
        void initialize_quadTree();
        
        void update_quadTree();

        sf::Vector2f getScale();
        void setScale(sf::Vector2f scale);

        uint8_t getDensityByType(particle_type);
        bool    getStateByType(particle_type);
        bool    getDynamicStateByType(particle_type);

        matter_state getMatterStateByType(particle_type);

        sf::Color generateColor(particle_type);

        void processByQuadTree();
        void renderQuadRegions(sf::RenderWindow&);

        bool inbound(int, int);
        bool update_particle(int, int);
        void update_particle_type(int, int, particle_type);
        void update_particle_color(int, int, sf::Color);
        void update_particle_state(int, int, bool);
        void update_all();
        void update_region(Boundary);

        void add_particle(int, int, particle);
        void add_bucket(int, int, particle_type);
        void erase(int);
        void add_random_cells();
        void reset_cells();
        void swap_particle(int,int,int,int);

        uint8_t check_neighbors(int,int);
        
        particle get_particle(int,int);

        void renderRegion(sf::RenderWindow&, Boundary);
        void render(sf::RenderWindow&);
        




};