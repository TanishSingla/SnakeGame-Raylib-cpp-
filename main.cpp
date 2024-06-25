#include <bits/stdc++.h>
#include <raylib.h>
#include <raymath.h>
using namespace std;

Color c1 = {173,204,96,255};
Color c2 = {43,51,24,255};
Color c3 = {224,65,56,255};
Color c4 = {1,1,1,255};

int cellSize = 30; 
int cellCount = 25; 
int offset = 75;
double lastUpdateTime = 0;

vector<vector<int>> v;
bool Automate = false;
void CreateSortestPath(deque<Vector2> SnakeBody, Vector2 Food){

}

/* ----------Functions------- */
bool eventTriggered(double interval){
    // function for checking if certain amount of time has been passed or not.
    // as we want the snake to move at slower speed so we will not update its movement
    // every frame , we will skip some frame
    double currentTime =  GetTime();
    if(currentTime - lastUpdateTime > interval){
        lastUpdateTime = currentTime;
        return 1;
    }
    return 0;
}

bool elementExistsInDeque(Vector2 ele , deque<Vector2>deq){
    for(unsigned int i = 0;i<deq.size();i++){
        if(Vector2Equals(ele,deq[i]))return 1;
    }
    return 0;
}
/* ------------------------------- */



/* ------------Classes--------------- */
class Snake {

    public:
    deque<Vector2> body = {Vector2{6,9},Vector2{5,9},Vector2{4,9}};
    Vector2 Direction = {1,0};
    bool addSegment = false;

    void Draw(){    
        for(unsigned int i = 0; i<body.size(); i++){
            float x = body[i].x;
            float y = body[i].y;

            Rectangle rect = Rectangle{offset + x*cellSize,offset +y*cellSize,(float)cellSize,(float)cellSize};
            Color snakeColor = i==0 ? c3 :c2;                  
            DrawRectangleRounded(rect,0.5,6,snakeColor);
        }
    }

    void Update(){
        body.push_front(Vector2Add(body[0],Direction));
        if(addSegment){
            addSegment = false;
        }else{
            body.pop_back();
        }
    }

    void Reset(){
        body = {Vector2{6,9},Vector2{5,9},Vector2{4,9}};
        Direction = {1,0};
    }
};

class Food{
    public:
    Vector2 position;
    Texture2D texture;

    // Constructor
    Food(deque<Vector2>snakeBody){
        Image img = LoadImage("food.png");
        texture = LoadTextureFromImage(img);
        UnloadImage(img);
        position = GenerateRandomPositonsForFood(snakeBody); 
    }

    void Draw(){
        // DrawRectangle(positon.x * cellSize,positon.y * cellSize,cellSize,cellSize,c2);
        DrawTexture(texture,offset + position.x*cellSize,offset + position.y*cellSize,c2);
    }

    Vector2 GenerateRandomCell(){
        // we have to guess the number between 1 and 25 inclusive or 0->24
        float x = GetRandomValue(0,cellCount-1);
        float y = GetRandomValue(0,cellCount-1);
        return Vector2{x,y};
    }
    Vector2 GenerateRandomPositonsForFood(deque<Vector2>SnakeBody){
        // we will check if the new random value lies on the snake if yes then we will generate another random 
        // value so that snake food will not overlap on snake's body.
        Vector2 pos = GenerateRandomCell();
        while(elementExistsInDeque(pos,SnakeBody)){
            Vector2 pos = GenerateRandomCell();
        }
        return pos;
    }
    // Destructor
    ~Food(){
        UnloadTexture(texture);
    }
};

int highScore = 0;
class Game{
public:
    int score = 0;
    Snake snake = Snake();
    Food food = Food(snake.body);
    bool GameIsRunning = true;

    void Draw(){
        food.Draw();
        snake.Draw();
    }

    void Update(){
        if(GameIsRunning){
            snake.Update();
            CheckCollisionWithFood();
            CheckCollisionWithEdges();
            checkCollisionWithTail();   
        }
    }

    void CheckCollisionWithFood(){
        if(Vector2Equals(snake.body[0],food.position)){
            score++;
            food.position = food.GenerateRandomPositonsForFood(snake.body);
            snake.addSegment = true;
        }
    }

    void CheckCollisionWithEdges(){
        if(snake.body[0].x == cellCount || snake.body[0].x == -1 || snake.body[0].y == cellCount || snake.body[0].y == -1)
            GameOver();
    }

    void GameOver(){
        snake.Reset();
        food.position = food.GenerateRandomPositonsForFood(snake.body);
        GameIsRunning  = false;
        score = 0;
    }

    void checkCollisionWithTail(){
        deque<Vector2> HeadlessBody = snake.body;
        HeadlessBody.pop_front();
        if(elementExistsInDeque(snake.body[0],HeadlessBody)){
            GameOver();
        }
    }
};


/* ------------------------------- */
int main () {

    InitWindow(2*offset + cellSize*cellCount,2*offset + cellSize*cellCount,"🐍 Snake Game 🐍");
    SetTargetFPS(60);

    Game game = Game();
    int cnt = 0;
    double timeToCheck = 0.3;
    while(!WindowShouldClose()){
        BeginDrawing();

        //Drawing :- 
        ClearBackground(c1);

        
        if(cnt+5 == game.score){
            timeToCheck -= 0.01;
            cnt = game.score;
        }
        if(eventTriggered(timeToCheck)){
            game.Update();
        }

        if(IsKeyPressed(KEY_UP) && game.snake.Direction.y!=1 ){game.snake.Direction = {0,-1};game.GameIsRunning=1;}
        if(IsKeyPressed(KEY_DOWN) && game.snake.Direction.y!=-1){game.snake.Direction = {0,1};game.GameIsRunning=1;}
        if(IsKeyPressed(KEY_LEFT) && game.snake.Direction.x!=1){game.snake.Direction = {-1,0};game.GameIsRunning=1;}
        if(IsKeyPressed(KEY_RIGHT) && game.snake.Direction.x!=-1){game.snake.Direction = {1,0};game.GameIsRunning=1;}
        // cout<<snake.Direction.x<<" "<<snake.Direction.y<<endl;

        game.Draw();
        if(game.score > highScore){
            highScore = game.score;
        }
        DrawText("Snake Game",offset-5,20,40,c4);
        DrawText(TextFormat("%i",game.score),offset-5,offset+cellSize*cellCount+10,40,c4);
        DrawText("HighScore",cellSize*cellCount - 150,offset+cellSize*cellCount+10,40,c4);
        DrawText(TextFormat("%i",highScore),cellSize*cellCount + 80,offset + cellSize*cellCount+10,40,c4);
        DrawRectangleLinesEx(Rectangle{(float)offset-5,(float)offset-5,(float)cellSize*cellCount+10,(float)cellSize*cellCount+10},5,c4);

        cout<<game.snake.body[0].x <<" "<<game.snake.body[0].y<<endl;
        EndDrawing();
    }

    CloseWindow();
    return 0;
}