/*!
@file CraftWindow.h
@brief クラフトウィンドウ
@author 小澤博貴
*/

#pragma once
#include "CraftUI.h"

namespace basecross
{	
	/*!
	@brief クラフトウィンドウ
	*/
	class CraftWindow : public CraftUI
	{
	public:

		/*!
		@brief コンストラクタ
		@param ステージポインタ
		*/
		CraftWindow(const shared_ptr<Stage>& stagePtr) :
			CraftUI(stagePtr, L"C_WINDOW_TX", Vec2(200.0f), 0.25f)
		{
		}

		/*!
		@brief デストラクタ
		*/
		virtual ~CraftWindow() {}

		/*!
		@brief 生成時に一度だけ呼び出される関数
		*/
		void OnCreate() override;

		/*!
		@brief 毎フレーム度に呼び出される関数
		*/
		void OnUpdate() override;

		/*!
		@brief 描画変更設定関数
		@param eVerticesRect
		*/
		void SetRectType(eRectType rect) override
		{
			CraftUI::SetRectType(rect);

			for (size_t i = 0; i < m_vertex.vertices.size(); i++)
			{
				m_vertex.vertices.at(i).textureCoordinate = m_rectUVMap.at(rect).at(i);
			}

			m_ptrDraw->UpdateVertices(m_vertex.vertices);
		}
	};
}