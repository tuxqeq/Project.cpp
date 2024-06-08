#include "Character.h"
#include "Level.h"
#include <fmt/core.h>
//TODO create animation class and handle animations there
Character::Character(float x, float y, float w, float h, std::string name, bool inGame)
        : x(x), y(y), w(w), h(h), name(name), inGame(inGame){
    image.loadFromFile("/Users/tuxqeq/Documents/CLion/Project.cpp/assets/Character/" + name);
    texture.loadFromImage(image);
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, w, h));
    sprite.setPosition(x, y);
    jumpH = 0;
    grdlevel = y;
    heart.loadFromFile("/Users/tuxqeq/Documents/CLion/Project.cpp/assets/Character/humanheart.png");
    hearts = std::vector<sf::Sprite>(5, sf::Sprite(heart));
    for (int i = 0; i < 5; i++){
        hearts[i].setTextureRect(sf::IntRect(0, 0, 64, 64));
        hearts[i].setScale(0.75, 0.75);
    }
}

auto Character::update(float time, sf::Vector2u vector2, sf::RenderWindow* wnd) -> void {
    if(not inGame) sprite.setScale(0.75, 0.75);
    //else sprite.setScale(1, 1);
    x += speed*time;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::R) and not jumping){
        hit = true;
    }
    if(inGame) {
        collisionX(0);
        EnemyCollision();
    }

    y += speedY*time;
    if(inGame) {
        collisionX(1);
    }
    //isOnGround = false;
    jumpH -= speedY*time;
    bool ableToJump = false;
    sprite.setPosition(x - offsetX, y-offsetY - 72);
    if(inGame and x > 200) offsetX = x - 200;
    if(y < 400 and inGame) offsetY = y - 400;
    speed = 0;
    jumpTimer += time;
    hitTimer += time;
    if (x > vector2.x and not inGame){
        x = -45;
    }
    if(x < -45 and not inGame){
        x = vector2.x;
    }
    if(jumping or not isOnGround){
        isOnGround = false;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) or sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) speedY += 0.025f*time;
        speedY += 0.005f*time;
        if(not crawling){
            if (dir == 1) {
                frame += 0.02f * time;
                if (frame > 8) frame -= 8;
                sprite.setTextureRect(sf::IntRect(96 * int(frame) + 96, 672, -96, 96));
            } else {
                frame += 0.02f * time;
                if (frame > 8) frame -= 8;
                sprite.setTextureRect(sf::IntRect(96 * int(frame), 672, 96, 96));
            }
        }
    }

    if(not inGame and jumpH <= 0){
        speedY = 0;
        //y = grdlevel;
        isOnGround = true;
        jumping = false;
    }
    if(isOnGround and jumpTimer > jumpCooldown and not inCave){
        ableToJump = true;
    }

    if((sf::Keyboard::isKeyPressed(sf::Keyboard::Space) or sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        and ableToJump
        and not crawling
        and not inCave){
        speedY = -1;
        jumping = true;
        jumpTimer = 0;

    }
    if((sf::Keyboard::isKeyPressed(sf::Keyboard::Space) or sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
       and crawling
       and not inCave){
        uncrawlingAnim(time, dir);

    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) or sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
        dir = 1;
        speed = -0.4;
        if(isOnGround) {
            frame += 0.02f * time;
            if (frame > 6) frame -= 6;
            sprite.setTextureRect(sf::IntRect(96 * int(frame) + 96, 192, -96, 96));
        }
        if(isOnGround and hit and hitTimer > hitCooldown){
            speed = 0;
            hitframe += 0.02f * time;
            if (hitframe > 4) {
                hitframe -= 4;
                hit = false;
                hitTimer = 0;
            }
            sprite.setTextureRect(sf::IntRect(96 * int(hitframe)+96, 576, -96, 96));
        }
        if(isOnGround and (sf::Keyboard::isKeyPressed(sf::Keyboard::S) or sf::Keyboard::isKeyPressed(sf::Keyboard::Down))){
            crawling = true;
        }
        crawlingAnim(time, dir, isOnGround);

    }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) or sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
        dir = 0;
        speed = 0.4;
        if(isOnGround) {
            frame += 0.02f * time;
            if (frame > 6) frame -= 6;
            sprite.setTextureRect(sf::IntRect(96 * int(frame), 192, 96, 96));
        }
        if(isOnGround and hit and hitTimer > hitCooldown){
            speed = 0;
            hitframe += 0.02f * time;
            if (hitframe > 4) {
                hitframe -= 4;
                hit = false;
                hitTimer = 0;
            }
            sprite.setTextureRect(sf::IntRect(96 * int(hitframe), 576, 96, 96));
        }
        if(isOnGround and (sf::Keyboard::isKeyPressed(sf::Keyboard::S) or sf::Keyboard::isKeyPressed(sf::Keyboard::Down))){
            crawling = true;
        }
        crawlingAnim(time, dir, isOnGround);
    }else if(not jumping and not inCave){
        frame += 0.009f * time;
        if(frame > 6) frame -= 6;
        if(dir == 1) sprite.setTextureRect(sf::IntRect(96*int(frame) + 96, 0, -96, 96));
        else sprite.setTextureRect(sf::IntRect(96*int(frame), 0, 96, 96));
        if(hit and hitTimer > hitCooldown){
            speed = 0;
            hitframe += 0.02f * time;
            if (hitframe > 4) {
                hitframe -= 4;
                hit = false;
                hitTimer = 0;
            }
            if(dir == 0) sprite.setTextureRect(sf::IntRect(96 * int(hitframe), 576, 96, 96));
            if(dir == 1) sprite.setTextureRect(sf::IntRect(96 * int(hitframe)+96, 576, -96, 96));
        }
        if(isOnGround and (sf::Keyboard::isKeyPressed(sf::Keyboard::S) or sf::Keyboard::isKeyPressed(sf::Keyboard::Down))){
            crawling = true;
        }
        crawlingAnim(time, dir, isOnGround);
    }
}

auto Character::setPosition(float x, float y) -> void {
    sprite.setPosition(x, y);
    this->x = x;
    this->y = y;
    grdlevel = y;
    isOnGround = true;
    jumpH = 0;
}

auto Character::crawlingAnim(float time, int dir, bool isOnGround) -> void {
    if(isOnGround and /*(sf::Keyboard::isKeyPressed(sf::Keyboard::S) or sf::Keyboard::isKeyPressed(sf::Keyboard::Down) or */crawling){
        speed = speed/2;
        crawling = true;
        if(crawlFrame < 5){
            crawlFrame += 0.05f * time;
        }
        if(dir == 0) sprite.setTextureRect(sf::IntRect(96 * int(crawlFrame), 288, 96, 96));
        else if(dir == 1) sprite.setTextureRect(sf::IntRect(96 * int(crawlFrame) + 96, 288, -96, 96));
    }
}

auto Character::uncrawlingAnim(float time, int dir) -> void {
    crawlFrame += 0.05f * time;
    while (crawlFrame < 10) {
        crawlFrame += 0.05f * time;
    }
    crawling = false;
    crawlFrame -= 9;
    if(dir == 0) sprite.setTextureRect(sf::IntRect(96 * int(crawlFrame), 288, 96, 96));
    else if(dir == 1) sprite.setTextureRect(sf::IntRect(96 * int(crawlFrame) + 96, 288, -96, 96));
}

auto Character::collisionX(int num) -> void {
    if(clevel->curlevel[(y)/48][(x+24)/48] == 'q'){
        //crawling = true;
        inCave = true;
    }
    if(clevel->curlevel[(y)/48][(x+24)/48] != 'q'){
        inCave = false;
    }
    for (int k = (x)/48; k < (x + 48)/48; k++){
        if(k >= (x)/48 and k <= (x + 24)/48) {
            if (clevel->curlevel[int(y + 48) / 48][k] == ' ' or clevel->curlevel[int(y + 48) / 48][k] == 's') {
                if (speedY == 0 and num == 1) {
                    isOnGround = false;
                }
            }
        }
        if(k >= (x)/48 and k < (x + 48)/48) {
            if (clevel->curlevel[int(y + 16) / 48][k] == 's') {
                health -= 1;
                offsetX = 0;
                x = 75;
                y = 720;
                minusheart = true;
                crawling = false;
            }
        }
    }
    for (int i = y/48; i < (y + 48)/48; i++) {
        for (int j = (x + 24)/48; j < (x + 48)/48; j++) {
            if(clevel->curlevel[i][j] == '0'
            or clevel->curlevel[i][j] == 'd'
            or clevel->curlevel[i][j] == 'C'
            or clevel->curlevel[i][j] == 'b'
            or clevel->curlevel[i][j] == 'w'){
                if(speedY > 0 and num == 1) {
                    isOnGround = true;
                    speedY = 0;
                    fmt::print("{}\n", i);
                    jumping = false;
                    y = i*48 - 48;
                    fmt::print("{}\n", y);

                    //jumpH = 0;
                }
                if (speedY < 0 and num == 1) {
                    y = i * 48 + 48;
                }
                if(speed > 0 and num == 0) {
                    if(hit){
                        if(clevel->curlevel[i-1][j] == 'b'){
                            clevel->curlevel[i][j] = 'q';
                            hit = false;
                        }if(clevel->curlevel[i-1][j] == ' '){
                            clevel->curlevel[i][j] = ' ';
                            hit = false;
                        }
                    }
                    x = j*48 - 48;
                }
                if(speed < 0 and num == 0) {
                    if(hit){
                        if(clevel->curlevel[i-1][j] == 'b'){
                            clevel->curlevel[i][j] = 'q';
                            hit = false;
                        }if(clevel->curlevel[i-1][j] == ' '){
                            clevel->curlevel[i][j] = ' ';
                            hit = false;
                        }
                    }
                    x = j*48 + 24;
                }
            }
            if(clevel->curlevel[i][j] == 'q'){
                if(speed > 0){
                    if(not crawling){
                        x = j*48 - 48;
                    }
                }
                if(speed < 0){
                    if(not crawling){
                        x = j*48 + 24;
                    }
                }
            }
        }
    }
}

auto Character::getOffsetXY() -> std::pair<float, float>{
    return std::make_pair(offsetX, offsetY);
}

auto Character::drawhealth(sf::RenderWindow* wnd) -> void {
    for (int i = 0; i < health; ++i) {
        hearts[i].setPosition(i * 32, 0);
        hearts[i].setScale(0.5, 0.5);
        wnd->draw(hearts[i]);
    }
}

auto Character::EnemyCollision() -> void {
    for (auto i : clevel->enemies){
        auto spritechar = sprite.getGlobalBounds();
        auto spritecharForhit = sprite.getGlobalBounds();
        auto enemychar = i->sprite.getGlobalBounds();
        auto enemycharForhit = i->sprite.getGlobalBounds();
        spritecharForhit.width = 72;
        spritecharForhit.height = 72;

        spritechar.width = 15;
        spritechar.height = 48;
        spritechar.left += 10;

        enemychar.width = 20;
        enemychar.height = 10;
        enemychar.left -= 5;
        if (spritecharForhit.intersects(enemycharForhit) and hit) {
            hit = false;
            i->enemyhit();
        }
        if (spritechar.intersects(enemychar)) {

            if (speedY > 0) {
                speedY = -1;
                jumping = true;
                jumpTimer = 0;
                i->enemyhit();
            } else {
                health -= 1;
                //x = x - 100;
                x = speed >= 0 ? x - 100 : x + 100;
                fmt::println("{}", health);
            }

        }
    }
}


