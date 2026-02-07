#include <SDL2/SDL.h>
#include <stdio.h>
#include <time.h>

#define TRUE 1
#define FALSE 0
#define byte unsigned char

#define resolutnionX 800
#define resolutionY 600

#define szerokoscDrogi 0.50f
#define polSzerokosciDrogi resolutnionX * 0.25


#define klawisz meowEvent.key.keysym.sym

typedef struct {int zero; int x; int y; int xy;} chmora;

int wysokosc = resolutionY / 4;

byte turnOn = TRUE;

int prawdopodobienstwo = 100;

int xG = polSzerokosciDrogi * 2;

float predkosc = 0;
float przyspieszenie = 0;

float dystans = 0;

int skrecenieDrogi[] = {FALSE,0,0,0};
float mocSkretu = 1.0f;

byte czyWcisnietyW = FALSE;

typedef struct {int x; int y; char side;} punkt;

byte czyWcisnolem = 25;

punkt trawa[94];

int wcC(int doWyciogania, int liczba) {return ((doWyciogania / liczba) % 10) * liczba;}


void gameMechaniks(int losowa)
{
    if (czyWcisnolem == FALSE)
    {
        if (przyspieszenie > 0 && przyspieszenie / 10 > 0.01f) przyspieszenie -= przyspieszenie / 10;
        else przyspieszenie = 0;

        if (predkosc > 0 && predkosc - predkosc/10 > 0) predkosc -= predkosc/10;
        else predkosc = 0;
        
    }

    if ((!losowa) && skrecenieDrogi[0] == FALSE && predkosc > 10  )
    {
        skrecenieDrogi[0] = TRUE;
        skrecenieDrogi[1] = dystans + 1000 + (rand() % 8000) + wysokosc;
        skrecenieDrogi[3] = skrecenieDrogi[1] + 10000 + (rand() % 100000);
        char plusMinut[] = {-1,1};
        mocSkretu = 0.75 + (rand() % 100) / 100.0f;
        skrecenieDrogi[2] = plusMinut[rand() % 2];

        printf("%d %f %f\n",skrecenieDrogi[1], dystans, mocSkretu);
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

    SDL_Window* meowOkno = SDL_CreateWindow ("Fajne Okno",resolutnionX*1.4,resolutionY*1.4,resolutnionX*1.4,resolutionY*1.4, SDL_WINDOW_SHOWN);
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
                if (klawisz == SDLK_d && xG < polSzerokosciDrogi * 6) xG+=(resolutnionX/160);
                else if (klawisz == SDLK_a && xG > polSzerokosciDrogi * -2 ) xG-=(resolutnionX/160);
                else if (klawisz == SDLK_w)
                {
                    czyWcisnolem = 25;
                    czyWcisnietyW++;

                    if (predkosc < 160) przyspieszenie += 0.10 + (float)predkosc/10;
                    else if (przyspieszenie - predkosc/100 > 0) przyspieszenie -= (float)predkosc/150;
                    else if (!(rand() % prawdopodobienstwo)) {
                        predkosc+=0.1;
                        prawdopodobienstwo+=10;
                    }
                    predkosc += przyspieszenie/100;  
                } 
                else if (klawisz == SDLK_s) predkosc--;
                else if (klawisz == SDLK_SPACE) dystans -= 100;
                
            }
        }

        if (czyWcisnietyW == czyWcisnietePrzed) prawdopodobienstwo = 100;
        if (czyWcisnolem > 0) czyWcisnolem--;
        
        gameMechaniks((rand() % 100));

        dystans+= predkosc;

        SDL_SetRenderDrawColor(meowRender,0, 0, 0,0);
        SDL_RenderClear(meowRender);

        SDL_SetRenderDrawColor(meowRender,36, 94, 201,0);
        SDL_Rect niboWymiary = {0,0,resolutnionX,wysokosc };
        SDL_RenderFillRect(meowRender, &niboWymiary);

        SDL_SetRenderDrawColor(meowRender,13, 186, 68,0);
        SDL_Rect ziemiaMeow = {0,wysokosc,resolutnionX,resolutionY-wysokosc};
        SDL_RenderFillRect(meowRender, &ziemiaMeow);

        SDL_SetRenderDrawColor(meowRender,0, 110, 0,0);

        
        int pTWczesniej = ((( wcC((int)dystans, 100) +  wcC((int)dystans, 10) + wcC((int)dystans, 1))) * (resolutionY-wysokosc) / 1000);

        for (int i = 0; i < 94; i++) for (int y = -9; y <= 9; y++) for (int x =-2; x <= 2; x++)
        {
            int pT = pTWczesniej;
            int xGT = xG;
            
            if (trawa[i].y + y + pT > resolutionY ) pT-=(resolutionY-wysokosc);

            if (trawa[i].x + x - xGT + trawa[i].side * 2  > resolutnionX) xGT+= resolutnionX * (int)((trawa[i].x + x - xGT + trawa[i].side * 2  ) / resolutnionX) ;
            else if (trawa[i].x + x - xGT + trawa[i].side * 2 < 1) xGT += resolutnionX * (((trawa[i].x + x - xGT + trawa[i].side * 2) / resolutnionX) - 1);
            
            if (y < -3) SDL_RenderDrawPoint(meowRender,trawa[i].x + x - xGT + trawa[i].side * 2  ,trawa[i].y + y + pT );
            else SDL_RenderDrawPoint(meowRender,trawa[i].x +x - xGT, trawa[i].y + y+ pT );
        }

        float zzz = resolutnionX * szerokoscDrogi - xG;
        float steps[] = {(zzz +  polSzerokosciDrogi),(zzz -  polSzerokosciDrogi), zzz };
        float points[] = {   polSzerokosciDrogi,  -  polSzerokosciDrogi, 0};


        for (int i = 0; i < sizeof(points)/sizeof(float); i++ ) points[i]+= (resolutnionX/2)+ (resolutnionX * szerokoscDrogi) -xG;
        
        byte kolorBarierkiKod = (byte)((wcC((int)dystans, 100) +  wcC((int)dystans, 10) + wcC((int)dystans, 1)) / 3.92f);
        byte kolorBarierkiPerSe = 0;

        float ofsetObl = 1;

        for (int i = resolutionY; wysokosc <= i; i--) {

            if (skrecenieDrogi[0] && skrecenieDrogi[1] + i < dystans+wysokosc && skrecenieDrogi[3] > dystans+wysokosc+i) ofsetObl +=  (ofsetObl/(resolutnionX/8.0f) * mocSkretu );
            if (i == wysokosc && skrecenieDrogi[3] < dystans+wysokosc+i) skrecenieDrogi[0] = FALSE;
            int ofset = ofsetObl * skrecenieDrogi[2];


            for (int b = 0; b < sizeof(steps) /sizeof(float) ; b++) points[b]-= steps[b] / resolutionY;
            
            SDL_SetRenderDrawColor(meowRender,54, 52, 47,0); //droga
            SDL_RenderDrawLine(meowRender,points[0]+ofset,i,points[1]+ofset,i);

            if (kolorBarierkiKod > 255 / 2) kolorBarierkiPerSe = 255;
            else kolorBarierkiPerSe = 0;

            kolorBarierkiKod+=(resolutionY/600) * 3;

            SDL_SetRenderDrawColor(meowRender,255, kolorBarierkiPerSe, kolorBarierkiPerSe,0);

            for (int b = 0; b < 2; b++ ) SDL_RenderDrawLine(meowRender,(int) (points[b]) + (resolutnionX/800) * -3 + ofset,i,(int) (points[b]) + (resolutnionX/800) * 3+ ofset,i  );

            if (kolorBarierkiKod > 200) SDL_SetRenderDrawColor(meowRender,54, 52, 47,0);
            else SDL_SetRenderDrawColor(meowRender,224, 189, 7 ,0);

            SDL_RenderDrawLine(meowRender,(int) (points[2]) + (resolutnionX/800) * -5+ofset,i,(int) (points[2]) + (resolutnionX/800) * 5+ofset,i  );
        }

        SDL_Rect sdkrejty[] = {{resolutnionX * 0.4f,resolutionY * 0.77f ,resolutnionX * 0.2f,resolutionY * 0.10f },
        {resolutnionX * 0.41f,resolutionY * 0.85f ,resolutnionX * 0.02f,resolutionY * 0.05f },
        {resolutnionX * 0.57f,resolutionY * 0.85f ,resolutnionX * 0.02f,resolutionY * 0.05f }};

        SDL_SetRenderDrawColor(meowRender,110, 14, 0,0);
        SDL_RenderFillRect(meowRender, &sdkrejty[0]);
        SDL_SetRenderDrawColor(meowRender,0, 0, 0,0);
        for (int i = 1; i < 3; i++) SDL_RenderFillRect(meowRender, &sdkrejty[i]);

        SDL_RenderDrawLine(meowRender, resolutnionX * 0.4f,resolutionY * 0.77f,resolutnionX * 0.43f,resolutionY * 0.70f );
        SDL_RenderDrawLine(meowRender, resolutnionX * 0.6f,resolutionY * 0.77f,resolutnionX * 0.57f,resolutionY * 0.70f );
        SDL_RenderDrawLine(meowRender,resolutnionX * 0.43f,resolutionY * 0.70f,resolutnionX * 0.57f,resolutionY * 0.70f);

        SDL_RenderPresent(meowRender); SDL_Delay(60);
    }
    SDL_DestroyRenderer (meowRender);
    SDL_DestroyWindow (meowOkno);
}

