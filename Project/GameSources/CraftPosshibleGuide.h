/*!
@file CraftPosshibleGuide.h
@brief �N���t�g�\�ɂȂ������ɏo��G�t�F�N�g
@author ���V���M
*/

#pragma once
#include "SpriteParticle.h"
#include "GamePlayer.h"
#include "Bool_t.h"

namespace basecross
{
	/*!
	@brief �N���t�g�\���̃G�t�F�N�g
	*/
	class CraftPosshibleGuide : public TemplateObject
	{
		weak_ptr<SpriteParticle> m_effect;	// �p�[�e�B�N��
		weak_ptr<GamePlayer> m_playerPtr;	// �v���C���[
		weak_ptr<SoundItem> m_seItem;		// SE
		Bool8_t<eCraftItem> m_posshible;	// �N���t�g�\��
		map<eCraftItem, Vec3> m_uiPosMap;	// UI�̍��W�}�b�v

		// �N���t�g�\��Ԃ�\���ԊۃA�C�R��
		map<eCraftItem, ParticleData> m_posshibleIcons;

		const float m_drawTime;	// �\������
		const Vec2 m_effScale;	// �G�t�F�N�g�̃X�P�[��
		const Vec2 m_iconScale;	// �A�C�R���̃X�P�[��

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		CraftPosshibleGuide(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr),
			m_drawTime(1.0f),
			m_effScale(200.0f),
			m_iconScale(35.0f)
		{
			m_posshible = 0;
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~CraftPosshibleGuide() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;

		/*!
		@brief �N���t�g�\���ǂ����̃A�C�R���X�V�֐�
		@param ���݂̃t���O
		*/
		void UpdatePosshibleIcon(Bool8_t<eCraftItem>& flag);

		/*!
		@brief �N���t�g�\�ɂȂ������ǂ����̃`�F�b�N�֐�
		@param ���݂̃t���O
		@param �N���t�g����A�C�e��
		*/
		void CheckPosshible(Bool8_t<eCraftItem>& flag, eCraftItem type);

		/*!
		@brief �G�t�F�N�g�J�n�֐�
		@param �N���t�g����A�C�e��
		*/
		void StartEffect(eCraftItem type);

		/*!
		@brief �G�t�F�N�g�̐������pUI���W�ݒ�֐�
		@param �N���t�g����A�C�e��
		@param UI�̍��W
		*/
		void SetUIPosition(eCraftItem type, Vec3 position);
	}; 
}