#include <iostream>
#include <SFML/Graphics.hpp>
#include <X11/Xlib.h>
#include <thread>
#include <unistd.h>
#include <sys/wait.h>
#include <SFML/System/Clock.hpp>
#include <cstdlib>
#include <climits>
#include <vector>

// g++ Q.cpp -o run -lsfml-graphics -lsfml-window -lsfml-system -lX11 -pthread
// to run write this
using namespace sf;
using namespace std;

int Score_player_1 = 0;
int Score_player_2 = 0;
struct ThreadArgs
{
    int playerId;
    Sprite *characterSprite;
    RenderWindow *window;
    bool *running;
};

struct player
{
    string player1;
    string player2;
    int *Sp1;
    int *Sp2;
};

void show_pause()
{
    // Create the window
    RenderWindow window(VideoMode(400, 300), "PAUSED");

    // Create the text
    Text pauseText;
    pauseText.setString("Game Paused");
    pauseText.setCharacterSize(24);
    pauseText.setFillColor(Color::Black);
    pauseText.setPosition(100, 100);

    // Main event loop
    while (window.isOpen())
    {
        // Handle events
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
                exit(1);
            }
        }

        // Draw the window
        window.clear(Color::White);
        window.draw(pauseText);
        window.display();
    }
}

int generateBoardSize(int rollNum)
{
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    srand(time(NULL));                                                 // seed the random number generator with the current time
    int randomNum = rand() % 90 + 10;                                  // generates a random number between 10-99
    int generatedNum = randomNum * (rollNum % 10);                     // multiply by the last digit of the roll number
    float divisionResult = static_cast<float>(rollNum) / generatedNum; // divide roll number by generated number
    int boardSize = static_cast<int>(divisionResult) % 25;             // take mod with 25
    if (boardSize < 10)
    {
        boardSize += 15; // add 15 if the result is less than 10
    }
    return boardSize;
}

void handle_events(RenderWindow &window)
{
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case Event::Closed:
            window.close();
            break;
        }
    }
}

void *move_character_sprite(void *args)
{
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

    ThreadArgs *tArgs = static_cast<ThreadArgs *>(args);
    RenderWindow *window = tArgs->window;
    Sprite *characterSprite = tArgs->characterSprite;

    bool *runnin = tArgs->running;

    float speed = 0.04f;
    float minDistance = 0.01f;
    while (*runnin)
    {
        Vector2f currentPosition = characterSprite->getPosition();
        if (tArgs->playerId == 1)
        {
            if (Keyboard::isKeyPressed(Keyboard::Left))
            {
                if (characterSprite->getPosition().x - speed >= 0)
                {
                    characterSprite->move(-speed, 0.f);
                }
            }
            if (Keyboard::isKeyPressed(Keyboard::Right))
            {
                if (characterSprite->getPosition().x + characterSprite->getGlobalBounds().width + speed <= window->getSize().x)
                {
                    characterSprite->move(speed, 0.f);
                }
            }
            if (Keyboard::isKeyPressed(Keyboard::Up))
            {
                if (characterSprite->getPosition().y - speed >= 0)
                {
                    characterSprite->move(0.f, -speed);
                }
            }
            if (Keyboard::isKeyPressed(Keyboard::Down))
            {
                if (characterSprite->getPosition().y + characterSprite->getGlobalBounds().height + speed <= window->getSize().y)
                {
                    characterSprite->move(0.f, speed);
                }
            }
            if (Keyboard::isKeyPressed(Keyboard::Escape))
            {
                exit(1);
            }
        }
        else if (tArgs->playerId == 2)
        {
            if (Keyboard::isKeyPressed(Keyboard::A))
            {
                if (characterSprite->getPosition().x - speed >= 0)
                {
                    characterSprite->move(-speed, 0.f);
                }
            }
            if (Keyboard::isKeyPressed(Keyboard::D))
            {
                if (characterSprite->getPosition().x + characterSprite->getGlobalBounds().width + speed <= window->getSize().x)
                {
                    characterSprite->move(speed, 0.f);
                }
            }
            if (Keyboard::isKeyPressed(Keyboard::W))
            {
                if (characterSprite->getPosition().y - speed >= 0)
                {
                    characterSprite->move(0.f, -speed);
                }
            }
            if (Keyboard::isKeyPressed(Keyboard::S))
            {
                if (characterSprite->getPosition().y + characterSprite->getGlobalBounds().height + speed <= window->getSize().y)
                {
                    characterSprite->move(0.f, speed);
                }
            }
            if (Keyboard::isKeyPressed(Keyboard::Escape))
            {
                exit(1);
            }
        }
    }

    return NULL;
}

void draw(RenderWindow &window, Sprite &characterSprite1, Sprite &characterSprite2, int rows, int cols, int gridSize, bool **grid, int timeRemaining, sf::Sprite &obj)
{
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    window.clear(Color::Transparent);

    // draw the grid cells
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            RectangleShape cell(Vector2f(gridSize, gridSize));
            cell.setPosition(j * gridSize, i * gridSize);
            if (grid[i][j])
            {
                cell.setFillColor(Color::Black);
            }
            else
            {
                cell.setFillColor(Color::White);
            }
            window.draw(cell);
        }
    }

    static Clock clock;            // create a static clock to keep track of elapsed time
    static Sprite randomSprite[3]; // create a static sprite for the random image
    static bool spriteVisible[3];  // keep track of whether the sprite is currently visible
    for (int i = 0; i < 3; i++)
    {
        randomSprite[i].setTexture(*obj.getTexture());
        randomSprite[i].setScale(0.05f, 0.05f);
    }
    string imageFilename;
    // randomly display the sprite for 5 seconds
    if (clock.getElapsedTime().asSeconds() > 5.f)
    {
        for (int i = 0; i < 3; i++)
        {
            spriteVisible[i] = false; // hide the sprite after 5 seconds
        }

        clock.restart(); // reset the clock
        for (int i = 0; i < 3; i++)
        {
            srand(time(NULL));
            int randomImageIndex = rand() % 5 + 1; // randomly select an image index between 1-5
            imageFilename = to_string(randomImageIndex) + ".png";
            Texture temp;
            temp.loadFromFile(imageFilename);
            randomSprite[i].setTexture(temp);
        }
    }

    // if the sprite is not visible, randomly set its position and show it
    for (int i = 0; i < 3; i++)
    {
        if (!spriteVisible[i])
        {
            for (int i = 0; i < 3; i++)
            {
                int randomX = rand() % (int)(window.getSize().x - randomSprite[i].getGlobalBounds().width);
                int randomY = rand() % (int)(window.getSize().y - randomSprite[i].getGlobalBounds().height);
                randomSprite[i].setPosition(randomX, randomY);
                spriteVisible[i] = true;
            }
        }
    }
    for (int i = 0; i < 3; i++)
    {
        if (characterSprite1.getGlobalBounds().intersects(randomSprite[i].getGlobalBounds()))
        {
            Score_player_1++;         // increase player 1 score
            spriteVisible[i] = false; // hide the object sprite
            clock.restart();
        }

        if (characterSprite2.getGlobalBounds().intersects(randomSprite[i].getGlobalBounds()))
        {
            Score_player_2++;         // increase player 2 score
            spriteVisible[i] = false; // hide the object sprite
            clock.restart();
        }
    }
    for (int i = 0; i < 3; i++)
    {
        if (spriteVisible[i])
        {
            window.draw(randomSprite[i]);
        }
    }
    // Draw the character sprites
    window.draw(characterSprite1);
    window.draw(characterSprite2);

    window.display(); // display the contents of the window
    window.setView(window.getDefaultView());
}

void get_player_names(string &player1Name, string &player2Name)
{
    // Create the window
    RenderWindow window(VideoMode(400, 300), "Enter player names");

    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("Name.jpeg"))
    {
        cout << "Background image not found" << endl;
        exit(1);
    }

    // Create the background sprite
    Sprite backgroundSprite(backgroundTexture);

    sf::Font font;
    font.loadFromFile("GAB.ttf");
    // Create the input fields
    Text player1Text;
    player1Text.setString("Player 1 name: ");
    player1Text.setFont(font);
    player1Text.setCharacterSize(24);
    player1Text.setFillColor(Color::Black);
    player1Text.setPosition(50, 50);

    Text player2Text;
    player2Text.setString("Player 2 name: ");
    player2Text.setFont(font);
    player2Text.setCharacterSize(24);
    player2Text.setFillColor(Color::Black);
    player2Text.setPosition(50, 120);

    RectangleShape player1InputBox(Vector2f(200, 30));
    player1InputBox.setPosition(50, 80);
    player1InputBox.setFillColor(Color::White);
    player1InputBox.setOutlineThickness(2);
    player1InputBox.setOutlineColor(Color::Black);

    RectangleShape player2InputBox(Vector2f(200, 30));
    player2InputBox.setPosition(50, 150);
    player2InputBox.setFillColor(Color::White);
    player2InputBox.setOutlineThickness(2);
    player2InputBox.setOutlineColor(Color::Black);

    Text player1Input;
    player1Input.setFont(font);
    player1Input.setCharacterSize(24);
    player1Input.setFillColor(Color::Black);
    player1Input.setPosition(60, 80);

    Text player2Input;
    player2Input.setFont(font);
    player2Input.setCharacterSize(24);
    player2Input.setFillColor(Color::Black);
    player2Input.setPosition(60, 150);

    // Main event loop
    while (window.isOpen())
    {
        // Handle events
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
                exit(1);
            }
            else if (event.type == Event::TextEntered)
            {
                if (event.text.unicode == 13) // Enter key pressed
                {
                    if (player1Input.getString().getSize() > 0 && player2Input.getString().getSize() > 0)
                    {
                        player1Name = player1Input.getString();
                        player2Name = player2Input.getString();
                        window.close();
                    }
                }
                else if (event.text.unicode == 8) // Backspace key pressed
                {
                    if (player1InputBox.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
                    {
                        player1Input.setString(player1Input.getString().substring(0, player1Input.getString().getSize() - 1));
                    }
                    else if (player2InputBox.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
                    {
                        player2Input.setString(player2Input.getString().substring(0, player2Input.getString().getSize() - 1));
                    }
                }
                else if (event.text.unicode < 128) // ASCII character entered
                {
                    if (player1InputBox.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
                    {
                        player1Input.setString(player1Input.getString() + static_cast<char>(event.text.unicode));
                    }
                    else if (player2InputBox.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
                    {
                        player2Input.setString(player2Input.getString() + static_cast<char>(event.text.unicode));
                    }
                }
            }
            else if (event.type == Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == Mouse::Left)
                {
                    if (player1InputBox.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
                    {
                        player1Input.setFillColor(Color::Black);
                        player2Input.setFillColor(Color::Black);
                    }
                    else if (player2InputBox.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
                    {
                        player1Input.setFillColor(Color::Black);
                        player2Input.setFillColor(Color::Black);
                    }
                }
            }
        }

        // Draw the window
        window.clear(Color::White);
        window.draw(backgroundSprite);
        window.draw(player1Text);
        window.draw(player2Text);
        window.draw(player1InputBox);
        window.draw(player2InputBox);
        window.draw(player1Input);
        window.draw(player2Input);
        window.display();
    }
    player1Name = player1Name;
    player2Name = player2Name;
}

void *show_scores(void *arg)
{
    player *obj = static_cast<player *>(arg);
    int Score_player_1 = *(obj->Sp1);
    int Score_player_2 = *(obj->Sp2);
    RenderWindow window(VideoMode(500, 300), "SCORE BOARD");

    Texture character1;
    Texture character2;
    Texture backgroundTexture;
    if (!character1.loadFromFile("Character1.jpeg"))
    {
        cout << "Image not found" << endl;
        exit(1);
    }
    if (!character2.loadFromFile("Character2.jpg"))
    {
        cout << "Image not found" << endl;
        exit(1);
    }

    if (!backgroundTexture.loadFromFile("infoBackground.jpeg"))
    {
        cout << "Background image not found" << endl;
        exit(1);
    }

    Sprite backgroundSprite(backgroundTexture);
    backgroundSprite.setScale(1.f, 1.f);
    backgroundSprite.setPosition(0, 0);

    Sprite characterSprite1(character1);
    Sprite characterSprite2(character2);
    characterSprite1.setScale(1.f, 1.f);
    characterSprite2.setScale(.26f, .25f);
    characterSprite1.setPosition(0, 0);
    characterSprite2.setPosition(0, 170);

    Text player1Text;
    sf::Font font;
    if (!font.loadFromFile("GAB.ttf"))
    {
        exit(1);
    }
    player1Text.setString(obj->player1 + ": " + to_string(Score_player_1));
    player1Text.setFont(font);
    player1Text.setCharacterSize(24);
    player1Text.setFillColor(Color::Blue);
    player1Text.setPosition(200, 50);

    Text player2Text;
    player2Text.setString(obj->player2 + ": " + to_string(Score_player_2));
    player2Text.setFont(font);
    player2Text.setCharacterSize(24);
    player2Text.setFillColor(Color::Blue);
    player2Text.setPosition(200, 180);

    int remainingTime = 60;
    Text timerText;
    timerText.setFont(font);
    timerText.setCharacterSize(24);
    timerText.setFillColor(Color::Red);
    timerText.setPosition(200, 110);

    Clock clock;
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
                exit(1);
            }
        }

        int elapsedSeconds = clock.getElapsedTime().asSeconds();
        remainingTime = 60 - elapsedSeconds;
        timerText.setString("Time remaining: " + to_string(remainingTime) + " s");

        Score_player_1 = *(obj->Sp1);
        Score_player_2 = *(obj->Sp2);
        player1Text.setString(obj->player1 + ": " + to_string(Score_player_1));
        player2Text.setString(obj->player2 + ": " + to_string(Score_player_2));

        window.clear();

        window.draw(backgroundSprite);

        window.draw(characterSprite1);
        window.draw(characterSprite2);
        window.draw(player1Text);
        window.draw(player2Text);
        window.draw(timerText);
        window.display();
    }

    return NULL;
}

void show_winner(string winnerName)
{
    Texture character1;
    Texture character2;
    Texture backgroundTexture;

    // Create character

    // Create the window
    RenderWindow window(VideoMode(400, 300), "WINNER");
    sf::Font font;
    if (!font.loadFromFile("GAB.ttf"))
    {
        exit(1);
    }

    // Load the background image
    if (!backgroundTexture.loadFromFile("winnerbackground.jpeg"))
    {
        cout << "Background image not found" << endl;
        exit(1);
    }

    // Create the background sprite
    Sprite backgroundSprite(backgroundTexture);
    backgroundSprite.setScale(1.f, 1.f);
    backgroundSprite.setPosition(0, 0);

    // Create the text
    Text winnerText;
    winnerText.setFont(font);
    winnerText.setString(winnerName + " wins!");
    winnerText.setCharacterSize(24);
    winnerText.setFillColor(Color::Black);
    winnerText.setPosition(100, 100);

    // Main event loop
    while (window.isOpen())
    {
        // Handle events
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
                exit(1);
            }
        }
        if (Score_player_1 > Score_player_2)
        {
            if (!character1.loadFromFile("Character1.jpeg"))
            {
                cout << "Image not found" << endl;
                exit(1);
            }
        }
        else if (Score_player_2 > Score_player_1)
        {
            if (!character1.loadFromFile("Character2.jpg"))
            {
                cout << "Image not found" << endl;
                exit(1);
            }
        }
        Sprite characterSprite1(character1);
        characterSprite1.setScale(1.f, 1.f);
        characterSprite1.setPosition(0, 0);

        // Draw the window
        window.clear();

        // Draw the background sprite
        window.draw(backgroundSprite);

        // Draw the character sprite and text
        window.draw(characterSprite1);
        window.draw(winnerText);

        window.display();
    }
}

int main()
{
    XInitThreads();
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    string a = "Abdur Rafey";
    string b = "Hareem Rashid";

    get_player_names(a, b);

    int number = 20;
    const int gridSize = 41;
    int gridnumber = 40;
    bool **grid = new bool *[gridnumber];
    for (int i = 0; i < gridnumber; i++)
    {
        grid[i] = new bool[gridnumber];
    }

    for (int i = 0; i < gridnumber; i++)
    {
        for (int j = 0; j < gridnumber; j++)
        {
            if (i % 2 == 0)
            {
                if (j % 2 == 0)
                {
                    grid[i][j] = true;
                }
                else
                {
                    grid[i][j] = false;
                }
            }
            else
            {
                if (j % 2 == 1)
                {
                    grid[i][j] = true;
                }
                else
                {
                    grid[i][j] = false;
                }
            }
        }
    }

    RenderWindow window(VideoMode(gridnumber * 25, gridnumber * 20), "Fruit Catcher");

    // Load character textures
    Texture character1;
    Texture character2;
    if (!character1.loadFromFile("Character1.jpeg"))
    {
        cout << "Image not found" << endl;
        exit(1);
    }
    if (!character2.loadFromFile("Character2.jpg"))
    {
        cout << "Image not found" << endl;
        exit(1);
    }
    // Create character
    Sprite characterSprite1(character1);
    Sprite characterSprite2(character2);
    // Set initial character positions
    characterSprite1.setScale(0.2f, 0.2f);
    characterSprite2.setScale(0.07f, 0.07f);
    characterSprite1.setPosition(0, 0);
    characterSprite2.setPosition(950, 750);
    // game events
    player obj;
    obj.player1 = a;
    obj.player2 = b;
    obj.Sp1 = &Score_player_1;
    obj.Sp2 = &Score_player_2;
    // values assigning
    bool flag = true;
    ThreadArgs args1 = {1, &characterSprite1, &window, &flag};
    ThreadArgs args2 = {2, &characterSprite2, &window, &flag};

    pthread_t thread1, thread2, thread3;
    pthread_create(&thread1, NULL, move_character_sprite, (void *)&args1);
    pthread_create(&thread2, NULL, move_character_sprite, (void *)&args2);
    pthread_create(&thread3, NULL, show_scores, (void *)&obj);

    int totalTime = 60; // total time in seconds
    Clock clock;        // create a clock object to keep track of elapsed time
                        // Main game loop
    Texture randomTexture;
    string imageFilename;
    imageFilename = to_string(1) + ".png";
    randomTexture.loadFromFile(imageFilename);

    int counter = 0;
    while (window.isOpen())
    {
        srand(time(NULL));
        int randomImageIndex = rand() % 5 + 1; // randomly select an image index between 0-2
        if (counter == 1000)
        {
            imageFilename = to_string(randomImageIndex) + ".png";
            randomTexture.loadFromFile(imageFilename);
            counter = 0;
        }
        Sprite obj(randomTexture);
        obj.setScale(0.2f, 0.2f);
        const int rows = window.getSize().y / gridSize; // number of rows in the grid
        const int cols = window.getSize().x / gridSize; // number of columns in the grid
        handle_events(window);

        // Calculate remaining time
        int timeRemaining = totalTime - static_cast<int>(clock.getElapsedTime().asSeconds());
        if (timeRemaining < 0)
        {
            timeRemaining = 0;
            flag = false; // stop the sprite movement threads when time is up
            // End the game and show the results
            window.close();
            cout << "Game Over!" << endl;
            cout << "Player 1 Score: " << Score_player_1 << endl;
            cout << "Player 2 Score: " << Score_player_2 << endl;
            if (Score_player_1 > Score_player_2)
            {
                show_winner(a);
            }
            else if (Score_player_2 > Score_player_1)
            {
                show_winner(b);
            }
            else
            {
                cout << "The game is tied!" << endl;
            }
            break;
            break;
        }
        counter++;
        draw(window, characterSprite1, characterSprite2, rows, cols, gridSize, grid, timeRemaining, obj);
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    for (int i = 0; i < gridnumber; i++)
    {
        delete[] grid[i];
    }
    delete[] grid;
}
