#include "StageScene.h"
#include "Collision.h"
#include "GameData.h"
#include "Game.h"

void StageScene::Init()
{
	// カメラ生成
	m_pCamera = new Camera(Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT);
	m_pCamera->SetPosition(0.0f, 0.0f);

	m_pTex = m_pResourceManager->LoadTexture("asset/texture/title.png",m_pRenderer->GetDevice());

	//UIの初期化
	m_pUI = new GameUI();
	m_pUI->Init(m_pRenderer->GetDevice(), m_pResourceManager);

	//スコアのリセット
	GameData::Reset();
	
	//発射台(画面中央)
	m_pLauncher = new GameObject;
	m_pLauncher->Init(m_pTex);

    float bottomY = Game::SCREEN_HEIGHT / 2.0f;
	m_pLauncher->SetPosition(0.0f, bottomY - 50.0f);
	m_pLauncher->SetSize(64.0f, 64.0f);

}

void StageScene::Update()
{
    const int MAX_MISSILE_COUNT = 48;

    float bottomY = Game::SCREEN_HEIGHT / 2.0f;

    // ----------------------------------------------------------------
    // 1. 入力処理
    // ----------------------------------------------------------------
    if (m_pInput->GetKeyTrigger(VK_LBUTTON) || m_pInput->GetKeyTrigger(VK_SPACE))
    {
        // 画面にミサイルが1つもないなら「新規発射」
        if (m_Missiles.empty())
        {
            Missile* main = new Missile(Missile::Type::MAIN, 0);
            main->Init(m_pTex);
            main->SetPosition(0.0f, bottomY - 50.0f);
            main->SetSize(20.0f, 80.0f);
            main->SetVelocity(0.0f, -10.0f);
            m_Missiles.push_back(main);
        }
        else
        {
            // --------------------------------------------------------
            //分裂処理
            // --------------------------------------------------------

            //ここがポイント：現在のミサイル数が上限未満なら分裂許可
            if (m_Missiles.size() < MAX_MISSILE_COUNT)
            {
                std::vector<Missile*> nextMissiles; // 新ミサイルの一時置き場

                for (auto it = m_Missiles.begin(); it != m_Missiles.end(); )
                {
                    Missile* m = *it;
                    int currentGen = m->GetGeneration();

                    // 座標取得
                    DirectX::XMFLOAT2 pos = m->GetPosition();
                    bool split = false;

                    // ★もう世代チェック(currentGen < MAX)はしません。無条件で分裂させます。

                    // --- パターン1: MAIN(上) or VERTICAL(上下) -> 左右へ ---
                    if (m->GetType() == Missile::Type::MAIN || m->GetType() == Missile::Type::VERTICAL)
                    {
                        Missile* left = new Missile(Missile::Type::HORIZONTAL, currentGen + 1);
                        left->Init(m_pTex);
                        left->SetPosition(pos.x, pos.y);
                        left->SetSize(80.0f, 20.0f);
                        left->SetVelocity(-12.0f, 0.0f);
                        nextMissiles.push_back(left);

                        Missile* right = new Missile(Missile::Type::HORIZONTAL, currentGen + 1);
                        right->Init(m_pTex);
                        right->SetPosition(pos.x, pos.y);
                        right->SetSize(80.0f, 20.0f);
                        right->SetVelocity(12.0f, 0.0f);
                        nextMissiles.push_back(right);

                        split = true;
                    }
                    // --- パターン2: HORIZONTAL(左右) -> 上下へ ---
                    else if (m->GetType() == Missile::Type::HORIZONTAL)
                    {
                        Missile* up = new Missile(Missile::Type::VERTICAL, currentGen + 1);
                        up->Init(m_pTex);
                        up->SetPosition(pos.x, pos.y);
                        up->SetSize(20.0f, 80.0f);
                        up->SetVelocity(0.0f, -12.0f);
                        nextMissiles.push_back(up);

                        Missile* down = new Missile(Missile::Type::VERTICAL, currentGen + 1);
                        down->Init(m_pTex);
                        down->SetPosition(pos.x, pos.y);
                        down->SetSize(20.0f, 80.0f);
                        down->SetVelocity(0.0f, 12.0f);
                        nextMissiles.push_back(down);

                        split = true;
                    }

                    // 分裂したら元のミサイルを削除
                    if (split) 
                    {
                        delete m;
                        it = m_Missiles.erase(it);
                    }
                    else
                    {
                        ++it;
                    }
                }

                // 新しく生まれたミサイルを追加
                for (auto* nm : nextMissiles) 
                {
                    m_Missiles.push_back(nm);
                }
            }
        }
    }

    // ----------------------------------------------------------------
    // 2. 敵の生成
    // ----------------------------------------------------------------
    if (rand() % 100 < 3)
    {
        float spawnY = -(Game::SCREEN_HEIGHT / 2.0f)- 50.0f;
        float rangeX = (float)Game::SCREEN_WIDTH - 50.0f;

        Enemy* e = new Enemy();
        e->Init(m_pTex);
        e->SetPosition((float)(rand() % (int)rangeX - (rangeX / 2)), spawnY);
        e->SetSize(50.0f, 50.0f);
        m_Enemies.push_back(e);
    }

    // ----------------------------------------------------------------
    // 3. 更新と画面外削除
    // ----------------------------------------------------------------
    
    float limitX = (Game::SCREEN_WIDTH / 2.0f) + 50.0f;
    float limitY = (Game::SCREEN_HEIGHT / 2.0f) + 50.0f;
    for (auto it = m_Missiles.begin(); it != m_Missiles.end(); )
    {
        (*it)->Update();
        DirectX::XMFLOAT2 p = (*it)->GetPosition();
        if (p.y < -limitY || p.y > limitY || p.x < -limitX || p.x > limitX)
        {
            delete* it;
            it = m_Missiles.erase(it);
        }
        else it++;
    }

    for (auto it = m_Enemies.begin(); it != m_Enemies.end(); )
    {
        (*it)->Update();
        if ((*it)->GetPosition().y > limitY)
        {
            delete* it;
            it = m_Enemies.erase(it);
        }
        else it++;
    }

    // ----------------------------------------------------------------
    // 4. 当たり判定（貫通）
    // ----------------------------------------------------------------
    for (auto itM = m_Missiles.begin(); itM != m_Missiles.end(); ++itM)
    {
        for (auto itE = m_Enemies.begin(); itE != m_Enemies.end(); )
        {
            if (Col::Any(CollisionRect(**itM, **itE)))
            {
                delete* itE;
                itE = m_Enemies.erase(itE);
                GameData::AddScore(100);
            }
            else
            {
                itE++;
            }
        }
    }
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

	DirectX::XMMATRIX viewProj = m_pCamera->GetViewProjection();

	//発射台の描画
	m_pLauncher->Draw(m_pRenderer->GetContext(), m_pSpriteRenderer,viewProj);

	//ミサイルと敵の描画
	for (auto m : m_Missiles)
	{
		m->Draw(m_pRenderer->GetContext(), m_pSpriteRenderer, viewProj);
	}
	for(auto e : m_Enemies)
	{
		e->Draw(m_pRenderer->GetContext(), m_pSpriteRenderer, viewProj);
	}
    // スコア表示
    m_pUI->DrawNumber(m_pRenderer->GetContext(), m_pSpriteRenderer, GameData::GetScore(), -900.0f, -500.0f, 0.4f);
	m_pRenderer->EndFrame();
}

void StageScene::Uninit()
{
	if (m_pCamera) { delete m_pCamera; m_pCamera = nullptr; }
	for(auto m : m_Missiles) delete m;
	m_Missiles.clear();
	for(auto e : m_Enemies) delete e;
	m_Enemies.clear();

}
