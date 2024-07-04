/*!
@file ItemFly.h
@brief �A�C�e���ǉ�����UI�ɔ��ł����G�t�F�N�g
@author ���V���M
*/

#pragma once
#include "Sprite.h"

namespace basecross
{
	/*!
	@brief �A�C�e���ǉ����ɔ��ł����G�t�F�N�g
	*/
	class FlySprite : public Sprite
	{
		// �Ȑ���ԂɕK�v�ȍ��W
		Vec3 m_targetPointStart;	// �J�n�ʒu
		Vec3 m_targetPointEnd;		// �I���ʒu
		Vec3 m_targetPointMiddle;	// ���Ԉʒu

		// ���^�G�t�F�N�g
		weak_ptr<SpriteParticle> m_starEffect;

		const float m_flyTime;	// ���Ď���
		float m_totalTime;	// �o�ߎ���

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		FlySprite(const shared_ptr<Stage>& stagePtr) :
			Sprite(stagePtr, L"WHITE_TX", Vec2(50.0f)),
			m_flyTime(1.0f)
		{
			m_targetPointStart.zero();
			m_targetPointEnd.zero();
			m_targetPointMiddle.zero();

			m_totalTime = 0.0f;
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~FlySprite() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;

		/*!
		@brief �`��J�n�֐�
		@param �e�N�X�`���L�[
		@param �J�n���W
		@param ���ԍ��W
		@param �I�����W
		*/
		void StartDraw(const wstring& texWstr, const Vec3& start, const Vec3& middle, const Vec3& end);
	};

	/*!
	@brief ���Ă���A�C�e���G�t�F�N�g�̊Ǘ��I�u�W�F�N�g
	*/
	class FlyItemManager : public GameObject
	{
		// �X�v���C�g�z��
		vector<weak_ptr<FlySprite>> m_spriteVec;

		// �A�C�e��UI�Ɋւ���f�[�^
		map<eItemType, pair<Vec3, wstring>> m_itemUIPosMap;

		// �v���C���[�̃|�C���^
		weak_ptr<TemplateObject> m_playerPtr;

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �v���C���[�̃|�C���^
		*/
		FlyItemManager(const shared_ptr<Stage>& stagePtr,
			const shared_ptr<TemplateObject>& playerPtr
		) :
			GameObject(stagePtr),
			m_playerPtr(playerPtr)
		{
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~FlyItemManager() {}

		/*!
		@brief ���Z�b�g����
		*/
		void ResetFly();

		/*!
		@brief �G�t�F�N�g�J�n�֐�
		@param �A�C�e���̃^�C�v
		*/
		void StartFly(eItemType itemType);

		/*!
		@brief �A�C�e��UI�̃f�[�^�ݒ�֐�
		@param �A�C�e���̃^�C�v
		@param �e�N�X�`���L�[
		@param UI�̍��W
		*/
		void SetTargetUIData(eItemType itemType, wstring texture, Vec3 position)
		{
			m_itemUIPosMap.emplace(itemType, make_pair(position, texture));
		}
	};
}