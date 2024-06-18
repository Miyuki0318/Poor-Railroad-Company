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
		None,		// ��������
		Pick,		// �c���n�V
		Axe,		// �A�b�N�X
		Craft,		// �N���t�g
		Rail,		// ���[��
		Bridge,		// �؂̑���
		Crossing,	// ����
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

		const weak_ptr<Player> m_player;		// �v���C���[�̃|�C���^
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
			const shared_ptr<Player>& playerPtr,
			const shared_ptr<SelectIndicator>& indicatorPtr
		) :
			GameObject(stagePtr),
			m_player(playerPtr),
			m_indicator(indicatorPtr),
			m_scale(Vec2(100.0f)),
			m_diffPosition(Vec3(0.0f, 2.0f, 0.5f)),
			m_distanceX(60.0f)
		{
			// �A�C�R���^�C�v�ɉ��������W�e�[�u��
			m_typePos.emplace(eSpriteType::Icon, Vec3(0.0f, 0.0f, 0.2f));
			m_typePos.emplace(eSpriteType::Balloon, Vec3(0.0f, 0.0f, 0.3f));

			// �X�e�[�WID�ɉ������A�C�R���e�[�u��
			m_iconMap.emplace(eStageID::Stone1, eActionIcon::Pick);
			m_iconMap.emplace(eStageID::Stone2, eActionIcon::Pick);
			m_iconMap.emplace(eStageID::Stone3, eActionIcon::Pick);
			m_iconMap.emplace(eStageID::Tree1, eActionIcon::Axe);
			m_iconMap.emplace(eStageID::Tree2, eActionIcon::Axe);
			m_iconMap.emplace(eStageID::GuideRail, eActionIcon::Rail);
			m_iconMap.emplace(eStageID::Rail, eActionIcon::Crossing);
			m_iconMap.emplace(eStageID::Water, eActionIcon::Bridge);

			// �A�C�R���ɉ������e�N�X�`���ƗD��x�e�[�u��
			m_texMap.emplace(eActionIcon::Rail, TextureType(L"I_RAIL_TX", 1));
			m_texMap.emplace(eActionIcon::Bridge, TextureType(L"I_BRIDGE_TX", 1));
			m_texMap.emplace(eActionIcon::Crossing, TextureType(L"I_CROSSING_TX", 1));
			m_texMap.emplace(eActionIcon::Axe, TextureType(L"I_AXE_TX", 2));
			m_texMap.emplace(eActionIcon::Pick, TextureType(L"I_PICK_TX", 2));
			m_texMap.emplace(eActionIcon::Craft, TextureType(L"I_CRAFT_TX", 3));
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

		/*!
		@brief �A�N�V�����\�|�C���g��I�����Ă��邩�擾����֐�
		@param �v���C���[�̃V�F�A�h�|�C���^
		@param �C���f�B�P�[�^�[�̃V�F�A�h�|�C���^
		@return bool ����1�ł��A�N�V�����ł���Ȃ�true
		*/
		bool IsActionSelectPoint(const shared_ptr<Player>& player, const shared_ptr<SelectIndicator>& indicator);

		/*!
		@brief �C���f�B�P�[�^�[�̑I�����Ă���X�e�[�WID���擾����֐�
		@param �C���f�B�P�[�^�[�̃V�F�A�h�|�C���^
		@return int�^�ł̃X�e�[�WID
		*/
		int GetIntValueStageID(const shared_ptr<SelectIndicator>& indicator);

		/*!
		@brief �D��x���C���f�b�N�X���\�[�g����֐�
		@param �C���f�b�N�X�z��
		*/
		void SortedIndicesBuffer(vector<int>& indices);

		/*!
		@brief �D��x�C���f�b�N�X��p���ăX�v���C�g�̐ݒ���s���֐�
		@param �C���f�b�N�X�z��
		*/
		void SetIndicesIconTexture(vector<int>& indices);
	};
}