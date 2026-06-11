#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <string>

using namespace std;

// ========================================================
// CLASS: ConsoleRenderer (Handles all screen drawing)
// ========================================================
class ConsoleRenderer {
private:
    int screenW, screenH;
    HANDLE hConsole;
    vector<CHAR_INFO> screenBuffer;

public:
    ConsoleRenderer(int width, int height) : screenW(width), screenH(height) {
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        screenBuffer.resize(screenW * screenH);
        hideCursor();
    }

    void hideCursor() {
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(hConsole, &cursorInfo);
        cursorInfo.bVisible = false;
        SetConsoleCursorInfo(hConsole, &cursorInfo);
    }

    void clearBuffer() {
        for (int i = 0; i < screenW * screenH; i++) {
            screenBuffer[i].Char.AsciiChar = ' ';
            screenBuffer[i].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
        }
    }

    void drawChar(int x, int y, char c, WORD attr) {
        if (x >= 0 && x < screenW && y >= 0 && y < screenH) {
            screenBuffer[y * screenW + x].Char.AsciiChar = c;
            screenBuffer[y * screenW + x].Attributes = attr;
        }
    }

    void drawString(int x, int y, const string& str, WORD attr) {
        for (size_t i = 0; i < str.length(); i++) {
            drawChar(x + i, y, str[i], attr);
        }
    }

    void render() {
        SMALL_RECT writeArea = { 0, 0, (SHORT)(screenW - 1), (SHORT)(screenH - 1) };
        COORD bufferSize = { (SHORT)screenW, (SHORT)screenH };
        COORD bufferCoord = { 0, 0 };
        WriteConsoleOutputA(hConsole, screenBuffer.data(), bufferSize, bufferCoord, &writeArea);
    }

    WORD getColorAttribute(char c) {
        switch (c) {
            case '1': return FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            case '2': return FOREGROUND_RED | FOREGROUND_INTENSITY;
            case '3': return FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            case '4': return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            case '5': return FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            case '6': return FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            case '7': return FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            default:  return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
        }
    }
    
    HANDLE getConsoleHandle() { return hConsole; }
};

// ========================================================
// CLASS: Tetromino (Handles shape data and rotation)
// ========================================================
// ========================================================
// CLASS: Tetromino (Handles shape data and rotation)
// ========================================================
class Tetromino {
private:
    vector<vector<int>> shape;
    int x, y;

public:
    Tetromino() : x(0), y(0) {}

    void spawn(int boardWidth) {
        // Moved inside the function as a static constant!
        static const vector<vector<vector<int>>> allShapes = {
            {{1, 1, 1, 1}},                         // I
            {{2, 2}, {2, 2}},                       // O
            {{0, 3, 0}, {3, 3, 3}},                 // T
            {{0, 4, 4}, {4, 4, 0}},                 // S
            {{5, 5, 0}, {0, 5, 5}},                 // Z
            {{6, 0, 0}, {6, 6, 6}},                 // J
            {{0, 0, 7}, {7, 7, 7}}                  // L
        };
        
        shape = allShapes[rand() % allShapes.size()];
        x = boardWidth / 2 - shape[0].size() / 2;
        y = 0;
    }

    void rotate() {
        int rows = shape.size();
        int cols = shape[0].size();
        vector<vector<int>> rotated(cols, vector<int>(rows, 0));
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                rotated[j][rows - 1 - i] = shape[i][j];
        shape = rotated;
    }

    // Getters and Setters
    vector<vector<int>> getShape() const { return shape; }
    void setShape(const vector<vector<int>>& newShape) { shape = newShape; }
    int getX() const { return x; }
    int getY() const { return y; }
    void setX(int newX) { x = newX; }
    void setY(int newY) { y = newY; }
};

// ========================================================
// CLASS: Board (Handles grid logic, collisions, clearing)
// ========================================================
class Board {
private:
    int width, height;
    vector<vector<char>> grid;

public:
    Board(int w, int h) : width(w), height(h) {
        reset();
    }

    void reset() {
        grid.assign(height, vector<char>(width, '.'));
    }

    bool isValidMove(const Tetromino& tetro, int dx, int dy) {
        auto shape = tetro.getShape();
        for (size_t i = 0; i < shape.size(); i++) {
            for (size_t j = 0; j < shape[i].size(); j++) {
                if (shape[i][j]) {
                    int newX = tetro.getX() + j + dx;
                    int newY = tetro.getY() + i + dy;
                    if (newX < 0 || newX >= width || newY < 0 || newY >= height)
                        return false;
                    if (grid[newY][newX] != '.')
                        return false;
                }
            }
        }
        return true;
    }

    void lockTetromino(const Tetromino& tetro) {
        auto shape = tetro.getShape();
        for (size_t i = 0; i < shape.size(); i++) {
            for (size_t j = 0; j < shape[i].size(); j++) {
                if (shape[i][j]) {
                    grid[tetro.getY() + i][tetro.getX() + j] = char('0' + shape[i][j]);
                }
            }
        }
    }

    int clearFullLines() {
        int linesCleared = 0;
        for (int i = height - 1; i >= 0; i--) {
            bool full = true;
            for (int j = 0; j < width; j++) {
                if (grid[i][j] == '.') {
                    full = false;
                    break;
                }
            }
            if (full) {
                for (int row = i; row > 0; row--)
                    for (int col = 0; col < width; col++)
                        grid[row][col] = grid[row - 1][col];
                for (int col = 0; col < width; col++)
                    grid[0][col] = '.';
                linesCleared++;
                i++; // Recheck the current row index
            }
        }
        return linesCleared;
    }

    char getCell(int x, int y) const { return grid[y][x]; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
};

// ========================================================
// CLASS: Game (Handles game loop, input, state)
// ========================================================
class Game {
private:
    ConsoleRenderer renderer;
    Board board;
    Tetromino currentTetro;
    
    int score;
    int highScore;
    string userName;
    bool isGameOver;
    int fallSpeed;

public:
    Game() : renderer(60, 35), board(15, 25), score(0), highScore(0), isGameOver(false), fallSpeed(500) {
        srand((unsigned)time(0));
    }

    void showTitleScreen() {
        system("cls");
        HANDLE h = renderer.getConsoleHandle();
        SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        cout << "==========================================\n";
        SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        cout << "|            WELCOME TO TETRIS           |\n";
        SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        cout << "==========================================\n";
        SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        cout << "|   Prepare for the ultimate challenge!  |\n";
        SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        cout << "==========================================\n\n\n";
        
        cout << "Enter your name: ";
        getline(cin, userName);
        if (userName.empty()) userName = "Player";

        cout << "\nHello, " << userName << "!\n";
        cout << "Press any key to start the game...\n";
        _getch();
    }

    void handleInput() {
        while (_kbhit()) {
            int ch = _getch();
            if (ch == 224) {  // Arrow keys
                int arrow = _getch();
                if (arrow == 75 && board.isValidMove(currentTetro, -1, 0)) currentTetro.setX(currentTetro.getX() - 1);
                else if (arrow == 77 && board.isValidMove(currentTetro, 1, 0)) currentTetro.setX(currentTetro.getX() + 1);
                else if (arrow == 80 && board.isValidMove(currentTetro, 0, 1)) currentTetro.setY(currentTetro.getY() + 1);
                else if (arrow == 72) rotateCurrentTetromino();
            } else { // WASD and Space
                ch = toupper(ch);
                if (ch == 'A' && board.isValidMove(currentTetro, -1, 0)) currentTetro.setX(currentTetro.getX() - 1);
                else if (ch == 'D' && board.isValidMove(currentTetro, 1, 0)) currentTetro.setX(currentTetro.getX() + 1);
                else if (ch == 'S' && board.isValidMove(currentTetro, 0, 1)) currentTetro.setY(currentTetro.getY() + 1);
                else if (ch == 'W') rotateCurrentTetromino();
                else if (ch == 32) { // Hard Drop
                    while (board.isValidMove(currentTetro, 0, 1)) currentTetro.setY(currentTetro.getY() + 1);
                }
            }
        }
    }

    void rotateCurrentTetromino() {
        Tetromino temp = currentTetro;
        temp.rotate();
        if (board.isValidMove(temp, 0, 0)) {
            currentTetro = temp;
        }
    }

    void drawFrame() {
        renderer.clearBuffer();
        
        int drawY = 0;
        WORD borderAttr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;

        // Top border
        for (int i = 0; i < board.getWidth() + 2; i++) renderer.drawChar(i, drawY, '#', borderAttr);
        drawY++;

        // Grid and placed blocks
        for (int y = 0; y < board.getHeight(); y++) {
            renderer.drawChar(0, drawY, '#', borderAttr);
            for (int x = 0; x < board.getWidth(); x++) {
                char cell = board.getCell(x, y);
                if (cell == '.') renderer.drawChar(x + 1, drawY, '.', FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                else renderer.drawChar(x + 1, drawY, '#', renderer.getColorAttribute(cell));
            }
            renderer.drawChar(board.getWidth() + 1, drawY, '#', borderAttr);
            drawY++;
        }
        
        // Bottom border
        for (int i = 0; i < board.getWidth() + 2; i++) renderer.drawChar(i, drawY, '#', borderAttr);
        drawY -= board.getHeight(); // Reset Y to draw active piece

        // Active Tetromino
        auto shape = currentTetro.getShape();
        for (size_t i = 0; i < shape.size(); i++) {
            for (size_t j = 0; j < shape[i].size(); j++) {
                if (shape[i][j]) {
                    renderer.drawChar(currentTetro.getX() + j + 1, currentTetro.getY() + i + 1, '#', renderer.getColorAttribute(char('0' + shape[i][j])));
                }
            }
        }
        
        drawY += board.getHeight() + 1; // Move below board

        // UI Information
        renderer.drawString(0, drawY++, "Score: " + to_string(score) + "    |    High Score: " + to_string(highScore), borderAttr);
        drawY++;
        renderer.drawString(0, drawY++, "Controls:", FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        WORD textAttr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
        renderer.drawString(0, drawY++, "  Arrow Keys / WASD: Move & Rotate", textAttr);
        renderer.drawString(0, drawY++, "  Spacebar: Hard Drop", textAttr);

        renderer.render();
    }

    void startSession() {
        board.reset();
        score = 0;
        isGameOver = false;
        
        currentTetro.spawn(board.getWidth());
        if (!board.isValidMove(currentTetro, 0, 0)) return; // Immediate game over

        DWORD lastFallTime = GetTickCount();

        while (!isGameOver) {
            drawFrame();
            handleInput();
            
            DWORD now = GetTickCount();
            if (now - lastFallTime >= fallSpeed) {
                if (board.isValidMove(currentTetro, 0, 1)) {
                    currentTetro.setY(currentTetro.getY() + 1);
                } else {
                    board.lockTetromino(currentTetro);
                    int lines = board.clearFullLines();
                    score += lines * 100;

                    currentTetro.spawn(board.getWidth());
                    if (!board.isValidMove(currentTetro, 0, 0)) {
                        isGameOver = true;
                    }
                }
                lastFallTime = now;
            }
            Sleep(30);
        }
    }

    void run() {
        showTitleScreen();
        bool exitApp = false;

        while (!exitApp) {
            startSession();

            if (score > highScore) highScore = score;

            system("cls");
            HANDLE h = renderer.getConsoleHandle();
            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            cout << "\nGame Over, " << userName << "!\n";
            cout << "Your Score: " << score << "\n";
            cout << "High Score: " << highScore << "\n\n";
            cout << "Press [R] to Restart or [X] to Exit.\n";

            bool validChoice = false;
            while (!validChoice) {
                if (_kbhit()) {
                    char ch = toupper(_getch());
                    if (ch == 'R') validChoice = true;
                    else if (ch == 'X') {
                        validChoice = true;
                        exitApp = true;
                    }
                }
                Sleep(50);
            }
        }
        
        system("cls");
        cout << "Thanks for playing TETRIS, " << userName << "!\n";
        cout << "Final High Score: " << highScore << "\n";
        system("pause");
    }
};

// ========================================================
// MAIN EXECUTION
// ========================================================
int main() {
    Game tetrisGame;
    tetrisGame.run();
    return 0;
}