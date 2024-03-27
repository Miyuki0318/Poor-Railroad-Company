/*!
@file Station.h
@brief Tarminal�̌���}�X�ɔz�u�����w�I�u�W�F
@author �����
*/

#pragma once
#include "TemplateObject.h"

namespace basecross {
	class Station : public  TemplateObject {
	protected:
		// �������W
		const Vec3 m_spawnPos;
		// �J�n�������������������֐��p��bool
		bool m_setFlg;
		// �g�����X�t�H�[���ƃ��f���̍����s��
		Mat4x4 m_modelMat;

	public:
		// �R���X�g���N�^
		Station::Station(const shared_ptr<Stage>& stagePtr, // �X�e�[�W�̃|�C���^
			const Vec3 position // �������W
		) :
			TemplateObject(stagePtr), // �X�e�[�W�̃|�C���^
			m_spawnPos(position) // �������W
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
		void OnMining();

		/*
		@brief	 �I�u�W�F�N�g�����Z�b�g����֐�
		*/
		void OnReset();
	};
}