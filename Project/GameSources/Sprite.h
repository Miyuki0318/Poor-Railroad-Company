/*!
@file Sprite.h
@brief �X�v���C�g�e�N���X
*/

#pragma once
#include "stdafx.h"
#include "DrawObject.h"

namespace basecross
{
	// ----------------------------------------------------------------------------------
	//	Sprite : public GameObject �P���ȕ\�����s���X�v���C�g�e�N���X
	// ----------------------------------------------------------------------------------
	class Sprite : public DrawObject
	{
	protected:

		shared_ptr<PCTSpriteDraw> m_ptrDraw; // �`��R���|�[�l���g

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		Sprite(const shared_ptr<Stage>& stagePtr) :
			DrawObject(stagePtr)
		{
		}

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �e�N�X�`���L�[
		*/
		Sprite(const shared_ptr<Stage>& stagePtr,
			const wstring& texWstr
		) :
			DrawObject(stagePtr, texWstr, Vec3(0.0f), Vec3(0.0f), Vec2(0.0f))
		{
		}
		
		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �e�N�X�`���L�[
		@param �X�P�[��
		@param �|�W�V����
		*/
		Sprite(const shared_ptr<Stage>& stagePtr,
			const wstring& texWstr, const Vec2& scale
		) :
			DrawObject(stagePtr, texWstr, Vec3(0.0f), Vec3(0.0f), scale)
		{
		}

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �e�N�X�`���L�[
		@param �X�P�[��
		*/
		Sprite(const shared_ptr<Stage>& stagePtr,
			const wstring& texWstr, const Vec2& scale, 
			const Vec3& position
		) :
			DrawObject(stagePtr, texWstr, position, Vec3(0.0f), scale)
		{
		}

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �e�N�X�`���L�[
		@param �X�P�[��
		@param �|�W�V����
		@param ���[�e�[�V����Z
		*/
		Sprite(const shared_ptr<Stage>& stagePtr,
			const wstring& texWstr,	const Vec2& scale, 
			const Vec3& position, const float rotZ
		) :
			DrawObject(stagePtr, texWstr, position, Vec3(0.0f, 0.0f, rotZ), scale)
		{
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~Sprite() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		virtual void OnCreate() override;
		
		/*!
		@brief �f�B�t���[�Y�F���g���ĐF��ύX����֐�
		@param �F
		*/
		void SetDiffuseColor(Col4& color);

		/*!
		@brief ���_�F���g���ĐF��ύX����֐�
		@param �F
		*/
		void SetVerticesColor(Col4& color);

		/*!
		@brief �G�~�b�V�u�F���g���ĐF��ύX����֐�
		@param �F
		*/
		void SetEmissiveColor(Col4& color);

		/*!
		@brief �e�N�X�`����ύX����֐�
		@param �e�N�X�`���L�[
		*/
		void SetTexture(const wstring& textureKey);

		/*!
		@brief �`��ύX�ݒ�֐�
		@param eVerticesRect
		*/
		void SetVerticesRect(eRectType rect) override;

		/*!
		@brief �t�F�[�h�C����������֐�
		@param �t�F�[�h�Ɋ|���鎞��
		@return �t�F�[�h�������������̐^�U
		*/
		bool FadeInColor(const float time);

		/*!
		@brief �t�F�[�h�C����������֐�
		@param �t�F�[�h�Ɋ|���鎞��
		@param �t�F�[�h�̏I���l
		@return �t�F�[�h�������������̐^�U
		*/
		bool FadeInColor(const float time, const float alpha);

		/*!
		@brief �t�F�[�h�C����������֐�
		@param �t�F�[�h�Ɋ|���鎞��
		@return �t�F�[�h�������������̐^�U
		*/
		bool FadeOutColor(const float time);

		/*!
		@brief �t�F�[�h�C����������֐�
		@param �t�F�[�h�Ɋ|���鎞��
		@param �t�F�[�h�̏I���l
		@return �t�F�[�h�������������̐^�U
		*/
		bool FadeOutColor(const float time, const float alpha);

		/*!
		@brief �e�N�X�`�����x���V�e�B�ɉ����ĉ�]������֐�
		*/
		void RotateTexture();
	};
}