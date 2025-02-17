#include "..\image.h"
#include <math.h>
#include <stdlib.h>

int swapval(int *v1, int *v2)
{
	int dum;

	dum = *v1;
	*v1 = *v2;
	*v2 = dum;
}

int main(int ac, char *av[])
{
	ImageData *img, *outimg;
	int res;
	int nx, ny;

	if(ac < 3) {
		printf("invalid parameters!");
		return 0;
	}

	res = readBMPfile(av[1], &img);
	if(res < 0) {
		printf("image not found!");
		return 0;
	}

	nx = img->width;
	ny = img->height;

	outimg = createImage(nx, ny, 24);

	transform(img, outimg);

	writeBMPfile(av[2], outimg);
	disposeImage(img);
	disposeImage(outimg);

	return 0;
}

int transform(ImageData *img, ImageData *outimg)
{
	int x, y;
	int x1, y1;
	int x2, y2;
	int rx1, ry1, rx2, ry2;
	Pixel col;
	int h1, w1;
	int h2, w2;
	double fx, fy;

	x1 = 0;
	y1 = 0;
	x2 = outimg->width - 1;
	y2 = outimg->height - 1;

	rx1 = 0;
	ry1 = 0;
	rx2 = img->width - 1;
	ry2 = img->height - 1;
	
	//input size
	w1 = rx2 - rx1 + 1;
	h1 = ry2 - ry1 + 1;
	//output size
	w2 = x2 - x1 + 1;
	h2 = y2 - y1 + 1;

	col.r = col.g = col.b = 128;

	for(y = y1; y <= y2; y++) {
		for(x = x1; x <= x2; x++) {
			//reverse filled
			fx = x2 - ((double)(x - x1) * w1 / (double)w2 + rx1);
			fy = y2 - ((double)(y - y1) * h1 / (double)h2 + ry1);

			if(interpolate(img, fx, fy, &col))
				setPixel(outimg, x, y, &col);
		}
	}
}

int interpolate(ImageData *img, double x, double y, Pixel *col)
{
	int px, py;

	px = (int)(x + 0.5);
	py = (int)(y + 0.5);

	return getPixel(img, px, py, col);
}