#include <SDL2/SDL.h>
#include <stdio.h>
#include <time.h>

#define TRUE 1
#define FALSE 0
#define byte unsigned char

#define resolutnionX 800
#define resolutionY 600


#define szerokoscDrogi 0.50f
#define polSzerokosciDrogi 200

typedef struct {int zero; int x; int y; int xy;} chmora;

int wysokosc = resolutionY / 3;
byte turnOn = TRUE;

int prawdopodobienstwo = 100;

int xG = polSzerokosciDrogi * 2;

float predkosc = 0;
float przyspieszenie = 0;

float dystans = 0;

byte czyWcisnietyW = FALSE;
byte czyPoprzedniToBylW = FALSE;

typedef struct {int x; int y; char side;} punkt;

punkt trawa[94];

int wcC(int doWyciogania, int liczba)
{
    return ((doWyciogania / liczba) % 10) * liczba;
}

void gameMechaniks()
{
    if (czyPoprzedniToBylW == FALSE)
    {
        if (przyspieszenie > 0 && przyspieszenie / 10 > 0.01f)
        {
            przyspieszenie -= przyspieszenie / 10;
        }
        else przyspieszenie = 0;
        if (predkosc > 0 && predkosc - predkosc/10 > 0) predkosc -= predkosc/10;
        else predkosc = 0;
        
    }
}

int main()
{
    srand(time(NULL));

    for (int i = 0; i < 94; i++)
    {
        trawa[i].y = wysokosc + (rand() % (int)(resolutionY - wysokosc));
        trawa[i].x = (rand() % resolutnionX  ) + xG;
        trawa[i].side = (rand  () % 3 ) - 1;
    }

    SDL_Window* meowOkno = SDL_CreateWindow ("FajneOkno",resolutnionX*1.4,resolutionY*1.4,resolutnionX*1.4,resolutionY*1.4, SDL_WINDOW_SHOWN);
    SDL_Renderer* meowRender = SDL_CreateRenderer (meowOkno, -1, SDL_RENDERER_ACCELERATED);
    SDL_Event meowEvent;
    SDL_RenderSetLogicalSize(meowRender,resolutnionX,resolutionY);

    while (turnOn)
    {

        byte czyWcisnietePrzed = czyWcisnietyW;

        while (SDL_PollEvent(&meowEvent)) {
            if (meowEvent.type == SDL_QUIT ) turnOn = FALSE; 
            else if (meowEvent.type == SDL_KEYDOWN)
            {
                if (meowEvent.key.keysym.sym == SDLK_d) xG+=(resolutnionX/160);
                else if (meowEvent.key.keysym.sym == SDLK_a) xG-=(resolutnionX/160);
                else if (meowEvent.key.keysym.sym == SDLK_w)
                {
                    czyWcisnietyW++;

                        if (predkosc < 160) przyspieszenie += 0.10 + (float)predkosc/10;
                        else if (przyspieszenie - predkosc/100 > 0) przyspieszenie -= (float)predkosc/100;
                        else if (!(rand() % prawdopodobienstwo)) {
                            predkosc+=0.1;
                            prawdopodobienstwo+=10;
                        }

                        predkosc += przyspieszenie/100;  

                }  
            }
        }

        czyPoprzedniToBylW = TRUE;
        if (czyWcisnietyW == czyWcisnietePrzed) 
        {
            prawdopodobienstwo = 100;
            czyPoprzedniToBylW = FALSE;
        }
        
        gameMechaniks();

        dystans+= predkosc;

        SDL_SetRenderDrawColor(meowRender,0, 0, 0,0);
        SDL_RenderClear(meowRender);

        
        SDL_SetRenderDrawColor(meowRender,36, 94, 201,0);
        for (int y = 0; y < wysokosc+1; y++) for (int x = 0; x < resolutnionX; x++) SDL_RenderDrawPoint(meowRender, x, y);

        SDL_SetRenderDrawColor(meowRender,13, 186, 68,0);
        for (int y = wysokosc; y < resolutionY; y++) for (int x = 0; x < resolutnionX; x++) SDL_RenderDrawPoint(meowRender, x, y);

        SDL_SetRenderDrawColor(meowRender,0, 110, 0,0);
        for (int i = 0; i < 94; i++) for (int y = -9; y <= 9; y++) for (int x =-2; x <= 2; x++)
        {
            int pT = ((wcC((int)dystans, 100) +  wcC((int)dystans, 10) + wcC((int)dystans, 1))) * (resolutionY-wysokosc) / 2000;
            int xGT = xG;


            if (trawa[i].y + y + pT > resolutionY ) pT-=(resolutionY-wysokosc);

            if (trawa[i].x + x - xGT + trawa[i].side * 2  > resolutnionX) xGT+= resolutnionX * (int)((trawa[i].x + x - xGT + trawa[i].side * 2  ) / resolutnionX) ;
            else if (trawa[i].x + x - xGT + trawa[i].side * 2 < 1) xGT += resolutnionX * (((trawa[i].x + x - xGT + trawa[i].side * 2) / resolutnionX) - 1);
            
            if (y < -3) SDL_RenderDrawPoint(meowRender,trawa[i].x + x - xGT + trawa[i].side * 2  ,trawa[i].y + y + pT);
            else SDL_RenderDrawPoint(meowRender,trawa[i].x +x - xGT,trawa[i].y + y+ pT);
        }

        float stepXA = (resolutnionX * szerokoscDrogi - xG + (resolutnionX/800) * polSzerokosciDrogi) / resolutionY;
        float stepXB = (resolutnionX * szerokoscDrogi - xG - (resolutnionX/800) * polSzerokosciDrogi) / resolutionY;

        float punktStartowyA[] = {(resolutnionX/2)+ (resolutnionX * szerokoscDrogi) - xG + (resolutnionX/800) * polSzerokosciDrogi , resolutionY};
        float punktStartowyB[] = {(resolutnionX/2)+ (resolutnionX * szerokoscDrogi) - xG - (resolutnionX/800) * polSzerokosciDrogi , resolutionY};

        byte kolorBarierkiKod = (byte)((wcC((int)dystans, 100) +  wcC((int)dystans, 10) + wcC((int)dystans, 1)) / 3.92f);
        byte kolorBarierkiPerSe = 0;

        for (int i = wysokosc; punktStartowyA[1] > wysokosc; i++) {
            punktStartowyA[0]-= stepXA;
            punktStartowyA[1]-= 1;
            punktStartowyB[0]-= stepXB;
            punktStartowyB[1]-= 1;

            SDL_SetRenderDrawColor(meowRender,54, 52, 47,0);
            for (int b = (int) (punktStartowyB[0]); b <= (int) (punktStartowyA[0]); b++)
            {SDL_RenderDrawPoint(meowRender,b, (int)punktStartowyB[1]); }

            if (kolorBarierkiKod > 255 / 2) kolorBarierkiPerSe = 255;
            else kolorBarierkiPerSe = 0;

            kolorBarierkiKod+=(resolutionY/600) * 3;

            SDL_SetRenderDrawColor(meowRender,255, kolorBarierkiPerSe, kolorBarierkiPerSe,0);
            for (int i = (resolutnionX/800) * -3; i <= (resolutnionX/800) * 3; i++ )
            {
                SDL_RenderDrawPoint(meowRender,(int) (punktStartowyA[0]) +i, (int) punktStartowyA[1] ); 
                SDL_RenderDrawPoint(meowRender,(int) (punktStartowyB[0]) +i, (int) punktStartowyB[1] ); 
            }

        }

        SDL_RenderPresent(meowRender);
        SDL_Delay(20);
    }
    SDL_DestroyRenderer (meowRender);
    SDL_DestroyWindow (meowOkno);
}

