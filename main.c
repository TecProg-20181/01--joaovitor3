#include <stdio.h>
#define WIDTH 512
#define HEIGHT 512
#define RGB 3

//Dúvida T é para Transparency

typedef struct _pixel {
    unsigned short int r;
    unsigned short int g;
    unsigned short int b;
} Pixel;

typedef struct _image {
    // [width][height][rgb]
    // 0 -> r
    // 1 -> g
    // 2 -> b
    unsigned short int pixel[WIDTH][HEIGHT][RGB];
    unsigned int width;
    unsigned int height;
} Image;


int maior_inteiro(int numero_a, int numero_b) {
    if (numero_a > numero_b)
        return numero_a;
    return numero_b;
}

int verifica_pixel_igual(Pixel pixel_1, Pixel pixel_2) {
  if (pixel_1.r == pixel_2.r &&
      pixel_1.g == pixel_2.g &&
      pixel_1.b == pixel_2.b)
      return 1;
}

int pixel_igual(Pixel pixel_1, Pixel pixel_2) {
    if (verifica_pixel_igual(pixel_1, pixel_2))
        return 1;
    return 0;
}


Image escala_de_cinza(Image img) {
    /*for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            print("%u", img.pixel[i][j][0] + img.pixel[i][j][1] + img.pixel[i][j][2]);
        }
    }*/

    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            int media = img.pixel[i][j][0] +
                        img.pixel[i][j][1] +
                        img.pixel[i][j][2];
            media /= 3;
            img.pixel[i][j][0] = media;
            img.pixel[i][j][1] = media;
            img.pixel[i][j][2] = media;
        }
    }

    return img;
}

void blur(unsigned int height, unsigned short int pixel[WIDTH][HEIGHT][RGB], int transparency, unsigned int width) {
    for (unsigned int i = 0; i < height; ++i) {
        for (unsigned int j = 0; j < width; ++j) {
            Pixel media = {0, 0, 0};

            int smaller_height = (height - 1 > i + transparency/2) ? i + transparency/2 : height - 1;
            int min_width = (width - 1 > j + transparency/2) ? j + transparency/2 : width - 1;
            for(int x = (0 > i - transparency/2 ? 0 : i - transparency/2); x <= smaller_height; ++x) {
                for(int y = (0 > j - transparency/2 ? 0 : j - transparency/2); y <= min_width; ++y) {
                    media.r += pixel[x_axis][y_axis][0];
                    media.g += pixel[x_axis][y_axis][1];
                    media.b += pixel[x_axis][y_axis][2];
                }
            }

            // printf("%u", media.r)
            media.r /= transparency * transparency;
            media.g /= transparency * transparency;
            media.b /= transparency * transparency;

            pixel[i][j][0] = media.r;
            pixel[i][j][1] = media.g;
            pixel[i][j][2] = media.b;
        }
    }
}

Image rotacionar90direita(Image img) {
    Image rotacionada;

    rotacionada.width = img.height;
    rotacionada.height = img.width;

    for (unsigned int i = 0, y_axis = 0; i < rotacionada.height; ++i, ++y_axis) {
        for (int j = rotacionada.width - 1, x_axis = 0; j >= 0; --j, ++x_axis) {
            rotacionada.pixel[i][j][0] = img.pixel[x_axis][y_axis][0];
            rotacionada.pixel[i][j][1] = img.pixel[x_axis][y_axis][1];
            rotacionada.pixel[i][j][2] = img.pixel[x_axis][y_axis][2];
        }
    }

    return rotacionada;
}

void inverter_cores(unsigned short int pixel[WIDTH][HEIGHT][RGB],
                    unsigned int width, unsigned int height) {
    for (unsigned int i = 0; i < height; ++i) {
        for (unsigned int j = 0; j < width; ++j) {
            pixel[i][j][0] = 255 - pixel[i][j][0];
            pixel[i][j][1] = 255 - pixel[i][j][1];
            pixel[i][j][2] = 255 - pixel[i][j][2];
        }
    }
}

Image cortar_imagem(Image img, int x_axis, int y_axis, int width, int height) {
    Image cortada;

    cortada.width = width;
    cortada.height = height;

    for(int i = 0; i < height; ++i) {
        for(int j = 0; j < width; ++j) {
            cortada.pixel[i][j][0] = img.pixel[i + y_axis][j + x_axis][0];
            cortada.pixel[i][j][1] = img.pixel[i + y_axis][j + x_axis][1];
            cortada.pixel[i][j][2] = img.pixel[i + y_axis][j + x_axis][2];
        }
    }

    return cortada;
}


int main() {
    Image img;

    // read type of image
    char p3[4];
    scanf("%s", p3);

    // read width height and color of image
    int max_color;
    scanf("%u %u %d", &img.width, &img.height, &max_color);

    // read all pixels of image
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            scanf("%hu %hu %hu", &img.pixel[i][j][0],
                                 &img.pixel[i][j][1],
                                 &img.pixel[i][j][2]);

        }
    }

    int n_opcoes;
    scanf("%d", &n_opcoes);

    for(int i = 0; i < n_opcoes; ++i) {
        int opcao;
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: { // Escala de Cinza
                img = escala_de_cinza(img);
                break;
            }
            case 2: { // Filtro Sepia
                for (unsigned int x_axis = 0; x_axis < img.height; ++x_axis) {
                    for (unsigned int j = 0; j < img.width; ++j) {
                        unsigned short int pixel[3];
                        pixel[0] = img.pixel[x_axis][j][0];
                        pixel[1] = img.pixel[x_axis][j][1];
                        pixel[2] = img.pixel[x_axis][j][2];

                        int p =  pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
                        int smaller_r = (255 >  p) ? p : 255;
                        img.pixel[x_axis][j][0] = smaller_r;

                        p =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
                        smaller_r = (255 >  p) ? p : 255;
                        img.pixel[x_axis][j][1] = smaller_r;

                        p =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
                        smaller_r = (255 >  p) ? p : 255;
                        img.pixel[x][j][2] = smaller_r;
                    }
                }

                break;
            }
            case 3: { // Blur
                int tamanho = 0;
                scanf("%d", &tamanho);
                blur(img.height, img.pixel, tamanho, img.width);
                break;
            }
            case 4: { // Rotacao
                int quantas_vezes = 0;
                scanf("%d", &quantas_vezes);
                quantas_vezes %= 4;
                for (int j = 0; j < quantas_vezes; ++j) {
                    img = rotacionar90direita(img);
                }
                break;
            }
            case 5: { // Espelhamento
                int horizontal = 0;
                scanf("%d", &horizontal);

                int width = img.width, height = img.height;

                if (horizontal == 1) width /= 2;
                else height /= 2;

                for (int i2 = 0; i2 < height; ++i2) {
                    for (int j = 0; j < width; ++j) {
                        int x_axis = i2, y = j;

                        if (horizontal == 1) y = img.width - 1 - j;
                        else x_axis = img.height - 1 - i2;

                        Pixel aux1;
                        aux1.r = img.pixel[i2][j][0];
                        aux1.g = img.pixel[i2][j][1];
                        aux1.b = img.pixel[i2][j][2];

                        img.pixel[i2][j][0] = img.pixel[x_axis][y_axis][0];
                        img.pixel[i2][j][1] = img.pixel[x_axis][y_axis][1];
                        img.pixel[i2][j][2] = img.pixel[x_axis][y_axis][2];

                        img.pixel[x_axis][y_axis][0] = aux1.r;
                        img.pixel[x_axis][y_axis][1] = aux1.g;
                        img.pixel[x_axis][y_axis][2] = aux1.b;
                    }
                }
                break;
            }
            case 6: { // Inversao de Cores
                inverter_cores(img.pixel, img.width, img.height);
                break;
            }
            case 7: { // Cortar Imagem
                int x_axis, y_axis;
                scanf("%d %d", &x_axis, &y_axis);
                int width, height;
                scanf("%d %d", &width, &height);

                img = cortar_imagem(img, x_axis, y_axis, width, height);
                break;
            }
        }

    }

    // print type of image
    printf("P3\n");
    // print widthidth height and color of image
    printf("%u %u\n255\n", img.width, img.height);

    // print pixels of image
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            printf("%hu %hu %hu ", img.pixel[i][j][0],
                                   img.pixel[i][j][1],
                                   img.pixel[i][j][2]);

        }
        printf("\n");
    }
    return 0;
}
