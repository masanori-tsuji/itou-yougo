//=============================================================================
//
// タイトルの処理 [title.cpp]
// Author : 伊藤陽梧
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include <math.h>
#include "title.h"
#include "scene2d.h"
#include "renderer.h"
#include "manager.h"
#include "fade.h"
#include "bg.h"
#include "titlelogo.h"
#include "sound.h"

//=============================================================================
// タイトルのコンストラクタ
//=============================================================================
CTitle::CTitle()
{

}

//=============================================================================
// タイトルのデストラクタ
//=============================================================================
CTitle::~CTitle()
{

}

//=============================================================================
// タイトルのインスタンス生成
//=============================================================================
CTitle *CTitle::Create()
{
	// CTitleのポインタ
	CTitle *pTitle = NULL;

	// メモリの確保
	pTitle = new CTitle;

	// pTitleがNULLじゃないとき
	if (pTitle != NULL)
	{
		// 初期化処理
		pTitle->Init();
	}

	// pTitleを返す
	return pTitle;
}

//=============================================================================
// タイトルの初期化処理
//=============================================================================
HRESULT CTitle::Init()
{
	// タイトルロゴ生成
	CTitleLogo::Create(D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, CTitleLogo::LOGOTYPE_SIROHAIKEI);
	CTitleLogo::Create(D3DXVECTOR3(SCREEN_CENTER_X, -400.0f, 0.0f), 1473.0f, 633.0f, CTitleLogo::LOGOTYPE_TITLE);

	// サウンドの再生
	CManager::GetSound()->PlaySound(CManager::GetSound()->SOUND_LABEL_TITLEBGM);
	return S_OK;
}

//=============================================================================
// タイトルの終了処理
//=============================================================================
void CTitle::Uninit(void)
{
	// オブジェクトの終了処理とメモリの開放
	CScene::NotFadeReleaseAll();
}

//=============================================================================
// タイトルの更新処理
//=============================================================================
void CTitle::Update(void)
{

}

//=============================================================================
// タイトルの描画処理
//=============================================================================
void CTitle::Draw(void)
{

}