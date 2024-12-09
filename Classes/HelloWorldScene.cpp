#include "HelloWorldScene.h"
#include "C:\Users\alanb\OneDrive\Escritorio\AABOMB\proj.win32\Jugador.h"
#include "C:\Users\alanb\OneDrive\Escritorio\AABOMB\proj.win32\bomba.h"

USING_NS_CC;

Scene* HelloWorld::createScene() {
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

bool HelloWorld::init() {
    if (!Scene::init()) return false;

    generarMapa();      // Generar el mapa
    dibujarMapa();      // Dibujar el mapa en pantalla

    // Configurar el jugador
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float tileSizeX = visibleSize.width / 13;
    float tileSizeY = visibleSize.height / 11;
    float offsetX = origin.x + (visibleSize.width - (13 * tileSizeX)) / 2;
    float offsetY = origin.y + (visibleSize.height - (11 * tileSizeY)) / 2;

    jugador = new Jugador(gameLayer, 1, 1, tileSizeX, tileSizeY, offsetX, offsetY);

    inicializarMovimiento(); // Configurar eventos de teclado
    this->scheduleUpdate();  // Llamar a update cada frame
    return true;
}

void HelloWorld::generarMapa() {
    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 13; j++) {
            if (i == 0 || j == 0 || i == 10 || j == 12) {
                map[i][j] = 1; // Pared exterior
            }
            else if (i % 2 == 0 && j % 2 == 0) {
                map[i][j] = 1; // Pared fija
            }
            else {
                map[i][j] = (CCRANDOM_0_1() < 0.4f) ? 2 : 0; // 40% bloque destructible, 60% suelo
            }
        }
    }

    // Asegurar espacio libre para el jugador
    map[1][1] = 0;
    map[1][2] = 0;
    map[2][1] = 0;
}

void HelloWorld::dibujarMapa() {
    gameLayer = Layer::create();
    this->addChild(gameLayer);

    destructibleLayer = Layer::create();
    this->addChild(destructibleLayer, 1); // Capa encima de gameLayer

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    float tileSizeX = visibleSize.width / 13;
    float tileSizeY = visibleSize.height / 11;
    float offsetX = origin.x + (visibleSize.width - (13 * tileSizeX)) / 2;
    float offsetY = origin.y + (visibleSize.height - (11 * tileSizeY)) / 2;

    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 13; j++) {
            std::string spriteName;
            if (map[i][j] == 1) {
                spriteName = "wall.jpeg"; // Pared
            }
            else if (map[i][j] == 0) {
                spriteName = "ground.png"; // Suelo
            }

            auto sprite = Sprite::create(spriteName);
            if (sprite) {
                sprite->setScale(tileSizeX / sprite->getContentSize().width, tileSizeY / sprite->getContentSize().height);
                sprite->setPosition(Vec2(
                    offsetX + j * tileSizeX + tileSizeX / 2,
                    offsetY + (10 - i) * tileSizeY + tileSizeY / 2
                ));
                gameLayer->addChild(sprite, 0);
            }

            // Agregar bloques destructibles en la capa destructibleLayer
            if (map[i][j] == 2) {
                auto destructibleSprite = Sprite::create("box.png");
                destructibleSprite->setScale(tileSizeX / destructibleSprite->getContentSize().width, tileSizeY / destructibleSprite->getContentSize().height);
                destructibleSprite->setPosition(Vec2(
                    offsetX + j * tileSizeX + tileSizeX / 2,
                    offsetY + (10 - i) * tileSizeY + tileSizeY / 2
                ));
                destructibleLayer->addChild(destructibleSprite);
                bloquesDestructibles[std::make_pair(i, j)] = destructibleSprite; // Guardar el sprite para referencia futura
            }
        }
    }
}

void HelloWorld::inicializarMovimiento() {
    auto listener = EventListenerKeyboard::create();

    listener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event) {
        teclasPresionadas[keyCode] = true;

        // Colocar bomba al presionar espacio
        if (keyCode == EventKeyboard::KeyCode::KEY_SPACE) {
            colocarBomba();
        }
        };

    listener->onKeyReleased = [this](EventKeyboard::KeyCode keyCode, Event* event) {
        teclasPresionadas[keyCode] = false;
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}


void HelloWorld::update(float delta) {
    for (auto it = bombas.begin(); it != bombas.end();) {
        auto bomba = *it;
        bomba->actualizar(delta);

        if (bomba->getEstado() == Bomba::Estado::DESAPARECER) {
            map[bomba->getFila()][bomba->getColumna()] = 0; // Actualizar el mapa
            delete bomba; // Eliminar la instancia de la bomba
            it = bombas.erase(it); // Remover del vector
        }
        else {
            ++it;
        }
    }

    manejarMovimiento(); // Actualizar el movimiento del jugador
}





void HelloWorld::manejarMovimiento() {
    int nuevaFila = jugador->getFila();
    int nuevaColumna = jugador->getColumna();

    bool moviendo = false;

    // Detectar teclas presionadas
    if (teclasPresionadas[EventKeyboard::KeyCode::KEY_W]) {
        nuevaFila--;
        moviendo = true;
    }
    if (teclasPresionadas[EventKeyboard::KeyCode::KEY_A]) {
        nuevaColumna--;
        moviendo = true;
    }
    if (teclasPresionadas[EventKeyboard::KeyCode::KEY_S]) {
        nuevaFila++;
        moviendo = true;
    }
    if (teclasPresionadas[EventKeyboard::KeyCode::KEY_D]) {
        nuevaColumna++;
        moviendo = true;
    }

    // Mover al jugador si hay movimiento
    if (moviendo) {
        jugador->mover(nuevaFila, nuevaColumna, map);
    }
    else {
        jugador->detenerAnimacion(); // Detener animación si no hay movimiento
    }
}



void HelloWorld::colocarBomba() {
    if (!jugador) return;

    int fila = jugador->getFila();
    int columna = jugador->getColumna();

    if (map[fila][columna] == 0) { // Solo coloca la bomba en una celda transitable
        map[fila][columna] = 3; // Marcar la celda como ocupada por bomba

        // Crear la bomba con los parámetros adecuados
        auto bomba = new Bomba(gameLayer, fila, columna, tileSizeX, tileSizeY, offsetX, offsetY, map, bloquesDestructibles);
        bombas.push_back(bomba);
    }
}



