/*!
@file Player.h
@brief �v���C���[�I�u�W�F�N�g
@author ���V���M
*/

#pragma once
#include "TemplateObject.h"

namespace basecross
{
	// �v���C���[�̃A�j���[�V�����^�C�v
	enum class ePlayerAnimation
	{
		Wait,			// �ҋ@
		Walk,			// �ړ�
		Mining,			// �̌@
		Cutting,		// ����
		CraftStart,		// �N���t�g�J�n
		Crafting,		// �N���t�g��
		CraftFinish,	// �N���t�g�I��
		QTESucces,		// QTE����
		QTEFailed,		// QTE���s
	};

	/*!
	@brief �A�j���[�V�����f�[�^�\����
	*/
	struct AnimationMap
	{
		wstring animeKey;	// �A�j���[�V�����L�[
		UINT flameNum;		// �t���[����
		bool loopActive;	// ���[�v���邩�̐^�U

		/*!
		@brief �R���X�g���N�^
		@param �A�j���[�V�����L�[
		@param �J�n���̃t���[��
		@param �I�����̃t���[��
		@param ���[�v���邩(�f�t�H���g��false)
		*/
		AnimationMap(wstring key, UINT frame, bool loop = false) :
			animeKey(key),
			flameNum(frame),
			loopActive(loop)
		{
		}
	};

	/*!
	@brief �v���C���[
	*/
	class Player : public TemplateObject
	{
	protected:

		// �`��R���|�[�l���g
		shared_ptr<BcPNTBoneModelDraw> m_ptrDraw; 

		const float m_radius;	 // �Փ˔���p���a
		const float m_moveSpeed; // �ړ����x
		const float m_rotSpeed;  // ��]���x

		// �ʂ�Ȃ��X�e�[�WID���X�g
		set<eStageID> m_impassableSet; 

		// �A�j���[�V�����}�b�v
		map<ePlayerAnimation, AnimationMap> m_animationMap;

		Vec3 m_rotTarget;	// ��]��
		Vec3 m_currentRot;  // �O��̉�]��
		Mat4x4 m_modelMat;	// ���f���ƃg�����X�t�H�[�������s��

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		Player(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr, Vec3(0.0f, 3.0f, 0.0f), Vec3(0.0f), Vec3(1.0f)),
			m_moveSpeed(5.0f), // ����CSV���瑬�x���̃X�e�[�^�X���擾�\��
			m_rotSpeed(0.5f),  // ����CSV���瑬�x���̃X�e�[�^�X���擾�\��
			m_radius(1.0f)
		{
			m_rotTarget.zero(); // ��]���0.0f�ŏ�����
			m_currentRot.zero(); // ��]���0.0f�ŏ�����

			// �A�j���[�V�����L�[
			m_animationMap.emplace(ePlayerAnimation::Wait, AnimationMap(L"P_WAIT", 24, true));			// �ҋ@
			m_animationMap.emplace(ePlayerAnimation::Walk, AnimationMap(L"P_WALK", 24, true));			// �ړ�
			m_animationMap.emplace(ePlayerAnimation::Mining, AnimationMap(L"P_MINING", 24, false));		// �̌@
			m_animationMap.emplace(ePlayerAnimation::Cutting, AnimationMap(L"P_CUTTING", 24, false));		// ����
			m_animationMap.emplace(ePlayerAnimation::CraftStart, AnimationMap(L"P_CSTART", 10, false));	// �N���t�g�J�n
			m_animationMap.emplace(ePlayerAnimation::Crafting, AnimationMap(L"P_CNOW", 15, true));		// �N���t�g��
			m_animationMap.emplace(ePlayerAnimation::CraftFinish, AnimationMap(L"P_CEND", 10, false));	// �N���t�g�I��
			m_animationMap.emplace(ePlayerAnimation::QTESucces, AnimationMap(L"P_SUCCES", 24, false));	// QTE����
			m_animationMap.emplace(ePlayerAnimation::QTEFailed, AnimationMap(L"P_FAILED", 24, false));	// QTE���s

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
		@brief �w�肵���s�񂪏Փ˔�������O���b�h���̐^�U�擾�֐�
		@param row
		@param col
		@return bool �Փ˔�������O���b�h�Ȃ�true,���Ȃ��O���b�h�Ȃ�false
		*/
		virtual bool GetIsImpassable(size_t row, size_t col);

		/*!
		@brief �ړ��o���Ȃ��O���b�h(��)���܂�����֐�
		@param �|�W�V����
		@param �X�e�[�W�}�b�v
		*/
		virtual void GridHitFlontResponse(Vec3& pos);

		/*!
		@brief �ړ��o���Ȃ��O���b�h(��)���܂�����֐�
		@param �|�W�V����
		@param �X�e�[�W�}�b�v
		*/
		virtual void GridHitBackResponse(Vec3& pos);

		/*!
		@brief �ړ��o���Ȃ��O���b�h(��)���܂�����֐�
		@param �|�W�V����
		@param �X�e�[�W�}�b�v
		*/
		virtual void GridHitLeftResponse(Vec3& pos);

		/*!
		@brief �ړ��o���Ȃ��O���b�h(�E)���܂�����֐�
		@param �|�W�V����
		@param �X�e�[�W�}�b�v
		*/
		virtual void GridHitRightResponse(Vec3& pos);
	};
}