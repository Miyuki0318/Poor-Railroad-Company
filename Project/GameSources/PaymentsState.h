/*!
@file PaymentsState.h
@brief 開発成功時の収入精算表示ステート
@author 小澤博貴
*/

#pragma once
#include "Number.h"
#include "GameStage.h"

namespace basecross
{
	// 精算時のステートenum
	enum class eGamePaymentsState : char
	{
		MenuFadeIn,			// メニューフェードイン
		RewardCount,		// 開発報酬
		RailsSales,			// レール売却収入
		GoldBarSales,		// 金塊売却収入
		ResourceSales,		// 資源売却収入
		RailsInstallations,	// レール設置費用
		TotalIncome,		// 合計収入
		DrawStandBy,		// 表示待機
		MenuFadeOut,		// メニューフェードアウト
		StandBy,			// 待機状態
	};

	// ゲームステージの名前宣言
	class GameStage;

	/*!
	@brief 報酬精算時のステート
	*/
	class PaymentsState
	{
		// 所属ステージポインタ
		weak_ptr<GameStage> m_stage;

		// メニュースプライト
		weak_ptr<Sprite> m_menuSprite;	

		// 数値スプライトマップ
		map<eGamePaymentsState, NumberCount> m_numbersMap;
		
		// ステート関数マップ
		map<eGamePaymentsState, function<void()>> m_stateFunc;

		const float m_numberScale;	// 数字のスケール
		const float m_menuFadeTime;	// フェード時間
		const float m_standByTime;	// 観るための時間
		const Vec3 m_startPosition;	// 開始座標
		const Vec3 m_numberMarginX;	// 数字同士の余白
		const Vec3 m_numberMarginY;	// 数字同士の余白
		const Vec3 m_totalMargin;	// 合計収入の余白
		const Vec3 m_menuPosition;	// メニューの座標

		eGamePaymentsState m_state;	// 処理ステート

		float m_fadeTotalTime;		// フェード用経過時間
		vector<float> m_totalTime;	// 経過時間配列

	public:
		
		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		PaymentsState(const shared_ptr<GameStage>& stagePtr) :
			m_stage(stagePtr),
			m_menuFadeTime(1.5f),
			m_standByTime(2.5f),
			m_numberScale(55.0f),
			m_startPosition(135.0f, 75.0f, 0.0f),
			m_numberMarginX(45.0f, 0.0f, 0.0f),
			m_numberMarginY(0.0f, -70.0f, 0.0f),
			m_totalMargin(0.0f, -25.0f, 0.0f),
			m_menuPosition(0.0f, -200.0f, 0.1f)
		{
			m_state = eGamePaymentsState::MenuFadeIn;
			m_fadeTotalTime = 0.0f;
			m_totalTime = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

			m_stateFunc.emplace(eGamePaymentsState::MenuFadeIn, bind(&PaymentsState::MenuFadeInState, this));
			m_stateFunc.emplace(eGamePaymentsState::RewardCount, bind(&PaymentsState::RewardCountState, this));
			m_stateFunc.emplace(eGamePaymentsState::RailsSales, bind(&PaymentsState::RailsSalesState, this));
			m_stateFunc.emplace(eGamePaymentsState::GoldBarSales, bind(&PaymentsState::GoldBarSalesState, this));
			m_stateFunc.emplace(eGamePaymentsState::ResourceSales, bind(&PaymentsState::ResourceSalesState, this));
			m_stateFunc.emplace(eGamePaymentsState::RailsInstallations, bind(&PaymentsState::RailsInstallationsState, this));
			m_stateFunc.emplace(eGamePaymentsState::TotalIncome, bind(&PaymentsState::TotalIncomeState, this));
			m_stateFunc.emplace(eGamePaymentsState::DrawStandBy, bind(&PaymentsState::DrawStandByState, this));
			m_stateFunc.emplace(eGamePaymentsState::MenuFadeOut, bind(&PaymentsState::MenuFadeOutState, this));
		}

		/*!
		@brief デストラクタ
		*/
		~PaymentsState() {}

		/*!
		@brief スプライトの生成関数
		*/
		void CreateState();

		/*!
		@brief ステートの更新処理関数
		*/
		void UpdateState();

		/*!
		@brief ステートの初期化関数
		*/
		void ResetState();

		/*!
		@brief 数字の桁数とインデックスの設定関数
		@param タイプ
		@param 設定する金額
		*/
		void SetNumberGoal(eGamePaymentsState type, int money)
		{
			if (m_numbersMap.find(type) == m_numbersMap.end()) return;

			auto& numbers = m_numbersMap.at(type);
			numbers.goal = money;
			numbers.degit = max(Utility::GetUpperDigit(money) - 1, 0);
			numbers.index = numbers.degit;
		}

		/*!
		@brief ステート取得関数
		@return eGamePaymentsState
		*/
		eGamePaymentsState GetPaymentsState() const
		{
			return m_state;
		}

	private:

		/*!
		@brief メニュースプライトフェードインステート
		*/
		void MenuFadeInState();

		/*!
		@brief 開発報酬表示ステート
		*/
		void RewardCountState();

		/*!
		@brief 線路売却収入表示テート
		*/
		void RailsSalesState();

		/*!
		@brief 金塊売却収入表示ステート
		*/
		void GoldBarSalesState();

		/*!
		@brief 資源売却収入表示ステート
		*/
		void ResourceSalesState();

		/*!
		@brief レール設置費用表示ステート
		*/
		void RailsInstallationsState();

		/*!
		@brief 合計収入表示ステート
		*/
		void TotalIncomeState();

		/*!
		@brief 表示待機状態ステート
		*/
		void DrawStandByState();

		/*!
		@brief メニュースプライトフェードアウトステート
		*/
		void MenuFadeOutState();

		/*!
		@brief 数字移動関数
		@param 数値構造体
		@param 開始時の座標からの移動量
		*/
		void MoveNumbersPosition(const NumberCount& count, const Vec3& move);
	};
}