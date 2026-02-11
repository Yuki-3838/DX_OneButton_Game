#include "StageScene.h"
#include "Collision.h"
#include "GameData.h"

void StageScene::Init()
{
	// カメラ生成
	m_pCamera = new Camera(1920,1080);
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
	m_pLauncher->SetPosition(1920.0f / 2.0f - 32.0f, 900.0f);
	m_pLauncher->SetSize(64.0f, 64.0f);

}

void StageScene::Update()
{
    //入力と分裂
    if (m_pInput->GetKeyTrigger(VK_LBUTTON))
    {
        //画面内にミサイルがあるかを探す
        Missile* pMissile = nullptr;
        size_t index = 0;
        bool found = false;

        // 最初に見つかったミサイルを取得
        for (size_t i = 0; i < m_Missiles.size(); ++i)
        {
            pMissile = m_Missiles[i];
            index = i;
            found = true;
            break;
        }

        if (found)
        {
            // 分裂処理
            DirectX::XMFLOAT2 pos = pMissile->GetPosition();
            Missile::Type type = pMissile->GetType();

            // 元のミサイルを削除
            delete m_Missiles[index];
            m_Missiles.erase(m_Missiles.begin() + index);

            if (type == Missile::Type::MAIN)
            {
                // 縦ミサイル → 横2つに分裂
                float speeds[] = { -15.0f, 15.0f };  // 左右の速度
                for (float vx : speeds)
                {
                    Missile* sub = new Missile(Missile::Type::SUB);
                    sub->Init(m_pTex);
                    sub->SetPosition(pos.x, pos.y);
                    sub->SetSize(80.0f, 20.0f);  // 横長
                    sub->SetVelocity(vx, 0.0f);  // 横方向のみ
                    m_Missiles.push_back(sub);
                }
            }
            else if (type == Missile::Type::SUB)
            {
                // 横ミサイル → 縦2つに分裂
                float speeds[] = { -10.0f, 10.0f };  // 上下の速度
                for (float vy : speeds)
                {
                    Missile* main = new Missile(Missile::Type::MAIN);
                    main->Init(m_pTex);
                    main->SetPosition(pos.x, pos.y);
                    main->SetSize(20.0f, 80.0f);  // 縦長
                    main->SetVelocity(0.0f, vy);  // 縦方向のみ
                    m_Missiles.push_back(main);
                }
            }
        }
        else
        {
            // ミサイルがない場合は新規発射（最初の縦ミサイル）
            Missile* main = new Missile(Missile::Type::MAIN);
            main->Init(m_pTex);
            main->SetPosition(960.0f, 800.0f); // 発射位置
            main->SetSize(20.0f, 80.0f);
            main->SetVelocity(0.0f, -10.0f);  // 上に飛ぶ
            m_Missiles.push_back(main);
        }
    }

    // --- 2. 敵の生成 ---
    if (rand() % 100 < 3) 
    {
        Enemy* e = new Enemy();
        e->Init(m_pTex);
        e->SetPosition((float)(rand() % 1600 - 800), -600.0f);
        e->SetSize(50.0f, 50.0f);
        m_Enemies.push_back(e);
    }

    // --- 3. 更新と画面外削除 ---
    for (auto it = m_Missiles.begin(); it != m_Missiles.end(); )
    {
        (*it)->Update();
        DirectX::XMFLOAT2 p = (*it)->GetPosition();
        if (p.y < -700.0f || p.x < -1000.0f || p.x > 1000.0f)
        {
            delete* it;
            it = m_Missiles.erase(it);
        }
        else it++;
    }

    for (auto it = m_Enemies.begin(); it != m_Enemies.end(); ) 
    {
        (*it)->Update();
        if ((*it)->GetPosition().y > 600.0f) 
        {
            delete* it;
            it = m_Enemies.erase(it);
        }
        else it++;
    }

    // --- 4. 当たり判定 ---
    for (auto itM = m_Missiles.begin(); itM != m_Missiles.end(); )
    {
        bool hit = false;
        for (auto itE = m_Enemies.begin(); itE != m_Enemies.end(); )
        {
            if (Col::Any(CollisionRect(**itM, **itE))) {
                delete* itE;
                itE = m_Enemies.erase(itE);
                GameData::AddScore(100); // スコア加算
                hit = true;
                break;
            }
            else itE++;
        }
        if (hit)
        {
            delete* itM;
            itM = m_Missiles.erase(itM);
        }
        else itM++;
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
