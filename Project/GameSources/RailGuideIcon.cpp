/*!
@file RailGuideIcon.cpp
@brief ���[����ݒu����ꏊ�̃K�C�h�\��
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �������̏���
	void RailGuideIcon::OnCreate()
	{
		// �X�e�[�W�̎擾
		const auto& stagePtr = GetStage();

		// �v���C���[�̃|�C���^��ێ�
		m_playerPtr = stagePtr->GetSharedGameObject<GamePlayer>(L"Player");

		// �A�C�R���̐���
		for (size_t i = 0; i < m_iconVec.size(); i++)
		{
			auto& ptr = stagePtr->AddGameObject<Billboard>(L"GUIDE_RAIL_TX", m_deffScale, Vec3(0.0f));
			ptr->SetDrawActive(false);
			ptr->SetUpdateActive(false);

			m_iconVec.at(i) = ptr;
		}
	}

	// ���t���[���X�V
	void RailGuideIcon::OnUpdate()
	{
		// �v���C���[�����[���������Ă邩�ŕ\����\��
		const auto& player = GetStage()->GetSharedGameObject<GamePlayer>(L"Player");
		bool isHaveRail = player->GetStatus(ePlayerStatus::IsHaveRail);
		
		// ��x��\���ɂ���
		SetIconDraw(false);

		// �v���C���[�����[�����������Ă���Ȃ�
		if (isHaveRail)
		{
			// �e��X�V����
			UpdateIconMove();
			UpdateGuide();
			UpdateRange();
		}
	}

	// �����̍X�V
	void RailGuideIcon::UpdateIconMove()
	{
		// �J�������擾
		const auto& camera = GetStage()->GetView()->GetTargetCamera();

		// �ړ��ʂ����߂�
		float moveVal = Utility::SinCurve(m_totalTime, 0.0f, m_boundValue);
		Quat rot = Utility::GetBillboardRotateQuat(camera->GetAt() - camera->GetEye(), Vec3(0.0f, m_totalTime, 0.0f));

		// ���������Z
		m_totalTime += DELTA_TIME * XM_PI;
		if (m_totalTime >= XM_2PI) m_totalTime = 0.0f;

		// ���W�̍X�V
		for (size_t i = 0; i < m_pastGuidePoint.size(); i++)
		{
			Point2D<size_t> point = m_pastGuidePoint.at(i);
			Vec3 pos = Vec3(float(point.y), m_deffPosY, -float(point.x));
			m_iconVec.at(i).lock()->SetPosition(pos + Vec3(0.0f, moveVal, 0.0f));
			m_iconVec.at(i).lock()->SetQuaternion(rot);
		}
	}

	// �K�C�h�̍X�V
	void RailGuideIcon::UpdateGuide()
	{
		// �K�C�h��csv��̃|�C���g�z��
		const auto& railManager = GetStage()->GetSharedGameObject<RailManager>(L"RailManager");
		const auto& guidePoints = railManager->GetGuidePoints();

		bool isIdentity = (guidePoints == m_pastGuidePoint);
		m_pastGuidePoint = guidePoints;

		// ���W�̍X�V
		for (size_t i = 0; i < m_iconVec.size(); i++)
		{
			bool isRange = Utility::WithInElemRange(i, guidePoints.size());
			m_iconVec.at(i).lock()->SetDrawActive(isRange);
			if (!isRange) continue;
			if (isIdentity) continue;

			Point2D<size_t> point = guidePoints.at(i);
			m_iconVec.at(i).lock()->SetPosition(Vec3(float(point.y), m_deffPosY, -float(point.x)));
		}
	}

	// �v���C���[�Ƃ̋����ŕ\�����邩
	void RailGuideIcon::UpdateRange()
	{
		// �v���C���[�̎擾
		const auto& playerPtr = m_playerPtr.lock();
		if (!playerPtr) return;

		// �v���C���[�̍��W
		Vec3 playerPos = playerPtr->GetPosition();

		// �������r���A�`�拗����藣��Ă���Ε\��
		for (auto& icon : m_iconVec)
		{
			float length = (icon.lock()->GetPosition() - playerPos).length();
			icon.lock()->SetDrawActive(m_drawRange <= length);
		}
	}

	// �A�C�R���S�̂̕\���ݒ�
	void RailGuideIcon::SetIconDraw(bool b)
	{
		for (auto& icon : m_iconVec)
		{
			icon.lock()->SetDrawActive(b);
		}
	}
}