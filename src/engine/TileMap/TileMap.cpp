#include "TileMap.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include "json.hpp"

using namespace nlohmann;

/**
 * @brief Componente responsável pelo carregamento do mapa no jogo
 */
namespace TileEngine{
//====================================================================================================
// Tile entity
//====================================================================================================
/***
 * @brief Construtora do tilemap, a posição absoluta tanto do tile na tela quanto da textura na imagem são calculadas internamente
 * @param x posição x na matriz
 * @param y posição y na matriz
 * @param xTile posição x da textura no tileset
 * @param yTile posição y da textura no tileset
 * @param tileSize tamanho da textura do célula
 * @param colide 0 -> sem colisão / 1 -> com colisão
 */
TileEntity::TileEntity(sf::Texture &textura, int x, int y, int xTile, int yTile, int tileSize, bool colide)
:Entity(textura, x * tileSize, y * tileSize, colide)
{
    sprite.setTextureRect(sf::IntRect(xTile * tileSize, yTile * tileSize, tileSize, tileSize));
    criarHitBox();
    setHitBoxSize(sf::Vector2f(16, 16));
    atualizaHitBox();
}

//====================================================================================================
// Tilemap
//====================================================================================================
TileMap::TileMap()
{
    loadSheet(Gerenciador::ResourceManager::getInstance()->getTexture("mapa"));
    tileSize = 16;
    width = 25;
    height = 25;
    criaGridAleatoria(0.4f);
    aplicarRegras(5);
    for(auto a : lTiles)
        a->atualizaHitBox();
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j) 
            std::cout << matrizTiles[i][j] << " "; 
        std::cout << "\n";
    }
    
}
TileMap::~TileMap(){}

/***
 * @brief
 * @param densidadeParedes porcentagem do mapa que vai ser parede
 */
void TileMap::criaGridAleatoria(float densidadeParedes) {
    // Inicializa a matriz de tiles com 0 (vazio)
    matrizTiles.resize(height, std::vector<int>(width, 0));

    // Preenche aleatoriamente a matriz com paredes (1) e espaços vazios (0)
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            // Adiciona parede aleatória com base na densidade
            if (static_cast<float>(rand()) / RAND_MAX < densidadeParedes) {
                matrizTiles[i][j] = 1; // Parede
            }
            // std::cout << matrizTiles[i][j] << " "; //============== Impressao mapa
        }
        // std::cout << "\n"; //====================================== Impressao mapa
    }

    // Agora criamos as entidades (TileEntities) para cada célula
    matrizEntidades.resize(height, std::vector<TileEntity*>(width, nullptr));
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            // Se for uma parede, cria uma TileEntity correspondente
            if (matrizTiles[i][j] == 1) {
                TileEntity* tile = new TileEntity(sheet, j, i, 0, 0, tileSize, false); // true para colidir
                matrizEntidades[i][j] = tile;
                lTiles.push_back(tile); // Adiciona à lista de renderização
            }
        }
    }
}


void TileMap::aplicarRegras(int numeroIteracoes) {
    std::vector<std::vector<int>> novaMatrizTiles = matrizTiles; // Cópia da matriz para aplicar mudanças

    for (int iteracao = 0; iteracao < numeroIteracoes; ++iteracao) {
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                int vizinhosVivos = contarVizinhosVivos(i, j);
                
                // Se tem mais de 4 vizinhos vivos (parede), se torna parede
                if (vizinhosVivos > 4) {
                    novaMatrizTiles[i][j] = 1;
                }
                // Se tem menos de 3 vizinhos vivos (parede), se torna espaço
                else if (vizinhosVivos < 3) {
                    novaMatrizTiles[i][j] = 0;
                }
            }
        }

        // Após aplicar as regras, substitui a matriz original pela nova
        matrizTiles = novaMatrizTiles;
    }

    // Após as iterações, atualizamos as entidades para refletir as mudanças
    atualizarEntidades();
}

int TileMap::contarVizinhosVivos(int i, int j) {
    int contador = 0;

    // Verifica as 8 células vizinhas (ao redor da célula [i, j])
    for (int di = -1; di <= 1; ++di) {
        for (int dj = -1; dj <= 1; ++dj) {
            int x = i + di;
            int y = j + dj;
            // Verifica se a célula vizinha está dentro dos limites
            if (x >= 0 && x < height && y >= 0 && y < width) {
                // Não conta a própria célula
                if (di != 0 || dj != 0) {
                    if (matrizTiles[x][y] == 1) {
                        contador++;
                    }
                }
            }
        }
    }

    return contador;
}

void TileMap::atualizarEntidades() {
    // Limpa a lista de entidades
    for (TileEntity* tile : lTiles) {
        delete tile;
    }
    lTiles.clear();

    // Recria as entidades baseadas na nova matriz de tiles
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (matrizTiles[i][j] == 1) {
                TileEntity* tile = new TileEntity(sheet, j, i, 0, 0, tileSize, true); // true para colidir
                matrizEntidades[i][j] = tile;
                lTiles.push_back(tile);
            } else {
                matrizEntidades[i][j] = nullptr;
            }
        }
    }
}

/// Retona as entidades para renderizar 
std::vector<Entity*> TileMap::getEntitys()
{
    std::vector<Entity*> tiles;
    for(TileEntity *t : lTiles)
        tiles.push_back(t);
    return tiles;
}
void TileMap::loadSheet(sf::Texture& textura)
{
    sheet = textura;
}

/// @brief Canto inferior esquerdo 
/// @return (x, y)
sf::Vector2f TileMap::getOrigem() const
{
    return sf::Vector2f( 0 * tileSize, height * tileSize);
}

void TileMap::renderizar()
{
    for(auto l : lTiles)
    {
        l->renderizar();
        // Gerenciador::Renderer::getInstance()->addDrawable(l->getHitBox(), 5);
    }
}

}