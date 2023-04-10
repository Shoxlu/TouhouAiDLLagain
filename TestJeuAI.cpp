
#include <iostream>
#include <time.h>
#include <d3d9.h>
#include <d2d1.h>
#include <wincodec.h>
#include <Windows.h>
#include "GenerationPerso.h"
#include "Personnage.h"
#include "Reseau.h"
#include "Hidden_neurone.h"
#include "Input.h"
#include "TestJeuAi.h"
#include "utils.h"


#pragma comment(lib, "d2d1")
#pragma comment(lib, "windowscodecs")

using namespace std;


struct boolTextures {
    IDirect3DTexture9* Active;
    IDirect3DTexture9* notActive;
};
struct boolTextures2d {
    ID2D1Bitmap* Active;
    ID2D1Bitmap* notActive;
};
// Global variables
HWND hWnd;
ID2D1Factory* pD2DFactory = NULL;
ID2D1HwndRenderTarget* pRenderTarget = NULL;
IWICImagingFactory* pIWICFactory;
ID2D1Bitmap* bitmap_test;
boolTextures2d sprite_Input2d;
boolTextures2d sprite_Hidden_layer2d;



LPDIRECT3D9 d3d;    // Direct3D object
LPDIRECT3DDEVICE9 d3ddev;    // Direct3D device object
int NbreEnemy = 60;
const int NbrePerso_generation = 500;
GenerationJoueur* generation;
Joueur** joueur;
int64_t frame_counter = 0;
int frame_per_second = 30;
IDirect3DTexture9* digitTextures[10];
boolTextures InputTextures[1];
boolTextures Hidden_layer_texture;

#ifndef screenHeight
#define screenHeight 720
#endif
#ifndef screenWidth
#define screenWidth 1280
#endif
#define magic_number 38.8198

// Function prototypes
void initD3D(HWND hWnd);
void cleanD3D(void);
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void create_console();


HRESULT DLoadBitmapFromFile(
    ID2D1RenderTarget* pRenderTarget,
    IWICImagingFactory* pIWICFactory,
    PCWSTR uri,
    UINT destinationWidth,
    UINT destinationHeight,
    ID2D1Bitmap** ppBitmap
);



// The entry point for any Windows program
int main()
{
    srand(time(0));
    // Initialize Direct3D

    init_direct2d();
   
    create_console();
    
    // Enter the main loop
    MSG msg;

    generation = new GenerationJoueur(NbrePerso_generation);
    joueur = generation->get_joueurs();
    
    time_t start = time(0);
    int a = 1;
    time_t elapsed = 0;
    test1();
    FreeConsole();
    return 0;
}


void init_direct2d()
{
    HRESULT hr = D2D1CreateFactory(
        D2D1_FACTORY_TYPE_MULTI_THREADED,
        &pD2DFactory
    );
    // Obtain the size of the drawing area.
    RECT rc;
    GetClientRect(hWnd, &rc);

    // Create a Direct2D render target          

    hr = pD2DFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(
            hWnd,
            D2D1::SizeU(
                rc.right - rc.left,
                rc.bottom - rc.top)
        ),
        &pRenderTarget);

    hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, reinterpret_cast<void**>(&pIWICFactory));
}

void test1()
{

    MSG msg;
    time_t start = time(0);
    time_t elapsed;
    load_Inputs_2dsprites();
    float fps = 1 / 60;
    while (1)
    {
        elapsed = time(0) - start;
        if(elapsed >= fps)
        {
            start = time(0);
            
            generation->update();

            if (GetKeyState(VK_F1) == 1)
            {
                render_frame2d();
            }

            frame_counter += 1;
        }
        
    }
    pRenderTarget->EndDraw();
    pD2DFactory->Release();
    pD2DFactory = NULL;
    pRenderTarget->Release();
    pRenderTarget = NULL;
}





void load_Inputs_2dsprites()
{
    DLoadBitmapFromFile(pRenderTarget, pIWICFactory, L"Input1.png", 50, 50, &sprite_Input2d.Active);
    DLoadBitmapFromFile(pRenderTarget, pIWICFactory, L"Input0.png", 50, 50, &sprite_Input2d.notActive);
    DLoadBitmapFromFile(pRenderTarget, pIWICFactory, L"SousRes0.png", 50, 50, &sprite_Hidden_layer2d.Active);
    DLoadBitmapFromFile(pRenderTarget, pIWICFactory, L"SousRes1.png", 50, 50, &sprite_Hidden_layer2d.notActive);
}



// This is the function that draws the sprite

void draw_2dsprite(ID2D1Bitmap* sprite, Pos pos)
{
    
    D2D1_SIZE_F size = sprite->GetSize();
    D2D1_POINT_2F upperLeftCorner = D2D1::Point2F(pos.x, pos.y);
    pRenderTarget->DrawBitmap(sprite,
        D2D1::RectF(
            upperLeftCorner.x,
            upperLeftCorner.y,
            upperLeftCorner.x + size.width,
            upperLeftCorner.y + size.height),
        1.0f,
        D2D1_BITMAP_INTERPOLATION_MODE_LINEAR
    );
   
}

void render_frame2d()
{
    LARGE_INTEGER start;
    LARGE_INTEGER end;
    QueryPerformanceCounter(&start);
    pRenderTarget->BeginDraw();
    pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

    //Draw the players sprites
    for (int i = 0; i < generation->get_n_systemes(); i++)
    {
        if (GetKeyState(VK_SPACE) == 1)
        {
            draw_reseau2d(joueur[i]->get_reseau());
        }
       // draw_2dsprite(joueur[i]->get_sprite1(), Pos{ joueur[i]->get_x() - 25, joueur[i]->get_y() - 25 });
    }
    //draw_2dsprite(bitmap_test, Pos{random_float() * 1000, 0});
    
    
    pRenderTarget->EndDraw();
    QueryPerformanceCounter(&end);
    //cout << end.QuadPart - start.QuadPart << "\n";
}
void draw_reseau2d(Reseau* reseau)
{

    //for (int i = 0; i < reseau->get_n_Hidden_layer(); i++)
    //{
    //    Hidden_neurone* hidden_neurone = reseau->get_Hidden_layer(i);
    //    Joueur* joueur = reseau->get_joueur();
    //    //pRenderTarget->DrawLine(D2D1_POINT_2F(joueur->get_x(), joueur->get_y()), D2D1_POINT_2F(Hidden_layer->get_pos().x, Hidden_layer->get_pos().y), );
    //    draw_Hidden_layer2d(hidden_neurone);
    //}
}


void draw_Hidden_layer2d(Hidden_layer* Hidden_layer)
{
    /*for (int i = 0; i < Hidden_layer->get_n_Inputs(); i++)
    {
        Pos pos = Hidden_layer->get_pos();
        draw_Input2d(Hidden_layer->get_Input(i), &pos);
    }

    if (Hidden_layer->isActive())
    {
        draw_2dsprite(sprite_Hidden_layer2d.Active, Pos{ Hidden_layer->get_pos().x - 12.5, Hidden_layer->get_pos().y - 12 });
        return;
    }
    draw_2dsprite(sprite_Hidden_layer2d.notActive, Pos{ Hidden_layer->get_pos().x - 12.5, Hidden_layer->get_pos().y - 12 });*/

}

void draw_Input2d(Input* Input, Pos* pos_milieu_connex)
{
    //dessine les connexions, flemme de le mettre dans une autre fonction
    //draw_line(D3DXVECTOR2(Input->get_pos().x, Input->get_pos().y), D3DXVECTOR2(pos_milieu_connex->x, pos_milieu_connex->y), D3DCOLOR_XRGB(255, 255, 0));
    //dessine le Input
    /*if (Input->isActive())
    {
        draw_2dsprite(sprite_Input2d.Active, Pos{ Input->get_pos().x - 25, Input->get_pos().y - 25 });
        return;
    }
    draw_2dsprite(sprite_Input2d.notActive, Pos{ Input->get_pos().x - 25, Input->get_pos().y - 25 });*/
}


// Clean up Direct3D
void cleanD3D(void)
{
    d3ddev->Release();
    d3d->Release();
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

        // Other message handling goes here

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

void create_console()
{
    AllocConsole();

    // Redirect the standard input and output streams to the console
    FILE* stream;
    freopen_s(&stream, "CONIN$", "r", stdin);
    freopen_s(&stream, "CONOUT$", "w", stdout);
    freopen_s(&stream, "CONOUT$", "w", stderr);
}
