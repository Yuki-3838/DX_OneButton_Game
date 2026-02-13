#pragma once
#include "Renderer.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "SceneManager.h"
#include "TitleScene.h"  // 最初のシーン用

class Game
{
private:
    // 主要システム
    Renderer* m_pRenderer;
    ResourceManager* m_pResourceManager;
    SpriteRenderer* m_pSpriteRenderer;
    SceneManager* m_pSceneManager;
    Input* m_pInput;

    // ウィンドウハンドル（Initで受け取る）
    HWND m_hWnd;

public:
    //画面サイズを定義
    static const int SCREEN_WIDTH = 1280;
	static const int SCREEN_HEIGHT = 720;
    Game();
    ~Game();

    void Init(HWND hWnd, int width, int height); // ウィンドウ情報を追加
    void Update();
    void Draw();
    void Uninit();
};