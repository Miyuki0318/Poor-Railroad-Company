/*!
@file RailGuideBlinking.cpp
@brief ���[����ݒu����ꏊ�̃K�C�h�\��
@author ���V���M
*/

#include "stdafx.h"
#include "RailGuideBlinking.h"
#include "GamePlayer.h"
#include "RailManager.h"

namespace basecross
{
	// �������̏��������s
	void RailGuideBlinking::OnCreate()
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
	void RailGuideBlinking::OnUpdate()
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

		SetDrawActive(isHaveRail);
	}

	// �_�ł̍X�V
	void RailGuideBlinking::UpdateBlinking()
	{
		// ���������Z
		m_blinkRatio += DELTA_TIME / m_blinkTime;
		if (m_blinkRatio >= XM_2PI) m_blinkRatio = 0.0f;

		// �T�C���J�[�u�������ŋ��߁A�����F����ʏ�F�͈̔͂œ_�ł�����
		Col4 blinkColor = Utility::SinCurve(m_blinkRatio, COL_ALPHA, m_defColor);
		m_ptrDraw->SetDiffuse(blinkColor);
	}

	// �K�C�h�̍X�V
	void RailGuideBlinking::UpdateGuide()
	{
		// �K�C�h��csv��̃|�C���g�z��
		const auto& railManager = GetStage()->GetSharedGameObject<RailManager>(L"RailManager");
		const auto& guidePoints = railManager->GetGuidePoints();

		// �K�C�h�|�C���g�ɑ��Ⴊ�Ȃ���Ζ���
		if (m_pastGuidePoint == guidePoints) return;
		m_pastGuidePoint = guidePoints;

		// �s��p�ϐ��ƍs��z��
		Mat4x4 matrix, mtxPosition, mtxRotation;
		auto& matrixVec = m_ptrDraw->GetMatrixVec();
		matrixVec.clear();
		
		// �Ō�̃��[���ݒu���W
		Vec3 pastPos = railManager->GetPastRailPos();

		// �z��̐����[�v
		for (const auto& guide : guidePoints)
		{
			// �s��̐ݒ�ƒǉ�
			Vec3 addPos = ROWCOL2POS(guide.x, guide.y);
			mtxPosition.translation(addPos);
			mtxRotation = pastPos.x != addPos.x ? m_mtxRotAxisX : m_mtxRotAxisZ;
			matrix = m_mtxScale * mtxRotation * mtxPosition;
			matrixVec.push_back(matrix);
		}
	}
}