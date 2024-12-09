/*#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "cocos2d.h"
#include "Jugador.h"
#include "Bomba.h"

class Enemigo {
public:
    Enemigo(cocos2d::Layer* layer, int filaInicial, int columnaInicial, float tileSizeX, float tileSizeY, float offsetX, float offsetY);
    ~Enemigo();

    void actualizar(float delta, Jugador* jugador, std::vector<Bomba*>& bombas, int map[11][13]);
    void moverHaciaJugador(Jugador* jugador, int map[11][13]);
    void actualizarPosicion();
    void cambiarSprite(const std::string& direccion);
    void eliminar();

private:
    cocos2d::Layer* layer;    // Capa donde se dibuja el enemigo
    cocos2d::Sprite* sprite;  // Sprite del enemigo
    int fila, columna;        // Posición en la matriz
    float tileSizeX, tileSizeY; // Tamaño de cada celda
    float offsetX, offsetY;   // Offset para centrar el mapa
};

#endif */
