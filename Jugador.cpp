#include "Jugador.h"

USING_NS_CC;

Jugador::Jugador(Layer* layer, int filaInicial, int columnaInicial, float tileSizeX, float tileSizeY, float offsetX, float offsetY)
    : fila(filaInicial), columna(columnaInicial), tileSizeX(tileSizeX), tileSizeY(tileSizeY), offsetX(offsetX), offsetY(offsetY), animacionActual(nullptr) {
    // Crear el sprite del jugador
    sprite = Sprite::create("player_walk_down_1.png");
    if (!sprite) {
        CCLOG("Error: No se pudo cargar el sprite del jugador.");
        return;
    }

    // Escalar el sprite para que ocupe un 80% del bloque
    float escalaX = tileSizeX / sprite->getContentSize().width;
    float escalaY = tileSizeY / sprite->getContentSize().height;
    float escalaFinal = std::min(escalaX, escalaY) * 0.8f;
    sprite->setScale(escalaFinal);

    // Posicionar al jugador en la celda inicial
    actualizarPosicion();

    // Agregar el sprite a la capa
    layer->addChild(sprite, 10);
}

Jugador::~Jugador() {
    if (sprite) {
        sprite->removeFromParent();
    }
}

void Jugador::mover(int nuevaFila, int nuevaColumna, int map[11][13]) {
    // Control de tiempo para evitar movimiento excesivamente rápido
    static float tiempoUltimoMovimiento = 0.0f;
    float tiempoActual = Director::getInstance()->getTotalFrames() / 60.0f;

    // Limitar el movimiento a un paso cada 0.2 segundos
    if (tiempoActual - tiempoUltimoMovimiento < 0.2f) {
        return;
    }

    // Determinar la dirección de la animación
    std::string direccionAnimacion;
    if (nuevaFila < fila) {
        direccionAnimacion = "up";
    }
    else if (nuevaFila > fila) {
        direccionAnimacion = "down";
    }
    else if (nuevaColumna < columna) {
        direccionAnimacion = "left";
    }
    else if (nuevaColumna > columna) {
        direccionAnimacion = "right";
    }

    // Validar que el movimiento sea hacia una celda transitable
    if (nuevaFila >= 0 && nuevaFila < 11 &&
        nuevaColumna >= 0 && nuevaColumna < 13 &&
        map[nuevaFila][nuevaColumna] == 0) {

        // Actualizar posición en la matriz
        fila = nuevaFila;
        columna = nuevaColumna;

        // Actualizar la posición en pantalla
        actualizarPosicion();

        // Iniciar la animación correspondiente si existe dirección
        if (!direccionAnimacion.empty()) {
            // Detener animación anterior si existe
            if (animacionActual) {
                sprite->stopAction(animacionActual);
                animacionActual = nullptr;
            }
            animacionActual = crearAnimacion(direccionAnimacion);
            sprite->runAction(animacionActual);
        }
    }
    else {
        // Si no se puede mover, detener la animación
        detenerAnimacion();
    }

    tiempoUltimoMovimiento = tiempoActual;
}

void Jugador::actualizarPosicion() {
    Vec2 nuevaPosicion = Vec2(
        offsetX + columna * tileSizeX + tileSizeX / 2,
        offsetY + (10 - fila) * tileSizeY + tileSizeY / 2
    );

    // Movimiento suave del jugador a la nueva posición
    sprite->runAction(MoveTo::create(0.2f, nuevaPosicion));
}

cocos2d::Action* Jugador::crearAnimacion(const std::string& direccion) {
    Vector<SpriteFrame*> frames;

    for (int i = 1; i <= 3; i++) {
        std::string frameName = "player_walk_" + direccion + "_" + std::to_string(i) + ".png";
        auto frame = SpriteFrame::create(frameName, Rect(0, 0, 16, 30));
        if (frame) {
            frames.pushBack(frame);
        }
        else {
            CCLOG("Error al cargar el frame: %s", frameName.c_str());
        }
    }

    auto animation = Animation::createWithSpriteFrames(frames, 0.1f);
    return RepeatForever::create(Animate::create(animation));
}

void Jugador::detenerAnimacion() {
    if (animacionActual) {
        sprite->stopAction(animacionActual);
        animacionActual = nullptr;
    }
    // Regresar al sprite estático
    sprite->setTexture("player_walk_down_1.png");
}

Sprite* Jugador::getSprite() const {
    return sprite;
}

int Jugador::getFila() const {
    return fila;
}

int Jugador::getColumna() const {
    return columna;
}

