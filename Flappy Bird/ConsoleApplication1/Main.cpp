#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <SFML/Audio.hpp>
#include <iomanip>
#include <time.h>

// Namespaces
using namespace std;
using namespace sf;


// Function Prototypes
void Start();
void Update();
void Draw();
void BuildMainMenu();
void BuildPauseMenu();
void BuildCreditsMenu();
void UpdateMainMenu();
void UpdatePauseMenu();

// Global Structs

struct Sounds {
    SoundBuffer scorebuff;
    SoundBuffer flapbuff;
    SoundBuffer crashbuff;
    SoundBuffer menuLoopBuffer;
    Sound score;
    Sound flap;
    Sound crash;
    Sound menuLoop;
} sounds;

struct Textures {
    Texture flappytxt;
    Texture pipetxt;
    Texture backgroundtxt;
    Texture groundtxt;
} textures;

// Gloabal Variables
RenderWindow window;
Event event;
Font font;
float velocity = 0.f;
float power = -500.f;
float gravity = 1500.f;
float timer = 0;
int currentframe = 0;
float frameWidth = 67;
int frameheight = 39;
bool started = false;
float gap = 200.0f;
float heights[] = { 150.0f,300.0f,200.0f ,250.0f };
bool isDead = false;
int heightIndex = 0;

// Could be changed
float pipeX = 900.0F;


// Sprits
Sprite background;
Sprite flappy;
Sprite pipe;
Sprite groundOne;
Sprite groundTwo;
Sprite pipeUp, pipeDown;


// Main menu variables
vector<string> MenuStr = { "Play", "Settings", "Credits", "Exit"};
vector<Text> MenuText; // resize in Start()
int selectedIndex = 0;

// Pause menu variables
vector<string> PauseStr = { "Resume", "Main Menu", "Exit" };
vector<Text> PauseText;
int pauseSelectedIndex = 0;

// Credits menu variables
vector<string> CreditsStr = {"CREDITS","Saif Karaly", "Saif Mousa", "Yahia Salah", "Ziad Abdelaal", "Click Esc to go to main"};
vector<Text> CreditsText;

int currentWindow = 0; // 0 main menu, 1 game, 2 pause , 3 credits ... whatever


// Game Start
int main()
{
    Clock clock;
    
    Start();

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        timer += dt;

        Update();
        if (started)
        {
            velocity += gravity * dt;
            flappy.move(0, velocity * dt);

            if (timer >= 0.12f)
            {
                timer = 0;
                currentframe++;
                if (currentframe >= 3)currentframe = 0;
                flappy.setTextureRect(IntRect(currentframe * frameWidth, 0, frameWidth, frameheight));
            }

            // Ground Movement
            groundOne.move(-200.f * dt, 0);
            groundTwo.move(-200.f * dt, 0);


            // Ground Position Reset
            if (groundOne.getPosition().x <= -840)

            {

                groundOne.setPosition(840, 500);

            }

            if (groundTwo.getPosition().x <= -840)

            {

                groundTwo.setPosition(840, 500);

            }

            pipeUp.move(-500.f * dt, 0);
            pipeDown.move(-500.f * dt, 0);

            if (pipeUp.getPosition().x < -100)
            {

                heightIndex = (heightIndex + 1) % 4;

                pipeUp.setPosition(pipeX, heights[heightIndex] - (gap / 2));

                pipeDown.setPosition(pipeX, heights[heightIndex] + (gap / 2));

            }

            if ((flappy.getGlobalBounds().intersects(pipeDown.getGlobalBounds()) || flappy.getGlobalBounds().intersects(pipeUp.getGlobalBounds())))
            {
                currentWindow = 0;
                isDead = true;
                sounds.crash.play();
            }

            if (flappy.getGlobalBounds().intersects(groundOne.getGlobalBounds()) || flappy.getGlobalBounds().intersects(groundTwo.getGlobalBounds()))
            {
                currentWindow = 0;
                isDead = true;
                sounds.crash.play();
            }
        }

        if (isDead)
        {
            started = false;
            flappy.setPosition(800 / 2, 600 / 2);
        }

        Draw();
    }

    return 0;
}

void Start()
{
    window.create(VideoMode(800, 600), "Flappy Bird");

    // Resize Vectors
    MenuText.resize(MenuStr.size());
    PauseText.resize(PauseStr.size());
    CreditsText.resize(CreditsStr.size());

    // Sounds loading
    sounds.scorebuff.loadFromFile("score.wav");
    sounds.flapbuff.loadFromFile("flap.wav");
    sounds.crashbuff.loadFromFile("audio_crash.wav");
    sounds.menuLoopBuffer.loadFromFile("MenuLoop.mp3");
    sounds.menuLoop.setBuffer(sounds.menuLoopBuffer);
    sounds.score.setBuffer(sounds.scorebuff);
    sounds.flap.setBuffer(sounds.flapbuff);
    sounds.crash.setBuffer(sounds.crashbuff);
    sounds.flap.setVolume(70.f);
    sounds.menuLoop.setVolume(100.f);
    sounds.menuLoop.setLoop(true);
    sounds.crash.setVolume(70.f);

    // Textures Loading
    textures.backgroundtxt.loadFromFile("background-day.png");
    textures.pipetxt.loadFromFile("pipe-green.png");
    textures.flappytxt.loadFromFile("birdframes.png");
    textures.groundtxt.loadFromFile("base.png");
    textures.pipetxt.loadFromFile("pipe-green.png");


    // Load font
    font.loadFromFile("flappy.ttf");
    

    // Set background texture
    background.setTexture(textures.backgroundtxt);
    pipe.setTexture(textures.pipetxt);

    // Set flappy
    flappy.setTexture(textures.flappytxt);
    flappy.setTextureRect(IntRect(0, 0, 67, 39));
    flappy.setOrigin(flappy.getGlobalBounds().width / 2, flappy.getGlobalBounds().height / 2);
    flappy.setScale(1.5, 1.5);
    flappy.setPosition(800 / 2, 600 / 2);

    // Set Ground
    groundOne.setTexture(textures.groundtxt);
    groundTwo.setTexture(textures.groundtxt);
    groundOne.setScale(2.5f, 1.f);
    groundTwo.setScale(2.5f, 1.f);
    groundOne.setPosition(0, 500);
    groundTwo.setPosition(840, 500);

    // Set Pipe
    pipeUp.setTexture(textures.pipetxt);
    pipeDown.setTexture(textures.pipetxt);
    pipeUp.setScale(1.5f, -1.5f);
    pipeDown.setScale(1.5f, 1.5f);
    pipeUp.setPosition(pipeX, heights[heightIndex] - (gap / 2));
    pipeDown.setPosition(pipeX, heights[heightIndex] + (gap / 2));

    // Menu Loop
    // Guess the Music ;)
    sounds.menuLoop.play();

    // Building Main at game start
    BuildMainMenu();
    UpdateMainMenu();

    // Building Pase Menu at game start
    BuildPauseMenu();
    UpdatePauseMenu();

    // Building Credits Menu at game start
    BuildCreditsMenu();
}

void Update()
{

    while (window.pollEvent(event))
    {
        if (event.type == Event::Closed)
            window.close();

        // handle input for current screen
        if (currentWindow == 0) // Main menu
        {
            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Up)
                {
                    selectedIndex--;
                    if (selectedIndex < 0) selectedIndex = (int)MenuText.size() - 1;
                    UpdateMainMenu();
                }
                else if (event.key.code == Keyboard::Down)
                {
                    selectedIndex++;
                    if (selectedIndex >= (int)MenuText.size()) selectedIndex = 0;
                    UpdateMainMenu();
                }
                else if (event.key.code == Keyboard::Enter)
                {
                    if (selectedIndex == 0)
                    {
                        currentWindow = 1; // Play
                        isDead = false;
                        pipeUp.setPosition(pipeX, heights[heightIndex] - (gap / 2));
                        pipeDown.setPosition(pipeX, heights[heightIndex] + (gap / 2));
                    }
                    else if (selectedIndex == 1)  currentWindow = 0; // Settings
                    else if (selectedIndex == 2)  currentWindow = 3; // Credits
                    else if (selectedIndex == 3)  window.close();    // Exit
                }          
            }
        }
        else if (currentWindow == 1) // Game Menu
        {
            // Game Pause
            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Escape or event.key.code == Keyboard::P)
                {
                    currentWindow = 2;
                    velocity = 0;
                    started = false;
                    pipeUp.move(0, 0);
                    pipeDown.move(0, 0);
                }
            }

            // Jumping
            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Up or event.key.code == Keyboard::Space)
                {
                    velocity = power;
                    sounds.flap.play();
                    started = true;
                }
            }
        }
        else if (currentWindow == 2) // Pause Menu
        {
            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Up)
                {
                    pauseSelectedIndex--;
                    if (pauseSelectedIndex < 0) pauseSelectedIndex = (int)PauseText.size() - 1;
                    UpdatePauseMenu();
                }
                else if (event.key.code == Keyboard::Down)
                {
                    pauseSelectedIndex++;
                    if (pauseSelectedIndex >= (int)PauseText.size()) pauseSelectedIndex = 0;
                    UpdatePauseMenu();
                }
                else if (event.key.code == Keyboard::Enter)
                {
                    if (pauseSelectedIndex == 0) currentWindow = 1;
                    else if (pauseSelectedIndex == 1) currentWindow = 0;
                    else if (pauseSelectedIndex == 2) window.close();
                }
            }
        }
        else if (currentWindow == 3) // Credits Menu
        {
            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Escape)
                {
                    currentWindow = 0;
                }
            }
        }
    }
}

void Draw()
{
    window.clear();

    if (currentWindow == 0) // Main menu draw
    {
        background.setScale(4, 1.5);
        window.draw(background);
        window.draw(pipeUp);
        window.draw(pipeDown);
        for (int i = 0; i < (int)MenuStr.size(); i++)
            window.draw(MenuText[i]);
    }
    else if (currentWindow == 1)
    {
        // draw game stuff
        window.draw(background);
        window.draw(flappy);
        window.draw(pipeUp);
        window.draw(pipeDown);
        window.draw(groundOne);
        window.draw(groundTwo);
    }
    else if (currentWindow == 2)
    {
        // draw settings/pause
        window.draw(background);
        window.draw(pipeUp);
        window.draw(pipeDown);
        window.draw(flappy);
        for (int i = 0; i < (int)PauseStr.size(); i++)
            window.draw(PauseText[i]);
    }
    else if (currentWindow == 3)
    {
        window.draw(background);
        for (int i = 0; i < (int)CreditsStr.size(); i++)
            window.draw(CreditsText[i]);
    }

    window.display();
}

void BuildMainMenu()
{
    for (int i = 0; i < (int)MenuStr.size(); i++)
    {
        MenuText[i].setFont(font);
        MenuText[i].setString(MenuStr[i]);
        MenuText[i].setCharacterSize(60);
        MenuText[i].setPosition(200.f, 150.f + (100.f * i));
        MenuText[i].setOutlineColor(Color::Black);
        MenuText[i].setOutlineThickness(10);
    }
}

void BuildCreditsMenu()
{
    for (int i = 0; i < (int)CreditsStr.size(); i++)
    {
        CreditsText[i].setFont(font);
        CreditsText[i].setString(CreditsStr[i]);
        CreditsText[i].setCharacterSize(50);
        CreditsText[i].setOutlineColor(Color::Black);
        CreditsText[i].setOutlineThickness(5);
        CreditsText[i].setPosition(150.f, 80.f + (70.f * i));
        if (i == 0)
            CreditsText[i].setFillColor(Color::Red);
    }
}

void BuildPauseMenu()
{
    for (int i = 0; i < (int)PauseStr.size(); i++)
    {
        PauseText[i].setFont(font);
        PauseText[i].setString(PauseStr[i]);
        PauseText[i].setCharacterSize(60);
        PauseText[i].setPosition(200.f, 150.f + (100.f * i));
        PauseText[i].setOutlineColor(Color::Black);
        PauseText[i].setOutlineThickness(10);
    }
}

void UpdateMainMenu()
{
    for (int i = 0; i < (int)MenuText.size(); i++)
    {
        if (i == selectedIndex) MenuText[i].setFillColor(Color::Red);
        else MenuText[i].setFillColor(Color::White);
    }
}

void UpdatePauseMenu()
{
    for (int i = 0; i < (int)PauseText.size(); i++)
    {
        if (i == pauseSelectedIndex) PauseText[i].setFillColor(Color::Red);
        else PauseText[i].setFillColor(Color::White);
    }
}