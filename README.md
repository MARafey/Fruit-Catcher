# Fruit-Catcher

This code is a game that involves two players moving their characters on a grid to catch falling fruit within a time limit. The game is implemented using the SFML library and multi-threading.

The main function first initializes the game window and the character sprites. It then creates two threads to handle the movement of the characters. Another thread is created to continuously display the scores of the two players.

The game loop runs until the time limit is reached, during which the falling fruit is generated randomly and handled using the _'draw'_ function. The remaining time is calculated using a clock object. Once the time limit is reached, the threads handling the character movement are stopped, and the winner is determined based on the scores. The _'show_winner'_ function is then called to display the winning player along with a character image on a separate window.

Overall, the code demonstrates the use of _multi-threading_ and graphical display in game development.

The code is divided into two main functions, 'main' and 'show_winner'. The 'main' function is responsible for initializing the game window and objects, handling game events, and controlling the game loop. The 'show_winner' function is responsible for displaying the winner on a separate window.

In the 'main' function, the first part initializes the game window using the SFML library. It also loads the character sprites and sets their initial positions. The 'player' object is then created, which stores the player names and their scores. Two threads are then created using the 'pthread_create' function to handle the movement of the character sprites. These threads execute the 'move_character_sprite' function, which updates the position of the character sprite at a fixed interval.

The third thread is created to continuously display the scores of the two players. This thread executes the 'show_scores' function, which takes the 'player' object as a parameter and updates the scores on the game window.

After the threads are created, the game loop begins. In each iteration of the loop, the falling fruit is generated randomly, and its position is updated using the -_'draw'_ function. The remaining time is also calculated using the 'Clock' object.

Once the time limit is reached, the threads handling the character movement are stopped by setting the _'flag'_ variable to false. The winner is then determined based on the scores stored in the _'player'_ object. If there is a winner, the _'show_winner'_ function is called to display the winner's name and image on a separate window. If there is a tie, the game simply ends without displaying the winner.

Overall, the code demonstrates how multi-threading can be used to handle multiple game events simultaneously, and how graphical display can be implemented using the _SFML library_.

### void show_winner(string winnerName): 
This function is responsible for displaying the winner on a separate window. It takes the name of the winner as a parameter and displays it along with a character image on a window. It loads the required textures, creates the necessary sprites and text, and handles the main event loop using the SFML library.

### int main(): 
This is the main function of the game. It initializes the game window and objects, handles game events, controls the game loop, and manages threads. Here is an explanation of the main parts of this function:

a. Initializing the game window: This part creates the game window using the SFML library and sets the initial positions of the character sprites.

b. Creating threads: Three threads are created using the *pthread_create()* function. Two threads are responsible for moving the character sprites, and the third thread is responsible for displaying the scores of the two players.

c. Game loop: The game loop runs until the time limit is reached. In each iteration of the loop, the falling fruit is generated randomly and its position is updated using the draw() function. The remaining time is also calculated using the Clock object.

d. Handling game events: The game events are handled using the SFML event loop. This loop listens for events such as window closing, key presses, and mouse clicks.

e. Ending the game: Once the time limit is reached, the threads handling the character movement are stopped, and the winner is determined based on the scores. If there is a winner, the show_winner() function is called to display the winner's name and image on a separate window.

### void draw(RenderWindow &window, Sprite &characterSprite1, Sprite &characterSprite2, int rows, int cols, int gridSize, bool **grid, int timeRemaining, Sprite &randomSprite)
This function is responsible for drawing the falling fruit on the game window. It takes the game window, character sprites, grid parameters, remaining time, and the sprite for the falling fruit as parameters. It generates a random position for the falling fruit, checks if the character sprites catch it, and updates the score accordingly.

### void handle_events(RenderWindow &window): 
This function is responsible for handling game events such as key presses and mouse clicks. It takes the game window as a parameter and listens for events such as key presses and mouse clicks using the SFML event loop.

### void* move_character_sprite(void* arg): 
This function is responsible for updating the position of the character sprite at a fixed interval. It takes a ThreadArgs structure as a parameter, which stores the sprite pointer, window pointer, and flag to stop the thread.

### void* show_scores(void* args): 
This function is responsible for continuously displaying the scores of the two players on the game window. It takes a player object as a parameter, which stores the player names and scores.

Overall, these functions work together to create a functional game that involves two players moving their characters on a grid to catch falling fruit within a time limit. The code demonstrates how multi-threading and graphical display can be implemented using the SFML library.
