#include <stdio.h>
#define WIDTH 512
#define HEIGHT 512

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
    Pixel pixel[WIDTH][HEIGHT];
    unsigned int width;
    unsigned int height;
} Image;


int max_numero(int numero_a, int numero_b) {
    if (numero_a > numero_b)
        return numero_a;
    return numero_b;
}

int min_numero(int numero_a, int numero_b) {
    if (numero_a < numero_b)
        return numero_a;
    return numero_b;
}

Image escala_de_cinza(Image img) {
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


Image blur(Image img) {
    int tamanho = 0;
    scanf("%d", &tamanho);
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
          Pixel media = {0, 0, 0};

          //declara variáveis para realização de comparação com funções max e min
          int numero_a_menor_height = img.height - 1;
          int numero_b_menor_height = i + tamanho/2;
          int numero_a_min_width = img.width - 1;
          int numero_b_min_width = j + tamanho/2;
          int numero_a_iterator_x = 0;
          int numero_b_iterator_x = i - tamanho/2;
          int numero_a_iterator_y = 0;
          int numero_b_iterator_y = j - tamanho/2;


          int menor_height = min_numero(numero_a_menor_height, numero_b_menor_height);
          int min_width = min_numero(numero_a_min_width, numero_b_min_width);
          for(int iterator_x = max_numero(numero_a_iterator_x, numero_b_iterator_x); iterator_x <= menor_height; ++iterator_x) {
              for(int iterator_y = max_numero(numero_a_iterator_y, numero_b_iterator_y); iterator_y <= min_width; ++iterator_y) {
                  media.r += img.pixel[iterator_x][iterator_y].r;
                  media.g += img.pixel[iterator_x][iterator_y].g;
                  media.b += img.pixel[iterator_x][iterator_y].b;
              }
          }

          media.r /= tamanho * tamanho;
          media.g /= tamanho * tamanho;
          media.b /= tamanho * tamanho;

          img.pixel[i][j].r = media.r;
          img.pixel[i][j].g = media.g;
          img.pixel[i][j].b = media.b;
        }
    }
    return img;
}

Image rotacionar90direita(Image img) {
    Image rotacionada;

    rotacionada.width = img.height;
    rotacionada.height = img.width;

    for (unsigned int i = 0, iterator_y = 0; i < rotacionada.height; ++i, ++iterator_y) {
        for (int j = rotacionada.width - 1, iterator_x = 0; j >= 0; --j, ++iterator_x) {
            rotacionada.pixel[i][j].r = img.pixel[iterator_x][iterator_y].r;
            rotacionada.pixel[i][j].g = img.pixel[iterator_x][iterator_y].g;
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

Image cortar_imagem(Image img) {
    int iterator_x, iterator_y;
    scanf("%d %d", &iterator_x, &iterator_y);
    int width, height;
    scanf("%d %d", &width, &height);
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


Image sepia(Image img){
  for (unsigned int iterator_x = 0; iterator_x < img.height; ++iterator_x) {
      for (unsigned int j = 0; j < img.width; ++j) {
          unsigned short int pixel[3];
          pixel[0] = img.pixel[iterator_x][j].r;
          pixel[1] = img.pixel[iterator_x][j].g;
          pixel[2] = img.pixel[iterator_x][j].b;

          int p =  pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
          int menor_r = min_numero(255, p);
          img.pixel[iterator_x][j].r = menor_r;

          p =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
          menor_r = min_numero(255, p);
          img.pixel[iterator_x][j].g = menor_r;


          p =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
          menor_r = min_numero(255, p);
          img.pixel[iterator_x][j].b = menor_r;
      }
  }
  return img;
}

Image rotaciona_90(Image img){
  int quantas_vezes = 0;
  scanf("%d", &quantas_vezes);
  quantas_vezes %= 4;
  for (int j = 0; j < quantas_vezes; ++j) {
    img = rotacionar90direita(img);
  }
  return img;
}

Image espelhamento(Image img){
  int horizontal = 0;
  scanf("%d", &horizontal);

  int width = img.width, height = img.height;

  if (horizontal == 1) width /= 2;
  else height /= 2;

  for (int iterator_2 = 0; iterator_2 < height; ++iterator_2) {
      for (int j = 0; j < width; ++j) {
          int iterator_x = iterator_2, iterator_y = j;

          if (horizontal == 1) iterator_y = img.width - 1 - j;
          else iterator_x = img.height - 1 - iterator_2;

          Pixel aux1;
          aux1.r = img.pixel[iterator_2][j].r;
          aux1.g = img.pixel[iterator_2][j].g;
          aux1.b = img.pixel[iterator_2][j].b;

          img.pixel[iterator_2][j].r = img.pixel[iterator_x][iterator_y].r;
          img.pixel[iterator_2][j].g = img.pixel[iterator_x][iterator_y].g;
          img.pixel[iterator_2][j].b = img.pixel[iterator_x][iterator_y].b;

          img.pixel[iterator_x][iterator_y].r = aux1.r;
          img.pixel[iterator_x][iterator_y].g = aux1.g;
          img.pixel[iterator_x][iterator_y].b = aux1.b;
      }
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

Image read_pixels(Image img){
  for (unsigned int i = 0; i < img.height; ++i) {
      for (unsigned int j = 0; j < img.width; ++j) {
          scanf("%hu %hu %hu", &img.pixel[i][j].r,
                               &img.pixel[i][j].g,
                               &img.pixel[i][j].b);

      }
  }
  return img;
}

int main() {
    Image img;

    // read type of image
    char tipo_imagem[4];
    scanf("%s", tipo_imagem);

    // read width height and color of image
    int max_color;
    scanf("%u %u %d", &img.width, &img.height, &max_color);

    // read all pixels of image
    img = read_pixels(img);

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
                img = blur(img);
                break;
            }
            case 4: { // Rotacao
                img = rotaciona_90(img);
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
                img = cortar_imagem(img);
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
