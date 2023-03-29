#pragma once

#include "resource.h"

class Input;
struct Pos;
class Reseau;
class Hidden_layer;


void test1();
void draw_2d(Input* Input, Pos* pos_milieu_connex);
void draw_reseau2d(Reseau* reseau);
void draw_Hidden_layer2d(Hidden_layer* Hidden_layer);
void render_frame2d();
void init_direct2d();
void draw_2dsprite(ID2D1Bitmap* sprite, Pos pos);
void load_Inputs_2dsprites();

HRESULT DLoadBitmapFromFile(
    ID2D1RenderTarget* pRenderTarget,
    IWICImagingFactory* pIWICFactory,
    PCWSTR uri,
    UINT destinationWidth,
    UINT destinationHeight,
    ID2D1Bitmap** ppBitmap
);
