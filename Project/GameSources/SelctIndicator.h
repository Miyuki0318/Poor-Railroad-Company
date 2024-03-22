/*!
@file SelectIndicator.h
@brief セレクトインディケーター
@author 小澤博貴
*/

#pragma once
#include "TemplateObject.h"

namespace basecross
{
	/*!
	@brief 選択肢描画
	*/
	class SelectIndicator : public TemplateObject
	{
		VertexData m_vertex; // 頂点データ
		shared_ptr<PCTStaticDraw> m_ptrDraw; // 描画コンポーネント
		weak_ptr<TemplateObject> m_player; // プレイヤー

	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		@param プレイヤーポインタ
		*/
		SelectIndicator(const shared_ptr<Stage>& stagePtr,
			const shared_ptr<TemplateObject>& playerPtr
		) :
			TemplateObject(stagePtr),
			m_player(playerPtr)
		{
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~SelectIndicator() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;
	};
}