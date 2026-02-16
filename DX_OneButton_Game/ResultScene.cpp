#include "ResultScene.h"
#include "Game.h"
#include "GameData.h"

void ResultScene::Init()
{
	m_pCamera = new Camera(Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT);

	m_pUI = new GameUI();
	m_pUI->Init(m_pRenderer->GetDevice(), m_pResourceManager);
	// タイトル画面用テクスチャの読み込み
	m_pResultTex = m_pResourceManager->LoadTexture("asset/texture/Bg.png", m_pRenderer->GetDevice());

	m_IsFinished = false;
}

void ResultScene::Update()
{
	// スペースキーが押されたら次のシーン（Stage1）へ
	if (m_pInput->GetKeyTrigger(VK_LBUTTON))
	{
		m_IsFinished = true;
	}
}

void ResultScene::Draw()
{
	// 背景を黒でクリア
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_pRenderer->StartFrame(clearColor);

	//タイトル画像を画面中央付近に表示
	DirectX::XMMATRIX viewProj = m_pCamera->GetViewProjection();
	if (m_pResultTex)
	{
		float leftX = -(Game::SCREEN_WIDTH / 2.0f);
		float topY = -(Game::SCREEN_HEIGHT / 2.0f);
		//背景オブジェクトに描画を任せる
		m_pSpriteRenderer->Draw(m_pRenderer->GetContext(), m_pResultTex, leftX, topY, Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT, viewProj);
	}
	if (m_pUI)
	{
		// スコアの表示位置（画面中央の少し下あたり）
		// ※ リザルト画像のレイアウトに合わせて数値を微調整してください！
		float scoreX = -300.0f; // 左右の位置
		float scoreY = 0.0f;   // 上下の位置
		float scale = 0.7f;    // 文字の大きさ

		m_pUI->DrawNumber(m_pRenderer->GetContext(), m_pSpriteRenderer, GameData::GetScore(), scoreX, scoreY, scale);
	}
	m_pRenderer->EndFrame();
}

void ResultScene::Uninit()
{
	if (m_pCamera) { delete m_pCamera; m_pCamera = nullptr; }
	if (m_pUI) { m_pUI->Uninit(); delete m_pUI; m_pUI = nullptr; }
}
