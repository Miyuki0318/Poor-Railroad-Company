/*!
@file DrawObject.h
@brief �`��p�I�u�W�F�N�g�̌p����
*/

#pragma once
#include "TemplateObject.h"
#include "OzawaUtility.h"

namespace basecross
{
	// �E�B���h�E�`�����enum
	enum class eVerticesRect
	{
		Center,		// ����
		Upper,		// ��
		Under,		// ��
		Right,		// �E
		Left,		// ��
		UpperRight,	// �E��
		UpperLeft,	// ����
		UnderRight,	// �E��
		UnderLeft,	// ����
	};

	/*!
	@brief �`��p�I�u�W�F�N�g�̌p����
	*/
	class DrawObject : public TemplateObject
	{
	protected:

		Vec2 m_velocity;		// UV�p�x���V�e�B
		Col4 m_diffuseColor;	// �f�B�q���[�Y�F
		Col4 m_verticesColor;	// ���_�F
		Col4 m_emissiveColor;	// �G�~�b�V�u�F
		wstring m_texWstr;		// �e�N�X�`���L�[
		VertexData m_vertex;	// ���_�f�[�^

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		DrawObject(const shared_ptr<Stage>& stagePtr) :
			TemplateObject(stagePtr)
		{
			m_scale.zero();
			m_velocity.zero();
			m_position.zero();
			m_rotation.zero();
			m_texWstr = L"WHITE_TX";
			m_emissiveColor = COL_NONE;
			m_diffuseColor = COL_WHITE;
			m_verticesColor = COL_WHITE;
			Utility::SimpleVerticesIndices(m_vertex);
		}

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �e�N�X�`���L�[
		@param �X�P�[��
		@param �|�W�V����
		*/
		DrawObject(const shared_ptr<Stage>& stagePtr,
			const wstring& texWstr, const Vec3& position,
			const Vec3& rotation, const Vec2& scale
		) :
			TemplateObject(stagePtr, position, rotation, Vec3(scale.x, scale.y, 1.0f)),
			m_texWstr(texWstr)
		{
			m_velocity.zero();
			m_emissiveColor = COL_NONE;
			m_diffuseColor = COL_WHITE;
			m_verticesColor = COL_WHITE;
			Utility::SimpleVerticesIndices(m_vertex);
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~DrawObject() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		virtual void OnCreate() override
		{
			// �p�����̐������̊֐��ƃg�����X�t�H�[���̊ȈՐݒ�𑗂�
			TemplateObject::OnCreate();
			SetTransParam();
		}

		/*!
		@brief �F�͈̔͂�0.0f�`1.0f�̊ԂɂȂ��Ă邩�̏C���֐�
		@param �F
		*/
		void CorrectionColor(Col4& color)
		{
			// �F�͈̔͂�0.0f�`1.0f�̊ԂɂȂ��Ă邩�̏C��
			for (int i = 0; i < 4; i++)
			{
				color.setElem(i, min(color.getElem(i), 1.0f));
				color.setElem(i, max(color.getElem(i), 0.0f));
			}
		}

		/*!
		@brief �x���V�e�B��ύX����֐�
		@param �x���V�e�B
		*/
		void SetVelocity(const Vec2& velo)
		{
			m_velocity = velo;
		}

		/*!
		@brief �x���V�e�B��ύX����֐�
		@param �x���V�e�B
		*/
		void SetVelocity(float x, float y)
		{
			SetVelocity(Vec2(x, y));
		}

		/*!
		@brief ���_���W�̒��S��ύX����֐�
		@param �ݒ�^�C�v
		*/
		virtual void SetVerticesRect(eVerticesRect rect) = 0;

		/*!
		@brief �f�B�t���[�Y�F��Ԃ��֐�
		@return �F
		*/
		Col4 GetDiffuseColor() const
		{
			return m_diffuseColor;
		}

		/*!
		@brief ���_�F��Ԃ��֐�
		@return �F
		*/
		Col4 GetVerticesColor() const
		{
			return m_verticesColor;
		}

		/*!
		@brief �G�~�b�V�u�F��Ԃ��֐�
		@return �F
		*/
		Col4 GetEmissiveColor() const
		{
			return m_emissiveColor;
		}

		/*!
		@brief �e�N�X�`���L�[��Ԃ��֐�
		@return �e�N�X�`���L�[
		*/
		wstring GetTextureKey() const
		{
			return m_texWstr;
		}
	};
}