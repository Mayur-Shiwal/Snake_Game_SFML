#include <SFML/Graphics.hpp>
#include <time.h>
#include <sstream>

using namespace sf;

int N = 40, M = 30;
int size = 14;
int w = size * N;
int h = size * M;
float timer = 0, delay = 0.1;
int dir, snakeSize = 2, score = 0, highestScore = 0;

struct Snake{
    int x, y;
}   s[100];

struct Fruit{
    int x, y;
}   f;

std::string to_string(int num) {
    std::stringstream ss;
    ss << num;
    return ss.str();
}

void update(){
    for (int i = snakeSize; i > 0; --i){
        s[i].x = s[i - 1].x;
        s[i].y = s[i - 1].y;
    }

    if (dir == 0) s[0].y += 1;
    if (dir == 1) s[0].x -= 1;
    if (dir == 2) s[0].x += 1;
    if (dir == 3) s[0].y -= 1;

    if ((s[0].x == f.x) && (s[0].y == f.y)){
        snakeSize++;
        score++;
        if (score > highestScore) highestScore = score;
        if (score % 5 == 0 && score != 0) delay -= 0.01;
        f.x = rand() % (N - 1);
        f.y = rand() % (M - 1);
        if (f.x >= 0 && f.x <= 3) f.x = 4;
        if (f.y >= 0 && f.y <= 4) f.y = 4;
    }

    if (s[0].x > N-1) s[0].x = 0;
    if (s[0].x < 0) s[0].x = N-1;
    if (s[0].y > M-1) s[0].y = 0;  
    if (s[0].y < 0) s[0].y = M-1;

    for (int i = 1; i < snakeSize; i++) {
        if (s[0].x == s[i].x && s[0].y == s[i].y) {
            snakeSize = 2;
            score = 0;
            delay = 0.1;
        }
    }
}

int main(){
    srand(time(0));

    RenderWindow window(VideoMode(w, h), "Snake Game!");

    Texture t1, t2, t3;
    t1.loadFromFile("images/white.png");
    t2.loadFromFile("images/purple.png");
    t3.loadFromFile("images/green.png");

    Sprite sprite1(t1);
    Sprite sprite2(t2);
    Sprite sprite3(t3);

    Font font;
    font.loadFromFile("images/arial.ttf");

    Text scoreText, HighestScoreText;
    scoreText.setFont(font);
    scoreText.setFillColor(Color::Black);
    scoreText.setPosition(10, 10);
    scoreText.setCharacterSize(18);
    scoreText.setString("Current Score: 0");

    HighestScoreText.setFont(font);
    HighestScoreText.setFillColor(Color::Black);
    HighestScoreText.setPosition(400, 10);
    HighestScoreText.setCharacterSize(18);
    HighestScoreText.setString("Highest Score: 0");

    Clock clock;

    f.x = rand() % (N - 1);
    f.y = rand() % (M - 1);

    while (window.isOpen()){
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        Event e;
        while (window.pollEvent(e)){
            if (e.type == Event::Closed) window.close();
        }

        if (Keyboard::isKeyPressed(Keyboard::Left) && dir != 2) dir = 1;
        if (Keyboard::isKeyPressed(Keyboard::Right) && dir != 1) dir = 2;
        if (Keyboard::isKeyPressed(Keyboard::Up) && dir != 0) dir = 3;
        if (Keyboard::isKeyPressed(Keyboard::Down) && dir != 3) dir = 0;

        if (timer > delay){ 
            timer = 0; 
            update();
        }

        scoreText.setString("Current Score: " + to_string(score));
        HighestScoreText.setString("Highest Score: " + to_string(highestScore));

        /////// Draw ///////
        window.clear();

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                sprite1.setPosition(i * size, j * size);  
                window.draw(sprite1);
            }
        }

        for (int i = 0; i < snakeSize; i++){
            sprite2.setPosition(s[i].x * size, s[i].y * size);  
            window.draw(sprite2);
        }
        
        window.draw(scoreText);
        window.draw(HighestScoreText);

        sprite3.setPosition(f.x * size, f.y * size);  
        window.draw(sprite3);

        window.display();
    }
    return 0;
}
