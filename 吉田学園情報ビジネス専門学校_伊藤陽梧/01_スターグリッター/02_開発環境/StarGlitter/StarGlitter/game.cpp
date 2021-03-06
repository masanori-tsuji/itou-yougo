//=============================================================================
//
// ゲームの処理 [game.cpp]
// Author : 伊藤陽梧
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "game.h"
#include "scene2d.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "player2.h"
#include "explosion.h"
#include "bg.h"
#include "enemy.h"
#include "bullet.h"
#include "lifepolygon.h"
#include "effect.h"
#include "gauge.h"
#include "ojamaplayer.h"
#include "ojamaplayer2.h"
#include "input.h"
#include "select.h"
#include "text.h"
#include "sound.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CGame::GAMESTATE CGame::m_gamestate = GAMESTATE_NONE;
CSelect *CGame::m_pSelect[MAX_SELECT] = {};

//=============================================================================
// ゲームクラスのコンストラクタ
//=============================================================================
CGame::CGame()
{
	m_nCountCreate = 200;
	m_nSelectCount = 0;
	m_nCountText = 0;
	m_bPause = true;
	m_bGameStart = false;
	m_pSelect[0] = NULL;
	m_pSelect[1] = NULL;
	m_gamestate = GAMESTATE_NORMAL;
}

//=============================================================================
// ゲームクラスのデストラクタ
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
// ゲームクラスのインスタンス生成
//=============================================================================
CGame *CGame::Create()
{
	// CGameのポインタ
	CGame *pGame = NULL;

	// メモリの確保
	pGame = new CGame;

	// pGameがNULLじゃないとき
	if (pGame != NULL)
	{
		// 初期化処理
		pGame->Init();
	}

	// pGameを返す
	return pGame;
}

//=============================================================================
// ゲームクラスの初期化処理
//=============================================================================
HRESULT CGame::Init()
{
	// m_pBgメモリの確保
	CBg::Create(CBg::BGTYPE_GAME, CScene::OBJTYPE_BG);
	CBg::Create(CBg::BGTYPE_FRAME, CScene::OBJTYPE_BG);

	// m_pPlayerのクリエイト
	CPlayer::Create(D3DXVECTOR3(432.0f, SCREEN_HEIGHT - 200.0f, 0.0f), D3DXVECTOR3(PLAYER_SPEED, PLAYER_SPEED, 0.0f), PLAYER_SIZE_X, PLAYER_SIZE_Y, PLAYER_LIFE, CScene::OBJTYPE_PLAYER);

	// m_pPlayerのクリエイト
	CPlayer2::Create(D3DXVECTOR3(1296.0f, SCREEN_HEIGHT - 200.0f, 0.0f), D3DXVECTOR3(PLAYER_SPEED, PLAYER_SPEED, 0.0f), PLAYER_SIZE_X, PLAYER_SIZE_Y, PLAYER_LIFE, CScene::OBJTYPE_PLAYER2);

	// サウンドの再生
	CManager::GetSound()->PlaySound(CManager::GetSound()->SOUND_LABEL_GAMEBGM);

	srand((unsigned int)time(NULL));

	return S_OK;
}

//=============================================================================
// ゲームクラスの終了処理
//=============================================================================
void CGame::Uninit(void)
{
	// オブジェクトの終了処理とメモリの開放
	CScene::NotFadeReleaseAll();
}

//=============================================================================
// ゲームクラスの更新処理
//=============================================================================
void CGame::Update(void)
{
	// CInputJoypadのポインタのインスタンス生成の受け取り
	CInputJoypad *pInputJoypad = CManager::GetInputJoypad();

	switch (m_gamestate)
	{
		// ノーマルの状態
	case GAMESTATE_NORMAL:
		// ゲームが始まってないとき
		if (m_bGameStart == false)
		{
			// テキストのカウントの加算
			m_nCountText++;

			// テキストのカウントが120以上になったら
			if (m_nCountText >= 120)
			{
				m_nCountText = 0;

				// READYの文字の生成
				CText::Create(D3DXVECTOR3(PLAYER_START_POS, SCREEN_CENTER_Y, 0.0f), 832.0f, 0.0f, CText::TEXTTYPE_READY);
				CText::Create(D3DXVECTOR3(PLAYER2_START_POS, SCREEN_CENTER_Y, 0.0f), 832.0f, 0.0f, CText::TEXTTYPE_READY);

				// ゲームが始まった状態にする
				m_bGameStart = true;
			}
		}
		break;

		// ゲームが始まった状態
	case GAMESTATE_START:
		// ゲームが始まった時
		if (m_bGameStart == true)
		{
			if (CScene::GetPause() != true)
			{
				// 生成するカウント
				m_nCountCreate++;

				// 生成するカウントが200以上になったら
				if (m_nCountCreate >= 200)
				{
					// ランダムで出す敵を変える
					switch (rand() % 6)
					{
					case 0:
						// m_pEnemyのクリエイト
						CEnemy::Create(D3DXVECTOR3(144.0f, -100.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X, ENEMY_SIZE_Y, 5, CScene::OBJTYPE_ENEMY);
						CEnemy::Create(D3DXVECTOR3(432.0f, -100.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X, ENEMY_SIZE_Y, 4, CScene::OBJTYPE_ENEMY);
						CEnemy::Create(D3DXVECTOR3(720.0f, -100.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X, ENEMY_SIZE_Y, 3, CScene::OBJTYPE_ENEMY);

						CEnemy::Create(D3DXVECTOR3(1008.0f, -100.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X, ENEMY_SIZE_Y, 2, CScene::OBJTYPE_ENEMY);
						CEnemy::Create(D3DXVECTOR3(1296.0f, -100.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X, ENEMY_SIZE_Y, 1, CScene::OBJTYPE_ENEMY);
						CEnemy::Create(D3DXVECTOR3(1584.0f, -100.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X, ENEMY_SIZE_Y, 5, CScene::OBJTYPE_ENEMY);
						m_nCountCreate = 0;
						break;

					case 1:
						// m_pEnemyのクリエイト
						CEnemy::Create(D3DXVECTOR3(144.0f, -100.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X - 25.0f, ENEMY_SIZE_Y - 25.0f, 1, CScene::OBJTYPE_ENEMY);
						CEnemy::Create(D3DXVECTOR3(144.0f, -200.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X - 20.0f, ENEMY_SIZE_Y - 20.0f, 2, CScene::OBJTYPE_ENEMY);
						CEnemy::Create(D3DXVECTOR3(144.0f, -300.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X - 15.0f, ENEMY_SIZE_Y - 15.0f, 3, CScene::OBJTYPE_ENEMY);
						CEnemy::Create(D3DXVECTOR3(144.0f, -400.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X - 10.0f, ENEMY_SIZE_Y - 10.0f, 4, CScene::OBJTYPE_ENEMY);
						CEnemy::Create(D3DXVECTOR3(144.0f, -500.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X - 5.0f, ENEMY_SIZE_Y - 5.0f, 5, CScene::OBJTYPE_ENEMY);

						CEnemy::Create(D3DXVECTOR3(1008.0f, -100.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X - 25.0f, ENEMY_SIZE_Y - 25.0f, 1, CScene::OBJTYPE_ENEMY);
						CEnemy::Create(D3DXVECTOR3(1008.0f, -200.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X - 20.0f, ENEMY_SIZE_Y - 20.0f, 2, CScene::OBJTYPE_ENEMY);
						CEnemy::Create(D3DXVECTOR3(1008.0f, -300.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X - 15.0f, ENEMY_SIZE_Y - 15.0f, 3, CScene::OBJTYPE_ENEMY);
						CEnemy::Create(D3DXVECTOR3(1008.0f, -400.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X - 10.0f, ENEMY_SIZE_Y - 10.0f, 4, CScene::OBJTYPE_ENEMY);
						CEnemy::Create(D3DXVECTOR3(1008.0f, -500.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X - 5.0f, ENEMY_SIZE_Y - 5.0f, 5, CScene::OBJTYPE_ENEMY);
						m_nCountCreate = 0;
						break;

					case 2:
						// m_pEnemyのクリエイト
						CEnemy::Create(D3DXVECTOR3(720.0f, -100.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X - 25.0f, ENEMY_SIZE_Y - 25.0f, 1, CScene::OBJTYPE_ENEMY);
						CEnemy::Create(D3DXVECTOR3(720.0f, -200.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X - 20.0f, ENEMY_SIZE_Y - 20.0f, 2, CScene::OBJTYPE_ENEMY);
						CEnemy::Create(D3DXVECTOR3(720.0f, -300.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X - 15.0f, ENEMY_SIZE_Y - 15.0f, 3, CScene::OBJTYPE_ENEMY);
						CEnemy::Create(D3DXVECTOR3(720.0f, -400.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X - 10.0f, ENEMY_SIZE_Y - 10.0f, 4, CScene::OBJTYPE_ENEMY);
						CEnemy::Create(D3DXVECTOR3(720.0f, -500.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X - 5.0f, ENEMY_SIZE_Y - 5.0f, 5, CScene::OBJTYPE_ENEMY);

						CEnemy::Create(D3DXVECTOR3(1584.0f, -100.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X - 25.0f, ENEMY_SIZE_Y - 25.0f, 1, CScene::OBJTYPE_ENEMY);
						CEnemy::Create(D3DXVECTOR3(1584.0f, -200.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X - 20.0f, ENEMY_SIZE_Y - 20.0f, 2, CScene::OBJTYPE_ENEMY);
						CEnemy::Create(D3DXVECTOR3(1584.0f, -300.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X - 15.0f, ENEMY_SIZE_Y - 15.0f, 3, CScene::OBJTYPE_ENEMY);
						CEnemy::Create(D3DXVECTOR3(1584.0f, -400.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X - 10.0f, ENEMY_SIZE_Y - 10.0f, 4, CScene::OBJTYPE_ENEMY);
						CEnemy::Create(D3DXVECTOR3(1584.0f, -500.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X - 5.0f, ENEMY_SIZE_Y - 5.0f, 5, CScene::OBJTYPE_ENEMY);
						m_nCountCreate = 0;
						break;

					case 3:
						// m_pEnemyのクリエイト
						CEnemy::Create(D3DXVECTOR3(432.0f, -100.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X, ENEMY_SIZE_Y, 2, CScene::OBJTYPE_ENEMY);
						CEnemy::Create(D3DXVECTOR3(432.0f, -200.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X, ENEMY_SIZE_Y, 1, CScene::OBJTYPE_ENEMY);
						CEnemy::Create(D3DXVECTOR3(432.0f, -300.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X, ENEMY_SIZE_Y, 5, CScene::OBJTYPE_ENEMY);

						CEnemy::Create(D3DXVECTOR3(1296.0f, -100.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X, ENEMY_SIZE_Y, 4, CScene::OBJTYPE_ENEMY);
						CEnemy::Create(D3DXVECTOR3(1296.0f, -200.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X, ENEMY_SIZE_Y, 3, CScene::OBJTYPE_ENEMY);
						CEnemy::Create(D3DXVECTOR3(1296.0f, -300.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X, ENEMY_SIZE_Y, 2, CScene::OBJTYPE_ENEMY);
						m_nCountCreate = 0;
						break;

					case 4:
						// m_pEnemyのクリエイト
						CEnemy::Create(D3DXVECTOR3(144.0f, -100.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X - 5.0f, ENEMY_SIZE_Y - 5.0f, 5, CScene::OBJTYPE_ENEMY);
						CEnemy::Create(D3DXVECTOR3(144.0f, -200.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X - 10.0f, ENEMY_SIZE_Y - 10.0f, 4, CScene::OBJTYPE_ENEMY);
						CEnemy::Create(D3DXVECTOR3(144.0f, -300.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X - 15.0f, ENEMY_SIZE_Y - 15.0f, 3, CScene::OBJTYPE_ENEMY);
						CEnemy::Create(D3DXVECTOR3(144.0f, -400.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X - 20.0f, ENEMY_SIZE_Y - 20.0f, 2, CScene::OBJTYPE_ENEMY);
						CEnemy::Create(D3DXVECTOR3(144.0f, -500.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X - 25.0f, ENEMY_SIZE_Y - 25.0f, 1, CScene::OBJTYPE_ENEMY);

						CEnemy::Create(D3DXVECTOR3(1008.0f, -100.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X - 5.0f, ENEMY_SIZE_Y - 5.0f, 5, CScene::OBJTYPE_ENEMY);
						CEnemy::Create(D3DXVECTOR3(1008.0f, -200.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X - 10.0f, ENEMY_SIZE_Y - 10.0f, 4, CScene::OBJTYPE_ENEMY);
						CEnemy::Create(D3DXVECTOR3(1008.0f, -300.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X - 15.0f, ENEMY_SIZE_Y - 15.0f, 3, CScene::OBJTYPE_ENEMY);
						CEnemy::Create(D3DXVECTOR3(1008.0f, -400.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X - 20.0f, ENEMY_SIZE_Y - 20.0f, 2, CScene::OBJTYPE_ENEMY);
						CEnemy::Create(D3DXVECTOR3(1008.0f, -500.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X - 25.0f, ENEMY_SIZE_Y - 25.0f, 1, CScene::OBJTYPE_ENEMY);
						m_nCountCreate = 0;
						break;

					case 5:
						// m_pEnemyのクリエイト
						CEnemy::Create(D3DXVECTOR3(720.0f, -100.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X - 5.0f, ENEMY_SIZE_Y - 5.0f, 5, CScene::OBJTYPE_ENEMY);
						CEnemy::Create(D3DXVECTOR3(720.0f, -200.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X - 10.0f, ENEMY_SIZE_Y - 10.0f, 4, CScene::OBJTYPE_ENEMY);
						CEnemy::Create(D3DXVECTOR3(720.0f, -300.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X - 15.0f, ENEMY_SIZE_Y - 15.0f, 3, CScene::OBJTYPE_ENEMY);
						CEnemy::Create(D3DXVECTOR3(720.0f, -400.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X - 20.0f, ENEMY_SIZE_Y - 20.0f, 2, CScene::OBJTYPE_ENEMY);
						CEnemy::Create(D3DXVECTOR3(720.0f, -500.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X - 25.0f, ENEMY_SIZE_Y - 25.0f, 1, CScene::OBJTYPE_ENEMY);

						CEnemy::Create(D3DXVECTOR3(1584.0f, -100.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X - 5.0f, ENEMY_SIZE_Y - 5.0f, 5, CScene::OBJTYPE_ENEMY);
						CEnemy::Create(D3DXVECTOR3(1584.0f, -200.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X - 10.0f, ENEMY_SIZE_Y - 10.0f, 4, CScene::OBJTYPE_ENEMY);
						CEnemy::Create(D3DXVECTOR3(1584.0f, -300.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X - 15.0f, ENEMY_SIZE_Y - 15.0f, 3, CScene::OBJTYPE_ENEMY);
						CEnemy::Create(D3DXVECTOR3(1584.0f, -400.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X - 20.0f, ENEMY_SIZE_Y - 20.0f, 2, CScene::OBJTYPE_ENEMY);
						CEnemy::Create(D3DXVECTOR3(1584.0f, -500.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), ENEMY_SIZE_X - 25.0f, ENEMY_SIZE_Y - 25.0f, 1, CScene::OBJTYPE_ENEMY);
						m_nCountCreate = 0;
						break;
					default:
						break;
					}
				}
			}

			// ポーズ画面の処理
			// 選んだ時のカウントを減算する
			m_nSelectCount--;

			// 選んだ時のカウントが0以下になったら
			if (m_nSelectCount <= 0)
			{
				// ポーズ状態じゃなく、モードがゲームの時
				if (m_bPause == true && CManager::GetMode() == CManager::MODE_GAME)
				{
					// スタートボタンを押したら
					if (pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START))
					{
						//ポーズ状態にする
						CScene::SetPause(true);

						// セレクトクラスの生成
						m_pSelect[0] = CSelect::Create(D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, CSelect::SELECTTYPE_KUROHAIKEI);
						m_pSelect[1] = CSelect::Create(D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f), 850.0f, 625.0f, CSelect::SELECTTYPE_MENU);

						// ポーズ状態にする
						m_bPause = false;
						m_nSelectCount = 20;

						// サウンドの停止と再生
						CManager::GetSound()->StopSound(CManager::GetSound()->SOUND_LABEL_GAMEBGM);
						CManager::GetSound()->PlaySound(CManager::GetSound()->SOUND_LABEL_MENUCREATE);
						return;
					}
					if (pInputJoypad->GetJoypadTrigger(PLAYER_2, CInputJoypad::BUTTON_START))
					{
						//ポーズ状態にする
						CScene::SetPause(true);

						// セレクトクラスの生成
						m_pSelect[0] = CSelect::Create(D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, CSelect::SELECTTYPE_KUROHAIKEI);
						m_pSelect[1] = CSelect::Create(D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f), 850.0f, 625.0f, CSelect::SELECTTYPE_MENU);

						// ポーズ状態にする
						m_bPause = false;
						m_nSelectCount = 20;

						// サウンドの停止と再生
						CManager::GetSound()->StopSound(CManager::GetSound()->SOUND_LABEL_GAMEBGM);
						CManager::GetSound()->PlaySound(CManager::GetSound()->SOUND_LABEL_MENUCREATE);
						return;
					}

				}
				// ポーズ状態の時
				else if (m_bPause == false)
				{
					// スタートボタンを押したら
					if (pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START))
					{
						//ポーズ状態を解除する
						CScene::SetPause(false);

						// サウンドの再生
						CManager::GetSound()->PlaySound(CManager::GetSound()->SOUND_LABEL_GAMEBGM);

						for (int nCount = 0; nCount < MAX_SELECT; nCount++)
						{
							// m_pSelectがNULLじゃないとき
							if (m_pSelect[nCount] != NULL)
							{
								// m_pSelectの終了処理
								m_pSelect[nCount]->Uninit();
								m_pSelect[nCount] = NULL;
							}
						}

						// ポーズ状態を解除する
						m_bPause = true;
						m_nSelectCount = 20;
						return;
					}
					if (pInputJoypad->GetJoypadTrigger(PLAYER_2, CInputJoypad::BUTTON_START))
					{
						//ポーズ状態を解除する
						CScene::SetPause(false);

						// サウンドの再生
						CManager::GetSound()->PlaySound(CManager::GetSound()->SOUND_LABEL_GAMEBGM);

						for (int nCount = 0; nCount < MAX_SELECT; nCount++)
						{
							// m_pSelectがNULLじゃないとき
							if (m_pSelect[nCount] != NULL)
							{
								// m_pSelectの終了処理
								m_pSelect[nCount]->Uninit();
								m_pSelect[nCount] = NULL;
							}
						}

						// ポーズ状態を解除する
						m_bPause = true;
						m_nSelectCount = 20;
						return;
					}

					// Aボタンを押したら
					if (pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_A))
					{
						for (int nCount = 0; nCount < MAX_SELECT; nCount++)
						{
							if (m_pSelect[nCount] != NULL)
							{
								// m_pSelectがNULLじゃないとき
								m_pSelect[nCount]->Uninit();
								m_pSelect[nCount] = NULL;
							}
						}

						// ポーズ状態を解除する
						m_bPause = true;
						m_nSelectCount = 180;
						return;
					}
					if (pInputJoypad->GetJoypadTrigger(PLAYER_2, CInputJoypad::BUTTON_A))
					{
						for (int nCount = 0; nCount < MAX_SELECT; nCount++)
						{
							if (m_pSelect[nCount] != NULL)
							{
								// m_pSelectがNULLじゃないとき
								m_pSelect[nCount]->Uninit();
								m_pSelect[nCount] = NULL;
							}
						}

						// ポーズ状態を解除する
						m_bPause = true;
						m_nSelectCount = 180;
						return;
					}
				}
			}
		}
		break;

		// ゲームが終わった状態
	case GAMESTATE_END:
		if (m_bGameStart == true)
		{
			//ポーズ状態にする
			CScene::SetPause(true);

			// テキストクラスのKOの生成
			CText::Create(D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f), 530.0f, 232.0f, CText::TEXTTYPE_KO);

			// ゲームが終わる状態にする
			m_bGameStart = false;

			// サウンドの停止と再生
			CManager::GetSound()->PlaySound(CManager::GetSound()->SOUND_LABEL_KO);
			CManager::GetSound()->StopSound(CManager::GetSound()->SOUND_LABEL_GAMEBGM);
		}
		break;

	default:
		break;

	}
}

//=============================================================================
// ゲームクラスの描画処理
//=============================================================================
void CGame::Draw(void)
{

}


//=============================================================================
// ゲームクラスのゲームの状態の設定
//=============================================================================
void CGame::SetGameState(GAMESTATE gamestate)
{
	m_gamestate = gamestate;
}

//=============================================================================
// ゲームクラスのゲームの状態を渡す処理
//=============================================================================
CGame::GAMESTATE CGame::GetGameState(void)
{
	return m_gamestate;
}