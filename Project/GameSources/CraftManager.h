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
	enum class eCraftItem
	{
		Rail, // レール
	};

	// クラフトCSVパラメータ
	enum class eCraftParam
	{
		ItemType,	// 作るアイテムタイプ
		StoneValue,	// 石の要求数
		WoodValue,	// 木の要求数
	};

	/*!
	@brief クラフト管理クラス
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

		~CraftManager() {}

		/*!
		@brief アイテムクラフト関数
		*/
		void Crafting(const shared_ptr<TemplateObject>& tempPtr);

		/*!
		@brief クラフト有効化関数
		*/
		void CraftingEnabled(bool enable);



	private:

		/*!
		@brief クラフト要求素材数取得関数
		@param クラフトアイテム
		@param 素材タイプ
		*/
		int GetRacipeValue(eCraftItem item, eCraftParam param)
		{
			return stoi(m_racipe.at(static_cast<int>(item)).at(static_cast<int>(param)));
		}

	};
}