#include <stdio.h>
#define WIDTH 512
#define HEIGHT 512
#define RGB 3
#define RED 0
#define GREEN 1
#define BLUE 2

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

Image set_escala_cinza(Image img){
  for (unsigned int i = 0; i < img.height; ++i) {
      for (unsigned int j = 0; j < img.width; ++j) {
          int media = img.pixel[i][j][RED] +
                      img.pixel[i][j][GREEN] +
                      img.pixel[i][j][BLUE];
          media /= 3;
          img.pixel[i][j][RED] = media;
          img.pixel[i][j][GREEN] = media;
          img.pixel[i][j][BLUE] = media;
      }
  }
  return img;
}


Image escala_de_cinza(Image img) {

    img = set_escala_cinza(img);

    return img;
}

int condicao_e_verdadeira(int condition){
  if(condition)
    return 1;
  return 0;
}

int transform_ternary_operator_in_condition(int condition, int if_case, int else_case){
  if(condicao_e_verdadeira(condition))
    return if_case;
  return else_case;
}


void blur(unsigned int height, unsigned short int pixel[WIDTH][HEIGHT][RGB], unsigned int width) {
    int size = 0;
    scanf("%d", &size);
    for (unsigned int i = 0; i < height; ++i) {
        for (unsigned int j = 0; j < width; ++j) {
            Pixel media = {0, 0, 0};

            int smaller_height = (height - 1 > i + size/2) ? i + size/2 : height - 1;
            // int smaller_height = transform_ternary_operator_in_condition((height - 1 > i + size/2), i + size/2, height - 1);
            int min_width = (width - 1 > j + size/2) ? j + size/2 : width - 1;
            // int min_width = transform_ternary_operator_in_condition(width - 1 > j + size/2, j + size/2, width - 1)
            for(int iterator_x = (0 > i - size/2 ? 0 : i - size/2); iterator_x <= smaller_height; ++iterator_x) {
                for(int iterator_y = (0 > j - size/2 ? 0 : j - size/2); iterator_y <= min_width; ++iterator_y) {
                    media.r += pixel[iterator_x][iterator_y][RED];
                    media.g += pixel[iterator_x][iterator_y][GREEN];
                    media.b += pixel[iterator_x][iterator_y][BLUE];
                }
            }

            // printf("%u", media.r)
            media.r /= size * size;
            media.g /= size * size;
            media.b /= size * size;

            pixel[i][j][RED] = media.r;
            pixel[i][j][GREEN] = media.g;
            pixel[i][j][BLUE] = media.b;
        }
    }
}

Image rotacionar90direita(Image img) {
    Image rotacionada;

    rotacionada.width = img.height;
    rotacionada.height = img.width;

    for (unsigned int i = 0, iterator_y = 0; i < rotacionada.height; ++i, ++iterator_y) {
        for (int j = rotacionada.width - 1, iterator_x = 0; j >= 0; --j, ++iterator_x) {
            rotacionada.pixel[i][j][RED] = img.pixel[iterator_x][iterator_y][RED];
            rotacionada.pixel[i][j][GREEN] = img.pixel[iterator_x][iterator_y][BLUE];
            rotacionada.pixel[i][j][BLUE] = img.pixel[iterator_x][iterator_y][BLUE];
        }
    }

    return rotacionada;
}

void inverter_cores(unsigned short int pixel[WIDTH][HEIGHT][RGB],
                    unsigned int width, unsigned int height) {
    for (unsigned int i = 0; i < height; ++i) {
        for (unsigned int j = 0; j < width; ++j) {
            pixel[i][j][RED] = 255 - pixel[i][j][RED];
            pixel[i][j][GREEN] = 255 - pixel[i][j][GREEN];
            pixel[i][j][BLUE] = 255 - pixel[i][j][BLUE];
        }
    }
}

Image corta_imagem_parcialmente(Image img, int iterator_x, int iterator_y, int width, int height) {
    Image cortada;

    cortada.width = width;
    cortada.height = height;

    for(int i = 0; i < height; ++i) {
        for(int j = 0; j < width; ++j) {
            cortada.pixel[i][j][RED] = img.pixel[i + iterator_y][j + iterator_x][RED];
            cortada.pixel[i][j][GREEN] = img.pixel[i + iterator_y][j + iterator_x][GREEN];
            cortada.pixel[i][j][BLUE] = img.pixel[i + iterator_y][j + iterator_x][BLUE];
        }
    }

    return cortada;
}

int set_pixel_size(int pixel){
  if(pixel < 255)
    return pixel;
  return 255;
}

Image sepia(Image img){
  for (unsigned int i = 0; i < img.height; ++i) {
      for (unsigned int j = 0; j < img.width; ++j) {
          unsigned short int pixel[3];
           pixel[RED] = img.pixel[i][j][RED];
           pixel[GREEN] = img.pixel[i][j][GREEN];
           pixel[BLUE] = img.pixel[i][j][BLUE];

          int final_pixel =  pixel[RED] * .393 + pixel[GREEN] * .769 + pixel[BLUE] * .189;
          int smaller_r = set_pixel_size(final_pixel);
          img.pixel[i][j][RED] = smaller_r;

          final_pixel =  pixel[RED] * .349 + pixel[GREEN] * .686 + pixel[BLUE] * .168;
          smaller_r = set_pixel_size(final_pixel);
          img.pixel[i][j][GREEN] = smaller_r;

          final_pixel =  pixel[RED] * .272 + pixel[GREEN] * .534 + pixel[BLUE] * .131;
          smaller_r = set_pixel_size(final_pixel);
          img.pixel[i][j][BLUE] = smaller_r;
      }
  }
  return img;
}

Image read_image_pixels(Image img){
  for (unsigned int i = 0; i < img.height; ++i) {
      for (unsigned int j = 0; j < img.width; ++j) {
          scanf("%hu %hu %hu", &img.pixel[i][j][RED],
                               &img.pixel[i][j][GREEN],
                               &img.pixel[i][j][BLUE]);

      }
  }
  return img;
}

Image corta_imagem_totalmente(Image img){
  int iterator_x, iterator_y;
  scanf("%d %d", &iterator_x, &iterator_y);
  int width, height;
  scanf("%d %d", &width, &height);

  img = corta_imagem_parcialmente(img, iterator_x, iterator_y, width, height);
  return img;
}

Image espelhamento(Image img){
  int horizontal = 0;
  scanf("%d", &horizontal);

  int width = img.width, height = img.height;

  if (horizontal == 1) width /= 2;
  else height /= 2;

  for (int i2 = 0; i2 < height; ++i2) {
      for (int j = 0; j < width; ++j) {
          int iterator_x = i2, iterator_y = j;

          if (horizontal == 1) iterator_y = img.width - 1 - j;
          else iterator_x = img.height - 1 - i2;

          Pixel aux1;
          aux1.r = img.pixel[i2][j][RED];
          aux1.g = img.pixel[i2][j][GREEN];
          aux1.b = img.pixel[i2][j][BLUE];

          img.pixel[i2][j][RED] = img.pixel[iterator_x][iterator_y][RED];
          img.pixel[i2][j][GREEN] = img.pixel[iterator_x][iterator_y][GREEN];
          img.pixel[i2][j][BLUE] = img.pixel[iterator_x][iterator_y][BLUE];

          img.pixel[iterator_x][iterator_y][RED] = aux1.r;
          img.pixel[iterator_x][iterator_y][GREEN] = aux1.g;
          img.pixel[iterator_x][iterator_y][BLUE] = aux1.b;
      }
  }
  return img;
}

Image rotaciona(Image img){
  int how_many_times = 0;
  scanf("%d", &how_many_times);
  how_many_times %= 4;
  for (int j = 0; j < how_many_times; ++j) {
    img = rotacionar90direita(img);
  }
  return img;
}

void print_pixels(Image img){
  for (unsigned int i = 0; i < img.height; ++i) {
      for (unsigned int j = 0; j < img.width; ++j) {
          printf("%hu %hu %hu ", img.pixel[i][j][RED],
                                 img.pixel[i][j][GREEN],
                                 img.pixel[i][j][BLUE]);

      }
      printf("\n");
  }
}

void menu(int opcao, Image img){
  switch(opcao) {
    case 1: { // Escala de Cinza
      img = escala_de_cinza(img);
      break;
    }
    case 2: { // Filtro Sepia
      img = sepia(img);
      break;
    }
    case 3: { // Blur
      blur(img.height, img.pixel, img.width);
      break;
    }
    case 4: { // Rotacao
      img = rotaciona(img);
      break;
    }
    case 5: { // Espelhamento
      img = espelhamento(img);
      break;
    }
    case 6: { // Inversao de Cores
      inverter_cores(img.pixel, img.width, img.height);
      break;
    }
    case 7: { // Cortar Imagem
      img = corta_imagem_totalmente(img);
      break;
    }
  }
}

int main() {
    Image img;

    // read type of image
    char type_of_image[4];
    scanf("%s", type_of_image);

    // read width height and color of image
    int max_color;
    scanf("%u %u %d", &img.width, &img.height, &max_color);

    // read all pixels of image
    img = read_image_pixels(img);

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
                img = sepia(img);
                break;
            }
            case 3: { // Blur
                blur(img.height, img.pixel, img.width);
                break;
            }
            case 4: { // Rotacao
                img = rotaciona(img);
                break;
            }
            case 5: { // Espelhamento
                img = espelhamento(img);
                break;
            }
            case 6: { // Inversao de Cores
                inverter_cores(img.pixel, img.width, img.height);
                break;
            }
            case 7: { // Cortar Imagem
                img = corta_imagem_totalmente(img);
                break;
            }
        }
    }

    // print type of image
    printf("P3\n");
    // print width height and color of image
    printf("%u %u\n255\n", img.width, img.height);

    // print pixels of image
    print_pixels(img);
    return 0;
}
