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
	@brief �v���C���[
	*/
	class Player : public TemplateObject
	{
	protected:

		shared_ptr<BcPNTBoneModelDraw> m_ptrDraw; // �`��R���|�[�l���g

		const float m_radius;	 // �Փ˔���p���a
		const float m_margin;	 // �O���b�h�Ƃ̗]��
		const float m_moveSpeed; // �ړ����x
		const float m_rotSpeed;  // ��]���x

		set<eStageID> m_impassableSet; // �ʂ�Ȃ��X�e�[�WID���X�g
		Vec3 m_rotTarget;	// ��]��
		Vec3 m_currentRot;  // �O��̉�]��
		Mat4x4 m_modelMat;	// ���f���ƃg�����X�t�H�[�������s��
		size_t m_row;		// csv��̗�
		size_t m_col;		// csv��̍s

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		Player(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr, Vec3(0.0f, 3.0f, 0.0f), Vec3(0.0f), Vec3(1.0f)),
			m_moveSpeed(5.0f), // ����CSV���瑬�x���̃X�e�[�^�X���擾�\��
			m_rotSpeed(0.5f),  // ����CSV���瑬�x���̃X�e�[�^�X���擾�\��
			m_radius(1.0f),
			m_margin(0.1f)
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
		virtual ~Player() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		virtual void OnCreate() override;

	protected:

		/*!
		@brief �R���|�[�l���g�̐����֐�
		*/
		virtual void CreateComponent() = 0;

		/*!
		@brief �ړ��X�V�֐�
		*/
		virtual void UpdateMove() = 0;

		/*!
		@brief ��]�X�V�֐�
		*/
		virtual void UpdateRotation();

		/*!
		@brief ��]��ݒ�֐�
		@param L�X�e�B�b�N���͗�
		*/
		virtual void SetRotateTarget(const Vec3& stickValue);

		/*!
		@brief �R���g���[���[�ړ��֐�
		@param L�X�e�B�b�N���͗�
		*/
		virtual void ControllerMovement(const Vec3& stickValue);

		/*!
		@brief �ړ��o���Ȃ��O���b�h���܂�����֐�
		@param �|�W�V����
		*/
		virtual void GridHitResponse(Vec3& pos);

	private:

		/*!
		@brief �ړ��o���Ȃ��O���b�h(��)���܂�����֐�
		@param �|�W�V����
		@param �X�e�[�W�}�b�v
		*/
		virtual void GridHitFlontResponse(Vec3& pos, const vector<vector<int>>& stageMap);

		/*!
		@brief �ړ��o���Ȃ��O���b�h(��)���܂�����֐�
		@param �|�W�V����
		@param �X�e�[�W�}�b�v
		*/
		virtual void GridHitBackResponse(Vec3& pos, const vector<vector<int>>& stageMap);

		/*!
		@brief �ړ��o���Ȃ��O���b�h(��)���܂�����֐�
		@param �|�W�V����
		@param �X�e�[�W�}�b�v
		*/
		virtual void GridHitLeftResponse(Vec3& pos, const vector<vector<int>>& stageMap);

		/*!
		@brief �ړ��o���Ȃ��O���b�h(�E)���܂�����֐�
		@param �|�W�V����
		@param �X�e�[�W�}�b�v
		*/
		virtual void GridHitRightResponse(Vec3& pos, const vector<vector<int>>& stageMap);
	};
}