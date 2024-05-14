/*!
@file TitleTrain.h
@brief 列車オブジェクト
@author 矢吹悠葉
*/

#pragma once
#include "stdafx.h"
#include "TemplateObject.h"

namespace basecross {
	class TitleTrain : public Train
	{
		enum class State {
			None, // 待機
			GameStart, // ゲームスタート
		};

	public:
		TitleTrain(const shared_ptr<Stage>& stagePtr) :
			Train(stagePtr)
		{
		}
		~TitleTrain() {}
	};

}