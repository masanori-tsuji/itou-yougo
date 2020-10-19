//=============================================================================
//
// チュートリアルの処理 [tutroial.h]
// Author : 伊藤陽梧
//
//=============================================================================
#ifndef _TUTRIAL_H_
#define _TUTRIAL_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"
#include "scene.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CScene2D;
class CSound;

//=============================================================================
// チュートリアルクラス
//=============================================================================
class CTutorial
{
public:
	CTutorial();
	~CTutorial();

	static CTutorial *Create();

	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);
private:
	static LPDIRECT3DTEXTURE9 m_pTexture; // textureのポインタ
	bool	 m_bTrriger; //	押したときの判定
};
#endif