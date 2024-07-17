/*!
@file ShopShoppingState.cpp
@brief �V���b�v�̖��I����ԃX�e�[�g
@author �����
*/

#include "stdafx.h"
#include "TitleStage.h"
#include "Scene.h"
#include "Input.h"
#include "Company.h"
#include "Shop.h"
#include "ShopCloseState.h"
#include "ShopShoppingState.h"
#include "ShopConfirmationState.h"

namespace basecross
{
	// �X�e�[�g�J�n���̏���
	void ShopShoppingState::Enter(const shared_ptr<Shop>& shop)
	{
		// �X�e�[�W���擾
		const weak_ptr<TitleStage>& stagePtr = shop->GetTypeStage<TitleStage>();
		// Scene���擾
		const weak_ptr<Scene> scenePtr = App::GetApp()->GetScene<Scene>();

		// ���݂̃X�e�[�g��Shopping�ɕύX
		shop->m_currentState = Shop::eShopState::Shopping;

		// �^�C�g���X�e�[�W�̃v���O���X��ύX
		auto& progless = stagePtr.lock()->GetTitleProgress();
		progless = eTitleProgress::shop;

		// �X�v���C�g�̕\������
		shop->m_purchaseScreenSprite.lock()->SetDrawActive(true);
		shop->m_selectPointSprite.lock()->SetDrawActive(true);

		// �X�v���C�g�̍��W��I����e�ɉ����čX�V
		shop->m_selectPointSprite.lock()->SetPosition(shop->m_selectPointSpritePos.at(shop->m_currentEnhancements));

		// �eLv�擾�p�ϐ�
		vector<ePL> Lv;

		for (int i = 0; i < Shop::eEnhancements::EnhancementsSize; i++)
		{
			switch (i)
			{
			case Shop::eEnhancements::LimitChoices:
				// ���ޏ�������̃��x�����擾
				Lv.push_back(scenePtr.lock()->GetLimitChoicesLevel());
				break;

			case Shop::eEnhancements::Status:
				// �̌@/�ړ����x�����̃��x�����擾
				Lv.push_back(scenePtr.lock()->GetStatusLevel());
				break;

			case Shop::eEnhancements::Gear:
				// ���ؑf�ޏ����������̃��x�����擾
				Lv.push_back(scenePtr.lock()->GetStartGearLevel());
				break;
			default:
				break;
			}
			// Lv�p�����X�v���C�g�̐���
			auto& lvSprite = stagePtr.lock()->AddGameObject<Number>(Vec2(shop->m_numberScale), Vec3(shop->m_playerLvSpritePos.at(i)), L"SHOP_NUMBER", int(Lv.at(i)) + 1);
			// �����X�v���C�g�ێ��p�̔z��Ɋi�[
			shop->m_playerLvNumSprite[i] = lvSprite;

			// ������p������ϐ�
			int cost = 0;
			// 10^(������-1)�̐��l���擾����ׂ̕ϐ�
			int place = 0;

			// ���݂̃��x�����ő�ɂȂ��Ă��Ȃ������`�F�b�N
			if (Lv.at(i) != ePL::Level5) {
				// ������p���擾
				m_cost.at(i) = shop->m_enhancementsCost.at(i).at(int(Lv.at(i)));
				// ������������
				m_digit.at(i) = 0;
			}
			// ���݂̃��x�����ő�̏ꍇ��0��\��
			else {
				m_cost.at(i) = 0;
				// ������1�ɐݒ�
				m_digit.at(i) = 1;
				// 0��0�Ŋ��邱�Ƃ͂ł��Ȃ��̂�1������
				place = 1;
			}
			// ������p�̌���
			int& digit = m_digit.at(i);
			// ���݂̃��x�����ő�ɂȂ��Ă��Ȃ������`�F�b�N
			if (Lv.at(i) != ePL::Level5) {
				// ������p���擾
				cost = m_cost.at(i);
				// �������v�Z
				while (cost > 0) {
					digit++;
					cost /= 10;
				}

				// 10^(����-1)�̐��l���擾����
				place = int(pow(10, digit - 1));

				// ������p���Ď擾
				cost = m_cost.at(i);
			}

			// ��p����ԏ�̌����琶��
			for (int j = 0; j < digit; j++) {
				// �\�����鐔�����v�Z
				int num = cost / place;
				// ��p�\���X�v���C�g�̐���
				auto& costSprite = stagePtr.lock()->AddGameObject<Number>(Vec2(shop->m_numberScale),
					Vec3(shop->m_enhancementsCostSpritePos.at(i) + (-shop->m_numberMargin * float(digit - j))), L"SHOP_NUMBER", num);
				// �����X�v���C�g�ێ��p�̔z��Ɋi�[
				shop->m_enhancementsCostNumSprite.at(i).at(j) = costSprite;
				// ��p���Čv�Z
				cost = m_cost.at(i) % place;
				// digit�̌��������炷
				place /= 10;
			}

			// �������̃X�v���C�g�̃��C���[��ݒ�
			for (int j = 0; j < digit; j++) {
				if (!shop->m_enhancementsCostNumSprite.at(i).at(j).lock()) continue;
				shop->m_enhancementsCostNumSprite.at(i).at(j).lock()->SetDrawLayer(6);
			}

			// ���������擾
			int money = scenePtr.lock()->GetMoney();

			// ���݂̃��x�����ő�ɂȂ��Ă��Ȃ����ƁA��p�Ə��������r���邱�ƂŁA�����\���𔻕�
			if (Lv.at(i) != ePL::Level5 && m_cost.at(i) <= money) {
				// �����\
				shop->m_canEnhancements.at(i) = true;
			}
			else {
				// �����s��
				shop->m_canEnhancements.at(i) = false;
			}
		}

		// �����ł��Ȃ��ꏊ�̃X�v���C�g��\������
		for (int i = 0; i < shop->m_notSelectableSprite.size(); i++) {
			if (!shop->m_canEnhancements.at(i)) {
				// �I���ӏ����O���[�̃X�v���C�g�ŕ���
				shop->m_notSelectableSprite.at(i).lock()->SetDrawActive(true);
			}
		}

		// �����X�v���C�g�̃��C���[��ݒ�
		for (int i = 0; i < shop->m_playerLvNumSprite.size(); i++) {
			if (!shop->m_playerLvNumSprite.at(i).lock()) continue;
			shop->m_playerLvNumSprite.at(i).lock()->SetDrawLayer(6);
		}
	}

	// �X�e�[�g�X�V���̏���
	void ShopShoppingState::Execute(const shared_ptr<Shop>& shop)
	{
		// A�{�^�����͂̎擾(�Q�[���p�b�h���ڑ����͉E�N���b�N�̎擾)
		bool isPushA = Input::GetPushA();
		// B�{�^�����͂̎擾(�Q�[���p�b�h���ڑ����͍��N���b�N�̎擾)
		bool isPushB = Input::GetPushB();

		// �R���g���[���[�̐ڑ��ɉ����đI���������s��
		Input::GetPadConected() ? ControllerSelect(shop) : MouseSelect(shop);

		// A�{�^�����͂��������ꍇ�A�V���b�v�����
		if (isPushA) {
			shop->StartSE(L"CLOSE_SE", 1.0f);
			// �X�e�[�W���擾
			const weak_ptr<TitleStage>& stagePtr = shop->GetTypeStage<TitleStage>();
			// �^�C�g���X�e�[�W�̃v���O���X��ύX
			auto& progless = stagePtr.lock()->GetTitleProgress();
			progless = eTitleProgress::normal;
			// ��Ђ�SELECT�^�O���O��
			weak_ptr<Company> companyPtr = stagePtr.lock()->GetSharedGameObject<Company>(L"Company");
			companyPtr.lock()->RemoveTag(L"SELECT");

			// �X�e�[�g��ύX
			shop->SetState(ShopCloseState::Instance());
			return;
		}

		// B�{�^�����͂��������ꍇ�A�w���m�F��ʂɑJ��
		if (isPushB) {
			if (shop->m_canEnhancements.at(shop->m_currentEnhancements)) {
				// ���莞�̌��ʉ����Đ�
				shop->StartSE(L"PUSH_SE", 1.0f);

				// �X�e�[�g��ύX
				shop->SetState(ShopConfirmationState::Instance());
				return;
			}
			else {
				// �I��s�̌��ʉ����Đ�
				shop->StartSE(L"C_FAILURE_SE", 1.0f);
			}
		}

		// �I�����̓_�ŏ���
		float transparency = 0.0f;
		float hulfLimit = m_totalTimeLimit / 2.0f;

		// �o�ߎ��Ԃɉ����ē_�ł̐؂�ւ�
		if (m_totalTime <= hulfLimit)
		{
			transparency = Utility::Lerp(shop->m_transparencyLimit, 0.0f, m_totalTime * 2.0f);
		}
		else {
			transparency = Utility::Lerp(0.0f, shop->m_transparencyLimit, (m_totalTime - hulfLimit) * 2.0f);
		}

		// �o�ߎ��Ԃ��X�V
		m_totalTime += DELTA_TIME;

		// �o�ߎ��Ԃ�1�b���o�߂��Ă����ꍇ�A�o�ߎ��Ԃ����Z�b�g����
		if (m_totalTime > m_totalTimeLimit) {
			m_totalTime = 0.0f;
		}

		// �����x���X�V
		shop->m_selectPointSprite.lock()->SetDiffuseColor(Col4(1.0f, 1.0f, 1.0f, transparency));
	}

	// �X�e�[�g�I�����̏���
	void ShopShoppingState::Exit(const shared_ptr<Shop>& shop)
	{
		// �X�v���C�g�̔�\������
		shop->m_purchaseScreenSprite.lock()->SetDrawActive(false);
		shop->m_selectPointSprite.lock()->SetDrawActive(false);
		shop->m_iconSprite.at(Shop::eOperationType::Controller).lock()->SetDrawActive(false);
		shop->m_iconSprite.at(Shop::eOperationType::Keyboard).lock()->SetDrawActive(false);

		for (int i = 0; i < shop->m_notSelectableSprite.size(); i++) {
			shop->m_notSelectableSprite.at(i).lock()->SetDrawActive(false);
		}
		for (int i = 0; i < shop->m_playerLvNumSprite.size(); i++) {
			shop->m_playerLvNumSprite.at(i).lock()->SetDrawActive(false);
		}
		for (int i = 0; i < shop->m_enhancementsCostNumSprite.size(); i++) {
			for (int j = 0; j < m_digit.at(i); j++) {
				shop->m_enhancementsCostNumSprite.at(i).at(j).lock()->SetDrawActive(false);
			}
		}
	}

	void ShopShoppingState::ControllerSelect(const shared_ptr<Shop>& shop) {
		// �X�v���C�g�̕\����ύX
		shop->m_iconSprite.at(Shop::eOperationType::Controller).lock()->SetDrawActive(true);
		shop->m_iconSprite.at(Shop::eOperationType::Keyboard).lock()->SetDrawActive(false);

		// LStick��Y�����͗ʂ��擾
		float stickValueY = Input::GetLStickValue().y;

		// ���ȏ��L�X�e�B�b�N���͂��������ꍇ�̂ݑI���������s��
		if (abs(stickValueY) >= m_LStickLowerLimit) {
			if (!m_isInputLStick) {
				// LStick�̓��͕����ɉ����đI������ύX����
				if (stickValueY < 0.0f) {
					// ���݂̑I����e�ɉ����Ď��̑I����e��ύX����
					switch (shop->m_currentEnhancements)
					{
					case Shop::eEnhancements::LimitChoices:
						// �I����e���̌@/�ړ����x�����ɕύX
						shop->m_currentEnhancements = Shop::eEnhancements::Status;
						break;
					case Shop::eEnhancements::Status:
						// �I����e�𓥐ؑf�ޏ����������ɕύX
						shop->m_currentEnhancements = Shop::eEnhancements::Gear;
						break;
					case Shop::eEnhancements::Gear:
						// �I����e�����ޏ�������㏸�ɕύX
						shop->m_currentEnhancements = Shop::eEnhancements::LimitChoices;
						break;
					default:
						break;
					}
				}
				else {
					// ���݂̑I����e�ɉ����Ď��̑I����e��ύX����
					switch (shop->m_currentEnhancements)
					{
					case Shop::eEnhancements::LimitChoices:
						// �I����e�𓥐ؑf�ޏ����������ɕύX
						shop->m_currentEnhancements = Shop::eEnhancements::Gear;
						break;
					case Shop::eEnhancements::Status:
						// �I����e�����ޏ�������㏸�ɕύX
						shop->m_currentEnhancements = Shop::eEnhancements::LimitChoices;
						break;
					case Shop::eEnhancements::Gear:
						// �I����e���̌@/�ړ����x�����ɕύX
						shop->m_currentEnhancements = Shop::eEnhancements::Status;
						break;
					default:
						break;
					}
				}

				// �X�v���C�g�̍��W��I����e�ɉ����čX�V
				shop->m_selectPointSprite.lock()->SetPosition(shop->m_selectPointSpritePos.at(shop->m_currentEnhancements));

				// �X�e�B�b�N���͂̌��ʉ����Đ�
				shop->StartSE(L"SELECT_SE", 1.0f);

				// �o�ߎ��Ԃ����Z�b�g����
				m_totalTime = 0.0f;
			}
			// ���͗ʂ������l�𒴂��Ă����ׁA�X�e�B�b�N���͂��������Ɣ��肷��
			m_isInputLStick = true;
		}
		else {
			// L�X�e�B�b�N���͂̉����l�M���M���œ��͂��ꑱ���邱�Ƃ�h�~����ׂ̏���
			if (stickValueY <= 0.1f) {
				// ���͗ʂ������l��������Ă����ׁA�X�e�B�b�N���͂����������Ɣ��肷��
				m_isInputLStick = false;
			}
		}
	}

	void ShopShoppingState::MouseSelect(const shared_ptr<Shop>& shop) {
		// �X�v���C�g�̕\����ύX
		shop->m_iconSprite.at(Shop::eOperationType::Controller).lock()->SetDrawActive(false);
		shop->m_iconSprite.at(Shop::eOperationType::Keyboard).lock()->SetDrawActive(true);

		// �}�E�X�̍��W���擾
		Vec2 mousePos = Input::GetMousePosition();
		// �I��͈͂̔����̃X�P�[�����擾
		Vec2 helfScale = shop->m_selectPointScale / 2.0f;

		// �I�����̐����[�v
		for (int i = 0; i < shop->m_selectPointSpritePos.size(); i++) {
			// �I�����̒��S���W
			Vec2 selectPos = Vec2(shop->m_selectPointSpritePos.at(i).x, shop->m_selectPointSpritePos.at(i).y);
			// �I��͈͂Ƀ}�E�X�J�[�\�������邩���`�F�b�N
			if (Utility::GetBetween(mousePos, selectPos + helfScale, selectPos - helfScale)) {
				if (shop->m_currentEnhancements != Shop::eEnhancements(i)) {
					// �X�e�B�b�N���͂̌��ʉ����Đ�
					shop->StartSE(L"SELECT_SE", 1.0f);
				}
				// �I����e��ύX
				shop->m_currentEnhancements = Shop::eEnhancements(i);

				// �X�v���C�g�̍��W��I����e�ɉ����čX�V
				shop->m_selectPointSprite.lock()->SetPosition(shop->m_selectPointSpritePos.at(shop->m_currentEnhancements));

				// �o�ߎ��Ԃ����Z�b�g����
				m_totalTime = 0.0f;
			}
		}
	}

	// �C���X�^���X����
	shared_ptr<ShopShoppingState> ShopShoppingState::Instance()
	{
		// new���Z�q�Ŏ��g�𐶐�
		static shared_ptr<ShopShoppingState> instance(new ShopShoppingState);

		// �V�����������ꂽthis�|�C���^
		return instance;
	}
}