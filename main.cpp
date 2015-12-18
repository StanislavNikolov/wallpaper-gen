#include <png++/png.hpp>
#include <random>

const unsigned long long SIZE = ((1<<13) + 1); // Must be 2^n+1

unsigned world[SIZE][SIZE];

std::random_device randDev;
std::uniform_int_distribution<unsigned> dist(0, 255);
std::default_random_engine eng(randDev());

unsigned rnd(unsigned n)
{
	return dist(eng) % n;
}

// You can tweak the output with these settings.

// A - the maximum size of the error in the center step
#define A 5
// B - the maximum size of the error in the corner step
#define B 42

// is the error part of the average or added after it
#define inA 1
#define inB 1

void generate(unsigned x, unsigned y, unsigned sx, unsigned sy)
{
	unsigned c[4]; // corners
	c[0] = world[x][y];
	c[1] = world[x+sx][y];
	c[2] = world[x][y+sy];
	c[3] = world[x+sx][y+sy];

	if(world[x+sx/2][y+sy/2] == 0)
		world[x+sx/2][y+sy/2] = (c[0] + c[1] + c[2] + c[3] + inA*rnd(A)) / 5 + (1-inA)*rnd(A);

	unsigned cen = world[x+sx/2][y+sy/2];

	world[x+sx/2][y] = (cen + c[0] + c[1] + inB*rnd(B)) / 4 + (1-inB)*rnd(B);
	world[x][y+sy/2] = (cen + c[0] + c[2] + inB*rnd(B)) / 4 + (1-inB)*rnd(B);
	world[x+sx][y+sy/2] = (cen + c[1] + c[3] + inB*rnd(B)) / 4 + (1-inB)*rnd(B);
	world[x+sx/2][y+sy] = (cen + c[2] + c[3] + inB*rnd(B)) / 4 + (1-inB)*rnd(B);

	if(sx > 3)
	{
		generate(x, y, sx/2, sy/2);
		generate(x+sx/2, y, sx/2, sy/2);
		generate(x, y+sy/2, sx/2, sy/2);
		generate(x+sx/2, y+sy/2, sx/2, sy/2);
	}
}

int main()
{
	png::image< png::rgb_pixel > image(SIZE, SIZE);

	world[0][0] = 100;
	world[SIZE-1][0] = 100;
	world[0][SIZE-1] = 100;
	world[SIZE-1][SIZE-1] = 100;
	generate(0, 0, SIZE-1, SIZE-1);

	// Converts the height map to an png::image
	for (png::uint_32 y = 0; y < image.get_height(); ++y)
		for (png::uint_32 x = 0; x < image.get_width(); ++x)
			image[y][x] = png::rgb_pixel(world[x][y], world[x][y], world[x][y]);

	image.write("rgb.png");

	return 0;
}
