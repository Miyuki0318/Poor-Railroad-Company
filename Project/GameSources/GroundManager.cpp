/*!
@file GroundManager.cpp
@brief �n�ʊǗ��I�u�W�F�N�g�N���X
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

#define END_INDEX 10 // ��؂�܂ł̐�
#define WIN_RANGE 30 // �J���������ʊO�܂ł̃Q�[����ł̋���

namespace basecross
{
	// �l�[���X�y�[�X�̏ȗ�
	using namespace Utility;

	// �������̏���
	void InstanceGround::OnCreate()
	{
		// �`��R���|�[�l���g�̐ݒ�
		m_ptrDraw = AddComponent<PNTStaticInstanceDraw>();
		m_ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		m_ptrDraw->SetOwnShadowActive(true);
		m_ptrDraw->SetDiffuse(m_groundColor);
	}

	// �������̏���
	void GroundManager::OnCreate()
	{
		// �I�u�W�F�N�g�̐���
		const shared_ptr<Stage>& stagePtr = GetStage();

		// �n�ʃ}�b�v
		auto& groundMap = GetTypeStage<BaseStage>()->GetGroundMap();
		map<int, weak_ptr<InstanceGround>> grass, unGrass, sand, water, rock;
		for (int i = 0; i < groundMap.front().size(); i += END_INDEX)
		{
			// ��؂萔���ǉ�
			grass.emplace(i, stagePtr->AddGameObject<InstanceGround>(COL_GREAN));
			unGrass.emplace(i, stagePtr->AddGameObject<InstanceGround>(COL_GREAN));
			sand.emplace(i, stagePtr->AddGameObject<InstanceGround>(COL_YELOW));
			water.emplace(i, stagePtr->AddGameObject<InstanceGround>(COL_BG));
			rock.emplace(i, stagePtr->AddGameObject<InstanceGround>(Col4(0.1f, 0.1f, 0.1f, 1.0f)));
		}

		// �e�^�C�v�Ő��������}�b�v��StageID���L�[�ɓ񎟌��}�b�v��
		m_groundMap.emplace(eStageID::Grass, grass);		// ���n
		m_groundMap.emplace(eStageID::UnGrass, unGrass);	// �ʉߕs�̑��n
		m_groundMap.emplace(eStageID::Sand, sand);			// ���n
		m_groundMap.emplace(eStageID::Water, water);		// ����
		m_groundMap.emplace(eStageID::Rock, rock);		// ��n


		// ��d���[�v
		for (int row = 0; row < groundMap.size(); row++)
		{
			for (int col = 0; col < groundMap.at(row).size(); col++)
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
					// eStageID����v���Ȃ��Ȃ疳��
					if (groud.first != id) continue;

					// �^�C�v���Ƀ��[�v
					for (auto& type : groud.second)
					{
						// col���C���f�b�N�X����C���f�b�N�X+��؂鐔�͈͓̔���
						if (type.first <= col && type.first + END_INDEX > col)
						{
							type.second.lock()->AddMatrix(matrix);
						}
					}
				}
			}
		}
	}

	// ���t���[���X�V����
	void GroundManager::OnUpdate()
	{
		// �J�����̒����_���WX��
		int atX = int(GetStage()->GetView()->GetTargetCamera()->GetAt().x);

		// �J�����̒����_X������v�����āA��܂��ɉ�ʓ��ł���Ε\������
		for (auto& groud : m_groundMap)
		{
			for (auto& type : groud.second)
			{
				// �J�n�C���f�b�N�X����ʓ��ł��邩
				type.second.lock()->SetDrawActive(GetBetween(type.first, atX + WIN_RANGE, atX - WIN_RANGE));
			}
		}
	}
}