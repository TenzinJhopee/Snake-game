#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 50
#define HEIGHT 50
#define MAX_LENGTH 100

// Snake structure
struct Position {
    int x;
    int y;
};

struct Position snake[MAX_LENGTH];
int length;
struct Position food;
int direction; // 0: Up, 1: Right, 2: Down, 3: Left

// Initialize the game
void init_game() {
    // Start snake at center
    snake[0].x = WIDTH / 2;
    snake[0].y = HEIGHT / 2;
    length = 1;
    direction = 1; // Start moving right

    // Seed random number generator
    srand(time(0));
    // Generate first food
    food.x = rand() % WIDTH;
    food.y = rand() % HEIGHT;
}

// Generate new food (not on snake)
void generate_food() {
    int collision;
    do {
        food.x = rand() % WIDTH;
        food.y = rand() % HEIGHT;
        collision = 0;
        for (int i = 0; i < length; i++) {
            if (food.x == snake[i].x && food.y == snake[i].y) {
                collision = 1;
                break;
            }
        }
    } while (collision);
}

int main() {
    // Initialize ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE); // Non-blocking input
    curs_set(0); // Hide cursor

    // Enable colors if supported
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_GREEN, COLOR_BLACK); // Snake
        init_pair(2, COLOR_RED, COLOR_BLACK);   // Food
    }

    init_game();

    // Game loop
    while (1) {
        // Handle input
        int ch = getch();
        switch (ch) {
            case KEY_UP:
                if (direction != 2) direction = 0;
                break;
            case KEY_RIGHT:
                if (direction != 3) direction = 1;
                break;
            case KEY_DOWN:
                if (direction != 0) direction = 2;
                break;
            case KEY_LEFT:
                if (direction != 1) direction = 3;
                break;
            case 'q':
                endwin();
                return 0;
        }

        // Calculate new head position
        struct Position new_head = snake[0];
        switch (direction) {
            case 0: new_head.y--; break; // Up
            case 1: new_head.x++; break; // Right
            case 2: new_head.y++; break; // Down
            case 3: new_head.x--; break; // Left
        }

        // Check wall collision
        if (new_head.x < 0 || new_head.x >= WIDTH || new_head.y < 0 || new_head.y >= HEIGHT) {
            break; // Game over
        }

        // Check self-collision
        for (int i = 0; i < length; i++) {
            if (new_head.x == snake[i].x && new_head.y == snake[i].y) {
                endwin();
                printf("Game Over! Score: %d\n", length - 1);
                return 0;
            }
        }

        // Check food collision
        if (new_head.x == food.x && new_head.y == food.y) {
            length++;
            if (length >= MAX_LENGTH) {
                break; // Snake too long
            }
            generate_food();
        } else {
            // Shift body segments
            for (int i = length - 1; i > 0; i--) {
                snake[i] = snake[i - 1];
            }
        }
        snake[0] = new_head;

        // Clear screen and draw
        clear();

        // Draw snake
        for (int i = 0; i < length; i++) {
            if (i == 0) {
                attron(COLOR_PAIR(1));
                mvaddch(snake[i].y, snake[i].x, 'O'); // Head
            } else {
                attron(COLOR_PAIR(1));
                mvaddch(snake[i].y, snake[i].x, 'o'); // Body
            }
            attroff(COLOR_PAIR(1));
        }

        // Draw food
        attron(COLOR_PAIR(2));
        mvaddch(food.y, food.x, '*');
        attroff(COLOR_PAIR(2));

        // Refresh screen
        refresh();

        // Control game speed (100ms delay)
        napms(100);
    }

    endwin();
    printf("Game Over! Score: %d\n", length - 1);
    return 0;
}