/*!
@file ActionGuide.h
@brief �s���̃K�C�h�A�C�R���`��
@author ���V���M
*/

#pragma once
#include "Sprite.h"
#include "GamePlayer.h"

namespace basecross
{
	// �A�C�R���^�C�venum
	enum class eActionIcon : size_t
	{
		None,	// ��������
		Pick,	// �c���n�V
		Axe,	// �A�b�N�X
		Craft,	// �N���t�g
		Rail,	// ���[��
	};

	// �A�C�R���ʒu�^�C�venum
	enum class eIconPivot : uint8_t
	{
		Center,	// ����
		Left,	// ��
		Right,	// �E
	};

	// �X�v���C�g�^�C�venum
	enum class eSpriteType
	{
		Icon,	 // �A�C�R��
		Balloon, // �����o��
	};

	// �e�N�X�`���^�C�v�\����
	struct TextureType
	{
		wstring textureStr; // �e�N�X�`���L�[ 
		uint8_t layerIndex; // �D��x���C���[��

		TextureType(wstring texStr, uint8_t index) :
			textureStr(texStr),
			layerIndex(index)
		{
		}
	};

	// �X�v���C�g�^�C�v�\����
	struct SpriteType
	{
		eIconPivot pivot;		 // �A�C�R���̈ʒu
		eSpriteType type;		 // �e�N�X�`���^�C�venum
		weak_ptr<Sprite> sprite; // �X�v���C�g�̃|�C���^
		bool active;			 // �A�N�e�B�u���̐^�U

		SpriteType(eIconPivot piv, eSpriteType eType, const shared_ptr<Sprite>& ptr) :
			pivot(piv),
			type(eType),
			sprite(ptr)
		{
			active = false;
		}
	};

	/*!
	@brief �s���K�C�h�A�C�R��
	*/
	class ActionGuide : public GameObject
	{
		map<eStageID, eActionIcon> m_iconMap;		// �A�C�R���e�[�u��
		map<eActionIcon, TextureType> m_texMap;		// �e�N�X�`���e�[�u��
		map<eSpriteType, Vec3> m_typePos;			// �^�C�v�ɉ��������W
		vector<SpriteType> m_spriteMap;				// �X�v���C�g�e�[�u��
		map<int, eActionIcon> m_iconBuffer;			// �A�C�R���^�C�v�o�b�t�@
		Vec3 m_position;							// ���W

		const weak_ptr<GamePlayer> m_player;		// �v���C���[�̃|�C���^
		const weak_ptr<SelectIndicator> m_indicator;// �C���f�B�P�[�^�[�̃|�C���^
		const Vec2 m_scale;							// �X�v���C�g�̃X�P�[��
		const Vec3 m_diffPosition;					// ����܂ł̍��W�̂���
		const float m_distanceX;					// �����\������X���̗]��

	public:

		/*!
		@brief �R���X�g���N�^
		@param �X�e�[�W�|�C���^
		@param �v���C���[�|�C���^
		@param �C���f�B�P�[�^�[�|�C���^
		*/
		ActionGuide(const shared_ptr<Stage>& stagePtr,
			const shared_ptr<GamePlayer>& playerPtr,
			const shared_ptr<SelectIndicator>& indicatorPtr
		) :
			GameObject(stagePtr),
			m_player(playerPtr),
			m_indicator(indicatorPtr),
			m_scale(Vec2(50.0f)),
			m_diffPosition(Vec3(0.0f, 2.0f, 0.5f)),
			m_distanceX(30.0f)
		{
			// �A�C�R���^�C�v�ɉ��������W�e�[�u��
			m_typePos.insert(make_pair(eSpriteType::Icon, Vec3(0.0f)));
			m_typePos.insert(make_pair(eSpriteType::Balloon, Vec3(0.0f, 0.0f, 0.1f)));

			// �X�e�[�WID�ɉ������A�C�R���e�[�u��
			m_iconMap.insert(make_pair(eStageID::Rock, eActionIcon::Pick));
			m_iconMap.insert(make_pair(eStageID::Tree, eActionIcon::Axe));
			m_iconMap.insert(make_pair(eStageID::GuideRail, eActionIcon::Rail));

			// �A�C�R���ɉ������e�N�X�`���ƗD��x�e�[�u��
			m_texMap.insert(make_pair(eActionIcon::Rail, TextureType(L"I_RAIL_TX", 1)));
			m_texMap.insert(make_pair(eActionIcon::Axe, TextureType(L"I_AXE_TX", 2)));
			m_texMap.insert(make_pair(eActionIcon::Pick, TextureType(L"I_PICK_TX", 2)));
			m_texMap.insert(make_pair(eActionIcon::Craft, TextureType(L"I_CRAFT_TX", 3)));
		}

		/*!
		@brief �f�X�g���N�^
		*/
		virtual ~ActionGuide() {}

		/*!
		@brief �������Ɉ�x�����Ăяo�����֐�
		*/
		void OnCreate() override;

		/*!
		@brief ���t���[���x�ɌĂяo�����֐�
		*/
		void OnUpdate() override;

		/*!
		@brief �`�悷�邩�̐ݒ�֐�
		@param �`�悷�邩�̐^�U
		*/
		void SetDrawActive(bool b) override
		{
			// �p�����̕`��ݒ�𑗂�
			GameObject::SetDrawActive(b);

			// �e�X�v���C�g�ɕ`��ݒ�𑗂�
			for (const auto& sprite : m_spriteMap)
			{
				sprite.sprite.lock()->SetDrawActive(b);
			}
		}

		/*!
		@brief �A�N�e�B�u�ɂ��邩�̐ݒ�֐�
		@param �A�N�e�B�u�ɂ��邩�̐^�U
		*/
		void SetSpriteActive(bool b)
		{
			// �e�X�v���C�g�ɕ`��ݒ�𑗂�
			for (auto& sprite : m_spriteMap)
			{
				sprite.active = b;
			}
		}

	private:

		/*!
		@brief ���W�̍X�V�����֐�
		*/
		void UpdatePosition();

		/*!
		@brief �A�C�R���^�C�v�̐ݒ�X�V�����֐�
		*/
		void UpdateIconType();

		/*!
		@brief �A�C�R���Z�b�g�̍X�V�����֐�
		*/
		void UpdateIconSet();

		/*!
		@brief �A�C�R���`��̍X�V�����֐�
		*/
		void UpdateIconDraw();
	};
}