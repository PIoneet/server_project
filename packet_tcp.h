#pragma once

#include <iostream>
#include <vector>
#include <cstring>      // memcpy, memmove
#include <sys/socket.h> // recv
#include <arpa/inet.h>  // ntohs
#include <unistd.h>     // close

using namespace std;


class ReceiveBuffer {
    vector<char> _buffer;
    int _writePos = 0; // 커널이 데이터를 쓸 위치 (커널에 넣은 데이터 크기)
    int _readPos = 0;  // 우리가 데이터를 읽을 위치 (유저가 얼마나 처리했는지)
    int _capacity = 0;

public:
    ReceiveBuffer(int size = 4096) : _capacity(size) {
        _buffer.resize(size);
    }

    // A. 커널이 데이터를 쏟아부을 빈 공간의 포인터 반환
    char* GetWritePtr() { return &_buffer[_writePos]; }

    // B. 커널이 데이터를 쏟아부을 수 있는 남은 공간 크기
    int GetFreeSize() { return _capacity - _writePos; }

    // C. 커널이 데이터를 쓴 후, writePos를 이동시키는 함수
    bool OnWrite(int bytes) { //recv에서 실제로 받은 바이트 수
        if (bytes <= 0) return false;
        _writePos += bytes;
        return true;
    }

    // D. 현재 유효한(처리되지 않은) 데이터의 크기
    int GetDataSize() { return _writePos - _readPos; }

    // E. 패킷 처리를 마친 후, readPos를 이동시키는 함수
    void OnRead(int bytes) {
        _readPos += bytes;

        // [최적화] 버퍼 정리가 필요한가? (데이터를 다 읽었으면 커서 초기화)
        if (_readPos == _writePos) {
            _readPos = 0;
            _writePos = 0;
        } 
        // [중요] 남은 공간이 별로 없으면, 안 읽은 데이터를 맨 앞으로 땡겨오기 (memmove)
        // 안 그러면 버퍼가 꽉 차서 더 이상 recv를 못함
        else if (GetFreeSize() < 100) { 
            int remaining = GetDataSize();
            // 겹치는 구간 복사 가능하도록 memmove 사용 (memcpy는 위험)
            memmove(&_buffer[0], &_buffer[_readPos], remaining);
            _readPos = 0;
            _writePos = remaining;
        }
    }

    // F. 읽을 데이터의 시작 포인터
    char* GetReadPtr() { return &_buffer[_readPos]; }
};
void HandlePacket(int clientSock, ReceiveBuffer* recvBuffer);
void ProcessRecv(int clientSock, ReceiveBuffer& recvBuffer);