/*!
@file GroundManager.cpp
@brief �n�ʊǗ��I�u�W�F�N�g�N���X
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �������̏���
	void InstanceGround::OnCreate()
	{
		// �`��R���|�[�l���g�̐ݒ�
		m_ptrDraw = AddComponent<PNTStaticInstanceDraw>();
		m_ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		m_ptrDraw->SetDiffuse(m_groundColor);
	}

	// �������̏���
	void GroundManager::OnCreate()
	{
		// �I�u�W�F�N�g�̐���
		const shared_ptr<Stage>& stagePtr = GetStage();
		m_groundMap.emplace(eStageID::Grass, stagePtr->AddGameObject<InstanceGround>(COL_GREAN));
		m_groundMap.emplace(eStageID::Rock, stagePtr->AddGameObject<InstanceGround>(COL_YELOW));
		m_groundMap.emplace(eStageID::Water, stagePtr->AddGameObject<InstanceGround>(COL_BG));

		// �n�ʃ}�b�v
		auto& groundMap = GetTypeStage<StageCSV>()->GetGroundMap();

		// ��d���[�v
		for (size_t row = 0; row < groundMap.size(); row++)
		{
			for (size_t col = 0; col < groundMap.at(row).size(); col++)
			{
				// ���W�̐ݒ�
				Vec3 addPos = Vec3(float(col), m_defPosY, -float(row));

				// �g�����X�t�H�[���s��̐ݒ�
				Mat4x4 matrix, mtxPosition;
				mtxPosition.translation(addPos);

				// �s��̐ݒ�ƒǉ�
				matrix = m_mtxScale * m_mtxRotation * mtxPosition;

				// ID�ɉ����čs���ǉ�
				eStageID id = STAGE_ID(groundMap.at(row).at(col));
				for (auto& groud : m_groundMap)
				{
					if (groud.first == id)
					{
						groud.second.lock()->AddMatrix(matrix);
					}
				}
			}
		}
	}
}