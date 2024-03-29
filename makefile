CC       = pgCC
CXX      = pgCC
FLAGS    = -acc -fast -ta=nvidia,cc30 -Minfo=accel -Minform=inform -O2
CFLAGS   = $(FLAGS)
CXXFLAGS = $(FLAGS)
BOX_PROGRAM_NAME=mandelbox

$(BOX_PROGRAM_NAME): main.o savebmp.o getparams.o 3d.o getcolor.o raymarching.o renderer.o init3D.o
	$(CC) -o $@ $? $(CFLAGS) $(LDFLAGS) -DMANDELBOX

clean:
	rm *.o $(BOX_PROGRAM_NAME)
