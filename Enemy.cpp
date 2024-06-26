#include "Enemy.h"

Enemy::Enemy(std::string name, float speed, float x, float y, int health) : speed(speed), x(x), y(y), health(health){
    texture.loadFromFile("../assets/Enemies/" + name);
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    sprite.setScale(2, 2);
    life = true;
}


auto Enemy::setText(std::string name) -> void {
    texture.loadFromFile("../assets/Enemies/" + name);
    sprite.setTexture(texture);
}

auto Enemy::update(float time) -> void {
    x += speed * time;
    road += std::abs(speed) * time;
    if(road > maxlen) {
        speed = -speed;
        road = 0;
    }
    deadtimer += time * 0.009;
    curFrame += time * 0.009;
    if (curFrame > 4) curFrame-= 4;
    sprite.setTextureRect(sf::IntRect(24*int(curFrame), 0, 24, 24));
    if (hit) {
        deadtimer = 0;
        hit = false;
    }
    if(deadtimer < 2){
        x-=speed*time;
        road -= std::abs(speed) * time;
        sprite.setTextureRect(sf::IntRect(48, 48, 24, 24));
    }
}

auto Enemy::draw(sf::RenderWindow* wnd) -> void {
    wnd->draw(sprite);
}

auto Enemy::setPosition(float x, float y) -> void {
    sprite.setPosition(x, y - 48);
    this->x = x;
    this->y = y;

}

auto Enemy::setOffset(std::pair<float, float> pair) -> void {
    sprite.setPosition(x - pair.first, y-pair.second);
}


auto Enemy::enemyhit() -> void {
    hit = true;
    health -= 1;
    fmt::println("{}", health);
    if(health == 0){
        life = false;
        deadtimer = 0;
        x = 10000;
        y = 10000;
    }
}





