/*!
@file SpriteParticle.h
@brief �X�v���C�g�ł̃p�[�e�B�N��
@author ���V���M
*/

#pragma once
#include "Sprite.h"

namespace basecross
{
	/*!
	@brief �X�v���C�g�p�[�e�B�N���f�[�^�\����
	*/
	struct ParticleData
	{
		float totalTime = 0.0f;		// �o�ߎ���
		float drawTime = 0.0f;		// �`�掞��
		weak_ptr<Sprite> sprite;	// �X�v���C�g�I�u�W�F�N�g
	};

	/*!
	@brief �X�v���C�g�p�[�e�B�N��
	*/
	class SpriteParticle : public GameObject
	{
		// �p�[�e�B�N���z��
		vector<ParticleData> m_spriteVec;
		
		Vec3 m_emitterPosition;		// �G�~�b�^�[
		wstring m_textureStr;		// �e�N�X�`��
		const size_t m_maxSprite;	// �����

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �e�N�X�`���L�[
		@param �����(�f�t�H���g��60)
		*/
		SpriteParticle(const shared_ptr<Stage>& stagePtr,
			const wstring& textureStr,
			const size_t& maxSprite = 60
		) :
			GameObject(stagePtr),
			m_textureStr(textureStr),
			m_maxSprite(maxSprite)
		{
			m_spriteVec.reserve(m_maxSprite);
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~SpriteParticle() {}

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;

		/*!
		@brief ���Z�b�g����
		*/
		void ResetParticle();

		/*!
		@brief �p�[�e�B�N���ǉ��֐�
		@param �X�P�[��
		@param �ړ���
		@param ���[�e�[�V����Z��
		@param �`�掞��
		*/
		void AddParticle(Vec2 scale, Vec2 velo, float rotZ, float drawTime);

		/*!
		@brief �G�~�b�^�[���W�ݒ�֐�
		@param position
		*/
		void SetEmitterPosition(Vec3 position)
		{
			m_emitterPosition = position;
		}
	};
}