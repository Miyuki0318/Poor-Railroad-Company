#include "stdafx.h"
#include "NetworkComponent.h"
#include "PPDataConnecter.h"
#include <stdexcept>

namespace basecross
{
    class NetworkComponent::Impl 
    {
    public:
        Impl() 
        {
            // コンストラクタで特に初期化は必要ない
        }

        ~Impl() {}

        // GetNetworkPtrを使用してP2P通信クラスにアクセス
        PPDataConnecter* GetNetwork() const 
        {
            auto network = PPDataConnecter::GetNetworkPtr();
            if (!network) 
            {
                throw std::runtime_error("Network instance is not available");
            }
            return network;
        }
    };

    NetworkComponent::NetworkComponent(const std::shared_ptr<GameObject>& GameObjectPtr)
        : Component(GameObjectPtr)
        , pImpl(new Impl())
    {
    }

    NetworkComponent::~NetworkComponent() {}

    void NetworkComponent::AddDataToSendBuffer(const std::string& header, const std::string& data) const
    {
        try 
        {
            pImpl->GetNetwork()->AddToSendBuffer(header, data);
        }
        catch (...) {
            // エラーハンドリング（必要に応じてログ出力など）
            throw;
        }
    }

    bool NetworkComponent::GetDataFromReceiveBuffer(const string& header, string& outData) const
    {
        try 
        {
            BufferedData buffer;
            if (pImpl->GetNetwork()->GetFromRecvBufferByHeader(header, buffer))
            {
                outData = buffer.data;
                return true;
            }

            return false;
        }
        catch (...) {
            // エラーハンドリング（必要に応じてログ出力など）
            throw;
        }
    }
}