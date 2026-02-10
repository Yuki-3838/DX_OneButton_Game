#include "StageScene.h"
#include "Collision.h"

void StageScene::Init()
{
	// カメラ生成
	m_pCamera = new Camera(1920,1080);
	m_pCamera->SetPosition(0.0f, 0.0f);

	m_pTex = m_pResourceManager->LoadTexture("assets/texture/title.png",m_pRenderer->GetDevice());

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
		//画面内に「メインミサイル」があるかを探す
		Missile* pMain = nullptr;
		int mainIndex = -1;

		for(int i = 0; i < m_Missiles.size();i++)
		{
			if (m_Missiles[i]->GetType() == Missile::Type::MAIN)
			{
				pMain = m_Missiles[i];
				mainIndex = i;
				break;
			}
		}
		if (pMain)
		{
			//分裂処理
			DirectX::XMFLOAT2 pos = pMain->GetPosition();

			//メインミサイルを削除
			delete m_Missiles[mainIndex];
			m_Missiles.erase(m_Missiles.begin() + mainIndex);
			//サブミサイルを2つ生成
			Missile* left = new Missile(Missile::Type::SUB);
			left->Init(m_pTex);
			left->SetPosition(pos.x, pos.y);
			left->SetVelocity(-20.0f, .00f);
			m_Missiles.push_back(left);

			Missile* right = new Missile(Missile::Type::SUB);
			right->Init(m_pTex);
			right->SetPosition(pos.x, pos.y);
			right->SetVelocity(20.0f, .00f);
			m_Missiles.push_back(right);
		}
		else
		{
			//発射処理
			Missile* main = new Missile(Missile::Type::MAIN);
			main->Init(m_pTex);
			//発射台の中央から
			main->SetPosition(m_pLauncher->GetPosition().x + 22.0f, m_pLauncher->GetPosition().y - 40.0f);
			main->SetVelocity(0.0f, -30.0f);
			m_Missiles.push_back(main);
		}
	}
	//敵の出現
	if(rand() % 100 < 2)
	{
		Enemy* enemy = new Enemy;
		enemy->Init(m_pTex);
		enemy->SetPosition(rand() % 1920, -64.0f);
		enemy->SetSize(40.0f, 40.0f);
		m_Enemies.push_back(enemy);
	}
	//更新処理
	for(auto m : m_Missiles)
	{
		m->Update();
	}
	for(auto e : m_Enemies)
	{
		e->Update();
	}
	//画面外の削除処理
	for(auto it = m_Missiles.begin(); it != m_Missiles.end();  ) 
	{
		DirectX::XMFLOAT2 pos = (*it)->GetPosition();
		if(pos.y < -100.0f || pos.x < -100 || pos.x > 2000)
		{
			delete *it;
			it = m_Missiles.erase(it);
		}
		else
		{
			++it;
		}
	}
	//画面下の敵を消してゲームオーバー
	for(auto it = m_Enemies.begin(); it != m_Enemies.end(); )
	{
		DirectX::XMFLOAT2 pos = (*it)->GetPosition();
		if(pos.y > 1200.0f)
		{
			delete *it;
			it = m_Enemies.erase(it);
			m_IsFinished = true;
		}
		else
		{
			++it;
		}
	}
	//当たり判定
	for (auto itM = m_Missiles.begin(); itM != m_Missiles.end();)
	{
		bool hit = false;
		for(auto itE = m_Enemies.begin(); itE != m_Enemies.end(); )
		{
			//当たり判定
			ColRes res = CollisionRect(**itM, **itE);
			if(Col::Any(res))
			{
				//ヒットしたら敵とミサイルを削除
				delete *itE;
				itE = m_Enemies.erase(itE);
				hit = true;
				break;
			}
			else
			{
				++itE;
			}
			if(hit)
			{
				delete *itM;
				itM = m_Missiles.erase(itM);
			}
			else
			{
				++itM;
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
