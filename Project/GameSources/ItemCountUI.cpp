#include "stdafx.h"
#include "Project.h"

#define ONE_PLACE(num) num / 1 % 10
#define TENS_PLACE(num) num / 10 % 10


namespace basecross
{
	void ItemCountUI::OnCreate()
	{
		const shared_ptr<Stage>& stagePtr = GetStage();

		m_backSprite = stagePtr->AddGameObject<Sprite>(L"MENU_TX", m_backScale, m_startPos + Vec3(m_numberScale, 0.0f, 0.1f));
		m_itemSprite = stagePtr->AddGameObject<Sprite>(m_texKey, Vec2(m_spriteScale), Vec3(m_startPos));
		m_numbers = make_pair(
			stagePtr->AddGameObject<Number>(Vec2(m_numberScale), Vec3(m_startPos + m_spriteMargin + m_numberMargin)),
			stagePtr->AddGameObject<Number>(Vec2(m_numberScale), Vec3(m_startPos + m_spriteMargin))
		);
	}

	void ItemCountUI::OnUpdate()
	{
		UpdateNumbers();
	}

	void ItemCountUI::UpdateNumbers()
	{
		const auto& player = GetStage()->GetSharedGameObject<GamePlayer>(L"Player");
		int count = player->GetItemCount(m_itemType);
		m_numbers.first.lock()->SetNumber(ONE_PLACE(count));
		m_numbers.second.lock()->SetNumber(TENS_PLACE(count));
	}
}