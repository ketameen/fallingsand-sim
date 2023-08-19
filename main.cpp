#include <SFML/Graphics.hpp>

#include <cmath>
#include <time.h>
#include <iostream>
#include <cstdlib>

#include "particles.hpp"


#define HEIGHT 800
#define WIDTH  800




int main()
{
    sf::Clock clock;
    sf::RenderWindow window(sf::VideoMode(SIZE_X, SIZE_Y), "sand");

    window.setFramerateLimit(1000);

    particleGrid particleGrid;


    /*
    particle * Particle = new particle();

    Particle->type = SAND;
    */

    particleGrid.initialize_cells();

    //particleGrid.add_random_cells();

    
    /*
    for(int x = SIZE_X/3; x < SIZE_Y/2; x++)
    {
        for(int y = SIZE_Y/3; y < SIZE_Y/2; y++)
        {
            
            particleGrid.add_particle(x, y, *Particle);
            
        }
    }
    */

    

    while (window.isOpen())
    {
        window.clear();
        
        sf::Event event;

        while (window.pollEvent(event))
        {

            switch (event.type)
            {
                case sf::Event::Closed :
                    window.close();
                    break;


                case sf::Event::MouseButtonPressed:
                {
                    if (event.mouseButton.button == sf::Mouse::Right)
                    {
                        //std::cout << "the right button was pressed" << std::endl;
                        //std::cout << "mouse x: " << event.mouseButton.x << std::endl;
                        //std::cout << "mouse y: " << event.mouseButton.y << std::endl;
                        std::cout << "adding particle" << std::endl;

                        //particleGrid.add_random_cells();
                        
                        for (int i = -10; i<20; i++)
                        {
                            for (int j = -10; j<20; j++)
                            {
                                int base          = 120;
                                int color_value_r =  base + std::rand() % (255-base);

                                particle * Particle = new particle();
                                particle_type type  = SAND;
                                Particle->type      = type;
                                Particle->color     = sf::Color(color_value_r,90,0);

                                particleGrid.add_particle(event.mouseButton.x + i, event.mouseButton.y + j, *Particle);
                            }
                        }
                        
                        
                    }

                    // to refactor 
                    if (event.mouseButton.button == sf::Mouse::Left)
                    {
                        //std::cout << "the right button was pressed" << std::endl;
                        //std::cout << "mouse x: " << event.mouseButton.x << std::endl;
                        //std::cout << "mouse y: " << event.mouseButton.y << std::endl;
                        //std::cout << "removing particle" << std::endl;

                        for (int i = -10; i<20; i++)
                        {
                            for (int j = -10; j<20; j++)
                            {
                                int base          = 120;
                                int color_value_b =  base + std::rand() % (255-base);

                                particle * Particle = new particle();
                                particle_type type  = WATER;
                                Particle->type      = type;
                                Particle->color     = sf::Color(100,120,color_value_b);

                                particleGrid.add_particle(event.mouseButton.x + i, event.mouseButton.y + j, *Particle);
                            }
                        }
                        
                    }
                    break;
                }
                default:
                    break;
            }
        }

        
        
        for(int x = 0; x < SIZE_X; x++)
        {
            for(int y = 0; y < SIZE_Y; y++)
            {
                particle Particle = particleGrid.get_particle(x,y); 
                if(Particle.type != AIR)
                {
                    //std::cout << particleGrid.get_particle(x,y).type << std::endl;
                    //std::cout << "pixel drawn at " << x <<", "<< y << std::endl;

                    /*
                    int color_value_r =  std::rand() % 255;
                    int color_value_g =  std::rand() % 255;
                    int color_value_b =  std::rand() % 255;
                    */
                    sf::Vertex point(sf::Vector2f(x,y),Particle.color);
                    //sf::Vertex point(sf::Vector2f(x,y),sf::Color(200, 200, 200));

                    window.draw(&point, 1, sf::Points);
                }
            }
        }

        //std::cout<< "displaying frame "<< std::endl;
        particleGrid.update_all();
        window.display();
    }

    return 0;

    
    


}