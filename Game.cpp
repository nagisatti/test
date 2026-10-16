
//Game.cpp

#include	"Manager.h"
#include	"sprite.h"
#include	"Game.h"
#include	"keyboard.h"

#include	"player.h"
#include	"Block.h"
#include	"field.h"
#include	"Effect.h"
#include	"score.h"
#include	"Audio.h"

#include "Polygon3D.h"

static	int		g_BgmID = NULL;	//サウンド管理ID

void Game_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{

	Player_Initialize(pDevice, pContext); // プレイヤーの初期化
	field_Initialize(pDevice, pContext); // フィールドの初期化
	Block_Initialize(pDevice, pContext);//ブロックの初期化
	Effect_Initialize(pDevice, pContext);//エフェクト初期化
	Score_Initialize(pDevice, pContext);//スコア初期化

	Polygon3D_Initialize(pDevice, pContext);//3Dテスト初期化



	g_BgmID = LoadAudio("asset\\Audio\\bgm.wav");	//サウンドロード
	//PlayAudio(g_BgmID, true);	//再生開始（ループあり）
	//PlayAudio(g_BgmID);			//再生開始（ループなし）
	//PlayAudio(g_BgmID, false);	//再生開始（ループなし）

}

void Game_Finalize()
{
	Block_Finalize();
	Player_Finalize();
	field_Finalize();
	Effect_Finalize();
	Score_Finalize();
	Polygon3D_Finalize();

	UnloadAudio(g_BgmID);//サウンドの解放
}

void Game_Update()
{
	//更新処理
	Player_Update();
	Block_Update();
	Effect_Update();
	Score_Update();
	Polygon3D_Update();

}

void Game_Draw()
{ 
	SetDepthTest(false);//深度テスト無効
	field_Draw();
	Block_Draw();
	Player_Draw();
	Effect_Draw();
	Score_Draw();

	SetDepthTest(true);//深度テスト有効
	Polygon3D_Draw();

}

