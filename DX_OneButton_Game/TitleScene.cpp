#include "TitleScene.h"
#include "Game.h"

void TitleScene::Init()
{
	m_pCamera = new Camera(Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT);
    m_pCamera->SetPosition(0.0f, 0.0f);
	// タイトル画面用テクスチャの読み込み
	m_pTitleTex = m_pResourceManager->LoadTexture("asset/texture/title.png", m_pRenderer->GetDevice());

	m_IsFinished = false;
}

void TitleScene::Update()
{
	// スペースキーが押されたら次のシーン（Stage1）へ
	if (m_pInput->GetKeyTrigger(VK_LBUTTON))
	{
		m_IsFinished = true;
	}
}

void TitleScene::Draw()
{
    // 背景を黒でクリア
    float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    m_pRenderer->StartFrame(clearColor);

     //タイトル画像を画面中央付近に表示
    DirectX::XMMATRIX viewProj = m_pCamera->GetViewProjection();
    if (m_pTitleTex) 
    {
        float leftX = -(Game::SCREEN_WIDTH / 2.0f);
        float topY = -(Game::SCREEN_HEIGHT / 2.0f);
         //背景オブジェクトに描画を任せる
        m_pSpriteRenderer->Draw(m_pRenderer->GetContext(), m_pTitleTex, leftX, topY, Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT, viewProj);
    }
    m_pRenderer->EndFrame();
}

void TitleScene::Uninit()
{
    if (m_pCamera) { delete m_pCamera; m_pCamera = nullptr; }
}
