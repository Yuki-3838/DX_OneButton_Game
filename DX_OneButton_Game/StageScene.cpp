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
	m_pLauncherTex = m_pResourceManager->LoadTexture("asset/texture/Launcher.png",m_pRenderer->GetDevice());
	m_pMissileTex = m_pResourceManager->LoadTexture("asset/texture/Missile.png",m_pRenderer->GetDevice());
	m_pLeftMissileTex = m_pResourceManager->LoadTexture("asset/texture/LeftMissile.png",m_pRenderer->GetDevice());
	m_pRightMissileTex = m_pResourceManager->LoadTexture("asset/texture/RightMissile.png",m_pRenderer->GetDevice());
	m_pBottomMissileTex = m_pResourceManager->LoadTexture("asset/texture/BottomMissile.png",m_pRenderer->GetDevice());
	m_pEnemyTex = m_pResourceManager->LoadTexture("asset/texture/Enemy.png",m_pRenderer->GetDevice());
	m_pFriendTex = m_pResourceManager->LoadTexture("asset/texture/Friend.png",m_pRenderer->GetDevice());

	//UIの初期化
	m_pUI = new GameUI();
	m_pUI->Init(m_pRenderer->GetDevice(), m_pResourceManager);

	//スコアのリセット
	GameData::Reset();
	
	//発射台(画面中央)
	m_pLauncher = new GameObject;
	m_pLauncher->Init(m_pLauncherTex);

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
        // 弾がないなら発射
        if (m_Missiles.empty())
        {
            Missile* main = new Missile(Missile::Type::MAIN, 0);
            main->Init(m_pMissileTex);
            main->SetPosition(0.0f, 400.0f);
            main->SetSize(20.0f, 80.0f);
            main->SetVelocity(0.0f, -10.0f);
            m_Missiles.push_back(main);
        }
        else
        {
            // ★現在の数が上限を超えているかチェック
            if (m_Missiles.size() >= MAX_MISSILE_COUNT)
            {
                // ----------------------------------------------------
                // 回転処理 (上限に達しているので、分裂せずに方向転換)
                // ----------------------------------------------------
                for (auto* m : m_Missiles)
                {
                    // 現在の速度を取得
                    DirectX::XMFLOAT2 v = m->GetVelocity();

                    // 90度回転させる計算 (X = -Y, Y = X)
                    // これで 上→右→下→左→上... と回転します
                    float newVx = -v.y;
                    float newVy = v.x;

                    m->SetVelocity(newVx, newVy);

                    // 向きに合わせて見た目（サイズ）とタイプも更新
                    if (m->GetType() == Missile::Type::MAIN || m->GetType() == Missile::Type::VERTICAL)
                    {
                        // 縦から横へ
                        m->SetType(Missile::Type::HORIZONTAL); //Type変更
                        m->SetSize(80.0f, 20.0f); // 横長
                    }
                    else if (m->GetType() == Missile::Type::HORIZONTAL)
                    {
                        // 横から縦へ
                        m->SetType(Missile::Type::VERTICAL);   // ★Type変更
                        m->SetSize(20.0f, 80.0f); // 縦長
                    }
                }
            }
            else
            {
                // ----------------------------------------------------
                // 分裂処理 (上限まで余裕があるので分裂)
                // ----------------------------------------------------
                std::vector<Missile*> nextMissiles;

                for (auto it = m_Missiles.begin(); it != m_Missiles.end(); )
                {
                    Missile* m = *it;
                    DirectX::XMFLOAT2 pos = m->GetPosition();
                    int currentGen = m->GetGeneration();
                    bool split = false;
					float offset = 40.0f; // 分裂後の弾の位置を少しずらすためのオフセット

                    // --- 分裂ロジック ---
                    if (m->GetType() == Missile::Type::MAIN || m->GetType() == Missile::Type::VERTICAL)
                    {
                        // 左右へ
                        Missile* left = new Missile(Missile::Type::HORIZONTAL, currentGen + 1);
                        left->Init(m_pLeftMissileTex);
                        left->SetPosition(pos.x - offset, pos.y);
                        left->SetSize(80.0f, 20.0f);
                        left->SetVelocity(-12.0f, 0.0f);
                        nextMissiles.push_back(left);

                        Missile* right = new Missile(Missile::Type::HORIZONTAL, currentGen + 1);
                        right->Init(m_pRightMissileTex);
                        right->SetPosition(pos.x + offset, pos.y);
                        right->SetSize(80.0f, 20.0f);
                        right->SetVelocity(12.0f, 0.0f);
                        nextMissiles.push_back(right);
                        split = true;
                    }
                    else if (m->GetType() == Missile::Type::HORIZONTAL)
                    {
                        // 上下へ
                        Missile* up = new Missile(Missile::Type::VERTICAL, currentGen + 1);
                        up->Init(m_pMissileTex);
                        up->SetPosition(pos.x, pos.y - offset);
                        up->SetSize(20.0f, 80.0f);
                        up->SetVelocity(0.0f, -12.0f);
                        nextMissiles.push_back(up);

                        Missile* down = new Missile(Missile::Type::VERTICAL, currentGen + 1);
                        down->Init(m_pBottomMissileTex);
                        down->SetPosition(pos.x, pos.y + offset);
                        down->SetSize(20.0f, 80.0f);
                        down->SetVelocity(0.0f, 12.0f);
                        nextMissiles.push_back(down);
                        split = true;
                    }

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

                // 新弾追加
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
        e->SetSize(50.0f, 50.0f);
        //10%の確率で「味方(FRIEND)」にする
        if (rand() % 100 < 10)
        {
            e->Init(m_pFriendTex);
            e->SetPosition((float)(rand() % (int)rangeX - (rangeX / 2)), spawnY);
            e->SetType(Enemy::Type::FRIEND);
            // 味方は見た目を変えたいので、少し小さくしたり色を変えたりしたい
            // (描画時に色を変えるのが一番わかりやすいです。後述のDraw参照)
        }
        else
        {
            e->Init(m_pEnemyTex);
            e->SetPosition((float)(rand() % (int)rangeX - (rangeX / 2)), spawnY);
            e->SetType(Enemy::Type::NORMAL);
        }
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
        // 画面下に行ったら？
        if ((*it)->GetPosition().y > limitY)
        {
            //もし「敵」が下まで来たらゲームオーバー！
            if ((*it)->GetType() == Enemy::Type::NORMAL)
            {
                m_IsFinished = true;
            }

            // 味方は下まで行ってもOK（削除するだけ）
            delete* it;
            it = m_Enemies.erase(it);
        }
        else
        {
            it++;
        }
    }

    // ----------------------------------------------------------------
    // 4. 当たり判定（貫通）
    // ----------------------------------------------------------------
    for (auto itM = m_Missiles.begin(); itM != m_Missiles.end(); ++itM)
    {
        bool hit = false; // このミサイルが何かと当たったか？

        for (auto itE = m_Enemies.begin(); itE != m_Enemies.end(); )
        {
            if (Col::Any(CollisionRect(**itM, **itE)))
            {
                // ★当たった相手の種類を確認
                if ((*itE)->GetType() == Enemy::Type::FRIEND)
                {
                    // 味方を撃ってしまった！ -> ゲームオーバー
                    m_IsFinished = true;
                }
                else
                {
                    // 敵を倒した！ -> スコア加算
                    GameData::AddScore(100);
                }

                // どっちにしろ当たったキャラは消える
                delete* itE;
                itE = m_Enemies.erase(itE);

                // ミサイルは貫通させたいならそのまま
                // 消したいなら hit = true; break;
            }
            else {
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
    float scoreX = -(Game::SCREEN_WIDTH / 2.0f) + 20.0f; // 左端 + 少し余白
    float scoreY = -(Game::SCREEN_HEIGHT / 2.0f) + 20.0f; // 上端 + 少し余白
    // スコア表示
    if (m_pUI)
    {
        m_pUI->DrawNumber(m_pRenderer->GetContext(), m_pSpriteRenderer, GameData::GetScore(), scoreX, scoreY, 0.3f);
    }
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
