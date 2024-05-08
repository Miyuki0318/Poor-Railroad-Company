/*!
@file ActionGuide.cpp
@brief �s���̃K�C�h�A�C�R���`��
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �������̏��������s
	void ActionGuide::OnCreate()
	{
		// �X�v���C�g�̐���
		const auto& stagePtr = GetStage();
		m_spriteMap.push_back(SpriteType(eIconPivot::Center, eSpriteType::Icon,    stagePtr->AddGameObject<Sprite>()));
		m_spriteMap.push_back(SpriteType(eIconPivot::Center, eSpriteType::Balloon, stagePtr->AddGameObject<Sprite>(L"I_BALOON_CENTER_TX")));
		m_spriteMap.push_back(SpriteType(eIconPivot::Right,  eSpriteType::Icon,    stagePtr->AddGameObject<Sprite>()));
		m_spriteMap.push_back(SpriteType(eIconPivot::Right,  eSpriteType::Balloon, stagePtr->AddGameObject<Sprite>(L"I_BALOON_LEFT_TX")));
		m_spriteMap.push_back(SpriteType(eIconPivot::Left,   eSpriteType::Icon,    stagePtr->AddGameObject<Sprite>()));
		m_spriteMap.push_back(SpriteType(eIconPivot::Left,   eSpriteType::Balloon, stagePtr->AddGameObject<Sprite>(L"I_BALOON_RIGHT_TX")));

		// �X�P�[���̐ݒ�
		for (auto& sprite : m_spriteMap)
		{
			sprite.sprite.lock()->SetScale(m_scale);
		}

		SetDrawActive(false);
	}

	// ���t���[���X�V����
	void ActionGuide::OnUpdate()
	{
		// �A�N�e�B�u��������
		SetSpriteActive(false);

		// �A�C�R���^�C�v�̍X�V
		UpdateIconType();

		// �A�C�R���̕`��X�V
		UpdateIconSet();

		// �r���{�[�h�ƍ��W�̍X�V
		UpdatePosition();

		// �`��̍X�V
		UpdateIconDraw();
	}

	// ���W�̍X�V
	void ActionGuide::UpdatePosition()
	{
		// �v���C���[�����݂��邩�̃G���[�`�F�b�N
		const auto& player = m_player.lock();
		if (!player) return;

		// �v���C���[�̍��W+���㕪�̍��W
		Vec3 headPos = player->GetPosition() + m_diffPosition;
		
		// 2D���W�ɕϊ����č��W���X�V
		const auto& view = GetStage()->GetView();
		m_position = Utility::ConvertToWorldPosition(view, headPos);
		m_position.z = 0.0f;
	}

	// �A�C�R���^�C�v�̍X�V
	void ActionGuide::UpdateIconType()
	{
		// �o�b�t�@�̏�����
		m_iconBuffer.clear();

		// �v���C���[�ƃC���f�B�P�[�^�[�����݂��邩
		const auto& player = m_player.lock();
		const auto& indicator = m_indicator.lock();
		if (!player || !indicator) return;

		// �N���t�g�\�Ȃ�N���t�g�A�C�R��
		if (player->GetCraftPosshible()) m_iconBuffer.insert(make_pair(m_texMap.at(eActionIcon::Craft).layerIndex, eActionIcon::Craft));

		// �X�e�[�WcsvID�ɉ����Đݒ�
		const auto& stageMap = GetStage()->GetSharedGameObject<RailManager>(L"RailManager")->GetGuideMap();
		const auto& selectPoint = indicator->GetSelectPoint();
		if (!Utility::WithInElemRange(selectPoint.x, selectPoint.y, stageMap)) return;

		// ID�ɉ����ăA�C�R���^�C�v��ݒ�
		eStageID id = STAGE_ID(stageMap.at(selectPoint.x).at(selectPoint.y));

		// �K�C�h���[�����̐^�U�ƍ̌@�I�u�W�F�N�g���̐^�U
		bool isGuide = (id == eStageID::GuideRail && player->GetStatus(ePlayerStatus::IsHaveRail));
		bool isMining = (id != eStageID::GuideRail);

		for (const auto& type : m_iconMap)
		{
			if (id != type.first) continue;

			// id�ƃ^�C�v����v����Ȃ�
			if (isGuide) m_iconBuffer.insert(make_pair(m_texMap.at(type.second).layerIndex, type.second));
			if (isMining) m_iconBuffer.insert(make_pair(m_texMap.at(type.second).layerIndex, type.second));
		}
	}

	// �A�C�R�����X�v���C�g�ɐݒ�
	void ActionGuide::UpdateIconSet()
	{
		// �D��x�C���f�b�N�X�z��̐ݒ�
		vector<int> indices;
		size_t buffSize = m_iconBuffer.size();
		for (const auto& buff : m_iconBuffer)
		{
			// �A�N�V�����A�C�R���ƈ�v������ǉ�
			if (m_texMap.find(buff.second) != m_texMap.end())
			{
				indices.push_back(m_texMap.at(buff.second).layerIndex);
			}
		}

		// ���C���[�̃\�[�g
		sort(indices.begin(), indices.end());
		if (indices.empty()) return;

		// �\�����ɉ����ăA�C�R���̐ݒ���@��ύX
		int loopNum = 0;
		for (auto& sprite : m_spriteMap)
		{
			bool isCenterPivot = (sprite.pivot == eIconPivot::Center); // �Z���^�[���ǂ���
			bool isNotBalloon = (sprite.type != eSpriteType::Balloon); // �����o������Ȃ���

			// �\�������P�Ȃ�Z���^�[�ɁA�����Ȃ�Z���^�[�ȊO��
			if ((buffSize == 1 && isCenterPivot) || (buffSize > 1 && !isCenterPivot))
			{
				if (isNotBalloon)
				{
					auto textureIter = (buffSize == 1) ? m_iconBuffer.begin()->second : std::next(m_iconBuffer.begin(), loopNum)->second;
					sprite.sprite.lock()->SetTexture(m_texMap.at(textureIter).textureStr);
					loopNum++;
				}
				sprite.active = true;
			}
		}
	}

	// �A�C�R���`��X�V
	void ActionGuide::UpdateIconDraw()
	{
		for (auto& sprite : m_spriteMap)
		{
			// �A�N�e�B�u�Ȃ�
			if (sprite.active)
			{
				//�����Ȃ�
				if (m_iconBuffer.size() > 1)
				{
					Vec3 diss = (sprite.pivot != eIconPivot::Left) ? Vec3(m_distanceX, 0.0f, 0.0f) : Vec3(-m_distanceX, 0.0f, 0.0f);
					sprite.sprite.lock()->SetPosition(m_position - diss + m_typePos.at(sprite.type));
				}
				else // 1�Ȃ�
				{
					sprite.sprite.lock()->SetPosition(m_position + m_typePos.at(sprite.type));
				}
			}

			// �`��^�U�ݒ�
			sprite.sprite.lock()->SetDrawActive(sprite.active);
		}
	}
}