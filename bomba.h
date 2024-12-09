#ifndef __BOMBA_H__
#define __BOMBA_H__

#include "cocos2d.h"
#include <map>

class Bomba {
public:
    enum class Estado {
        NORMAL,
        EXPLOSION,
        DESAPARECER
    };

    Bomba(cocos2d::Layer* layer, int fila, int columna, float tileSizeX, float tileSizeY, float offsetX, float offsetY,
        int (*map)[13], std::map<std::pair<int, int>, cocos2d::Sprite*>& bloquesDestructibles);
    ~Bomba();

    void actualizar(float delta);   // Actualiza el estado de la bomba
    Estado getEstado() const;       // Devuelve el estado actual de la bomba
    int getFila() const;            // Devuelve la fila de la bomba
    int getColumna() const;         // Devuelve la columna de la bomba

private:
    int fila, columna;              // Posición en la matriz
    float tileSizeX, tileSizeY;     // Tamaño de cada celda
    float offsetX, offsetY;         // Offset para centrar el mapa
    int (*map)[13];                 // Referencia al mapa
    Estado estado;                  // Estado actual de la bomba
    cocos2d::Sprite* sprite;        // Sprite de la bomba
    cocos2d::Layer* gameLayer;      // Capa donde se dibuja
    std::map<std::pair<int, int>, cocos2d::Sprite*>& bloquesDestructibles; // Referencia a los bloques destructibles

    float tiempoActual;             // Tiempo transcurrido
    float tiempoExplosion;          // Tiempo antes de la explosión

    void explotar();
    void desaparecer();
    void animacionBomba();
    void animacionExplosion();
    void afectarBloques();
};

#endif // __BOMBA_H__
