/*!
@file TitleGuide.cpp
@brief �^�C�g���X�e�[�W�̃K�C�h�A�C�R���`��
@author ���V���M
*/

#include "stdafx.h"
#include "TitleGuide.h"
#include "TitleStage.h"

namespace basecross
{
	// �������̏���
	void TitleGuide::OnCreate()
	{
		// �������̏���
		Sprite::OnCreate();

		// �`��ݒ�
		SetDrawLayer(1);
		SetDrawEnable(false);
		SetVerticesRect(eRectType::UpLeft);

		// �}�b�v�Ƀ^�C�g���X�e�[�W�I�u�W�F�N�g��ǉ�
		const auto& stagePtr = GetTypeStage<TitleStage>();
		m_titleObjMap.emplace(L"ROUTEMAP_TX", stagePtr->GetSharedGameObject<TemplateObject>(L"RouteMap"));
		m_titleObjMap.emplace(L"START_TRAIN_TX", stagePtr->GetSharedGameObject<TemplateObject>(L"TitleTrain"));
		m_titleObjMap.emplace(L"COMPANIY_TX", stagePtr->GetSharedGameObject<TemplateObject>(L"Company"));

		// �v���C���[�̕ێ�
		m_playerPtr = stagePtr->GetSharedGameObject<TemplateObject>(L"Player");
	}

	// ���t���[���X�V����
	void TitleGuide::OnUpdate()
	{
		// �p�����̍X�V����
		CraftUI::OnUpdate();

		// �^�C�g���X�e�[�W���C�x���g���Ȃ��\���ɂ��ďI��
		bool isStaging = GetTypeStage<TitleStage>()->GetIsStaging();
		if (isStaging)
		{
			SetDrawEnable(false);
			return;
		}

		// �����Ŕ�r���A�e�N�X�`����ݒ�
		if (!SetRangeTexture()) return;

		// ���W�̍X�V����
		UpdatePosition();
	}

	// �e�N�X�`���̐ݒ�
	bool TitleGuide::SetRangeTexture()
	{
		// �v���C���[�̎擾
		const auto& player = m_playerPtr.lock();
		if (!player) return false;

		// �v���C���[�Ƃ̋������r���A�͈͓��Ȃ�e�N�X�`����ݒ�
		bool isRange = false;
		Vec3 playerPos = player->GetPosition();
		for (auto& obj : m_titleObjMap)
		{
			// �I�u�W�F�N�g�̍��W���擾���A���������߂�
			Vec3 objPos = obj.second.lock()->GetPosition();
			float length = (playerPos - objPos).length();

			// �͈͓�����Ȃ���Ζ���
			if (length >= m_drawRange) continue;

			// �e�N�X�`����ݒ肵�A�\��������
			isRange = true;
			SetTexture(obj.first);
			SetDrawEnable(true);
		}

		// �͈͓��ɑ��݂��Ȃ���Δ�\���ɂ���
		if (!isRange) SetDrawEnable(false);

		// �͈͓��ɑ��݂�������Ԃ�
		return isRange;
	}

	// ���W�̍X�V
	void TitleGuide::UpdatePosition()
	{
		// �v���C���[�̎擾
		const auto& player = m_playerPtr.lock();
		if (!player) return;

		// �r���[�ƃv���C���[�̍��W����2D���W���擾
		const auto& view = GetStage()->GetView();
		Vec3 playerPos = player->GetPosition();
		Vec3 spritePos = Utility::ConvertToWorldPosition(view, playerPos);

		// Z����0�ɋ������A�����ƍ��킹�����W�ōX�V
		spritePos.z = 0.0f;
		SetPosition(spritePos + m_diffPosition);
	}
}