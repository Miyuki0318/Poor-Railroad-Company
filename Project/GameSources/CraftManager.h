/*!
@file CraftManager.h
@brief クラフト管理用マネージャ
@author 小澤博貴
*/

#pragma once
#include "TemplateObject.h"
#include "CraftWindow.h"
#include "CraftItemIcon.h"
#include "CraftingIcon.h"
#include "CraftingQTE.h"

namespace basecross
{
	// クラフトアイテムenum
	enum class eCraftItem : size_t
	{
		Rail, // レール
	};

	// クラフトCSVパラメータ
	enum class eCraftParam : size_t
	{
		ItemType,	 // 作るアイテムタイプ
		StoneValue,	 // 石の要求数
		WoodValue,	 // 木の要求数
		SuccesValue, // QTE成功時の作成量
		FailedValue, // QTE失敗時の作成量
	};

	/*!
	@brief クラフト管理クラス(プレイヤーとクラフト関連クラスの仲介クラス)
	*/
	class CraftManager
	{
		weak_ptr<CraftWindow> m_window;	// クラフトウィンドウ

		const vector<vector<string>> m_racipe; // クラフトレシピ

	public:

		/*!
		@brief コンストラクタ
		@param ウィンドウオブジェクトのポインタ
		*/
		CraftManager(const shared_ptr<CraftWindow>& windowPtr) :
			m_window(windowPtr),
			m_racipe(CSVLoader::LoadFile("CraftRacipe"))
		{
		}

		/*!
		@brief デストラクタ
		*/
		~CraftManager() {}

		/*!
		@brief アイテムクラフト関数
		@param プレイヤーのポインタ
		*/
		void Crafting(const shared_ptr<TemplateObject>& tempPtr);

		/*!
		@brief クラフト有効化関数
		@param 有効にするかの真偽
		*/
		void CraftingEnabled(bool enable);

		/*!
		@brief ウィンドウ描画が完了したかの真偽取得関数
		@return 描画完了してたらtrue、それ以外はfalse
		*/
		bool GetShowCraftWindow() const
		{
			return m_window.lock()->GetShowWindow();
		}

	private:

		/*!
		@brief クラフト要求素材数取得関数
		@param クラフトアイテム
		@param 素材タイプ
		@return 素材要求数
		*/
		int GetRacipeValue(eCraftItem item, eCraftParam param)
		{
			return stoi(m_racipe.at(static_cast<size_t>(item)).at(static_cast<size_t>(param)));
		}
	};
}