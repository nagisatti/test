/*
	３Dの中の人
	最初の一歩
*/


#pragma once

//Polygon3D.h
#include	"d3d11.h"

//ゲーム基本関数四天王
void	Polygon3D_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void	Polygon3D_Finalize();
void	Polygon3D_Update();
void	Polygon3D_Draw();




