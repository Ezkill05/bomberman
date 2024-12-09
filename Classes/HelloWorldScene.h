#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "C:\Users\alanb\OneDrive\Escritorio\AABOMB\proj.win32\bomba.h"
#include "C:\Users\alanb\OneDrive\Escritorio\AABOMB\proj.win32\Jugador.h"
#include "cocos2d.h"
#include <map>


class HelloWorld : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void update(float delta) override;
    CREATE_FUNC(HelloWorld);

private:
    cocos2d::Layer* gameLayer;
    Jugador* jugador;
    int map[11][13];
    std::map<cocos2d::EventKeyboard::KeyCode, bool> teclasPresionadas;

    void generarMapa();
    void dibujarMapa();
    void inicializarMovimiento();
    void manejarMovimiento();
    void colocarBomba();
    std::vector<Bomba*> bombas; // Lista de bombas activas
    float tileSizeX;
    float tileSizeY;
    float offsetX;
    float offsetY;
 
    cocos2d::Layer* destructibleLayer; // Nueva capa para bloques destructibles
    std::map<std::pair<int, int>, cocos2d::Sprite*> bloquesDestructibles; // Mapa para rastrear bloques



};

#endif // __HELLO_WORLD_SCENE_H__
