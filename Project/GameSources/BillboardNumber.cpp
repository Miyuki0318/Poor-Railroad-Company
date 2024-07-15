/*!
@file BillboardNumber.cpp
@brief �r���{�[�h�\�����鐔���I�u�W�F�N�g
@author ���V���M
*/

#include "stdafx.h"
#include "BillboardNumber.h"

namespace basecross
{
	/*!
	@brief �������Ɉ�x�����Ăяo�����֐�
	*/
	void BillboardNumber::OnCreate()
	{
		// �X�P�[���̏����l��1.0
		BillboardNumber::SetScale(Vec2(1.0f));

		// ���b�V���̐ݒ�
		Utility::NumberVerticesIndices(m_vertex, m_number);
		auto ptrMesh = AddComponent<PCTStaticDraw>();
		ptrMesh->SetOriginalMeshUse(true);
		ptrMesh->CreateOriginalMesh(m_vertex);
		ptrMesh->SetTextureResource(L"NUMBER");

		// �����F��\��
		SetAlphaActive(true);
	}
}