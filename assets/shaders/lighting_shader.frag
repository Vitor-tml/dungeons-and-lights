#version 330 core

uniform sampler2D texture; // Textura do mapa
uniform vec2 lightPos; // Posição da luz (normalizada)
uniform float lightRadius; // Raio da luz
uniform float intensity; // Intensidade da luz
uniform vec2 resolution; // Resolução da tela

out vec4 fragColor;

void main() {
    // Coordenadas da textura
    vec2 texCoord = gl_FragCoord.xy / resolution;

    // Calcular a distância do fragmento até a posição da luz
    float distance = length(texCoord - lightPos);

    // Calcular a intensidade da luz com base na distância
    float lightEffect = smoothstep(lightRadius, lightRadius - 0.1, distance);

    // Obter a cor da textura
    vec4 texColor = texture(texture, texCoord);

    // Aplicar a intensidade da luz
    fragColor = texColor * (1.0 - lightEffect * intensity);
}