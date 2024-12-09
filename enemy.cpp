#include "enemy.h"
#include <HelloWorldScene.h>

USING_NS_CC;

enemy::enemy(cocos2d::Layer* layer, int filaInicial, int columnaInicial, float tileSizeX, float tileSizeY, float offsetX, float offsetY)
    : layer(layer), fila(filaInicial), columna(columnaInicial), tileSizeX(tileSizeX), tileSizeY(tileSizeY), offsetX(offsetX), offsetY(offsetY), sprite(nullptr) {

    // Crear el sprite del enemigo
    sprite = Sprite::create("enemy_down.png"); // Sprite inicial
    if (!sprite) {
        CCLOG("Error: No se pudo cargar el sprite del enemigo.");
        return;
    }

    // Escalar el sprite al tamaño de una celda
    float escalaX = tileSizeX / sprite->getContentSize().width;
    float escalaY = tileSizeY / sprite->getContentSize().height;
    sprite->setScale(escalaX, escalaY);

    // Colocar al enemigo en la posición inicial
    actualizarPosicion();

    // Añadir el sprite al layer
    layer->addChild(sprite, 10);
}

Enemigo::~Enemigo() {
    if (sprite && sprite->getParent()) {
        sprite->removeFromParent();
    }
}

void Enemigo::actualizar(float delta, Jugador* jugador, std::vector<Bomba*>& bombas, int map[11][13]) {
    if (!sprite) return; // Asegurarse de que el sprite no sea nulo

    // Movimiento hacia el jugador
    moverHaciaJugador(jugador, map);

    // Verificar colisión con el jugador
    if (jugador->getFila() == fila && jugador->getColumna() == columna) {
        CCLOG("Jugador muerto!");
        Director::getInstance()->end(); // Termi#include "Enemigo.h"

        USING_NS_CC;

        Enemigo::Enemigo(cocos2d::Layer * layer, int filaInicial, int columnaInicial, float tileSizeX, float tileSizeY, float offsetX, float offsetY)
            : layer(layer), fila(filaInicial), columna(columnaInicial), tileSizeX(tileSizeX), tileSizeY(tileSizeY), offsetX(offsetX), offsetY(offsetY), sprite(nullptr) {

            // Crear el sprite del enemigo
            sprite = Sprite::create("enemy_down.png"); // Sprite inicial
            if (!sprite) {
                CCLOG("Error: No se pudo cargar el sprite del enemigo.");
                return;
            }

            // Escalar el sprite al tamaño de una celda
            float escalaX = tileSizeX / sprite->getContentSize().width;
            float escalaY = tileSizeY / sprite->getContentSize().height;
            sprite->setScale(escalaX, escalaY);

            // Colocar al enemigo en la posición inicial
            actualizarPosicion();

            // Añadir el sprite al layer
            layer->addChild(sprite, 10);
        }

        Enemigo::~Enemigo() {
            if (sprite && sprite->getParent()) {
                sprite->removeFromParent();
            }
        }

        void Enemigo::actualizar(float delta, Jugador * jugador, std::vector<Bomba*>&bombas, int map[11][13]) {
            if (!sprite) return;

            // Movimiento hacia el jugador
            moverHaciaJugador(jugador, map);

            // Verificar colisión con el jugador
            if (jugador->getFila() == fila && jugador->getColumna() == columna) {
                CCLOG("Jugador muerto!");
                Director::getInstance()->end(); // Termina el juego
            }

            // Verificar colisión con bombas
            for (auto bomba : bombas) {
                if (bomba->getFila() == fila && bomba->getColumna() == columna) {
                    CCLOG("Enemigo eliminado por bomba!");
                    eliminar();
                    return;
                }
            }
        }

        void Enemigo::moverHaciaJugador(Jugador * jugador, int map[11][13]) {
            if (!sprite) return;

            int jugadorFila = jugador->getFila();
            int jugadorColumna = jugador->getColumna();

            // Determinar movimiento hacia el jugador
            int nuevaFila = fila;
            int nuevaColumna = columna;
            std::string direccion = "down";

            if (jugadorFila < fila) {
                nuevaFila--;
                direccion = "up";
            }
            else if (jugadorFila > fila) {
                nuevaFila++;
                direccion = "down";
            }

            if (jugadorColumna < columna) {
                nuevaColumna--;
                direccion = "left";
            }
            else if (jugadorColumna > columna) {
                nuevaColumna++;
                direccion = "right";
            }

            // Validar si el movimiento es válido
            if (nuevaFila >= 0 && nuevaFila < 11 && nuevaColumna >= 0 && nuevaColumna < 13 && map[nuevaFila][nuevaColumna] == 0) {
                fila = nuevaFila;
                columna = nuevaColumna;

                // Actualizar sprite según la dirección
                cambiarSprite(direccion);

                // Actualizar la posición en pantalla
                actualizarPosicion();
            }
        }

        void Enemigo::actualizarPosicion() {
            if (!sprite) return;

            Vec2 nuevaPosicion = Vec2(
                offsetX + columna * tileSizeX + tileSizeX / 2,
                offsetY + (10 - fila) * tileSizeY + tileSizeY / 2
            );
            sprite->runAction(MoveTo::create(0.2f, nuevaPosicion));
        }

        void Enemigo::cambiarSprite(const std::string & direccion) {
            if (!sprite) return;

            std::string frameName = "enemy_" + direccion + ".png";
            sprite->setTexture(frameName);
        }

        void Enemigo::eliminar() {
            if (sprite) {
                sprite->removeFromParent();
            }
            sprite = nullptr;
        }
        