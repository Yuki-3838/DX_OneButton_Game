#include "StageScene.h"

void StageScene::Init()
{
	// カメラ生成
	m_pCamera = new Camera(1920,1080);
	m_pCamera->SetPosition(0.0f, 0.0f);

	// テクスチャ読み込み
	m_pPlayerTex = m_pResourceManager->LoadTexture("asset/texture/title.png",m_pRenderer->GetDevice());
	m_pFloorTex = m_pResourceManager->LoadTexture("title.png", m_pRenderer->GetDevice());

	// プレイヤー生成と初期化
	m_pPlayer = new Character();
	m_pPlayer->Init(m_pPlayerTex);
	m_pPlayer->SetPosition(100.0f, 100.0f);

	// 床オブジェクト生成と初期化
	m_pFloor = new GameObject();
	m_pFloor->Init(m_pFloorTex);
	m_pFloor->SetPosition(0.0f, 300.0f);
	m_pFloor->SetSize(800.0f, 50.0f);
}

void StageScene::Update()
{
	// スペースキーが押されたらシーン終了フラグを立てる
	if (m_pInput->GetKeyTrigger(VK_SPACE))
	{
		m_IsFinished = true;
	}
}

void StageScene::Draw()
{
	// 背景を黒でクリア
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_pRenderer->StartFrame(clearColor);

	//タイトル画像を画面中央付近に表示
	DirectX::XMMATRIX viewProj = m_pCamera->GetViewProjection();
	if (m_pPlayer)
	{
		//背景オブジェクトに描画を任せる
		m_pSpriteRenderer->Draw(m_pRenderer->GetContext(), m_pPlayerTex, 0.0f, 0.0f, 1000, 1000, viewProj);
	}
	m_pRenderer->EndFrame();
}

void StageScene::Uninit()
{
	if (m_pCamera) { delete m_pCamera; m_pCamera = nullptr; }
	if (m_pPlayer) { delete m_pPlayer; m_pPlayer = nullptr; }
	if (m_pFloor) { delete m_pFloor; m_pFloor = nullptr; }
}
