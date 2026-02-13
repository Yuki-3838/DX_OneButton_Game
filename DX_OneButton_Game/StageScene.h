#pragma once
#include "Scene.h"
#include "Character.h"
#include "GameObject.h"
#include "Camera.h"
#include "Missile.h"
#include "Enemy.h"
#include <vector>
#include "GameUI.h"

class StageScene : public Scene
{
private:
    Camera* m_pCamera;          // カメラ
    GameUI* m_pUI;          // ゲームUI

    // テクスチャ管理用
    ID3D11ShaderResourceView* m_pBgTex;
    ID3D11ShaderResourceView* m_pLauncherTex;

    bool m_IsFinished;          // ゲーム終了フラグ

    GameObject* m_pLauncher;    // ミサイル発射台
    ID3D11ShaderResourceView* m_pTex;
    std::vector<Missile*> m_Missiles; // ミサイルのリスト
    std::vector<Enemy*> m_Enemies; //敵管理リスト

public:
    // コンストラクタ：親クラスのコンストラクタを呼び出す
    StageScene(Renderer* renderer, ResourceManager* resManager, SpriteRenderer* spriteRenderer, Input* input)
        : Scene(renderer, resManager, spriteRenderer, input)
    {
        m_pCamera = nullptr;
        m_pPlayerTex = nullptr;
        m_pFloorTex = nullptr;
        m_pTex = nullptr;
        m_IsFinished = false;

        Init(); // 初期化実行
    }

    ~StageScene() { Uninit(); }

    void Init() override;
    void Update() override;
    void Draw() override;
    void Uninit() override;

    // シーン遷移判定用
    bool ShouldChangeScene() const override { return m_IsFinished; }
};