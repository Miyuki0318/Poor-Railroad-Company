/*!
@file Gear.cpp
@brief ���؂Ɏg�p���鎕��
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �������̏���
	void Gear::OnCreate()
	{
		// �p�����̏���������
		TemplateObject::OnCreate();
		SetTransParam();

		// �`��R���|�[�l���g�̐ݒ�
		m_ptrDraw = AddComponent<BcPNTBoneModelDraw>();
		m_ptrDraw->SetMeshResource(L"GEAR");
		m_ptrDraw->AddAnimation(L"ROTATE", 0, 60, true);
		m_ptrDraw->ChangeCurrentAnimation(L"ROTATE");

		// �e�p�`��R���|�[�l���g�̐ݒ�
		m_ptrShadow = AddComponent<Shadowmap>();
		m_ptrShadow->SetMeshResource(L"GEAR");
	}

	// �A�j���[�V�����̍X�V
	void Gear::UpdateAnimation()
	{
		m_ptrDraw->UpdateAnimation(DELTA_TIME);
	}

	// �M�A�̔�A�N�e�B�u��
	void Gear::GetGearSleap()
	{
		SetDrawActive(false);
		SetUpdateActive(false);
	}

	// �M�A�̃A�N�e�B�u��
	void Gear::ResetGear()
	{
		SetDrawActive(true);
		SetUpdateActive(true);
	}

	// �������̏���
	void GearManager::OnCreate()
	{
		// �X�e�[�W�ƃX�e�[�Wcsv�̎擾
		const auto& stagePtr = GetTypeStage<BaseStage>();
		
		// �v���C���[�̎擾
		m_playerPtr = stagePtr->GetSharedGameObject<GamePlayer>(L"Player");

		// ���Ԃ̐���
		CreateGear();
	}

	// �X�V����
	void GearManager::OnUpdate()
	{
		// �v���C���[�̎擾
		const auto& playerPtr = m_playerPtr.lock();
		if (!playerPtr) return;

		// �v���C���[���W���擾
		Vec3 playerPos = playerPtr->GetPosition();
		for (auto& gear : m_gearMap)
		{
			// ���݂��Ȃ��A��A�N�e�B�u�Ȃ疳��
			auto& ptr = gear.second.lock();
			if (!ptr) continue;
			if (!ptr->GetUpdateActive()) continue;

			// �������r���A�擾�͈͂���Ȃ���΃A�j���[�V�����̍X�V
			float length = (ptr->GetPosition() - playerPos).length();
			if (length > m_getGearRange)
			{
				ptr->UpdateAnimation();
				continue;
			}

			// �擾����
			GetGearPlayar(gear);
		}
	}

	void GearManager::CreateGear()
	{
		// �X�e�[�W�ƃX�e�[�Wcsv�̎擾
		const auto& stagePtr = GetTypeStage<BaseStage>();
		const auto& stageMap = stagePtr->GetStageMap();

		// ��d���[�v
		for (int row = 0; row < stageMap.size(); row++)
		{
			for (int col = 0; col < stageMap.at(row).size(); col++)
			{
				// ���ԈȊO��ID�Ȃ疳��
				eStageID id = STAGE_ID(stageMap.at(row).at(col));
				if (id != eStageID::Gear) continue;

				bool isBuff = false;
				for (auto& gear : m_gearMap)
				{
					// ���݂��Ȃ��Ȃ疳��
					auto& ptr = gear.second.lock();
					if (!ptr) continue;

					// ��A�N�e�B�u�������
					if (!ptr->GetUpdateActive()) 
					{
						ptr->SetPosition(ROWCOL2POS(row, col));
						ptr->ResetGear();
						isBuff = true;
						break;
					}
				}

				// �󂫂��g�p�����Ȃ疳��
				if (isBuff) continue;

				// ���Ԃ𐶐����A�}�b�v�ɒǉ�
				m_gearMap.emplace(ROWCOL2LINE(row, col), stagePtr->AddGameObject<Gear>(ROWCOL2POS(row, col)));
			}
		}

		// �����������Ԃɍ����s���ݒ�
		for (auto& gear : m_gearMap)
		{
			auto& ptr = gear.second.lock();
			ptr->m_ptrDraw->SetMeshToTransformMatrix(m_gearModelMat);
			ptr->m_ptrShadow->SetMeshToTransformMatrix(m_gearModelMat);
		}
	}

	// ���Ԃ̎擾����
	void GearManager::GetGearPlayar(const pair<string, weak_ptr<Gear>>& gear)
	{
		// �X�e�[�Wcsv�̏�������
		size_t row, col;
		GetLineStringToRowCol(row, col, gear.first);
		auto& stageMap = GetTypeStage<BaseStage>()->GetStageMap();
		stageMap.at(row).at(col) = UnSTAGE_ID(eStageID::None);

		// �v���C���[�Ɏ��Ԏ擾�����𑗂�
		const auto& playerPtr = dynamic_pointer_cast<GamePlayer>(m_playerPtr.lock());
		playerPtr->AddItemCount(eItemType::Gear);

		// ���ԂɎ擾��̏����𑗂�ASE���Đ�
		auto& ptr = gear.second.lock();
		ptr->GetGearSleap();
		ptr->StartSE(L"C_SUCCES_SE", 1.0f);
	}

	// ���Z�b�g����
	void GearManager::ResetGears()
	{
		// �S�Ă̎��ԂɃX���[�u�����𑗂�
		for (auto& gear : m_gearMap)
		{
			gear.second.lock()->GetGearSleap();
		}

		// �Đ���
		CreateGear();
	}
}