/*!
@file MiningObject.h
@brief 
@author �����
*/

#pragma once
#include "TemplateObject.h"

namespace basecross {
	class MiningObject : public  TemplateObject {
	protected:
		// �������W
		const Vec3 m_spawnPos;
		// �̌@������
		int m_miningCount;
		// �J�n�������������������֐��p��bool
		bool m_setFlg;

	public:
		// �R���X�g���N�^
		MiningObject::MiningObject(const shared_ptr<Stage>& stagePtr, // �X�e�[�W�̃|�C���^
			const Vec3 position // �������W
		) :
			TemplateObject(stagePtr), // �X�e�[�W�̃|�C���^
			m_spawnPos(position) // �������W
		{
			// �ϐ��̏�����
			m_miningCount = 0;
			m_setFlg = true;
		}

		/*!
		@brief	 �N���X���ŏ��ɍ쐬����鎞�Ɏ��s�����֐�
		*/
		virtual void OnCreate() override;

		/*!
		@brief	 �̌@���ɌĂяo�����֐�
		*/
		virtual void OnMining() = 0;

		/*
		@brief	 �I�u�W�F�N�g�����Z�b�g����֐�
		*/
		virtual void OnReset() = 0;
	};

	class Tree : public MiningObject {
		// �̌@�񐔂̏��
		const int m_miningCountLimit;
		// �g�����X�t�H�[���ƃ��f���̍����s��
		Mat4x4 m_modelMat;
	public:
		// �R���X�g���N�^
		Tree::Tree(const shared_ptr<Stage>& stagePtr, // �X�e�[�W�̃|�C���^
			const Vec3 position, // �������W
			const int miningCountLimit 	// �̌@�񐔏��
		) :
			MiningObject(stagePtr, position), // �X�e�[�W�̃|�C���^
			m_miningCountLimit(miningCountLimit) 	// �̌@�񐔏��
		{
			// �g�����X�t�H�[���ƃ��f���̍����s�����
			m_modelMat.affineTransformation(
				Vec3(1.0f, 1.0f, 1.0f),
				Vec3(0.0f),
				Vec3(0.0f),
				Vec3(0.0f)
			);
		}

		/*!
		@brief	 �N���X���ŏ��ɍ쐬����鎞�Ɏ��s�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief	 ���t���[�����s�����֐�
		*/
		void OnUpdate() override;

		/*!
		@brief	 �̌@���ɌĂяo�����֐�
		*/
		void OnMining() override;

		/*
		@brief	 �I�u�W�F�N�g�����Z�b�g����֐�
		*/
		void OnReset() override;

	};



	class Rock : public MiningObject {
		// �̌@�񐔂̏��
		const int m_miningCountLimit;
		// �g�����X�t�H�[���ƃ��f���̍����s��
		Mat4x4 m_modelMat;
	public:
		// �R���X�g���N�^
		Rock::Rock(const shared_ptr<Stage>& stagePtr, // �X�e�[�W�̃|�C���^
			const Vec3 position, // �������W
			const int miningCountLimit 	// �̌@�񐔏��
		) :
			MiningObject(stagePtr, position), // �X�e�[�W�̃|�C���^
			m_miningCountLimit(miningCountLimit) 	// �̌@�񐔏��
		{
			// �g�����X�t�H�[���ƃ��f���̍����s�����
			m_modelMat.affineTransformation(
				Vec3(1.0f, 1.0f, 1.0f),
				Vec3(0.0f),
				Vec3(0.0f),
				Vec3(0.0f)
			);
		}

		/*!
		@brief	 �N���X���ŏ��ɍ쐬����鎞�Ɏ��s�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief	 ���t���[�����s�����֐�
		*/
		void OnUpdate() override;

		/*!
		@brief	 �̌@���ɌĂяo�����֐�
		*/
		void OnMining() override;

		/*
		@brief	 �I�u�W�F�N�g�����Z�b�g����֐�
		*/
		void OnReset() override;

	};
}