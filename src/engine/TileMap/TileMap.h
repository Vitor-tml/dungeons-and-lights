#ifndef _TILEMAP_H
#define _TILEMAP_H

#include <string>
#include <vector>
#include "../../game/Entity/Entity.h"
namespace TileEngine {

class TileEntity : public Entity {
private:
    int id;
public:
    TileEntity(sf::Texture &textura, int x, int y, int xTile, int yTile, int tileSize, bool colide = 0);
    ~TileEntity(){};
    void executar(float deltaTime) override { atualizaHitBox();};
    void colidiuComTile(Entity* e) override {};
    void colidiu(Entity* e) override { e->colidiuComTile(this); };
};

class TileMap {
private:
    int tileSize;
    int height;
    int width;
    std::vector<std::vector<int>> matrizTiles;              ///< Lógica que gera o mapa, cada número signfica o estado da célula (chão, parede)
    std::vector<std::vector<TileEntity*>> matrizEntidades;  ///< Matriz das entidades que compoem o mapa, usado para colizão
    std::vector<std::vector<sf::Vector2f>> coordenadas;     ///< Coordenada de cada célula do mapa
    std::vector<TileEntity*> lTiles;                        ///< Lista de entidades para renderização
    sf::Texture sheet;                                      ///< Textura das células
public:
    TileMap();
    ~TileMap();
    void loadSheet(sf::Texture& textura);
    std::vector<Entity*> getEntitys();
    sf::Vector2f getOrigem() const;
    void criaGridAleatoria(float densidadeParedes = 0.4f);
    void aplicarRegras(int numeroIteracoes);
    int contarVizinhosVivos(int i, int j);
    void atualizarEntidades();
    void renderizar();
};
}
#endif