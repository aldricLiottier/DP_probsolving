CFLAGS = -std=c++17 -O2
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

SRC	=	main.cpp	\
		text.cpp	\
		image.cpp	\

Name = gui

VulkanTest:	$(SRC)
	g++ $(CFLAGS) -o $(Name) $(SRC) $(LDFLAGS)

.PHONY:	test clean

test:	$(Name)
	./$(Name)

clean:
	rm -f $(Name)