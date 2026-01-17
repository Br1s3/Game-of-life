#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define dec(x, y, w) ((y)*((w)) + (x))
#define moveto(y, x) printf("\033[%d;%dH", (y), (x));

void Rule(char **t, int w, int h, const char p, const char cel)
{
    char buf_t[h][w];
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++)
	    buf_t[y][x] = ',';
    
    for (int y = 0; y < h; y++) {
	for (int x = 0; x < w; x++) {

	    int cpt = 0;
	    for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
		    if (x+j < 0 || y+i < 0 || x+j > w-1 || y+i > h-1) continue;
		    if (t[y+i][x+j] == cel) cpt++;
		}
	    }
	    if (cpt < 3 || cpt > 4) buf_t[y][x] = 'M';
	    else if (cpt == 3)      buf_t[y][x] = 'V';
	}
    }

    for (int y = 0; y < h; y++) {
	for (int x = 0; x < w; x++) {
	    // printf("%c", buf_t[y][x]);
	    if      (buf_t[y][x] == 'M') t[y][x] = p;
	    else if (buf_t[y][x] == 'V') t[y][x] = cel;
	}
	// printf("\n");
    }
    // printf("\n");
}

int main(int argc, char *argv[])
{
    argc--; argv++;
    FILE *file = NULL;
    int width;
    int height;
    
    if (argv[0] == NULL) {
	fprintf(stderr, "ERROR: No argument\n");
	return 1;
    }
    file = fopen(argv[0], "r");
    if (file == NULL) {
	fprintf(stderr, "ERROR: Wrong argument or could not open the file: %s\n", argv[0]);
	exit(-1);
    }
    fseek(file, 0L, SEEK_END);
    int file_len = ftell(file);
    rewind(file);
    char buf[1024];
    fgets(buf, sizeof(buf), file);
    rewind(file);

    width = strlen(buf);
    height = file_len/width;

    char tab[height][width];

    for (int y = 0; fgets(buf, sizeof(buf), file); y++) {
	for (int x = 0; x < width; x++) {
	    tab[y][x] = buf[x];
	}
    }
    fclose(file);

    char *ptr_tab[width];
    for (int i = 0; i < height; i++) {
	ptr_tab[i] = tab[i];
    }

    for (int i = 0; i < 10000; i++) {
	moveto(0, 0);
	// puts(tab[0]);
	for (int y = 0; y < height; y++) {
	    for (int x = 0; x < width-1; x++) {
		printf("%c ", tab[y][x]);
	    }
	    printf("\n");
	}
	usleep(50*1000);
	Rule(ptr_tab, width-1, height, '.', '#');
    }
    
    return 0;
}
