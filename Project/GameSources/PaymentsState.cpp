/*!
@file PaymentsState.cpp
@brief 開発成功時の収入精算表示ステート
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"
#include "PaymentsState.h"

namespace basecross
{
	// ステート生成時の処理
	void PaymentsState::CreateState()
	{
		// スプライトの生成
		const auto& stagePtr = m_stage.lock();
		m_menuSprite = stagePtr->AddGameObject<Sprite>(L"PAYMENTS_MENU_TX", WINDOW_SIZE, Vec3(0.0f, WINDOW_HEIGHT, 0.0f) + m_menuPosition);

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
		Vec3 pos = Utility::Lerp(menu->GetStartPosition(), m_menuPosition, m_fadeTotalTime / m_menuFadeTime);

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
	}

	// 開発報酬表示
	void PaymentsState::RewardCountState()
	{
		// 数字構造体を取得し、表示処理を送る
		auto& numbers = m_numbersMap.at(m_state);
		ConsecutiveNumberDraw(numbers);

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
		ConsecutiveNumberDraw(numbers);

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
		ConsecutiveNumberDraw(numbers);

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
		ConsecutiveNumberDraw(numbers);

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
		ConsecutiveNumberDraw(numbers);

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
		ConsecutiveNumberDraw(numbers);

		// 処理が終わったらステートを切り替える
		if (numbers.degit < 0)
		{
			// コインSEを止め、購入SEを再生
			m_menuSprite.lock()->StopSE(L"COIN_SE");
			m_menuSprite.lock()->StartSE(L"BUY_SE", 1.5f);
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

		// フェード時間を過ぎたらステートを切り替え
		if (m_fadeTotalTime >= m_menuFadeTime)
		{
			m_state = eGamePaymentsState::StandBy;
			m_fadeTotalTime = 0.0f;
		}

		// 経過時間をデルタタイムで加算し、座標を更新
		m_fadeTotalTime += DELTA_TIME;
		menu->SetPosition(pos);
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

	// 連番カウンタ表示処理
	void PaymentsState::ConsecutiveNumberDraw(NumberCount& count)
	{
		// 桁事の比較様にwssに変換
		wstringstream time;
		time << count.goal;
		wstringstream total;
		total << static_cast<int>(m_totalTime.at(count.degit));

		// 配列の要素数を設定(元の桁数-現在の桁数)
		int index = count.index - count.degit;

		// 数字を経過時間の現在の桁数番目で更新
		count.sprite.at(index).lock()->SetDiffuseColor(COL_BLACK);
		count.sprite.at(index).lock()->SetNumber(static_cast<int>(m_totalTime.at(count.degit)));

		// 目標数の現在の桁数番目と、経過時間の0番目が同じなら
		bool notZero = (time.str()[count.degit] == '0'); // 目標数が0なら10まで数えてから切り替える
		if (time.str()[count.degit] == total.str()[int(notZero)])
		{
			// 現在の桁数を減らす
			count.degit--;

			// 0より小さくなったら
			if (count.degit < 0)
			{
				// 経過時間を初期化して終了
				for (auto& totalTime : m_totalTime)
				{
					totalTime = 0.0f;
				}
				return;
			}
		}

		// 現在の桁数番目の経過時間をデルタタイムで加算		
		m_totalTime.at(count.degit) += DELTA_TIME / 0.025f;
	}
}