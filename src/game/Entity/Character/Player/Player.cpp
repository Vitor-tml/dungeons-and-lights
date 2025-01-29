#include "Player.h"
#include <iostream>

Player::Player(sf::Texture& textura, int x, int y, int life, int atack, int points)
       :Character(textura, x, y,  life, atack)
{
    pontos = points;
    velocidadeHorizontal = 250.f;
    
}

void Player::executar(float deltaTime)
{
    velocidade.x = 0.f; // Reseta a velocidade horizontal para não ter aceleração infinita
    velocidade.y = 0.f; // Reseta a velocidade vertical para não ter aceleração infinita
    velocidadeHorizontal = 200.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        velocidade.x -= velocidadeHorizontal; // Mover para a esquerda
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        velocidade.x += velocidadeHorizontal;  // Mover para a direita
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        velocidade.y -= velocidadeHorizontal;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        velocidade.y += velocidadeHorizontal;
    }
    // std::cout << velocidade.x << " " << velocidade.y << std::endl;
    this->setPosicao(this->getPosicao() + sf::Vector2f(velocidade.x * deltaTime, velocidade.y * deltaTime));
    isMoving = (velocidade.x != 0)? 1 : 0;

    Character::executar(deltaTime); // Chama o executar da classe pai que aplica a movimentação
}

void Player::pular()
{
    fisica.pular();
}