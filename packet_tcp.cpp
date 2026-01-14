#include "packet_tcp.h"
#include "packet_type.h"

void ProcessRecv(int clientSock, ReceiveBuffer& recvBuffer) {
    
        while (true) { //패킷 데이더 하나씩 꺼내기

            int dataSize = recvBuffer.GetDataSize(); // 현재 쌓인 데이터 양

            // [조건 1] 헤더(2바이트)조차 안 모였으면 대기 (Stickiness 해결)
            if (dataSize < (int)sizeof(PacketHeader)) { //패킷이 아무리 작아도 헤더가 최소 2바이트임.
                break; // 다음 recv를 기다림
            }
            
            // 헤더 내용을 살짝 엿보기 (Peek)
            PacketHeader* header = reinterpret_cast<PacketHeader*>(recvBuffer.GetReadPtr());
            uint16_t bodyLen = ntohs(header->len); //헤더 데이터 해석해서 바디 길이 얻음

            // [조건 2] "헤더 + 바디" 전체가 다 모였는지 확인 (Fragmentation 해결)
            int totalPacketSize = sizeof(PacketHeader) + bodyLen;
            if (dataSize < totalPacketSize) {
                break; // 바디가 잘려옴. 다음 recv를 기다림
            }

            // [완성] 패킷 하나를 온전히 건져냄
            cout << "[패킷 수신 성공] Body 길이: " << bodyLen << ", 내용: ";
            
            // 데이터 읽기 (헤더 뒤에 있는 실제 데이터)
            char* bodyPtr = recvBuffer.GetReadPtr() + sizeof(PacketHeader);
            string msg(bodyPtr, bodyLen); // 문자열로 복사
            cout << msg << endl;
            send(clientSock, bodyPtr, bodyLen, 0);

            // [정리] 처리한 만큼 readPos 이동 (다음 패킷을 위해)
            recvBuffer.OnRead(totalPacketSize);
        }    
    
    close(clientSock);
}

void HandlePacket(int clientSock, ReceiveBuffer* recvBuffer) {

    while(true){

        if (recvBuffer->GetFreeSize() <= 0) {
            cerr << "버퍼 오버플로우! (해커의 공격이거나 로직 오류)" << endl;
            break;
        }

        int len = recv(clientSock, recvBuffer->GetWritePtr(), recvBuffer->GetFreeSize(), 0);
    
        if (len == 0) {
            cout << "연결 종료" << endl;
            break;
        }

        recvBuffer->OnWrite(len);

        PacketHeader* header = reinterpret_cast<PacketHeader*>(recvBuffer->GetReadPtr());
    
        switch (ntohs(header->id)) {
        case PKT_C_LOGIN:
            {
                uint16_t totalLen = ntohs(header->len);
                cout << "totalLen: " << totalLen << endl;
                PlayerInfoPacket* pkt = reinterpret_cast<PlayerInfoPacket*>(recvBuffer->GetReadPtr());
                cout << "[로그인 요청] HP: " << pkt->hp << ", ATT: " << pkt->attack << endl;
                recvBuffer->OnRead(totalLen);
            }
            break;

        case PKT_C_MOVE:
            {
                uint16_t totalLen = ntohs(header->len);
                cout << "totalLen: " << totalLen << endl;
                MovePacket* pkt = reinterpret_cast<MovePacket*>(recvBuffer->GetReadPtr());
                cout << "[이동 요청] X: " << pkt->x << " Y: " << pkt->y << " Z: " << pkt->z << endl;
                recvBuffer->OnRead(totalLen);
            }
            break;
        
        case PKT_C_CHAT:
            {
                ProcessRecv(clientSock, *recvBuffer);
                break;
            }
    
        
        }
    }
}