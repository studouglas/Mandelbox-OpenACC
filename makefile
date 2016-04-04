FRACTAL  = -DMandelBulb
CC       = pgCC
CXX      = pgCC
FLAGS    = -acc -fast -ta=nvidia,cc30 -Minfo=accel -Minform=inform -O2 $(FRACTAL)
CFLAGS   = $(FLAGS)
CXXFLAGS = $(FLAGS)
PROGRAM_NAME=mandelbulb

$(PROGRAM_NAME): main.o savebmp.o getparams.o 3d.o getcolor.o raymarching.o renderer.o init3D.o
	$(CC) -o $@ $? $(CFLAGS) $(LDFLAGS)


clean:
	rm *.o $(PROGRAM_NAME)
