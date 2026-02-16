#pragma once
#include "Scene.h"
#include "GameUI.h"
class ResultScene :public Scene
{
    private:
        bool m_IsFinished;
        // タイトル画面用のテクスチャポインタを追加
        ID3D11ShaderResourceView* m_pResultTex;

        GameUI* m_pUI; // ゲームUI
    public:
        using Scene::Scene;
        void Init() override;
        void Update() override; // スペースキーが押されたらフラグを立てる
        void Draw() override;   // タイトル画像を描画
        void Uninit() override;
        bool ShouldChangeScene() const override { return m_IsFinished; }
};

