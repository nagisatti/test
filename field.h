
//field.h
#pragma once

#include <d3d11.h>

void field_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void field_Finalize(void);
void field_Draw(void);


