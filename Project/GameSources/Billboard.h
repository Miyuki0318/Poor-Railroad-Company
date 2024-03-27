/*!
@file Billboard.h
@brief �r���{�[�h�I�u�W�F�N�g�̌p����
@author ���V���M
*/

#pragma once
#include "stdafx.h"
#include "DrawObject.h"

namespace basecross
{
	// ----------------------------------------------------------------------------------
	//	Billboard : public GameObject �r���{�[�h�I�u�W�F�N�g�̌p����
	// ----------------------------------------------------------------------------------
	class Billboard : public DrawObject
	{
	protected:

		shared_ptr<PCTStaticDraw> m_ptrDraw;

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		Billboard(const shared_ptr<Stage>& stagePtr) :
			DrawObject(stagePtr)
		{
		}

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �e�N�X�`���L�[
		@param �X�P�[��
		@param �|�W�V����
		*/
		Billboard(const shared_ptr<Stage>& stagePtr,
			const wstring& texWstr, const Vec2& scale, const Vec3& position
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
		@param ���[�e�[�V����
		*/
		Billboard(const shared_ptr<Stage>& stagePtr,
			const wstring& texWstr, const Vec2& scale,
			const Vec3& position, const Vec3& rotation
			) :
			DrawObject(stagePtr, texWstr, position, rotation, scale)
		{
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~Billboard() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		virtual void OnCreate() override;

		/*!
		@brief �r���{�[�h�X�V�֐�
		*/
		void UpdateBillboard();

		/*!
		@brief �f�B�t���[�Y�F���g���ĐF��ύX����֐�
		@param �F
		*/
		void SetDiffuseColor(const Col4& color);

		/*!
		@brief ���_�F���g���ĐF��ύX����֐�
		@param �F
		*/
		void SetVerticesColor(const Col4& color);

		/*!
		@brief �G�~�b�V�u�F���g���ĐF��ύX����֐�
		@param �F
		*/
		void SetEmissiveColor(const Col4& color);

		/*!
		@brief �e�N�X�`����ύX����֐�
		@param �e�N�X�`���L�[
		*/
		void SetTexture(const wstring& textureKey);

		/*!
		@brief �`��ύX�ݒ�֐�
		@param eVerticesRect
		*/
		void SetVerticesRect(eVerticesRect rect) override;

		/*!
		@brief �e�N�X�`�����x���V�e�B�ɉ����ĉ�]������֐�
		*/
		void RotateTexture();
	};
}