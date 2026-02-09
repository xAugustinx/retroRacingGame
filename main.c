#include <SDL2/SDL.h>
#include <stdio.h>
#include <time.h>
#include "textures.h"

#define TRUE 1
#define FALSE 0
#define byte unsigned char

#define resolutnionX 1000
#define resolutionY 600

#define szerokoscDrogi 0.50f
#define polSzerokosciDrogi resolutnionX * 0.25
#define defultPlayerX  polSzerokosciDrogi * 2

typedef struct {int zero; int x; int y; int xy;} chmora;
typedef struct {float dystans; float predkosc; int x; byte r; byte g; byte b;} samochod;

int oponaW = (int)(resolutnionX * 0.02f);
int oponaH = (int)(resolutionY * 0.05f);

samochod * samochodyNaDrodze;
short liczbaSamochodow = 1;

int wysokosc = resolutionY / 4;
byte turnOn = TRUE;
int prawdopodobienstwo = 100;

int xG = defultPlayerX;
int yGracza = resolutionY * 0.77f;

float predkosc = 0;
float przyspieszenie = 0;
float dystans = 0;

int skrecenieDrogi[] = {FALSE,0,0,0};
float mocSkretu = 1.0f;

byte czyWcisnietyW = FALSE;

typedef struct {int x; int y; char side;} punkt;

byte czyWcisnolem = 25;
punkt trawa[94];

int wcC(int doWyciogania, int liczba) {
    if (liczba == -1) return 0;
    return ((doWyciogania / liczba) % 10) * liczba;
}

const Uint8* state;


int kwadrat(int wejsciowa, int podniesionaDo)
{
    int i = wejsciowa;
    for (int y = 0; y < podniesionaDo-1; y++)
    {
        i*=wejsciowa;
    }
    return i;
}

void gameMechaniks(int losowa)
{
    
    if (czyWcisnolem == FALSE)
    {
        if (przyspieszenie > 0 && przyspieszenie / 10 > 0.01f) przyspieszenie -= przyspieszenie / 5;
        else przyspieszenie = 0;

        if (predkosc > 0 && predkosc - predkosc/10 > 0) predkosc -= predkosc/5;
        else predkosc = 0;
        
    }

    if ((!losowa) && skrecenieDrogi[0] == FALSE && predkosc > 10  )
    {
        skrecenieDrogi[0] = TRUE;
        skrecenieDrogi[1] = dystans + 1000 + (rand() % 8000) + wysokosc;
        skrecenieDrogi[3] = skrecenieDrogi[1] + 10000 + (rand() % 100000);
        char plusMinut[] = {-1,1};
        mocSkretu = 0.75 + (rand() % 50) / 100.0f;
        skrecenieDrogi[2] = plusMinut[rand() % 2];

        //printf("%d %f %f\n",skrecenieDrogi[1], dystans, mocSkretu);
    }
    else if (losowa == 6)
    {
        liczbaSamochodow++;
        samochod * tymczasowy = realloc(samochodyNaDrodze, liczbaSamochodow * sizeof(samochod));
        samochodyNaDrodze = tymczasowy;
        samochod tezTymczasowy = {dystans+1000, predkosc , defultPlayerX + (rand() % polSzerokosciDrogi * 2) - polSzerokosciDrogi , rand() % 255, rand() % 255, rand() % 255 };
        samochodyNaDrodze[liczbaSamochodow-1] = tezTymczasowy;

        //printf("\n %f %f %d \n%f\n\n", tezTymczasowy.dystans,tezTymczasowy.predkosc,tezTymczasowy.x,dystans);
    }

    if (skrecenieDrogi[0] && skrecenieDrogi[1] + yGracza < dystans+wysokosc && skrecenieDrogi[3] > dystans+wysokosc+yGracza && predkosc > 10)
    {
        if (skrecenieDrogi[2] == 1) xG += (int)(mocSkretu * -1.0f) * predkosc / 100.0f;
        else xG -= (int)(mocSkretu * -1.0f) * predkosc / 100.0f;
    }

    int liczbaNigerKonias = 0;

    for (int i = 1; i < liczbaSamochodow; i++)
    {
        samochodyNaDrodze[i].dystans+=samochodyNaDrodze[i].predkosc/2;

        if ((samochodyNaDrodze[i].dystans < dystans + resolutionY * 0.10f && samochodyNaDrodze[i].dystans > dystans - resolutionY * 0.10f) &&
        (samochodyNaDrodze[i].x < xG + resolutnionX * 0.1 && samochodyNaDrodze[i].x > xG - resolutnionX * 0.1   )  )
        {
            liczbaNigerKonias++;
        }
    }

    system("clear");

    printf("P: %f %f, N: %d \n\n", dystans, predkosc, liczbaNigerKonias);
    for (int i = 1; i < liczbaSamochodow; i++)
    {
        printf("%d: %f %f\n", i, samochodyNaDrodze[i].dystans, samochodyNaDrodze[i].predkosc );
    }

}

int main()
{
    srand(time(NULL));
    SDL_Init(SDL_INIT_VIDEO);

    byte textureOpona[oponaH*2][oponaW];

    state = SDL_GetKeyboardState(NULL);

    samochodyNaDrodze = malloc(liczbaSamochodow * sizeof(samochod));

    for (int i = 0; i < 94; i++)
    {
        trawa[i].y = wysokosc + (rand() % (int)(resolutionY - wysokosc));
        trawa[i].x = (rand() % resolutnionX  ) + xG;
        trawa[i].side = (rand  () % 3 ) - 1;
    }


    for (int y = 0; y < oponaH*2; y++ ) for (int x = 0; x < oponaW; x++) textureOpona[y][x] = rand() % 255;
    

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
                if (state[SDL_SCANCODE_D] && xG < polSzerokosciDrogi * 6) xG+=(resolutnionX/160);
                if (state[SDL_SCANCODE_A] && xG > polSzerokosciDrogi * -2 ) xG-=(resolutnionX/160);
                if (state[SDL_SCANCODE_W] )
                {
                    czyWcisnolem = 50;
                    czyWcisnietyW++;

                    if (predkosc < 160) przyspieszenie += 0.10 + (float)predkosc/10;
                    else if (przyspieszenie - predkosc/100 > 0) przyspieszenie -= (float)predkosc/150;
                    else if (!(rand() % prawdopodobienstwo)) {
                        predkosc+=0.1;
                        prawdopodobienstwo+=10;
                    }
                    predkosc += przyspieszenie/100;  
                } 
                else if (state[SDL_SCANCODE_S]) predkosc--;
                else if (state[SDL_SCANCODE_PAGEDOWN]) dystans -= 100;
                
            }
        }

        if (czyWcisnietyW == czyWcisnietePrzed) prawdopodobienstwo = 100;
        if (czyWcisnolem > 0) czyWcisnolem--;
        
        gameMechaniks((rand() % 100));

        dystans+= predkosc/2;

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
            if (i == wysokosc && skrecenieDrogi[3] < dystans+wysokosc*2) skrecenieDrogi[0] = FALSE;
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

        ofsetObl = 1;

        int ofsetNaWysokosci = -1;

        int ofset = 0;

        for (int i = resolutionY; wysokosc*-1 <= i; i--)
        {
            if (skrecenieDrogi[0] && skrecenieDrogi[1] + i < dystans+wysokosc && skrecenieDrogi[3] > dystans+wysokosc+i) ofsetObl +=  (ofsetObl/(resolutnionX/8.0f) * mocSkretu );
            if (i == wysokosc && skrecenieDrogi[3] < dystans+wysokosc*2) skrecenieDrogi[0] = FALSE;

            if (i >= wysokosc) ofset = ofsetObl * skrecenieDrogi[2];

            for (int s = 1; s < liczbaSamochodow; s++)
            {
                if ((int)samochodyNaDrodze[s].dystans + i >= (int)dystans+wysokosc - 3 &&
                    (int)samochodyNaDrodze[s].dystans + i <= (int)dystans+wysokosc + 3)
                {
                    int dX = samochodyNaDrodze[s].x;
                    float szerokosc = resolutnionX * 0.2;

                    int renderowany = i;

                    if (renderowany < wysokosc) renderowany = wysokosc;
                    
                    SDL_Rect sdkrejty2 = {(dX - szerokosc / 2) - (xG-defultPlayerX+ofset) ,renderowany ,szerokosc / 2,resolutionY * 0.10f };
                    SDL_SetRenderDrawColor(meowRender,samochodyNaDrodze[s].r, samochodyNaDrodze[s].g, samochodyNaDrodze[s].b,0);
                    SDL_RenderFillRect(meowRender, &sdkrejty2);


                }
            }
        }

        SDL_Rect sdkrejty[] = {
        {resolutnionX * 0.4f,yGracza ,resolutnionX * 0.2f,resolutionY * 0.10f },
        {resolutnionX * 0.41f,resolutionY * 0.85f ,oponaW,oponaH },
        {resolutnionX * 0.57f,resolutionY * 0.85f ,oponaW,oponaH }};

        SDL_SetRenderDrawColor(meowRender,110, 14, 0,0);
        SDL_RenderFillRect(meowRender, &sdkrejty[0]);
        SDL_SetRenderDrawColor(meowRender,0, 0, 0,0);
        for (int i = 1; i < 3; i++) SDL_RenderFillRect(meowRender, &sdkrejty[i]);
        SDL_RenderDrawLine(meowRender, resolutnionX * 0.4f,yGracza,resolutnionX * 0.43f,resolutionY * 0.70f );
        SDL_RenderDrawLine(meowRender, resolutnionX * 0.6f,yGracza,resolutnionX * 0.57f,resolutionY * 0.70f );
        SDL_RenderDrawLine(meowRender,resolutnionX * 0.43f,resolutionY * 0.70f,resolutnionX * 0.57f,resolutionY * 0.70f);


        float punktStartowyOF = ((wcC((int)dystans,10) + wcC((int)dystans,1) + wcC((int)dystans,100)) / 999.0f) * oponaH * 2;
        int punktStartowyTexturyOpony = (int)punktStartowyOF;

        SDL_SetRenderDrawColor(meowRender,28, 26, 26,0);


        for (int zgg = 0; zgg < oponaH; zgg++)
        {
            punktStartowyTexturyOpony++;
            if (punktStartowyTexturyOpony >= oponaH * 2) punktStartowyTexturyOpony = 0;
            
            for (int x = 0; x < oponaW; x++)
            {
                if (textureOpona[punktStartowyTexturyOpony][x] > 210 )
                {
                    SDL_RenderDrawPoint(meowRender, resolutnionX * 0.41f + x,resolutionY * 0.85f + zgg);
                    SDL_RenderDrawPoint(meowRender, resolutnionX * 0.57f + x,resolutionY * 0.85f + zgg);
                }
            }
        }

        byte iloscCyfr;
        int predkoscTutajObl = (int)(predkosc / 2);
        for (iloscCyfr = 0; predkoscTutajObl != 0; iloscCyfr++) predkoscTutajObl/=10;
        if (!iloscCyfr) iloscCyfr++;
        byte cyfry[iloscCyfr]; 

        predkoscTutajObl = (int)(predkosc / 2);
        for (int i = iloscCyfr -1 ; i >= 0; i--)
        {
            cyfry[i] = predkoscTutajObl %10;
            predkoscTutajObl/=10;
        }

        SDL_SetRenderDrawColor(meowRender,255,255,255,0);
        int obliczanie = 0;

        for (int i = 0; i < iloscCyfr; i++)
        {
            for (int y = 0; y < 30; y++)
            {
                for (int x = 0; x < 20; x++)
                {
                    int indeksBajtu = (y * 3) + (x / 8);
                    int numerBitu = 7 - (x % 8);

                    if (!((teksturyLiczb[ cyfry[i] ][indeksBajtu] >> numerBitu) & 1)) {
                        SDL_RenderDrawPoint(meowRender,x+obliczanie,y);
                    }
                }
            }

            obliczanie += 25;
        }

        SDL_RenderPresent(meowRender); 
        SDL_Delay(60);
    }
    SDL_DestroyRenderer (meowRender);
    SDL_DestroyWindow (meowOkno);


    free(samochodyNaDrodze);
}

