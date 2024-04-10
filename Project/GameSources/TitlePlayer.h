/*!
@file TitlePlayer.h
@brief �^�C�g���p�v���C���[�I�u�W�F�N�g
@author ���V���M
*/

#pragma once
#include "TemplateObject.h"

namespace basecross
{
	/*!
	@brief �^�C�g���p�v���C���[
	*/
	class TitlePlayer : public TemplateObject
	{
		shared_ptr<BcPNTBoneModelDraw> m_ptrDraw; // �`��R���|�[�l���g

		Vec3 m_rotTarget;	// ��]��
		Vec3 m_currentRot;  // �O��̉�]��
		Mat4x4 m_modelMat;	// ���f���ƃg�����X�t�H�[�������s��

		const float m_moveSpeed; // �ړ����x
		const float m_rotSpeed;  // ��]���x

	public:
		
		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		TitlePlayer(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr, Vec3(0.0f, 3.0f, 0.0f), Vec3(0.0f), Vec3(1.0f)),
			m_moveSpeed(5.0f), // ����CSV���瑬�x���̃X�e�[�^�X���擾�\��
			m_rotSpeed(0.5f)  // ����CSV���瑬�x���̃X�e�[�^�X���擾�\��
		{
			m_rotTarget.zero(); // ��]���0.0f�ŏ�����
			m_currentRot.zero(); // ��]���0.0f�ŏ�����

			// �X�P�[�������AY��������2�{�ɂ���
			m_modelMat.affineTransformation(
				Vec3(0.6f),
				Vec3(0.0f),
				Vec3(0.0f, -XM_PIDIV2, 0.0f),
				Vec3(0.0f, -1.0f, 0.0f)
			);
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~TitlePlayer() {}

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
		@brief �ړ��X�V�֐�
		*/
		void UpdateMove();

		/*!
		@brief ��]�X�V�֐�
		*/
		void UpdateRotation();

		/*!
		@brief ��]��ݒ�֐�
		@param L�X�e�B�b�N���͗�
		*/
		void SetRotateTarget(const Vec3& stickValue);

		/*!
		@brief �R���g���[���[�ړ��֐�
		@param L�X�e�B�b�N���͗�
		*/
		void ControllerMovement(const Vec3& stickValue);
	};
}