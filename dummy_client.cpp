#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "packet_type.h"

using namespace std;

int main() {
    // 1. 소켓 생성
    int clientSock = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSock == -1) {
        cout << "Socket Error" << endl;
        return 0;
    }

    // 2. 서버 연결 설정 (localhost:7777)
    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //클라이언트는 서버랑만 소통함.
    serverAddr.sin_port = htons(7777);

    if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        cout << "Connect Error" << endl;
        return 0;
    }

    cout << "Connected to Server!" << endl;
    while (true) {
        int select;
        cout << "1: Login Packet, 2: Move Packet, 3: Chat Packet" << endl;
        cin >> select;
        if (cin.fail()) {
            cin.clear(); 
            cin.ignore(10000, '\n'); 
    
            cout << "잘못된 입력입니다. 숫자를 입력해주세요." << endl;
            continue; // 다시 루프 처음으로
        }
        switch(select) {
            case 1: {
                PlayerInfoPacket loginPacket;         
                loginPacket.len = htons(sizeof(PlayerInfoPacket)); 
                loginPacket.id = htons(PKT_C_LOGIN);
                loginPacket.hp = 100;
                loginPacket.attack = 50;

                // 4. 전송 (구조체 자체를 바이트 배열인 척 보내기)
                // (char*)로 강제 형변환해서 통째로 보냅니다.
                send(clientSock, (char*)&loginPacket, sizeof(PlayerInfoPacket), 0);
                break;
            }
            case 2: {
                MovePacket movePacket;
                movePacket.len = htons(sizeof(MovePacket));
                movePacket.id = htons(PKT_C_MOVE);
                
                movePacket.x = 1.0f;
                movePacket.y = 2.0f;
                movePacket.z = 3.0f;
                
                send(clientSock, (char*)&movePacket, sizeof(MovePacket), 0);
                break;
            }
            case 3: 
            {
                string input;
                cout << "Send Msg: ";
                cin >> input; // 입력 받기

                if (input == "q") break;
        
                int dataLen = input.length();
                int packetSize = sizeof(PacketHeader) + dataLen;

                // 버퍼 만들기 (헤더 크기 + 데이터 크기만큼)
                vector<char> buffer(packetSize);

                // 1) 헤더 채우기
                PacketHeader* header = reinterpret_cast<PacketHeader*>(&buffer[0]);
                header->len = htons(dataLen + sizeof(PacketHeader)); // 바디 길이 저장 서버가 header->len 읽을 수 있게
                header->id = htons(PKT_C_CHAT);

                // 2) 데이터 복사하기 (헤더 바로 뒤에)
                memcpy(&buffer[sizeof(PacketHeader)], input.c_str(), dataLen);

                // 3) 전송
                send(clientSock, &buffer[0], packetSize, 0);
                break;
            }
                
        }

        
    }
    close(clientSock);
    return 0;
}