#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>


#include <cmath>
#include <time.h>
#include <iostream>
#include <cstdlib>

#include "particles.hpp"

#define  HEIGHT 1000
#define  WIDTH  1000

#define SCALE_FACTOR_X (WIDTH  / SIZE_X)
#define SCALE_FACTOR_Y (HEIGHT / SIZE_Y)





// need to implement the Quadtree
// the quadtree needs to ignore stable flagged particles
// and process unstable ones

int main()
{

    std::cout << "start" << std::endl;

    sf::Text fps;
    fps.setFillColor(sf::Color::White);
    fps.setCharacterSize(35);
    
    sf::Clock clock;

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "falling_sand");

    // initializing ImGui
    ImGui::SFML::Init(window);

    std::cout << "setting frame limit" << std::endl;
    window.setFramerateLimit(120);

    bool showTree   = false;
    bool treeActive = false;

    std::cout << "instantiating grid" << std::endl;
    particleGrid particleGrid;

    

    particle_type selected_type;

    particleGrid.setScale(sf::Vector2f(SCALE_FACTOR_X, SCALE_FACTOR_Y));

    std::cout << "initializing grid" << std::endl;
    particleGrid.initialize_cells();

    //particleGrid.add_random_cells();

    std::cout << "start loop" << std::endl;
    while (window.isOpen())
    {
        window.clear();
        
        sf::Event event;

        std::cout << "listening to mouse" << std::endl;
        if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
            particleGrid.add_bucket(mouse_position.x,
                                    mouse_position.y,
                                    selected_type);
        }

        while (window.pollEvent(event))
        {
            // uncomment this afterwards.
            //ImGui::SFML::ProcessEvent(event);

            switch (event.type)
            {
                case sf::Event::Closed :
                    window.close();
                    break;
                default:
                    break;
            }
        }

        std::cout << "init gui" << std::endl;
        // ImGui window config
        sf::Time elapsed = clock.restart();
        fps.setString("FPS: "+ std::to_string(1. / elapsed.asSeconds()));


        ImGui::SFML::Update(window, elapsed);

        ImGui::Begin("Elements");

        // Elements Buttons
        ImGui::Checkbox("Show QuadTree", &showTree);
        ImGui::Checkbox("Use QuadTree" , &treeActive);
        if(ImGui::Button( "Sand")) selected_type = SAND  ;
        if(ImGui::Button("Water")) selected_type = WATER ;
        if(ImGui::Button("Smoke")) selected_type = SMOKE ;
        if(ImGui::Button("Stone")) selected_type = STONE ;
        if(ImGui::Button("Erase")) selected_type = AIR   ;
        ImGui::SliderInt("Bucket_Size", &particleGrid.brush_size, MIN_BUCKET_SIZE, MAX_BUCKET_SIZE);
        


        std::cout << "init quadtree" << std::endl;
        particleGrid.initialize_quadTree();
        particleGrid.populateQuadTree();
        particleGrid.setQuadTreeLeaves();

        // update particles state
        std::cout << "updating cells" << std::endl;


        if(treeActive)
        particleGrid.processByQuadTree();
        else
        particleGrid.update_all();

        std::cout << "rendering cells" << std::endl;
        particleGrid.render(window);


        /*
         *
         For Debug:
         Show quadtree regions.
        *
        */
        if(showTree)
        particleGrid.renderQuadRegions(window);

        // FPS
        std::cout << std::to_string(1. / elapsed.asSeconds()) << std::endl;
        window.draw(fps);

        ImGui::End();

        // render menu
        ImGui::SFML::Render(window);

        
        
        window.display();
    }


    ImGui::SFML::Shutdown(window);
    return 0;

    
    


}