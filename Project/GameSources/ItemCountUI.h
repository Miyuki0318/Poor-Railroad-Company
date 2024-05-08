#pragma once
#include "Number.h"
#include "CraftManager.h"

namespace basecross
{
	class ItemCountUI : public GameObject
	{	
		weak_ptr<Sprite> m_backSprite;
		weak_ptr<Sprite> m_itemSprite;
		pair<weak_ptr<Number>, weak_ptr<Number>> m_numbers;

		const float m_spriteScale;
		const float m_numberScale;
		const Vec2 m_backScale;
		const Vec3 m_startPos;
		const Vec3 m_spriteMargin;
		const Vec3 m_numberMargin;
		const wstring m_texKey;
		const eItemType m_itemType;

	public:

		ItemCountUI(const shared_ptr<Stage>& stagePtr,
			const float scale,
			const Vec3& position,
			const wstring& texture,
			eItemType itemType
		) :
			GameObject(stagePtr),
			m_startPos(position),
			m_texKey(texture),
			m_itemType(itemType),
			m_spriteScale(scale * 1.3f),
			m_numberScale(scale * 0.7f),
			m_spriteMargin(scale * 1.15f, 0.0f, 0.0f),
			m_numberMargin(scale * 0.5f, 0.0f, 0.0f),
			m_backScale(Vec2(scale * 3.75f, scale * 2.0f))
		{
		}

		virtual ~ItemCountUI() {}

		void OnCreate() override;

		void OnUpdate() override;

		void UpdateNumbers();
	};
}