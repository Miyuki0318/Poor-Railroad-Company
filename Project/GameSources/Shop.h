/*!
@file Shop.h
@brief �V���b�v�@�\�̎���
@author �����
*/

#pragma once
#include "TemplateObject.h"

namespace basecross {
	class Shop : public TemplateObject
	{
	private:



	public:
		Shop(const shared_ptr<Stage>& stagePtr // �X�e�[�W�̃|�C���^
		) :
			TemplateObject(stagePtr)
		{

		}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;


	};
}