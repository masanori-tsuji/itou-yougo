//=============================================================================
//
// ポリゴンの描画の処理 [title.h]
// Author : 伊藤陽梧
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
// プレイヤークラス
//=============================================================================
class CTitle
{
public:
	CTitle();
	~CTitle();

	static CTitle *Create();

	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);
private:

};
#endif