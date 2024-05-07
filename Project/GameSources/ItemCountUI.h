#pragma once
#include "Number.h"
#include "CraftManager.h"

namespace basecross
{
	class ItemCountUI : public GameObject
	{
		weak_ptr<Sprite> m_backSprite;
		map<eItemType, weak_ptr<Sprite>> m_itemIconMap;
		map<eItemType, pair<weak_ptr<Number>, weak_ptr<Number>>> m_itemCountMap;

		const float m_spriteScale;
		const float m_numberScale;
		const Vec3 m_itemMargin;
		const Vec3 m_numMargin;
		const Vec3 m_spriteMargin;
		const Vec3 m_startPos;

	public:

		ItemCountUI(const shared_ptr<Stage>& stagePtr,
			const Vec3& position
		) :
			GameObject(stagePtr),
			m_startPos(position),
			m_spriteScale(45.0f),
			m_numberScale(31.5f),
			m_itemMargin(100.0f, 0.0f, 0.0f),
			m_numMargin(25.0f, 0.0f, 0.0f),
			m_spriteMargin(45.0f, 0.0f, 0.0f)
		{
		}

		virtual ~ItemCountUI() {}

		void OnCreate() override;

		void OnUpdate() override;

		void UpdateNumbers();
	};
}