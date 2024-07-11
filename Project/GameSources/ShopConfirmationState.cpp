/*!
@file ShopConfirmationState.cpp
@brief �V���b�v�̖��I����ԃX�e�[�g
@author �����
*/

#include "stdafx.h"
#include "TitleStage.h"
#include "Scene.h"
#include "Input.h"
#include "Number.h"
#include "Shop.h"
#include "ShopShoppingState.h"
#include "ShopConfirmationState.h"

namespace basecross
{
	// �X�e�[�g�J�n���̏���
	void ShopConfirmationState::Enter(const shared_ptr<Shop>& shop)
	{
		// ���݂̃X�e�[�g��Confirmation�ɕύX
		shop->m_currentState = Shop::eShopState::Confirmation;

		// �ϐ��̏�����
		m_isEnhancements = false;

		// �X�v���C�g�̕\������
		shop->m_confirmationScreenSprite.lock()->SetDrawActive(true);
		shop->m_yesSprite.lock()->SetDrawActive(true);
		shop->m_noSprite.lock()->SetDrawActive(true);
		switch (shop->m_currentEnhancements)
		{
		case Shop::eEnhancements::LimitChoices:
			shop->m_LimitChoicesSprite.lock()->SetDrawActive(true);
			break;

		case Shop::eEnhancements::Status:
			shop->m_statusSprite.lock()->SetDrawActive(true);
			break;

		case Shop::eEnhancements::Gear:
			shop->m_gearSprite.lock()->SetDrawActive(true);
			break;

		default:
			break;
		}


		// �X�e�[�W���擾
		const weak_ptr<TitleStage>& stagePtr = shop->GetTypeStage<TitleStage>();
		// Scene���擾
		const weak_ptr<Scene> scenePtr = App::GetApp()->GetScene<Scene>();

		// �I����e�ɉ����Ď擾����f�[�^��ς���
		switch (shop->m_currentEnhancements)
		{
		case Shop::eEnhancements::LimitChoices:
			// ���݂̃��x�����擾
			m_currentLv = int(scenePtr.lock()->GetLimitChoicesLevel());
			break;

		case Shop::eEnhancements::Status:
			// ���݂̃��x�����擾
			m_currentLv = int(scenePtr.lock()->GetStatusLevel());
			break;

		case Shop::eEnhancements::Gear:
			// ���݂̃��x�����擾
			m_currentLv = int(scenePtr.lock()->GetStartGearLevel());
			break;

		default:
			break;
		}

		// Lv�p�����X�v���C�g�̐���
		const weak_ptr<Number>& lvSprite = stagePtr.lock()->AddGameObject<Number>(Vec2(shop->m_numberScale), Vec3(shop->m_nextLevelSpritePos), L"SHOP_NUMBER", m_currentLv + 1);
		// ���̃��x���\���X�v���C�g�p�̕ϐ��Ɋi�[
		shop->m_enhancementsNextLvSprite = lvSprite;

		// ������p���擾
		m_cost = shop->m_enhancementsCost.at(shop->m_currentEnhancements).at(m_currentLv);

		// ��p���擾
		int cost = m_cost;
		// �������v�Z
		m_digit = 0;
		while (cost > 0) {
			m_digit++;
			cost /= 10;
		}

		// 10^(������-1)�̐��l���擾
		int place = int(pow(10, m_digit -	1));

		// ��p���擾
		cost = m_cost;

		// ��p����ԏ�̌����琶��
		for (int i = 0; i < m_digit; i++) {
			// �\�����鐔�����v�Z
			int num = cost / place;

			// ��p�\���X�v���C�g�̐���(�E�����Ő���)
			auto& costSprite = stagePtr.lock()->AddGameObject<Number>(Vec2(shop->m_numberScale),
				Vec3(shop->m_nextCostSpritePos + (-shop->m_numberMargin * float(m_digit - i))), L"SHOP_NUMBER", num);
			// �����X�v���C�g�ێ��p�̔z��Ɋi�[
			shop->m_enhancementsNextCostSprite.at(i) = costSprite;

			// ��p���Čv�Z
			cost = m_cost % place;
			// digit�̌��������炷
			place /= 10;
		}

		// �����X�v���C�g�̃��C���[�ݒ�
		shop->m_enhancementsNextLvSprite.lock()->SetDrawLayer(15);
		for (int i = 0; i < m_digit; i++) {
			if (!shop->m_enhancementsNextCostSprite.at(i).lock()) continue;
			shop->m_enhancementsNextCostSprite.at(i).lock()->SetDrawActive(true);
			shop->m_enhancementsNextCostSprite.at(i).lock()->SetDrawLayer(15);
		}
	}

	// �X�e�[�g�X�V���̏���
	void ShopConfirmationState::Execute(const shared_ptr<Shop>& shop)
	{
		// Scene���擾
		const weak_ptr<Scene> scenePtr = App::GetApp()->GetScene<Scene>();

		// A�{�^�����͂��������ꍇ�A�V���b�v�����
		if (Input::GetPushA()) {
			shop->SetState(ShopShoppingState::Instance());
			// ���莞�̌��ʉ����Đ�
			shop->StartSE(L"PUSH_SE", 1.0f);
			return;
		}

		// B�{�^�����͂��������ꍇ�A�I����e������
		if (Input::GetPushB()) {
			if (m_isEnhancements) {
				// �I����e�ɉ����Ď擾����f�[�^��ς���
				switch (shop->m_currentEnhancements)
				{
				case Shop::eEnhancements::LimitChoices:
					// ���x�����グ�鏈��
					scenePtr.lock()->SetLimitChoicesLevel(ePL(m_currentLv + 1));
					break;

				case Shop::eEnhancements::Status:
					// ���x�����グ�鏈��
					scenePtr.lock()->SetStatusLevel(ePL(m_currentLv + 1));
					break;

				case Shop::eEnhancements::Gear:
					// ���x�����グ�鏈��
					scenePtr.lock()->SetStartGearLevel(ePL(m_currentLv + 1));
					break;

				default:
					break;
				}

				// ���������擾
				int money = scenePtr.lock()->GetMoney();
				// ������������
				money -= m_cost;
				// �������̌�����K�p
				scenePtr.lock()->SetMoney(money);

				// �����������̌��ʉ���炷
				shop->StartSE(L"BUY_SE", 2.0f);

				// �X�e�[�g��ύX
				shop->SetState(ShopShoppingState::Instance());
				return;
			}
			else {
				// ���莞�̌��ʉ����Đ�
				shop->StartSE(L"PUSH_SE", 1.0f);

				// �X�e�[�g��ύX
				shop->SetState(ShopShoppingState::Instance());
				return;

			}
		}

		// ���ȏ��L�X�e�B�b�N���͂��������ꍇ�̂ݑI���������s��
		if (abs(Input::GetLStickValue().x) >= m_LStickLowerLimit) {
			if (!m_isInputLStick) {
				if (m_isEnhancements) {
					// �I�����̊g�k����
					m_isEnhancements = false;
				}
				else {
					m_isEnhancements = true;
				}
				// �o�ߎ��Ԃ����Z�b�g����
				m_totalTime = 0.0f;

				// �X�e�B�b�N���͂̌��ʉ����Đ�
				shop->StartSE(L"SELECT_SE", 1.0f);
			}
			// ���͗ʂ������l�𒴂��Ă����ׁA�X�e�B�b�N���͂��������Ɣ��肷��
			m_isInputLStick = true;
		}
		else {
			// ���͗ʂ������l��������Ă����ׁA�X�e�B�b�N���͂����������Ɣ��肷��
			m_isInputLStick = false;
		}

		// �I�����̊g�嗦
		float spriteSizeRange = 0.0f;

		// �o�ߎ��Ԃɉ����ē_�ł̐؂�ւ�
		if (m_totalTime <= 0.5f)
		{
			spriteSizeRange = Utility::Lerp(shop->m_spriteSizeRange, 1.0f, m_totalTime * 2.0f);
		}
		else {
			if (m_totalTime <= 1.0f) {
				spriteSizeRange = Utility::Lerp(1.0f, shop->m_spriteSizeRange, (m_totalTime - 0.5f ) * 2.0f);
			}
		}

		// �o�ߎ��Ԃ��X�V
		m_totalTime += DELTA_TIME;

		// �o�ߎ��Ԃ�1�b���o�߂��Ă����ꍇ�A�o�ߎ��Ԃ����Z�b�g����
		if (m_totalTime > 1.0f) {
			m_totalTime = 0.0f;
		}

		// �����x���X�V
		if (m_isEnhancements) {
			shop->m_yesSprite.lock()->SetScale(shop->m_confirmSpritedefScale * spriteSizeRange);
			shop->m_noSprite.lock()->SetScale(shop->m_confirmSpritedefScale);
		}
		else {
			shop->m_yesSprite.lock()->SetScale(shop->m_confirmSpritedefScale);
			shop->m_noSprite.lock()->SetScale(shop->m_confirmSpritedefScale * spriteSizeRange);
		}		
	}

	// �X�e�[�g�I�����̏���
	void ShopConfirmationState::Exit(const shared_ptr<Shop>& shop)
	{
		// �X�v���C�g�̔�\������
		shop->m_confirmationScreenSprite.lock()->SetDrawActive(false);
		shop->m_yesSprite.lock()->SetDrawActive(false);
		shop->m_noSprite.lock()->SetDrawActive(false);

		switch (shop->m_currentEnhancements)
		{
		case Shop::eEnhancements::LimitChoices:
			shop->m_LimitChoicesSprite.lock()->SetDrawActive(false);
			break;

		case Shop::eEnhancements::Status:
			shop->m_statusSprite.lock()->SetDrawActive(false);
			break;

		case Shop::eEnhancements::Gear:
			shop->m_gearSprite.lock()->SetDrawActive(false);
			break;

		default:
			break;
		}

		shop->m_enhancementsNextLvSprite.lock()->SetDrawActive(false);
		for (int i = 0; i < shop->m_enhancementsNextCostSprite.size(); i++) {
			if (!shop->m_enhancementsNextCostSprite.at(i).lock()) continue;
			shop->m_enhancementsNextCostSprite.at(i).lock()->SetDrawActive(false);
		}

	}

	// �C���X�^���X����
	shared_ptr<ShopConfirmationState> ShopConfirmationState::Instance()
	{
		// new���Z�q�Ŏ��g�𐶐�
		static shared_ptr<ShopConfirmationState> instance(new ShopConfirmationState);

		// �V�����������ꂽthis�|�C���^
		return instance;
	}
}