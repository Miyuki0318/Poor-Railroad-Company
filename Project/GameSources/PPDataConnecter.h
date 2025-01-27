#ifndef PPDATACONNECTER_H
#define PPDATACONNECTER_H

#include <string>
#include <fstream>
#include <queue>
#include <mutex>
#include <thread>
#include <functional>
#include <unordered_map>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <condition_variable>

using namespace std;

// コールバック関数の型定義
using ReceiveCallback = function<void(const string& data)>;

// wstringとUTF-8の相互変換を行うユーティリティ関数
string WStringToUTF8(const wstring& wstr); // wstringをUTF-8形式のstringに変換
wstring UTF8ToWString(const string& utf8Str); // UTF-8形式のstringをwstringに変換

// 10進数から64進数に変換する関数
static string ConvertToBase64(unsigned long number);
static unsigned long ConvertFromBase64(const string& base64Str);

// データバッファのための構造体
struct BufferedData {
    string header;  // データの種類を識別するヘッダー
    string data;    // 実際のデータ
};

// PPDataConnecterクラスは、データ通信やソケット管理を行う
class PPDataConnecter
{
private:

    static const int BUFFER_SIZE = 4096;
    queue<BufferedData> sendBuffer;    // 送信バッファ
    queue<BufferedData> receiveBuffer; // 受信バッファ
    mutex sendMutex;    // 送信バッファ用ミューテックス
    mutex receiveMutex; // 受信バッファ用ミューテックス

    // 非同期通信用のメンバ
    thread sendThread;
    thread receiveThread;
    atomic<bool> isRunning;
    condition_variable sendCondition;
    SOCKET currentSocket;
    mutex socketMutex;

    // シングルトンインスタンス
    static PPDataConnecter* instance;
    static mutex instanceMutex;

    // コールバック管理用のコンテナを追加
    unordered_map<string, vector<ReceiveCallback>> receiveCallbacks;
    mutex callbackMutex;

public:
    // コンストラクタとデストラクタ
    PPDataConnecter(); // クラスの初期化
    ~PPDataConnecter(); // クラスの終了処理

    // シングルトンアクセス
    static PPDataConnecter* GetNetworkPtr();

    // 初期化と終了
    void InitializeWinsock();
    void Initialize();
    void Finalize();

    // コンソールをUnicode（UTF-8）に設定
    void SetConsoleToUnicode();

    // 新しいソケットを作成
    SOCKET CreateSocket();

    // サーバーを開始し、クライアントからの接続を待機
    void StartServer(SOCKET& serverSocket, const wstring& username);
    
    // サーバーに接続して通信を開始
    void ConnectToServer(SOCKET& clientSocket, const wstring& username);

    // サーバーを開始し、クライアントからの接続を待機し、ファイルの送受信を行う
    void StartFileTransServer(SOCKET& socket, const std::wstring& username);

    // サーバーに接続して通信を開始し、ファイルの送受信を行う
    void ConnectToFileTransServer(SOCKET& socket, const std::wstring& username);

    // メッセージを送信する静的メソッド
    static void SendPPMessage(SOCKET sock, const wstring& username, const wstring& message);

    // メッセージを受信する静的メソッド
    static void ReceivePPMessages(SOCKET socket);
    
    // 通信制御
    void StartCommunication(SOCKET sock);
    void StopCommunication();

    // バッファ関連
    void AddToSendBuffer(const string& header, const string& data);
    void AddToSendBuffer(const string& header, const wstring& data);
    bool GetFromReceiveBuffer(const string& header, string& outData);
    bool GetFromReceiveBuffer(const string& header, wstring& outData);
    void ClearBuffers();

    // ファイルを送信する静的メソッド
    static void SendFile(SOCKET& socket, const wstring& filename, const wstring& fileContent);
    
    // ファイルを受信する静的メソッド
    static void ReceiveFile(SOCKET& clientSocket, wstring& receivedFile);

    // ローカルIPアドレスを取得する静的メソッド
    static string GetLocalIPAddress();
    static wstring GetLocalIPAddressW();

    // IPアドレスとポート番号をサーバーIDに変換する静的メソッド
    static string EncodeAndReverseIPPort(const string& ipAddress, unsigned short port);

    // サーバーIDをIPアドレスとポート番号に変換する静的メソッド
    static pair<string, unsigned short> DecodeAndReverseIPPort(const string& encodedReversed);

private:

    // サーバーソケットをバインドしてリッスン状態にする
    void BindAndListen(SOCKET& serverSocket);

    // クライアントの接続を受け入れる
    void AcceptConnection(SOCKET serverSocket, SOCKET& clientSocket);

    // 文字列をファイルに保存する
    static void SaveString(SOCKET& socket, const wstring& str);

    // ファイルから文字列を取得する
    static void ReadString(SOCKET& socket, string& str);

    // logフォルダ作成関数
    static void CreateLogDirectoryIfNotExist();

    // 非同期通信用の内部メソッド
    void SendThreadFunction();
    void ReceiveThreadFunction();
    void ProcessSendBuffer();
    void ProcessReceiveBuffer();

    // コールバック登録・解除用メソッド
    void RegisterReceiveCallback(const string& header, ReceiveCallback callback);
    void UnregisterReceiveCallback(const string& header);
    void ClearCallbacks();
};

#endif
