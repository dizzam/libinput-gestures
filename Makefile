OBJ = main.o utils.o MainHandler.o SwipeHandler.o PointerHandler.o TapHandler.o PinchHandler.o

.PHONY: clean

%.o: %.cc %.h
	g++ -g -c $< -o $@

main: $(OBJ)
	g++ -g -o main $(OBJ) -linput

clean:
	rm -f *.o main
