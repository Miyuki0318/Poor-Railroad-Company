/*!
@file Timer.h
@brief �^�C�}�[�N���X
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	/*!
	@brief �^�C�}�[
	*/
	class Timer : public GameObject
	{
		// �^�C�}�[�Ǘ��\����
		struct TimerParam
		{
			void* objectPtr = NULL; // �I�u�W�F�N�g�̃|�C���^
			float totalTime = 0.0f; // �o�ߎ���
			float limitTime = 0.0f; // �ݒ莞��

			/*!
			@brief �R���X�g���N�^
			*/
			TimerParam()
			{
				objectPtr = NULL;
				totalTime = 0.0f;
				limitTime = 0.0f;
			}

			/*!
			@brief �R���X�g���N�^
			@param �Ăяo�����|�C���^
			@param �ݒ莞��
			*/
			TimerParam(void* ptr, float limit) :
				objectPtr(ptr),
				limitTime(limit)
			{
				totalTime = 0.0f;
			}

		public:

			/*!
			@brief ���Z�b�g�֐�
			*/
			void Reset()
			{
				objectPtr = NULL;
				totalTime = 0.0f;
				limitTime = 0.0f;
			}

			/*!
			@brief �Z�b�g�֐�
			@param �ݒ莞��
			*/
			void Set(float limit)
			{
				totalTime = 0.0f;
				limitTime = limit;
			}

			/*!
			@brief �Z�b�g�֐�
			@param �I�u�W�F�N�g�|�C���^
			@param �ݒ莞��
			*/
			void Set(void* ptr, float limit)
			{
				objectPtr = ptr;
				totalTime = 0.0f;
				limitTime = limit;
			}
		};

		vector<TimerParam> m_timers; // �Ǘ��z��

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		Timer(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr)
		{
			m_timers = {};
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~Timer() {}

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;

		/*!
		@brief �^�C�}�[�Z�b�g�֐�
		@param �I�u�W�F�N�g�|�C���^
		@param �ݒ莞��
		@param �㏑���Z�b�g���邩
		@return �^�C�}�[���I���������̐^�U
		*/
		bool SetTimer(void* ptr, float time, bool reset = false);

		/*!
		@brief �^�C�}�[�o�ߎ��Ԏ擾�֐�
		@param �I�u�W�F�N�g�|�C���^
		@param �ݒ莞��
		@return �o�ߎ���
		*/
		float GetTime(void* ptr, float time);
	};
}