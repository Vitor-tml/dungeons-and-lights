#include "PhysicsComponent.h"
#include <iostream>

float PhysicsComponent::gravidade = 980.f;
PhysicsComponent::PhysicsComponent(sf::Vector2f  *position, sf::Vector2f *speed, float fPulo)
{
    velocidade = speed;
    posicao = position;
    estaNoChao = true;
    forcaPulo = fPulo;
}


void PhysicsComponent::aplicaFisica(float deltaTime)
{
    // Atualiza a posição com base na velocidade
    posicao->x += velocidade->x * deltaTime;
    posicao->y += velocidade->y * deltaTime;
}

void PhysicsComponent::setNoChao(bool estado)
{
    estaNoChao = estado;
}

bool PhysicsComponent::getNoChao()const
{
    return estaNoChao;
}

void PhysicsComponent::pular(float forcaPulo)
{
    if (estaNoChao) {
        velocidade->y = -forcaPulo; // Aplica a força do pulo
        estaNoChao = false; // Marca que não está mais no chão
        pulando = true; // Marca que está pulando
    }
}
