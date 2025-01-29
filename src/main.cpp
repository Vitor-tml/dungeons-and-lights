#include <iostream>
#include "SFML/Graphics.hpp"

#include "./engine/Renderer/Renderer.h"
#include "./engine/ResourceManager/ResourceManager.h"
#include "./engine/CollisionManager/CollisionManager.h"
#include "./engine/TileMap/TileMap.h"
#include "./game/Entity/Character/Player/Player.h"
int main()
{
    
    auto gGrafico = Gerenciador::Renderer::getInstance();
    auto gRecursos = Gerenciador::ResourceManager::getInstance();
    Gerenciador::CollisionManager gColisao;
    gRecursos->loadTexture("a", "../assets/player.png");
    gRecursos->loadTexture("mapa", "../assets/map.png");
    auto jogador = Player(gRecursos->getTexture("a"));
    jogador.setPosicao(200, 300);

    gGrafico->carregarShaders();
    TileEngine::TileMap mapa;
    
    auto entidaderMapa = mapa.getEntitys();
    for(auto e : entidaderMapa)
        gColisao.addEntity(e);
    gColisao.addEntity(&jogador);
    sf::Clock clock; // Relógio para calcular o delta time

    while(gGrafico->isOpen())
    {
        float deltaTime = clock.restart().asSeconds(); // Calcula o delta time

        sf::Event evento;
        while(gGrafico->pollEvent(evento)) // Importante pegar os eventos antes de tratar!
        {
            if(evento.type == sf::Event::Closed)
                gGrafico->close();
        }
        // std::cout << jogador.getPosicao().x << " " << jogador.getPosicao().y << std::endl;
        gColisao.tratarColisoes();
        
        jogador.executar(deltaTime);
        jogador.renderizar(2);
        mapa.renderizar();
        gGrafico->setCentroCamera(jogador.getPosicao().x, jogador.getPosicao().y);
        gGrafico->render();
    }
    
    return 0;
}