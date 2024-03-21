/*!
@file DrawObject.h
@brief �`��p�I�u�W�F�N�g�̌p����
*/

#pragma once
#include "stdafx.h"
#include "OzawaUtility.h"

namespace basecross
{
	/*!
	@brief �`��p�I�u�W�F�N�g�̌p����
	*/
	class DrawObject : public GameObject
	{
	protected:

		Vec2 m_scale;			// �X�P�[��
		Vec2 m_velocity;		// UV�p�x���V�e�B
		Vec3 m_position;		// �|�W�V����
		Vec3 m_rotation;		// ���[�e�[�V����
		Col4 m_diffuseColor;	// �f�B�q���[�Y�F
		Col4 m_verticesColor;	// ���_�F
		Col4 m_emissiveColor;	// �G�~�b�V�u�F
		wstring m_texWstr;		// �e�N�X�`���L�[
		VertexData m_vertex;	// ���_�f�[�^

		shared_ptr<Transform> m_ptrTrans; // �g�����X�t�H�[���R���|�[�l���g

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		*/
		DrawObject(const shared_ptr<Stage>& stagePtr) :
			GameObject(stagePtr)
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
			GameObject(stagePtr),
			m_texWstr(texWstr),
			m_position(position),
			m_rotation(rotation),
			m_scale(scale)
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
			m_ptrTrans = GetComponent<Transform>();
			m_ptrTrans->SetScale(m_scale.x, m_scale.y, 1.0f);
			m_ptrTrans->SetRotation(m_rotation);
			m_ptrTrans->SetPosition(m_position);
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
		@brief �X�P�[����ύX����֐�
		@param scale
		*/
		void SetScale(const Vec2& scale)
		{
			m_ptrTrans->SetScale(scale.x, scale.y, 1.0f);
			m_scale = scale;
		}

		/*!
		@brief �X�P�[����ύX����֐�
		@param x
		@param y
		*/
		void SetScale(const float x, const float y)
		{
			m_ptrTrans->SetScale(x, y, 1.0f);
			m_scale.x = x;
			m_scale.y = y;
		}

		/*!
		@brief �|�W�V������ύX����֐�
		@param �|�W�V����
		*/
		void SetPosition(const Vec3& position)
		{
			m_ptrTrans->SetPosition(position);
			m_position = position;
		}

		/*!
		@brief �|�W�V������ύX����֐�
		@param �|�W�V����
		*/
		void SetPosition(float x, float y, float z)
		{
			SetPosition(Vec3(x, y, z));
		}

		/*!
		@brief ���[�e�[�V������ύX����֐�
		@param ���[�e�[�V����
		*/
		void SetRotation(const Vec3& rotation)
		{
			m_ptrTrans->SetRotation(rotation);
			m_rotation = rotation;
		}

		/*!
		@brief ���[�e�[�V������ύX����֐�
		@param ���[�e�[�V����
		*/
		void SetRotation(float x, float y, float z)
		{
			SetRotation(Vec3(x, y, z));
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
		@brief �X�P�[����Ԃ��֐�
		@return �X�P�[��
		*/
		Vec2 GetScale() const
		{
			return m_scale;
		}

		/*!
		@brief �|�W�V������Ԃ��֐�
		@return �|�W�V����
		*/
		Vec3 GetPosition() const
		{
			return m_position;
		}

		/*!
		@brief ���[�e�[�V�����Ԃ��֐�
		@return ���[�e�[�V����
		*/
		Vec3 GetRotation() const
		{
			return m_rotation;
		}

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