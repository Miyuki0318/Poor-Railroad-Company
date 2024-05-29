/*!
@file BridgeManager.cpp
@brief �؂̑���Ǘ��N���X
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �l�[���X�y�[�X�̏ȗ�
	using namespace Utility;

	// �s��v�Z�Ɏg�����[�e�[�V�����ƃX�P�[��
	const Mat4x4 mtxRotation = Mat4x4().rotation((Quat)XMQuaternionRotationRollPitchYawFromVector(Vec3(0.0f, XM_PIDIV2, 0.0f)));
	const Mat4x4 mtxScale = Mat4x4().scale(Vec3(0.35f));

	// �������̏���
	void BridgeManager::OnCreate()
	{
		// �`��R���|�[�l���g�̐ݒ�
		m_ptrDraw = AddComponent<PNTStaticInstanceDraw>();
		m_ptrDraw->SetMeshResource(L"WOOD_BRIDGE");
		m_ptrDraw->SetTextureResource(L"WOOD_BRIDGE_TX");
		m_ptrDraw->SetSpecular(COL_WHITE);
		m_ptrDraw->SetDiffuse(COL_WHITE);
	}

	// ���̒ǉ�����
	void BridgeManager::AddBridge(const Point2D<size_t>& point)
	{
		// �X�e�[�W��csv�̎擾
		auto& stageMap = GetTypeStage<BaseStage>()->GetStageMap();
		if (!WithInElemRange(point.x, point.y, stageMap)) return;

		// ���W�̐ݒ�
		Vec3 addPos = ROWCOL2POS(point.x, point.y);

		// �g�����X�t�H�[���s��̐ݒ�
		Mat4x4 matrix, mtxPosition;
		mtxPosition.translation(addPos);

		// �s��̐ݒ�ƒǉ�
		matrix = mtxScale * mtxRotation * mtxPosition;
		m_ptrDraw->AddMatrix(matrix);

		// csv�̏�������
		stageMap.at(point.x).at(point.y) = UnSTAGE_ID(eStageID::WoodBridge);
	}

	// �w��̍��W�ɃK�C�h�����邩
	bool BridgeManager::GetIsWaterPoint(const Point2D<size_t>& point) const
	{
		// �z��͈͓̔����̃G���[�`�F�b�N
		const auto& stageMap = GetTypeStage<BaseStage>()->GetStageMap();
		if (!WithInElemRange(point.x, point.y, stageMap)) return false;

		// �K�C�hID���ǂ���
		return eStageID::Water == STAGE_ID(stageMap.at(point.x).at(point.y));
	}
}