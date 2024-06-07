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

	// ����������
	void BridgeManager::ResetBridge()
	{
		m_ptrDraw->ClearMatrixVec();
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
		matrix = m_mtxScale * m_mtxRotation * mtxPosition;
		m_ptrDraw->AddMatrix(matrix);

		// csv�̏�������
		stageMap.at(point.x).at(point.y) = UnSTAGE_ID(eStageID::WoodBridge);
	}

	// �w��̍��W�ɐ��ꂪ���邩
	bool BridgeManager::GetIsWaterPoint(const Point2D<size_t>& point) const
	{
		// �z��͈͓̔����̃G���[�`�F�b�N
		const auto& stageMap = GetTypeStage<BaseStage>()->GetStageMap();
		if (!WithInElemRange(point.x, point.y, stageMap)) return false;

		// �����ID���ǂ���
		return eStageID::Water == STAGE_ID(stageMap.at(point.x).at(point.y));
	}
}