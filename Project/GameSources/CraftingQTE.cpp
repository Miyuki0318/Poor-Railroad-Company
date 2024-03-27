/*!
@file CraftingQTE.cpp
@brief �N���t�g����QTE
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �������̏���
	void CraftingQTE::OnCreate()
	{
		// �p�����̐������̏��������s
		Sprite::OnCreate();

		// ���_�f�[�^�̍��W�����Z
		Vec3 diffPos = Vec3(0.5f, 0.5f, 0.0f);
		for (auto& v : m_vertex.vertices)
		{
			v.position += diffPos;
		}
		m_ptrDraw->UpdateVertices(m_vertex.vertices);

		// �X�e�[�W�̎擾
		const auto& stagePtr = GetStage();

		// �t���[����QTE�|�C���g�𐶐�
		//m_barFlame = stagePtr->AddGameObject<Sprite>(L"C_QTE_FLAME_TX", m_barScale + Vec2(5.0f), Vec3(0.0f));
		//m_qtePoint = stagePtr->AddGameObject<Sprite>(L"WHITE_TX", Vec2(40.0f, 10.0f), Vec3(0.0f));
	}

	// ���t���[���X�V����
	void CraftingQTE::OnUpdate()
	{

	}

	// QTE�̍X�V����
	void CraftingQTE::UpdateQTE()
	{

	}

	// QTE�̊J�n����
	void CraftingQTE::StartQTE(const Vec3& windowPos)
	{
		m_barRatio = 0.0f;
		m_qteRatio = Utility::RangeRand(0.9f, 0.55f);
	}

	// QTE�̒�~����
	bool CraftingQTE::StopQTE()
	{
		return true;
	}
}