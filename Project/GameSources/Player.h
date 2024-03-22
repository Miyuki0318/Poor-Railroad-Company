/*!
@file Player.h
@brief �v���C���[�I�u�W�F�N�g
@author ���V���M
*/

#pragma once
#include "TemplateObject.h"

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
	@brief �v���C���[
	*/
	class Player : public TemplateObject
	{
		weak_ptr<TemplateObject> m_indicator; // �Z���N�g�C���f�B�P�[�^�[

		shared_ptr<PNTStaticDraw> m_ptrDraw;  // �`��R���|�[�l���g
		shared_ptr<CollisionObb> m_ptrColl;   // �R���W����OBB�R���|�[�l���g
		Bool8_t<ePlayerStatus> m_status;	  // �t���O�Ǘ��N���X

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
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~Player() {}

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
		@brief �̌@���ɌĂяo�����֐�
		*/
		void OnMining();

		/*!
		@brief �N���t�g���ɌĂяo�����֐�
		*/
		void OnCraft();

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
	};
}