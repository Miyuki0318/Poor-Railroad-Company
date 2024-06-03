/*!
@file SmokeEffect.h
@brief ��Ԃ���o�鉌�̃G�t�F�N�g
@author ���V���M
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	/*!
	@brief ���̃G�t�F�N�g
	*/
	class SmokeEffect : public MultiParticle
	{
		const float m_drawTime;		// �`�掞��
		const float m_scale;		// �X�P�[��
		const float m_upVelo;		// ������ւ̈ړ���
		const float m_veloRange;	// X��Z���̈ړ��ʂ͈̔�

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		SmokeEffect(const shared_ptr<Stage>& stagePtr) :
			MultiParticle(stagePtr),
			m_scale(1.0f),
			m_drawTime(1.5f),
			m_upVelo(6.0f),
			m_veloRange(0.5f)
		{
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~SmokeEffect() {}

		/*!
		@brief ���̃G�t�F�N�g�����֌W
		*/
		void AddSmokeEffect();
	};
}