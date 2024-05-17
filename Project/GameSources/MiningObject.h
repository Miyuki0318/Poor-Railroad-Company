/*!
@file MiningObject.h
@brief 
@author �����
*/

#pragma once
#include "TemplateObject.h"

namespace basecross {
	class MiningObject : public  TemplateObject {
	public:
		// ��ԊǗ��p�萔
		enum class eState
		{
			Normal, //�ʏ�̏��
			Damage, // �����������
			Broken, // �j�󂳂ꂽ���
			None // ���ł������
		};

	protected:
		// �������W
		const Vec3 m_spawnPos;
		// �̌@������
		int m_miningCount;
		// �̌@�񐔂̏��
		const int m_miningLimit;
		// ��Ԃ�ێ�����ϐ�
		eState m_state;
		// CSV��̈ʒu
		Point2D<size_t> m_csvPos;

	public:
		// �R���X�g���N�^
		MiningObject::MiningObject(const shared_ptr<Stage>& stagePtr, // �X�e�[�W�̃|�C���^
			const Vec3 position,// �������W
			const int miningLimit
		) :
			TemplateObject(stagePtr), // �X�e�[�W�̃|�C���^
			m_spawnPos(Vec3(position.x, 1.0f, position.z)),// �������W
			m_miningLimit(miningLimit)
		{
			// �ϐ��̏�����
			m_miningCount = 0;
			m_state = eState::Normal;
		}

		/*!
		@brief	 �N���X���ŏ��ɍ쐬����鎞�Ɏ��s�����֐�
		*/
		virtual void OnCreate() override;

		/*!
		@brief	 �̌@���ɌĂяo�����֐�
		*/
		virtual void OnMining();

		/*
		@brief	 �I�u�W�F�N�g�����Z�b�g����֐�
		*/
		virtual void OnReset() = 0;

		/*
		@brief	 �I�u�W�F�N�g�̏����X�e�[�W�}�b�v�����������֐�
		*/
		virtual void OnDelete();
		
		/*
		@brief	 CSV���Row��Col��Ԃ��֐�
		@return�@ m_csvPos
		*/
		virtual Point2D<size_t> GetCSVPos() const
		{
			return m_csvPos;
		}
	};


	class Tree : public MiningObject {
		// �g�����X�t�H�[���ƃ��f���̍����s��
		Mat4x4 m_modelMat;
	public:
		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �������W
		@param �̌@�񐔏��
		*/
		Tree::Tree(const shared_ptr<Stage>& stagePtr, // �X�e�[�W�̃|�C���^
			const Vec3 position, // �������W
			const int miningLimit 	// �̌@�񐔏��
		) :
			MiningObject(stagePtr, position, miningLimit) // �X�e�[�W�̃|�C���^
		{
			// �g�����X�t�H�[���ƃ��f���̍����s�����
			m_modelMat.affineTransformation(
				Vec3(0.08f, 0.09f, 0.08f),
				Vec3(0.0f),
				Vec3(0.0f),
				Vec3(0.0f)
			);
		}

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �������W
		*/
		Tree::Tree(const shared_ptr<Stage>& stagePtr, // �X�e�[�W�̃|�C���^
			const Vec3 position // �������W
		) :
			MiningObject(stagePtr, position, 2) // �X�e�[�W�̃|�C���^
		{
			// �g�����X�t�H�[���ƃ��f���̍����s�����
			m_modelMat.affineTransformation(
				Vec3(0.08f, 0.09f, 0.08f),
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

		/*
		@brief	 �X�e�[�g�ɉ����ď������s���֐�
		*/
		void AccordingState();

	};



	class Rock : public MiningObject {
		// �g�����X�t�H�[���ƃ��f���̍����s��
		Mat4x4 m_modelMat;
	public:
		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �������W
		@param �̌@�񐔏��
		*/
		Rock::Rock(const shared_ptr<Stage>& stagePtr, // �X�e�[�W�̃|�C���^
			const Vec3 position, // �������W
			const int miningLimit 	// �̌@�񐔏��
		) :
			MiningObject(stagePtr, position, miningLimit) // �X�e�[�W�̃|�C���^
		{
			// �g�����X�t�H�[���ƃ��f���̍����s�����
			m_modelMat.affineTransformation(
				Vec3(0.28f, 0.28f, 0.28f),
				Vec3(0.0f),
				Vec3(0.0f),
				Vec3(0.0f)
			);
		}

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �������W
		*/
		Rock::Rock(const shared_ptr<Stage>& stagePtr, // �X�e�[�W�̃|�C���^
			const Vec3 position // �������W
		) :
			MiningObject(stagePtr, position, 2) // �X�e�[�W�̃|�C���^
		{
			// �g�����X�t�H�[���ƃ��f���̍����s�����
			m_modelMat.affineTransformation(
				Vec3(0.28f, 0.28f, 0.28f),
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

		/*
		@brief	 �X�e�[�g�ɉ����ď������s���֐�
		*/
		void AccordingState();

	};
}