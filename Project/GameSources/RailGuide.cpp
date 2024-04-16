/*!
@file RailGuide.cpp
@brief ���[����ݒu����ꏊ�̃K�C�h�\��
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �������̏��������s
	void RailGuide::OnCreate()
	{
		// �`��R���|�[�l���g�̐ݒ�
		m_ptrDraw = AddComponent<PNTStaticInstanceDraw>();
		m_ptrDraw->SetMeshResource(L"RAIL");
		m_ptrDraw->SetTextureResource(L"RAIL_TX");
		m_ptrDraw->SetSpecular(COL_WHITE);
		m_ptrDraw->SetDiffuse(m_defColor);

		// �`��ݒ�
		SetAlphaActive(true);
		SetDrawActive(false);
	}

	// ���t���[���X�V����
	void RailGuide::OnUpdate()
	{
		// �v���C���[�����[���������Ă邩�ŕ\����\��
		const auto& player = GetStage()->GetSharedGameObject<GamePlayer>(L"Player");
		bool isHaveRail = player->GetStatus(ePlayerStatus::IsHaveRail);
		if (isHaveRail)
		{
			// �e��X�V����
			UpdateBlinking();
			UpdateGuide();
		}

		Debug::Log(L"�K�C�h�̓����x", m_ptrDraw->GetDiffuse().w);

		SetDrawActive(isHaveRail);
	}

	// �_�ł̍X�V
	void RailGuide::UpdateBlinking()
	{
		// ���������Z
		m_blinkRatio += DELTA_TIME / m_blinkTime;
		if (m_blinkRatio >= XM_2PI) m_blinkRatio = 0.0f;

		// �T�C���J�[�u�������ŋ��߁A�����F����ʏ�F�͈̔͂œ_�ł�����
		Col4 blinkColor = Utility::SinCurve(m_blinkRatio, COL_ALPHA, m_defColor);
		m_ptrDraw->SetDiffuse(blinkColor);
	}

	// �K�C�h�̍X�V
	void RailGuide::UpdateGuide()
	{
		// �s��p�ϐ��ƍs��z��
		Mat4x4 matrix, mtxPosition;
		auto& matrixVec = m_ptrDraw->GetMatrixVec();
		matrixVec.clear();
		
		// �K�C�h��csv��̃|�C���g�z��
		const auto& guidePoints = GetStage()->GetSharedGameObject<RailManager>(L"RailManager")->GetGuidePoints();
		
		// �z��̐����[�v
		for (const auto& guide : guidePoints)
		{
			// �s��̐ݒ�ƒǉ�
			Vec3 addPos = Vec3(float(guide.y), m_posY, -float(guide.x));
			mtxPosition.translation(addPos);
			matrix = m_mtxScale * m_mtxRotation * mtxPosition;
			matrixVec.push_back(matrix);
		}
	}
}