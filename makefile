FRACTAL  = -DMandelBulb
CC       = pgc++
CFLAGS   = -acc -fast -ta=nvidia,cc30 -Minfo=accel -Minform=inform -O2 $(FRACTAL)

PROGRAM_NAME=mandelbox

$(PROGRAM_NAME): main.o print.o timing.o savebmp.o getparams.o 3d.o getcolor.o distance_est.o \
	mandelboxde.o raymarching.o renderer.o init3D.o
	$(CC) -o $@ $? $(CFLAGS) $(LDFLAGS)


clean:
	rm *.o $(PROGRAM_NAME) $(EXEEXT) *~
