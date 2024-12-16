# Trình biên dịch C++
CXX = g++                          # Trình biên dịch C++
CXXFLAGS = -Wall -std=c++20         # Các tùy chọn biên dịch

# Đường dẫn tới thư mục chứa các tệp tiêu đề của Stockfish và SFML
INCLUDES = -Ilib -I../lib -IC:/SFML-2.6.2/include

# Đường dẫn tới thư mục chứa các thư viện của SFML
LDFLAGS = -LC:/SFML-a/lib

# Các thư viện cần thiết để liên kết với SFML (tệp .lib)
LIBS = -lsfml-system -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-network

# Tên tệp thực thi
EXE = ChessGame

# Các tệp nguồn trong thư mục src
SRC = $(wildcard src/*.cpp)

# Các tệp đối tượng từ tệp nguồn
OBJ = $(SRC:.cpp=.o)

# Quy tắc mặc định
all: $(EXE)

# Quy tắc biên dịch và liên kết
$(EXE): $(OBJ)
	$(CXX) $(OBJ) -o $(EXE) $(LDFLAGS) $(LIBS)

# Quy tắc biên dịch các tệp nguồn thành các tệp đối tượng
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Quy tắc làm sạch các tệp tạm
clean:
	rm -f $(OBJ) $(EXE)

# Quy tắc làm mới hoàn toàn
rebuild: clean all
