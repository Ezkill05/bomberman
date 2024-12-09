#include "Bomba.h"

USING_NS_CC;

Bomba::Bomba(cocos2d::Layer* layer, int fila, int columna, float tileSizeX, float tileSizeY, float offsetX, float offsetY,
    int (*map)[13], std::map<std::pair<int, int>, cocos2d::Sprite*>& bloquesDestructibles)
    : gameLayer(layer), fila(fila), columna(columna), tileSizeX(tileSizeX), tileSizeY(tileSizeY),
    offsetX(offsetX), offsetY(offsetY), map(map), bloquesDestructibles(bloquesDestructibles),
    estado(Estado::NORMAL), tiempoActual(0.0f), tiempoExplosion(2.0f) {

    // Crear el sprite de la bomba
    sprite = Sprite::create("bomb_1.png");
    if (!sprite) {
        CCLOG("Error: No se pudo cargar el sprite de la bomba.");
        return;
    }

    sprite->setContentSize(Size(tileSizeX, tileSizeY));
    sprite->setPosition(offsetX + columna * tileSizeX + tileSizeX / 2,
        offsetY + (10 - fila) * tileSizeY + tileSizeY / 2);
    layer->addChild(sprite, 10);

    animacionBomba();
}

Bomba::~Bomba() {
    if (sprite) {
        sprite->removeFromParent();
    }
    sprite = nullptr;
}

void Bomba::actualizar(float delta) {
    tiempoActual += delta;
    if (estado == Estado::NORMAL && tiempoActual >= tiempoExplosion) {
        explotar();
    }
}

void Bomba::animacionBomba() {
    Vector<SpriteFrame*> frames;
    for (int i = 1; i <= 3; i++) {
        std::string frameName = "bomb_" + std::to_string(i) + ".png";
        auto frame = SpriteFrame::create(frameName, Rect(0, 0, 20, 20));
        if (frame) {
            frames.pushBack(frame);
        }
        else {
            CCLOG("Error al cargar el frame: %s", frameName.c_str());
        }
    }

    auto animation = Animation::createWithSpriteFrames(frames, 0.3f);
    auto animate = Animate::create(animation);
    sprite->runAction(RepeatForever::create(animate));
}

void Bomba::explotar() {
    estado = Estado::EXPLOSION;
    sprite->stopAllActions();
    animacionExplosion();
}

void Bomba::animacionExplosion() {
    Vector<SpriteFrame*> frames;
    for (int i = 1; i <= 4; i++) {
        std::string frameName = "explotion_" + std::to_string(i) + ".png";
        auto frame = SpriteFrame::create(frameName, Rect(0, 0, 20, 20));
        if (frame) {
            frames.pushBack(frame);
        }
        else {
            CCLOG("Error al cargar el frame: %s", frameName.c_str());
        }
    }

    auto animation = Animation::createWithSpriteFrames(frames, 0.1f);
    auto animate = Animate::create(animation);

    auto remove = CallFunc::create([this]() { desaparecer(); });
    sprite->runAction(Sequence::create(animate, remove, nullptr));
}

void Bomba::desaparecer() {
    estado = Estado::DESAPARECER;
    afectarBloques();

    if (sprite) {
        sprite->removeFromParent();
    }
    sprite = nullptr;
}

void Bomba::afectarBloques() {
    int rango = 2;

    auto destruirBloque = [this](int fila, int columna) {
        if (fila >= 0 && fila < 11 && columna >= 0 && columna < 13) {
            if (map[fila][columna] == 2) {
                map[fila][columna] = 0;

                auto it = bloquesDestructibles.find(std::make_pair(fila, columna));
                if (it != bloquesDestructibles.end()) {
                    it->second->removeFromParent();
                    bloquesDestructibles.erase(it);

                    auto sueloSprite = Sprite::create("ground.png");
                    if (sueloSprite) {
                        sueloSprite->setContentSize(Size(tileSizeX, tileSizeY));
                        sueloSprite->setPosition(Vec2(
                            offsetX + columna * tileSizeX + tileSizeX / 2,
                            offsetY + (10 - fila) * tileSizeY + tileSizeY / 2
                        ));
                        gameLayer->addChild(sueloSprite, 0);
                    }
                }
            }
        }
        };

    for (int i = 1; i <= rango; i++) {
        if (fila - i >= 0 && map[fila - i][columna] != 1) {
            destruirBloque(fila - i, columna);
            if (map[fila - i][columna] == 2) break;
        }
        else break;

        if (fila + i < 11 && map[fila + i][columna] != 1) {
            destruirBloque(fila + i, columna);
            if (map[fila + i][columna] == 2) break;
        }
        else break;

        if (columna - i >= 0 && map[fila][columna - i] != 1) {
            destruirBloque(fila, columna - i);
            if (map[fila][columna - i] == 2) break;
        }
        else break;

        if (columna + i < 13 && map[fila][columna + i] != 1) {
            destruirBloque(fila, columna + i);
            if (map[fila][columna + i] == 2) break;
        }
        else break;
    }
}

Bomba::Estado Bomba::getEstado() const {
    return estado;
}

int Bomba::getFila() const {
    return fila;
}

int Bomba::getColumna() const {
    return columna;
}
