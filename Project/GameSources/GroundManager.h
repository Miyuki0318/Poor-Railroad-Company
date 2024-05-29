/*!
@file GroundManager.h
@brief �n�ʊǗ��I�u�W�F�N�g�N���X
@author ���V���M
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	/*!
	@brief �n�ʗp�̃C���X�^���X�`��I�u�W�F�N�g
	*/
	class InstanceGround : public GameObject
	{
		// �`��R���|�[�l���g
		shared_ptr<PNTStaticInstanceDraw> m_ptrDraw;
		
		// �n�ʂ̐F
		const wstring m_groundTexture;

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �ݒ肷��F
		*/
		InstanceGround(const shared_ptr<Stage>& stagePtr,
			const wstring& texture
		) :
			GameObject(stagePtr),
			m_groundTexture(texture)
		{
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~InstanceGround() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		virtual void OnCreate() override;

		/*!
		@brief �n�ʕ`��ǉ��֐�
		@param �ǉ�����s��
		*/
		void AddMatrix(const Mat4x4 matrix)
		{
			if (!m_ptrDraw) return;
			m_ptrDraw->AddMatrix(matrix);
		}
	};

	/*!
	@brief �n�ʕ`��I�u�W�F�N�g�Ǘ��N���X
	*/
	class GroundManager : public GameObject
	{
		float m_defPosY;		// �C���X�^���X�`��p�̃|�W�V����Y
		Mat4x4 m_mtxScale;		// �C���X�^���X�`��p�̃X�P�[��
		Mat4x4 m_mtxRotation;	// �C���X�^���X�`��p�̃��[�e�[�V����

		// �C���X�^���X�`��I�u�W�F�N�g�}�b�v
		map<eStageID, map<int, weak_ptr<InstanceGround>>> m_groundMap;

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		GroundManager(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr),
			m_defPosY(0.5f)
		{
			Quat quatRot;
			quatRot.rotationRollPitchYawFromVector(Vec3(0.0f));
			m_mtxRotation.rotation(quatRot);
			m_mtxScale.scale(Vec3(1.0f));
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~GroundManager() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;
	};
}