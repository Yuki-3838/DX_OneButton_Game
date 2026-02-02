#include "TitleScene.h"

void TitleScene::Init()
{
	m_pCamera = new Camera(1920, 1080);
	// タイトル画面用テクスチャの読み込み
	m_pTitleTex = m_pResourceManager->LoadTexture("asset/texture/title.png", m_pRenderer->GetDevice());

	m_IsFinished = false;
}

void TitleScene::Update()
{
	// スペースキーが押されたら次のシーン（Stage1）へ
	if (m_pInput->GetKeyTrigger(VK_RETURN))
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
         //背景オブジェクトに描画を任せる
        m_pSpriteRenderer->Draw(m_pRenderer->GetContext(), m_pTitleTex,0.0f,0.0f,1920,1080, viewProj);
    }
    m_pRenderer->EndFrame();
}

void TitleScene::Uninit()
{
    if (m_pCamera) { delete m_pCamera; m_pCamera = nullptr; }
}
