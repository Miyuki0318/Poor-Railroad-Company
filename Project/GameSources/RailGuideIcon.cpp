#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void RailGuideIcon::OnCreate()
	{
		const auto& stagePtr = GetStage();

		for (size_t i = 0; i < m_iconVec.size(); i++)
		{
			auto& ptr = stagePtr->AddGameObject<Billboard>(L"GUIDE_RAIL_TX", m_deffScale, Vec3(0.0f));
			ptr->SetDrawActive(false);
			ptr->SetUpdateActive(false);

			m_iconVec.at(i) = ptr;
		}
	}

	void RailGuideIcon::OnUpdate()
	{
		// �v���C���[�����[���������Ă邩�ŕ\����\��
		const auto& player = GetStage()->GetSharedGameObject<GamePlayer>(L"Player");
		bool isHaveRail = player->GetStatus(ePlayerStatus::IsHaveRail);

		SetIconDraw(false);

		if (isHaveRail)
		{
			// �e��X�V����
			UpdateIconMove();
			UpdateGuide();
		}
	}

	// �_�ł̍X�V
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

	// �A�C�R���S�̂̕\���ݒ�
	void RailGuideIcon::SetIconDraw(bool b)
	{
		for (auto& icon : m_iconVec)
		{
			icon.lock()->SetDrawActive(b);
		}
	}
}