#include <gccore.h>
#include <grrlib.h>
#include <wiiuse/wpad.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Načítanie vygenerovaných obrázkov
#include "alza.h"
#include "mall.h"
#include "czc.h"
#include "datart.h"
#include "nay.h"
#include "amazon.h"

#include "chladnicka.h"
#include "tv.h"
#include "konzola.h"
#include "hriankovac.h"
#include "pracka.h"
#include "vysavac.h"

#define LANG_COUNT 6
#define SHOP_COUNT 6
#define PROD_COUNT 6

enum Language { SLOVENCINA, CESTINA, POLSTINA, ANGLICTINA, JAPONCINA, KOREJCINA };
enum OrderMode { REAL_ORDERING, FAKE_ORDERING };
enum Shop { ALZA, MALL, CZC, DATART, NAY, AMAZON };
enum ScreenState { SCREEN_LANGUAGE, SCREEN_MODE, SCREEN_SHOP, SCREEN_CATALOG, SCREEN_ORDER_CONFIRM };

typedef struct {
    char name[LANG_COUNT][30]; // Oprava polí pre 6 jazykov a 30 znakov
    GRRLIB_texImg *image;
    int base_price;
} FakeProduct;

int main() {
    GRRLIB_Init();
    WPAD_Init();

    enum ScreenState currentState = SCREEN_LANGUAGE;
    enum Language selectedLang = SLOVENCINA;
    enum Shop selectedShop = ALZA;
    int selectedProduct = 0;

    // Načítanie zabudovaného fontu z GRRLIB
    GRRLIB_texImg *tex_font = GRRLIB_CreateEmptyTexture(1, 1); 

    GRRLIB_texImg *tex_shops[SHOP_COUNT];
    tex_shops[ALZA]   = GRRLIB_LoadTexturePNG(alza);
    tex_shops[MALL]   = GRRLIB_LoadTexturePNG(mall);
    tex_shops[CZC]    = GRRLIB_LoadTexturePNG(czc);
    tex_shops[DATART] = GRRLIB_LoadTexturePNG(datart);
    tex_shops[NAY]    = GRRLIB_LoadTexturePNG(nay);
    tex_shops[AMAZON] = GRRLIB_LoadTexturePNG(amazon);

    for(int i = 0; i < SHOP_COUNT; i++) {
        if(tex_shops[i]) {
            tex_shops[i]->handlex = tex_shops[i]->w / 2;
            tex_shops[i]->handley = tex_shops[i]->h / 2;
        }
    }

    GRRLIB_texImg *img_chladnicka  = GRRLIB_LoadTexturePNG(chladnicka);
    GRRLIB_texImg *img_tv          = GRRLIB_LoadTexturePNG(tv);
    GRRLIB_texImg *img_konzola     = GRRLIB_LoadTexturePNG(konzola);
    GRRLIB_texImg *img_hriankovac  = GRRLIB_LoadTexturePNG(hriankovac);
    GRRLIB_texImg *img_pracka      = GRRLIB_LoadTexturePNG(pracka);
    GRRLIB_texImg *img_vysavac     = GRRLIB_LoadTexturePNG(vysavac);

    FakeProduct products[PROD_COUNT];

    // Nastavenie produktu Wii konzoly (Produkt č. 2)
    strcpy(products[2].name[SLOVENCINA], "Wii Konzola");
    strcpy(products[2].name[ANGLICTINA], "Wii Console");
    products[2].image = img_konzola;
    products[2].base_price = 100;

    for(int i = 0; i < PROD_COUNT; i++) {
        if(i != 2) {
            strcpy(products[i].name[SLOVENCINA], "Elektro Tovar");
            strcpy(products[i].name[ANGLICTINA], "Electronics");
            products[i].image = img_chladnicka;
            products[i].base_price = 120;
        }
    }

    while(1) {
        WPAD_ScanPads();
        u32 pressed = WPAD_ButtonsDown(0);

        if (pressed & WPAD_BUTTON_HOME) break;

        switch(currentState) {
            case SCREEN_LANGUAGE:
                if (pressed & WPAD_BUTTON_RIGHT) selectedLang = (selectedLang + 1) % LANG_COUNT;
                if (pressed & WPAD_BUTTON_A) currentState = SCREEN_MODE;
                break;

            case SCREEN_MODE:
                if (pressed & WPAD_BUTTON_A) currentState = SCREEN_SHOP;
                break;

            case SCREEN_SHOP:
                if (pressed & WPAD_BUTTON_RIGHT) selectedShop = (selectedShop + 1) % SHOP_COUNT;
                if (pressed & WPAD_BUTTON_LEFT)  selectedShop = (selectedShop - 1 + SHOP_COUNT) % SHOP_COUNT;
                if (pressed & WPAD_BUTTON_1)     selectedShop = rand() % SHOP_COUNT;

                if(tex_shops[selectedShop]) {
                    GRRLIB_DrawImg(320, 240, tex_shops[selectedShop], 0, 1.5f, 1.5f, 0xFFFFFFFF);
                }

                if (pressed & WPAD_BUTTON_A) currentState = SCREEN_CATALOG;
                break;

            case SCREEN_CATALOG:
                if (pressed & WPAD_BUTTON_DOWN) selectedProduct = (selectedProduct + 1) % PROD_COUNT;
                if (pressed & WPAD_BUTTON_UP)   selectedProduct = (selectedProduct - 1 + PROD_COUNT) % PROD_COUNT;

                if (products[selectedProduct].image) {
                    GRRLIB_DrawImg(320, 200, products[selectedProduct].image, 0, 1.2f, 1.2f, 0xFFFFFFFF);
                }

                int final_price = products[selectedProduct].base_price;
                if (selectedProduct == 2) {
                    switch(selectedShop) {
                        case ALZA:   final_price = 100; break;
                        case MALL:   final_price = 65;  break;
                        case CZC:    final_price = 71;  break;
                        case NAY:    final_price = 125; break;
                        case DATART: final_price = 115; break;
                        case AMAZON: final_price = 150; break;
                    }
                }

                if (pressed & WPAD_BUTTON_A) currentState = SCREEN_ORDER_CONFIRM;
                if (pressed & WPAD_BUTTON_B) currentState = SCREEN_SHOP;
                break;

            case SCREEN_ORDER_CONFIRM:
                if (pressed & WPAD_BUTTON_B) currentState = SCREEN_CATALOG;
                break;
        }

        GRRLIB_Render();
    }

    for(int i = 0; i < SHOP_COUNT; i++) {
        if(tex_shops[i]) GRRLIB_FreeTexture(tex_shops[i]);
    }
    GRRLIB_FreeTexture(img_chladnicka); GRRLIB_FreeTexture(img_tv);
    GRRLIB_FreeTexture(img_konzola);    GRRLIB_FreeTexture(img_hriankovac);
    GRRLIB_FreeTexture(img_pracka);     GRRLIB_FreeTexture(img_vysavac);
    GRRLIB_FreeTexture(tex_font);

    GRRLIB_Exit();
    return 0;
}
