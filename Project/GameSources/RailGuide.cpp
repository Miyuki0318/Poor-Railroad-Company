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
		m_ptrDraw = AddComponent<PCTStaticInstanceDraw>();
		m_ptrDraw->SetOriginalMeshUse(true);
		m_ptrDraw->CreateOriginalMesh(m_vertex);
		m_ptrDraw->SetTextureResource(L"ICON_RAIL_TX");

		// �`��ݒ�
		SetAlphaActive(true);
		SetDrawActive(false);
	}

	// ���t���[���X�V����
	void RailGuide::OnUpdate()
	{
		// �e��X�V����
		UpdateBillboard();
		UpdateGuide();

		// �v���C���[�����[���������Ă邩�ŕ\����\��
		const auto& player = GetStage()->GetSharedGameObject<GamePlayer>(L"Player");
		SetDrawActive(player->GetStatus(ePlayerStatus::IsHaveRail));
	}

	// �r���{�[�h�̍X�V
	void RailGuide::UpdateBillboard()
	{
		// �J���������Ƀr���{�[�h�֐��ŃN�H�[�^�j�I����ݒ�
		const auto& stage = GetStage();
		const auto& ptrCamera = stage->GetView()->GetTargetCamera();
		Quat qt = Utility::GetBillboardQuat(ptrCamera->GetAt() - ptrCamera->GetEye());
		m_mtxRotation.rotation(qt);
	}

	// �K�C�h�̍X�V
	void RailGuide::UpdateGuide()
	{
		// �s��p�ϐ��ƍs��z��
		Mat4x4 matrix, mtxPosition;
		auto& matrixVec = m_ptrDraw->GetMatrixVec();
		matrixVec.clear();
		
		// �K�C�h��csv��̃|�C���g�z��
		const auto guidePoints = GetStage()->GetSharedGameObject<RailManager>(L"RailManager")->GetGuidePoints();
		
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