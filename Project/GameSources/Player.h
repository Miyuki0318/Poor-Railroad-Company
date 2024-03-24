/*!
@file Player.h
@brief �v���C���[�I�u�W�F�N�g
@author ���V���M
*/

#pragma once
#include "TemplateObject.h"
#include "SelectIndicator.h"
#include "CraftManager.h"

namespace basecross
{
	/*!
	@brief �v���C���[�̏��
	*/
	enum class ePlayerStatus : uint8_t
	{
		IsIdle,		// �ҋ@���
		IsMove,		// �ړ����
		IsHaveWood,	// �؂�������
		IsHaveStone,// �΂�������
		IsCrafting,	// �N���t�g��
		IsHaveRail,	// ���H������
	};

	/*!
	@brief �A�C�e���^�C�v
	*/
	enum class eItemType : size_t
	{
		Wood,	// ��
		Stone,	// ��
		Rail,	// ���[��
	};

	/*!
	@brief �v���C���[
	*/
	class Player : public TemplateObject
	{
		weak_ptr<SelectIndicator> m_indicator; // �Z���N�g�C���f�B�P�[�^�[
		shared_ptr<CraftManager> m_craft;      // �N���t�g�}�l�[�W���[

		shared_ptr<PNTStaticDraw> m_ptrDraw;  // �`��R���|�[�l���g
		shared_ptr<CollisionObb> m_ptrColl;   // �R���W����OBB�R���|�[�l���g
		Bool8_t<ePlayerStatus> m_status;	  // �t���O�Ǘ��N���X

		vector<int> m_itemCount; // �A�C�e���J�E���^

		const float m_speed; // ���x

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		Player(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr, Vec3(0.0f, 1.5f, 0.0f), Vec3(0.0f), Vec3(1.0f, 2.0f, 1.0f)),
			m_speed(5.0f)
		{
			m_status = 0;
			m_itemCount = { 10, 10, 0 };
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~Player() 
		{
			m_craft.reset();
		}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;

	private:

		/*!
		@brief A�{�^�����͎��ɌĂяo�����֐�
		*/
		void OnPushA();

		/*!
		@brief �N���t�g���ɌĂяo�����֐�
		*/
		void OnCraft();

		/*!
		@brief �̌@���ɌĂяo�����֐�
		*/
		void OnMining(const shared_ptr<TemplateObject>& miningObj);

		/*!
		@brief ���[���ݒu���ɌĂяo�����֐�
		*/
		void OnRailed(const Vec3& railPosition);

		/*!
		@brief �ړ��X�V�֐�
		*/
		void UpdateMove();

		/*!
		@brief �R���g���[���[��]�֐�
		*/
		void ControllerRotation(const Vec3& stickValue);

		/*!
		@brief �R���g���[���[�ړ��֐�
		*/
		void ControllerMovement(const Vec3& stickValue);

	public:

		/*!
		@brief ��Ԏ擾�֐�
		@param �v���C���[�̏��enum
		@return ���̏�ԂɂȂ��Ă��邩�̐^�U
		*/
		bool GetStatus(ePlayerStatus status)
		{
			return m_status(status);
		}

		/*!
		@brief �A�C�e�����擾�֐�
		@param �A�C�e���^�C�venum
		@return �A�C�e����
		*/
		int GetItemCount(eItemType type)
		{
			return m_itemCount.at(static_cast<size_t>(type));
		}

		/*!
		@brief �A�C�e�����ǉ��֐�
		@param �A�C�e���^�C�venum
		@param �ǉ���(�f�t�H1)
		*/
		void AddItemCount(eItemType type, int addNum = 1)
		{
			m_itemCount.at(static_cast<size_t>(type)) += addNum;
		}
	};
}