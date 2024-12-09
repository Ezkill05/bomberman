#ifndef __JUGADOR_H__
#define __JUGADOR_H__

#include "cocos2d.h"

class Jugador {
public:
    Jugador(cocos2d::Layer* layer, int filaInicial, int columnaInicial, float tileSizeX, float tileSizeY, float offsetX, float offsetY);
    ~Jugador();

    void mover(int nuevaFila, int nuevaColumna, int map[11][13]);
    void actualizarPosicion();
    cocos2d::Sprite* getSprite() const;

    int getFila() const;
    int getColumna() const;

    void detenerAnimacion();

private:
    cocos2d::Sprite* sprite;
    cocos2d::Action* animacionActual;
    int fila, columna;
    float tileSizeX, tileSizeY;
    float offsetX, offsetY;

    cocos2d::Action* crearAnimacion(const std::string& direccion);
};

#endif // __JUGADOR_H__

