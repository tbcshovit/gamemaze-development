#include <SFML/Graphics.hpp> // helps us use graphics 
#include <iostream> // it allows do input and output in console 
#include <ctime> // this helps us to generate seed for random number 
#include <cstdlib> // this helps us to use rand() and srand()
#include <fstream>
#include <string>
using namespace std;

const int SIZE = 15;
const int TILE_SIZE = 60;

class Entity {
public:
    int x, y; // corrdinates of entity 
    char symbol;  // its symbol representating the entity

    Entity(int startX = 1, int startY = 1, char sym = ' ') { // its a constructor that gives the default value 
        x = startX;
        y = startY;
        symbol = sym;
    }

    virtual void placeOn(char maze[SIZE][SIZE]) { // points the symbol to be placed on which grid
        maze[x][y] = symbol;
    }
};

class Player : public Entity {
public:
    Player() : Entity(1, 1, 'P') {} // gives default value of player where it resides on 1,1 cordinate

    void move(int dx, int dy, char maze[SIZE][SIZE], int& score, int& moves, bool& gameOver, bool& levelUp) {
        int newX = x + dx; // shows new cordinate
        int newY = y + dy;

        if (maze[newX][newY] == '█') return; 

        if (maze[newX][newY] == '*') score += 10; // gets score 
        else if (maze[newX][newY] == 'X') { // enemy 
            cout << "ohh nooo you died !Game Over.\n"; 
            gameOver = true;
            return;
        }
        else if (maze[newX][newY] == 'E') { //exit door 
            cout << "Level Complete!\n";
            score += 50;
            levelUp = true;
            return;
        }

        maze[x][y] = ' '; // previous position of the player is replaced by blank
        x = newX;
        y = newY;
        maze[x][y] = symbol;
        moves++;
    }
};

class Enemy : public Entity {
public:
    Enemy() : Entity(6, 6, 'X') {} // corrdinates of the enemy initialize

    void move(char maze[SIZE][SIZE]) { // random motion of enemy
        int dx[] = { 1, -1, 0, 0 }; // array showing possible direction of enemy
        int dy[] = { 0, 0, 1, -1 };

        int dir = rand() % 4; // generates the random value from 0 to 3
        int nx = x + dx[dir]; // new cordinates of the enemy
        int ny = y + dy[dir];

        if (maze[nx][ny] == ' ' || maze[nx][ny] == 'P') { // possible direction of enemy where it  can go
            maze[x][y] = ' ';
            x = nx;
            y = ny;
            maze[x][y] = symbol;
        }
    }
};


class Game { // this holds the features of game like enemy,player,score,levels.
public:
    char maze[SIZE][SIZE];
    int level = 1;
    int score = 0;
    int moves = 0;

    Player player;
    Enemy enemy;

    void generateMaze() {
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                maze[i][j] = (i == 0 || j == 0 || i == SIZE - 1 || j == SIZE - 1) ? '█' : ' '; // generates boundry-wall and blank space

        for (int i = 6; i < SIZE - 6; i++) {
            maze[1][i] = '█'; //1,6 (1,7),(1,8) wall
            maze[i][8] = '█';
            maze[6][i] = '█';
        }
        for (int i = 2; i < SIZE - 10; i++) {
            maze[i][6] = '█';
            maze[i][8] = '█';
        }
        for (int i = 2; i < SIZE - 10; i++) {
            maze[11][i] = '█';
        }

        maze[SIZE - 2][SIZE - 2] = 'E'; // exit door at corrdinate 13*13
        maze[3][3] = '*'; // coordinate of collectables
        maze[7][6] = '*';
        maze[11][11] = '*';

        player = Player(); // to place player according to coordinate
        enemy = Enemy();   // to place enemy according to coordinate
        

        player.placeOn(maze); // placing the player (p) on maze
        enemy.placeOn(maze);  // placing the enemy (x) on maze
     }

    void generateMaze2() { // generates the maze for level 2
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                maze[i][j] = (i == 0 || j == 0 || i == SIZE - 1 || j == SIZE - 1) ? '█' : ' ';

        for (int i = 6; i < SIZE - 6; i++) {
            maze[1][i] = '█';
            maze[i][8] = '█';
            maze[6][i] = '█';
        }
        for (int i = 2; i < SIZE - 10; i++) {
            maze[i][6] = '█';
            maze[i][8] = '█';
        }
        for (int i = 2; i < SIZE - 9; i++) {
            maze[11][i] = '█';
        }
        for (int i = 3; i < SIZE; i++) {
            maze[i][11] = '█';
        }

        maze[SIZE - 2][SIZE - 2] = 'E';
        maze[3][3] = '*';
        maze[7][6] = '*';
        maze[11][11] = '*';

        player = Player();
        enemy = Enemy();
        player.placeOn(maze);
        enemy.placeOn(maze);
    }

    void saveGame() { // to save the game 
        ofstream out("savegame.txt"); // writes the info about the game in txt file 
        out << level << ' ' << score << ' ' << moves << '\n'; //writes the level, score and moves in the saved file
        out << player.x << ' ' << player.y << '\n';
        out << enemy.x << ' ' << enemy.y << '\n';
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j)
                out << maze[i][j]; // it writes the location of walls and collectables
            out << '\n';
        }
    }

    bool loadGame() { // to load the saved game
        ifstream in("savegame.txt"); // it reads the saved file 
        if (!in.is_open()) return false; // if cant be open return false

        in >> level >> score >> moves;
        in >> player.x >> player.y;
        in >> enemy.x >> enemy.y;
        string line; // This declares a string to temporarily hold each line of the maze from the file.

        getline(in, line); // this reads the line turn by turn
        for (int i = 0; i < SIZE; ++i) { // reads the evey line of the row cordinate
            getline(in, line);
            for (int j = 0; j < SIZE; ++j) // reads the every line of the column(character) cordinate
                maze[i][j] = line[j]; // symbols and charecter on the line will be placed on maze in (i,j) coordinates
        }
        return true;
    }

    void draw(sf::RenderWindow& window, sf::Font& font) { // draw is a function helps us to create window using sfml
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                sf::RectangleShape tile(sf::Vector2f(TILE_SIZE - 2, TILE_SIZE - 2)); // vector2f holds the size value of rectangle
                tile.setPosition(j * TILE_SIZE, i * TILE_SIZE); // this positions the tile 

                // coloring the entities of the maze
                if (maze[i][j] == '█') tile.setFillColor(sf::Color::Magenta);
                else if (maze[i][j] == 'P') tile.setFillColor(sf::Color::Blue);
                else if (maze[i][j] == '*') tile.setFillColor(sf::Color::Yellow);
                else if (maze[i][j] == 'X') tile.setFillColor(sf::Color::Red);
                else if (maze[i][j] == 'E') tile.setFillColor(sf::Color::White);
                else tile.setFillColor(sf::Color::Black);

                window.draw(tile);
            }
        }

        sf::Text info; // it give the game info like level, score moves
        info.setFont(font);
        info.setCharacterSize(20);
        info.setFillColor(sf::Color::White);
        info.setPosition(10, SIZE * TILE_SIZE + 5); 
        info.setString("Level: " + to_string(level) + "  Score: " + to_string(score) + "  Moves: " + to_string(moves));
        window.draw(info);
    }
};

int main() {
    srand(time(0)); // genretae the random number everytime 

    sf::RenderWindow window(sf::VideoMode(SIZE * TILE_SIZE, SIZE * TILE_SIZE + 50), "Maze Game - By Shovit Adhikari"); // it gives the resolation of the window and create and manage the window
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        cout << "Font loading failed.\n";
        return 1;
    }

    Game game;
    bool gameStarted = false;

    while (window.isOpen() && !gameStarted) { // funtionality of the main screen
        sf::Event event;
        while (window.pollEvent(event)) { // gives us option or poll to choose
            if (event.type == sf::Event::Closed) // for closing the game window in the case of even closed like game over or clicking 'x'etc
                window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num1) { // if choose '1' generate maze
                game.generateMaze();
                gameStarted = true;
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num2) { // load the game if chooseed '2' 
                if (!game.loadGame()) {
                    cout << "No saved game found.\n";
                }
                else {
                    gameStarted = true; 
                }
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) // game closed if choosed 'Esc'
                window.close();
        }

        window.clear(sf::Color::Black);

        sf::Text title("Maze Game", font, 30); // font writing with color
        title.setFillColor(sf::Color::Cyan);
        title.setPosition(100, 100);

        sf::Text start("Press 1 to Start New Game", font, 20);
        start.setFillColor(sf::Color::Green);
        start.setPosition(100, 180); // these are the coordinates 

        sf::Text cont("Press 2 to Continue", font, 20);
        cont.setFillColor(sf::Color::Yellow);
        cont.setPosition(100, 220); // these are coordinates 

        sf::Text quit("Press ESC to Quit", font, 20);
        quit.setFillColor(sf::Color::Red);
        quit.setPosition(100, 260); // these are coordinates

        // renders the below in screen
        window.draw(title);
        window.draw(start);
        window.draw(cont);
        window.draw(quit);
        window.display();
    }

    if (!gameStarted) return 0;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        bool gameOver = false;
        bool levelUp = false;

        // these are the corrdinates of the player when pressed the WSAD button
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) game.player.move(-1, 0, game.maze, game.score, game.moves, gameOver, levelUp);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) game.player.move(1, 0, game.maze, game.score, game.moves, gameOver, levelUp);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) game.player.move(0, -1, game.maze, game.score, game.moves, gameOver, levelUp);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) game.player.move(0, 1, game.maze, game.score, game.moves, gameOver, levelUp);

        
        if (gameOver) {
            game.saveGame();
            window.close();
        }

        //level up condition
        if (levelUp) {
            game.level++;
            if (game.level < 3)
            {
                game.saveGame();
                game.generateMaze2();
            }
            else {
                cout << "CONGRATULATIONS YOU DID IT YEAHHHH !Game Over.\n";
                game.saveGame();
                window.close();
            }
        }

        else {
            game.enemy.move(game.maze);
            if (game.player.x == game.enemy.x && game.player.y == game.enemy.y) {  // game over if playe and enemy have the same coordination
                cout << "ohh nooo you died !Game Over.\n";
                game.saveGame();
                window.close();
            }
            else if (game.level == 2 && game.player.x == SIZE - 2 && game.player.y == SIZE - 2) { // if player and exit position of level 2 is same game over
                cout << "CONGRATULATIONS YOU DID IT YEAHHHH !Game Over.\n";
                game.saveGame();
                window.close();
            }
        }

        window.clear(sf::Color::Black);
        game.draw(window, font);
        window.display();
        sf::sleep(sf::milliseconds(150)); // it pauses the game loop for 150 milliseconds before the next frame is drawn
    }

    return 0;
}