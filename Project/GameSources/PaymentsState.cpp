/*!
@file PaymentsState.cpp
@brief 開発成功時の収入精算表示ステート
@author 小澤博貴
*/

#include "stdafx.h"
#include "GamePlayer.h"
#include "PaymentsState.h"

namespace basecross
{
	// ステート生成時の処理
	void PaymentsState::CreateState()
	{
		// スプライトの生成
		const auto& stagePtr = m_stage.lock();
		m_menuSprite = stagePtr->AddGameObject<Sprite>(L"PAYMENTS_MENU_TX", WINDOW_SIZE, Vec3(0.0f, WINDOW_HEIGHT, 0.0f) + m_menuPosition);
		m_menuSprite.lock()->SetDrawLayer(8);

		// 金額UIの生成
		m_moneyCount = stagePtr->AddGameObject<MoneyCountUI>(m_moneyScale, m_moneyStartPos);

		// 数字スプライトの生成
		NumberCount reward, rSales, gold, resource, rInstallations, total;
		for (int i = 0; i < 5; i++)
		{
			float diff = float(5 - i);
			reward.sprite.push_back(stagePtr->AddGameObject<Number>(Vec2(m_numberScale), Vec3(m_startPosition + (m_numberMarginX * diff))));
			rSales.sprite.push_back(stagePtr->AddGameObject<Number>(Vec2(m_numberScale), Vec3(m_startPosition + (m_numberMarginX * diff) + (m_numberMarginY * 1.0f))));
			gold.sprite.push_back(stagePtr->AddGameObject<Number>(Vec2(m_numberScale), Vec3(m_startPosition + (m_numberMarginX * diff) + (m_numberMarginY * 2.0f))));
			resource.sprite.push_back(stagePtr->AddGameObject<Number>(Vec2(m_numberScale), Vec3(m_startPosition + (m_numberMarginX * diff) + (m_numberMarginY * 3.0f))));
			rInstallations.sprite.push_back(stagePtr->AddGameObject<Number>(Vec2(m_numberScale), Vec3(m_startPosition + (m_numberMarginX * diff) + (m_numberMarginY * 4.0f))));
			total.sprite.push_back(stagePtr->AddGameObject<Number>(Vec2(m_numberScale), Vec3(m_startPosition + m_totalMargin + (m_numberMarginX * diff) + (m_numberMarginY * 5.0f))));
		}

		// マップに数字構造体を登録
		m_numbersMap.emplace(eGamePaymentsState::RewardCount, reward);
		m_numbersMap.emplace(eGamePaymentsState::RailsSales, rSales);
		m_numbersMap.emplace(eGamePaymentsState::GoldBarSales, gold);
		m_numbersMap.emplace(eGamePaymentsState::ResourceSales, resource);
		m_numbersMap.emplace(eGamePaymentsState::RailsInstallations, rInstallations);
		m_numbersMap.emplace(eGamePaymentsState::TotalIncome, total);

		// 全ての数字スプライトを透明に
		for (auto& numbers : m_numbersMap)
		{
			for (auto& number : numbers.second.sprite)
			{
				number.lock()->SetDrawLayer(8);
				number.lock()->SetDiffuseColor(COL_ALPHA);
			}
		}
	}

	// ステートの更新
	void PaymentsState::UpdateState()
	{
		m_stateFunc.at(m_state)();
	}

	// ステートの初期化
	void PaymentsState::ResetState()
	{
		// ステートをメニューフェードにし、スプライトを開始時の座標に
		m_state = eGamePaymentsState::MenuFadeIn;
		m_menuSprite.lock()->SetStartPosition();
		m_moneyCount.lock()->SetMovePosition(Vec3(0.0f, WINDOW_HEIGHT / 2.0f, 0.0f));

		// 全ての数字スプライトを0番・透明・開始時の座標に
		for (auto& numbers : m_numbersMap)
		{
			for (auto& number : numbers.second.sprite)
			{
				number.lock()->SetNumber(0);
				number.lock()->SetDiffuseColor(COL_ALPHA);
				number.lock()->SetStartPosition();
			}
		}
	}

	// メニューフェードイン
	void PaymentsState::MenuFadeInState()
	{
		// シェアドポインタ化
		auto& menu = m_menuSprite.lock();

		// 線形補間で移動
		float ratio = m_fadeTotalTime / m_menuFadeTime;
		Vec3 pos = Utility::Lerp(menu->GetStartPosition(), m_menuPosition, ratio);
		float moveVal = Utility::Lerp(0.0f, -WINDOW_HEIGHT / 2.0f, ratio);

		// フェード時間を過ぎたらステートを切り替え
		if (m_fadeTotalTime >= m_menuFadeTime)
		{
			menu->StartSE(L"COIN_SE", 1.0f);
			m_state = eGamePaymentsState::RewardCount;
			m_fadeTotalTime = 0.0f;
		}

		// 経過時間をデルタタイムで加算し、座標を更新
		m_fadeTotalTime += DELTA_TIME;
		menu->SetPosition(pos);
		m_moneyCount.lock()->SetMovePosition(Vec3(0.0f, moveVal, 0.0f));
	}

	// 開発報酬表示
	void PaymentsState::RewardCountState()
	{
		// 数字構造体を取得し、表示処理を送る
		auto& numbers = m_numbersMap.at(m_state);
		NumberCount::ConsecutiveNumberDraw(numbers, m_totalTime);

		// 処理が終わったらステートを切り替える
		if (numbers.degit < 0)
		{
			m_state = eGamePaymentsState::RailsSales;
		}
	}

	// レール売却収入表示
	void PaymentsState::RailsSalesState()
	{
		// 数字構造体を取得し、表示処理を送る
		auto& numbers = m_numbersMap.at(m_state);
		NumberCount::ConsecutiveNumberDraw(numbers, m_totalTime);

		// 処理が終わったらステートを切り替える
		if (numbers.degit < 0)
		{
			m_state = eGamePaymentsState::GoldBarSales;
		}
	}

	// 金塊売却収入表示
	void PaymentsState::GoldBarSalesState()
	{
		// 数字構造体を取得し、表示処理を送る
		auto& numbers = m_numbersMap.at(m_state);
		NumberCount::ConsecutiveNumberDraw(numbers, m_totalTime);

		// 処理が終わったらステートを切り替える
		if (numbers.degit < 0)
		{
			m_state = eGamePaymentsState::ResourceSales;
		}
	}

	// 資源売却収入表示
	void PaymentsState::ResourceSalesState()
	{
		// 数字構造体を取得し、表示処理を送る
		auto& numbers = m_numbersMap.at(m_state);
		NumberCount::ConsecutiveNumberDraw(numbers, m_totalTime);

		// 処理が終わったらステートを切り替える
		if (numbers.degit < 0)
		{
			m_state = eGamePaymentsState::RailsInstallations;
		}
	}

	// レール設置費用表示
	void PaymentsState::RailsInstallationsState()
	{
		// 数字構造体を取得し、表示処理を送る
		auto& numbers = m_numbersMap.at(m_state);
		NumberCount::ConsecutiveNumberDraw(numbers, m_totalTime);

		// 処理が終わったらステートを切り替える
		if (numbers.degit < 0)
		{
			m_state = eGamePaymentsState::TotalIncome;
		}
	}

	// 合計収入表示
	void PaymentsState::TotalIncomeState()
	{
		// 数字構造体を取得し、表示処理を送る
		auto& numbers = m_numbersMap.at(m_state);
		NumberCount::ConsecutiveNumberDraw(numbers, m_totalTime);

		// 処理が終わったらステートを切り替える
		if (numbers.degit < 0)
		{
			// コインSEを止め、購入SEを再生
			m_menuSprite.lock()->StopSE(L"COIN_SE");
			m_menuSprite.lock()->StartSE(L"BUY_SE", 2.0f);
			m_state = eGamePaymentsState::DrawStandBy;
		}
	}

	// 観るための表示待機
	void PaymentsState::DrawStandByState()
	{
		// 観るための時間を過ぎたらステートを切り替え
		if (m_fadeTotalTime >= m_standByTime)
		{
			m_state = eGamePaymentsState::MenuFadeOut;
			m_fadeTotalTime = 0.0f;
		}

		// 跳んで行くお金アイコン
		auto& number = m_numbersMap.at(eGamePaymentsState::TotalIncome);
		float ratio = number.goal / 200.0f;
		ratio = ceil(ratio);
		float time = m_standByTime / ratio;
		if (m_menuSprite.lock()->SetTimer(time))
		{
			if (m_fadeTotalTime <= time * 2.0f)
			{
				const auto& scene = App::GetApp()->GetScene<Scene>();
				m_stage.lock()->AddMoney(number.goal);
				m_moneyCount.lock()->SetNumberGoal(scene->GetMoney());
			}

			const auto& itemFly = m_stage.lock()->GetSharedGameObject<FlyItemManager>(L"FlyItemManager");
			itemFly->StartFly(eItemType::Money, number.sprite.front().lock()->GetPosition());
		}

		// 経過時間をデルタタイムで加算し、座標を更新
		m_fadeTotalTime += DELTA_TIME;
	}

	// メニューフェードアウト
	void PaymentsState::MenuFadeOutState()
	{
		// シェアドポインタ化
		auto& menu = m_menuSprite.lock();

		// 線形補間で移動
		float ratio = m_fadeTotalTime / m_menuFadeTime;
		Vec3 pos = Utility::Lerp(m_menuPosition, menu->GetStartPosition(), ratio);
		Vec3 move = Utility::Lerp(Vec3(0.0f), Vec3(0.0f, WINDOW_HEIGHT, 0.0f), ratio);

		float moveVal = -WINDOW_HEIGHT / 2.0f;
		if (ratio >= 0.5f) moveVal = Utility::Lerp(-WINDOW_HEIGHT / 2.0f, 0.0f, ratio - 0.5f);

		// フェード時間を過ぎたらステートを切り替え
		if (m_fadeTotalTime >= m_menuFadeTime)
		{
			m_state = eGamePaymentsState::StandBy;
			m_fadeTotalTime = 0.0f;
		}

		// 経過時間をデルタタイムで加算し、座標を更新
		m_fadeTotalTime += DELTA_TIME;
		menu->SetPosition(pos);
		m_moneyCount.lock()->SetMovePosition(Vec3(0.0f, moveVal, 0.0f));
		for (auto& numbers : m_numbersMap)
		{
			MoveNumbersPosition(numbers.second, move);
		}
	}

	// 数字構造体の数字スプライト移動処理
	void PaymentsState::MoveNumbersPosition(const NumberCount& count, const Vec3& move)
	{
		for (auto& number : count.sprite)
		{
			auto& ptr = number.lock();

			// 開始時の座標に移動量を足して座標を更新
			ptr->SetPosition(ptr->GetStartPosition() + move);
		}
	}
}