#pragma once
#include "Scene.h"
#include "Character.h"
#include "GameObject.h"
#include "Camera.h"

class StageScene : public Scene
{
private:
    Camera* m_pCamera;          // カメラ

    Character* m_pPlayer;       // プレイヤー
    GameObject* m_pFloor;       // 床（シンプルなGameObjectとして扱う）

    // テクスチャ管理用
    ID3D11ShaderResourceView* m_pPlayerTex;
    ID3D11ShaderResourceView* m_pFloorTex;

    bool m_IsFinished;          // ゲーム終了フラグ

public:
    // コンストラクタ：親クラスのコンストラクタを呼び出す
    StageScene(Renderer* renderer, ResourceManager* resManager, SpriteRenderer* spriteRenderer, Input* input)
        : Scene(renderer, resManager, spriteRenderer, input)
    {
        m_pCamera = nullptr;
        m_pPlayer = nullptr;
        m_pFloor = nullptr;
        m_pPlayerTex = nullptr;
        m_pFloorTex = nullptr;
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