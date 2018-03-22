#include <stdio.h>
#define WIDTH 512
#define HEIGHT 512

typedef struct _pixel {
    unsigned short int r;
    unsigned short int g;
    unsigned short int b;
} Pixel;

typedef struct _image {
    Pixel pixel[WIDTH][HEIGHT];
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
          int media = img.pixel[i][j].r +
                      img.pixel[i][j].g +
                      img.pixel[i][j].b;
          media /= 3;
          img.pixel[i][j].r = media;
          img.pixel[i][j].g = media;
          img.pixel[i][j].b = media;
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

Image blur(Image img, int size) {
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            Pixel media = {0, 0, 0};

            int smaller_height = (img.height - 1 > i + size/2) ? i + size/2 : img.height - 1;
            int min_width = (img.width - 1 > j + size/2) ? j + size/2 : img.width - 1;
            for(int iterator_x = (0 > i - size/2 ? 0 : i - size/2); iterator_x <= smaller_height; ++iterator_x) {
                for(int iterator_y = (0 > j - size/2 ? 0 : j - size/2); iterator_y <= min_width; ++iterator_y) {
                    media.r += img.pixel[iterator_x][iterator_y].r;
                    media.g += img.pixel[iterator_x][iterator_y].g;
                    media.b += img.pixel[iterator_x][iterator_y].b;
                }
            }

            // printf("%u", media.r)
            media.r /= size * size;
            media.g /= size * size;
            media.b /= size * size;

            img.pixel[i][j].r = media.r;
            img.pixel[i][j].g = media.g;
            img.pixel[i][j].b = media.b;
        }
    }
    return img;
}


// void blur(unsigned int h, unsigned short int pixel[512][512][3], int T, unsigned int w) {
//     for (unsigned int i = 0; i < h; ++i) {
//         for (unsigned int j = 0; j < w; ++j) {
//             Pixel media = {0, 0, 0};
//
//             int menor_h = (h - 1 > i + T/2) ? i + T/2 : h - 1;
//             int min_w = (w - 1 > j + T/2) ? j + T/2 : w - 1;
//             for(int x = (0 > i - T/2 ? 0 : i - T/2); x <= menor_h; ++x) {
//                 for(int y = (0 > j - T/2 ? 0 : j - T/2); y <= min_w; ++y) {
//                     media.r += pixel[x][y][0];
//                     media.g += pixel[x][y][1];
//                     media.b += pixel[x][y][2];
//                 }
//             }
//
//             // printf("%u", media.r)
//             media.r /= T * T;
//             media.g /= T * T;
//             media.b /= T * T;
//
//             pixel[i][j][0] = media.r;
//             pixel[i][j][1] = media.g;
//             pixel[i][j][2] = media.b;
//         }
//     }
// }

Image rotacionar90direita(Image img) {
    Image rotacionada;

    rotacionada.width = img.height;
    rotacionada.height = img.width;

    for (unsigned int i = 0, iterator_y = 0; i < rotacionada.height; ++i, ++iterator_y) {
        for (int j = rotacionada.width - 1, iterator_x = 0; j >= 0; --j, ++iterator_x) {
            rotacionada.pixel[i][j].r = img.pixel[iterator_x][iterator_y].r;
            rotacionada.pixel[i][j].g = img.pixel[iterator_x][iterator_y].b;
            rotacionada.pixel[i][j].b = img.pixel[iterator_x][iterator_y].b;
        }
    }

    return rotacionada;
}

Image inverter_cores(Image img) {
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            img.pixel[i][j].r = 255 - img.pixel[i][j].r;
            img.pixel[i][j].g = 255 - img.pixel[i][j].g;
            img.pixel[i][j].b = 255 - img.pixel[i][j].b;
        }
    }
  return img;
}

Image corta_imagem_parcialmente(Image img, int iterator_x, int iterator_y, int width, int height) {
    Image cortada;

    cortada.width = width;
    cortada.height = height;

    for(int i = 0; i < height; ++i) {
        for(int j = 0; j < width; ++j) {
            cortada.pixel[i][j].r = img.pixel[i + iterator_y][j + iterator_x].r;
            cortada.pixel[i][j].g = img.pixel[i + iterator_y][j + iterator_x].g;
            cortada.pixel[i][j].b = img.pixel[i + iterator_y][j + iterator_x].b;
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
           pixel[0] = img.pixel[i][j].r;
           pixel[1] = img.pixel[i][j].g;
           pixel[2] = img.pixel[i][j].b;

          int final_pixel =  pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
          int smaller_r = set_pixel_size(final_pixel);
          img.pixel[i][j].r = smaller_r;

          final_pixel =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
          smaller_r = set_pixel_size(final_pixel);
          img.pixel[i][j].g = smaller_r;

          final_pixel =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
          smaller_r = set_pixel_size(final_pixel);
          img.pixel[i][j].b = smaller_r;
      }
  }
  return img;
}

Image read_image_pixels(Image img){
  for (unsigned int i = 0; i < img.height; ++i) {
      for (unsigned int j = 0; j < img.width; ++j) {
          scanf("%hu %hu %hu", &img.pixel[i][j].r,
                               &img.pixel[i][j].g,
                               &img.pixel[i][j].b);

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

  int width = img.width;
  int height = img.height;

  if (horizontal == 1) width /= 2;
  else height /= 2;

  for (int i2 = 0; i2 < height; ++i2) {
      for (int j = 0; j < width; ++j) {
          int iterator_x = i2, iterator_y = j;

          if (horizontal == 1) iterator_y = img.width - 1 - j;
          else iterator_x = img.height - 1 - i2;

          Pixel aux1;
          aux1.r = img.pixel[i2][j].r;
          aux1.g = img.pixel[i2][j].g;
          aux1.b = img.pixel[i2][j].b;

          img.pixel[i2][j].r = img.pixel[iterator_x][iterator_y].r;
          img.pixel[i2][j].g = img.pixel[iterator_x][iterator_y].g;
          img.pixel[i2][j].b = img.pixel[iterator_x][iterator_y].b;

          img.pixel[iterator_x][iterator_y].r = aux1.r;
          img.pixel[iterator_x][iterator_y].g = aux1.g;
          img.pixel[iterator_x][iterator_y].b = aux1.b;
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
          printf("%hu %hu %hu ", img.pixel[i][j].r,
                                 img.pixel[i][j].g,
                                 img.pixel[i][j].b);

      }
      printf("\n");
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
        // img = menu(opcao, img);

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
                int size = 0;
                scanf("%d", &size);
                img = blur(img, size);
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
                img = inverter_cores(img);
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
