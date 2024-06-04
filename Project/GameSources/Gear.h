/*!
@file Gear.h
@brief ���؂Ɏg�p���鎕��
@author ���V���M
*/

#pragma once
#include "TemplateObject.h"

namespace basecross
{
	// �t�����h�����邽�ߖ��O�����錾
	class GearManager;

	/*!
	@brief ���ԃI�u�W�F�N�g
	*/
	class Gear : public TemplateObject
	{
		shared_ptr<BcPNTBoneModelDraw> m_ptrDraw; // �`��R���|�[�l���g
		shared_ptr<Shadowmap> m_ptrShadow; // �e�`��R���|�[�l���g

		// �t�����h��
		friend GearManager;

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �|�W�V����
		*/
		Gear(const shared_ptr<Stage>& stagePtr, const Vec3& position) :
			TemplateObject(stagePtr, position, Vec3(0.0f), Vec3(1.0f))
		{
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~Gear() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief �A�j���[�V�����̍X�V�����֐�
		*/
		void UpdateAnimation();

		/*!
		@brief �擾��̔�A�N�e�B�u���֐�
		*/
		void GetGearSleap();

		/*!
		@brief ���Z�b�g�����֐�
		*/
		void ResetGear();
	};

	/*!
	@brief ���ԊǗ��I�u�W�F�N�g
	*/
	class GearManager : public GameObject
	{
		weak_ptr<TemplateObject> m_playerPtr;	// �v���C���[�̃|�C���^
		map<string, weak_ptr<Gear>> m_gearMap;	// ���Ԃ̔z��
		
		const Mat4x4 m_gearModelMat; // ���ԃ��f���̍����s��
		const float m_getGearRange;  // �擾�ł��鋗��

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		GearManager(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr),
			m_getGearRange(1.5f),
			m_gearModelMat((Mat4x4)XMMatrixAffineTransformation(
				Vec3(0.5f),
				Vec3(0.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, 1.0f, 0.0f)
			))
		{
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~GearManager() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;

		/*!
		@brief ���Ԃ̃��Z�b�g�֐�
		*/
		void ResetGears();

		/*!
		@brief �擾���̏����֐�
		@param ���ԃf�[�^
		*/
		void GetGearPlayar(const pair<string, weak_ptr<Gear>>& gear);
	};
}