/*!
@file Shop.h
@brief ショップ機能の実装
@author 小宅碧
*/

#pragma once
#include "TemplateObject.h"

namespace basecross {
	class Shop : public TemplateObject
	{
	private:



	public:
		Shop(const shared_ptr<Stage>& stagePtr // ステージのポインタ
		) :
			TemplateObject(stagePtr)
		{

		}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;


	};
}