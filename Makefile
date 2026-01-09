# 1. 컴파일러 설정
CXX = g++
# -MMD -MP 옵션 추가! (이게 마법의 옵션입니다 ✨)
# -MMD: 헤더 파일 의존성 정보(.d)를 자동으로 만들어라
# -MP: 헤더 파일이 지워져도 에러 안 나게 해라
CXXFLAGS = -std=c++14 -Wall -O2 -MMD -MP

# 2. 소스 파일 설정
SRCS = packet_tcp.cpp simple_server.cpp

# 3. 목적 파일(.o)과 의존성 파일(.d) 목록 생성
OBJS = $(SRCS:.cpp=.o)
DEPS = $(SRCS:.cpp=.d)  # .cpp를 .d로 바꾼 목록

# 4. 최종 실행 파일 이름
TARGET = server

# --- 빌드 규칙 ---

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# .cpp -> .o 컴파일 규칙
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ★ 핵심: 컴파일러가 만들어준 의존성 파일(.d)을 읽어오기
# 맨 앞에 '-'는 "파일이 없어도 에러 내지 마라"는 뜻 (첫 빌드 때는 .d가 없으니까)
-include $(DEPS)

# 청소 규칙 (생성된 .d 파일도 같이 지워야 함)
clean:
	rm -f $(OBJS) $(TARGET) $(DEPS)