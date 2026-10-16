
/*
	3D最初の一歩
*/

//==================必須ヘッダー
#include	<d3d11.h>
#include	<DirectXMath.h>

using namespace DirectX;
#include	"direct3d.h"
#include	"shader.h"
#include	"keyboard.h"
#include	"sprite.h"
//==================

#include	"Polygon3D.h"//自身のヘッダー

//グローバル変数
static	ID3D11Device* g_pDevice = NULL;
static	ID3D11DeviceContext* g_pContext = NULL;

//頂点バッファ
static	ID3D11Buffer* g_VertexBuffer = NULL;

//テクスチャ変数
static ID3D11ShaderResourceView* g_Texture;

//ポリゴン表示座標
static	XMFLOAT3	Position;
//ポリゴン回転角度
static	XMFLOAT3	Rotation;
//ポリゴン拡大率
static	XMFLOAT3	Scaling;

//頂点データの準備
#define		NUM_VERTEX	(100)
static	Vertex3D vdata[NUM_VERTEX] =
{
	{//頂点０　LEFT-TOP
		XMFLOAT3(-0.5f, 0.5f, 0.0f),		//座標
		XMFLOAT3(0.0f, 0.0f, 0.0f),			//
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	//色
		XMFLOAT2(0.0f, 0.0f)				//テクスチャ座標
	},
	{//頂点１　RIGHT-TOP
		XMFLOAT3(0.5f, 0.5f, 0.0f),			//座標
		XMFLOAT3(0.0f, 0.0f, 0.0f),			//
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	//色
		XMFLOAT2(1.0f, 0.0f)				//テクスチャ座標
	},
	{//頂点２　LEFT-BOTTOM
		XMFLOAT3(-0.5f, -0.5f, 0.0f),			//座標
		XMFLOAT3(0.0f, 0.0f, 0.0f),			//
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	//色
		XMFLOAT2(0.0f, 1.0f)				//テクスチャ座標
	},

	//後で増えます


};



void	Polygon3D_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{ 
	Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	Scaling = XMFLOAT3(1.0f, 1.0f, 1.0f);

	//頂点バッファ作成
	D3D11_BUFFER_DESC	bd;
	ZeroMemory(&bd, sizeof(bd));//０でクリア
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(Vertex3D) * NUM_VERTEX;//格納できる頂点数*頂点サイズ
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pDevice->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	g_pDevice = pDevice;
	g_pContext = pContext;


	// テクスチャ読み込み
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(L"Asset\\Texture\\texture.jpg", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(pDevice, image.GetImages(),
		image.GetImageCount(), metadata, &g_Texture);
	assert(g_Texture);

}
void	Polygon3D_Finalize()
{
	if (g_VertexBuffer != NULL)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	if (g_Texture != NULL)
	{
		g_Texture->Release();
		g_Texture = NULL;
	}


}
void	Polygon3D_Update()
{ 
	//ポリゴンの移動
	if (Keyboard_IsKeyDown(KK_UP))
	{
		Position.z += 0.05f;
	}
	if (Keyboard_IsKeyDown(KK_DOWN))
	{
		Position.z += -0.05f;
	}

	//ポリゴンの回転
	if (Keyboard_IsKeyDown(KK_LEFT))
	{
		Position.x += -0.05f;
	}
	if (Keyboard_IsKeyDown(KK_RIGHT))
	{
		Position.x += 0.05f;
	}

	//ポリゴンの回転
	if (Keyboard_IsKeyDown(KK_Z))
	{
		Rotation.x += 1.0f;
	}
	if (Keyboard_IsKeyDown(KK_X))
	{
		Rotation.x += -1.0f;
	}



}
void	Polygon3D_Draw()
{ 
	//====================
	//ワールド行列の作成
	//====================

	//スケーリング行列の作成
	XMMATRIX	ScalingMatrix = XMMatrixScaling
				(
					Scaling.x,
					Scaling.y,
					Scaling.z
				);
	//平行移動行列の作成
	XMMATRIX	TranslationMatrix = XMMatrixTranslation
				(
					Position.x,
					Position.y,
					Position.z
				);
	//回転行列の作成
	XMMATRIX	RotationMatrix = XMMatrixRotationRollPitchYaw
				(
					XMConvertToRadians(Rotation.x),
					XMConvertToRadians(Rotation.y),
					XMConvertToRadians(Rotation.z)
				);

	//乗算の順番に注意！！
	XMMATRIX	WorldMatrix =   ScalingMatrix *
								RotationMatrix *
								TranslationMatrix;

	
	//プロジェクション行列作成
	float	SCREEN_WIDTH = (float)Direct3D_GetBackBufferWidth();
	float	SCREEN_HEIGHT = (float)Direct3D_GetBackBufferHeight();
	XMMATRIX	Projection = XMMatrixPerspectiveFovLH
				(
					XMConvertToRadians(45.0f),		//視野角45度
					SCREEN_WIDTH / SCREEN_HEIGHT,	//画面のアスペクト比
					1.0f,							//近面クリップ距離
					10.0f							//遠面クリップ距離
				);

	//ビュー行列作成
	XMVECTOR	vPos = XMVectorSet(0.0f, 0.0f, -4.0f, 0.0f);//カメラの座標
	XMVECTOR	vAt = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);	//カメラの注視点
	XMVECTOR	vUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);	//上方ベクトル
	XMMATRIX	View = XMMatrixLookAtLH(vPos, vAt, vUp);

	//最終的な変換行列を作成 乗算の順番に注意！！
	XMMATRIX	WVP = WorldMatrix * View * Projection;

	//変換行列を頂点シェーダーへセット
	Shader_SetMatrix(WVP);

	//シェーダーを描画パイプラインへ設定
	Shader_Begin();


	//頂点データを頂点バッファへコピーする
	D3D11_MAPPED_SUBRESOURCE msr;
	g_pContext->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	Vertex3D* vertex = (Vertex3D*)msr.pData;
	//頂点データをコピーする
	CopyMemory(&vertex[0], &vdata[0], sizeof(Vertex3D) * NUM_VERTEX);
	//コピー完了
	g_pContext->Unmap(g_VertexBuffer, 0);

	//テクスチャをセット
	g_pContext->PSSetShaderResources(0, 1, &g_Texture);

	//頂点バッファをセット
	UINT	stride = sizeof(Vertex3D);	//頂点１個のデータサイズ
	UINT	offset = 0;
	g_pContext->IASetVertexBuffers(0,1, &g_VertexBuffer, &stride, &offset);

	//描画するポリゴンの種類をセット 3頂点でポリゴン１枚として表示
	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//描画リクエスト
	g_pContext->Draw(NUM_VERTEX, 0);


}



