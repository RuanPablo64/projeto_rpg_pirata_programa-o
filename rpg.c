#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


typedef enum {
    medalha_velocista = 1,
    medalha_amaldicoado,
    medalha_escudo_blindado,
    medalha_vida,
    medalha_guerreiro,
    medalha_homens_peixes,
    medalha_almirante,
    medalha_fome,
    medalha_cracas,
    medalha_lendas
} medalhas;

typedef enum {
   carne_lula = 1,
   essencia_amaldicoada,
   elixir_vida,
   furia_engarrafada,
   rum
} itens;
   

typedef enum {
   grito_pirata = 1,
   barreira_amaldicoada,
   comida_bolso,
   golpe_pesado,
   insulto,
   oracao,
   maldicao_pirata,
   golpe_lendario,
   corpus_maledictum,
} total_habilidades;

typedef enum {
   marujo = 1,
   xama,
   artilheiro,
} classes_personagens;

typedef enum {
   mar_aberto = 1,
   mar_agitado,
   mar_amaldicoado,
} fases;


typedef enum {
    casa_inicial = 0,
    mar_vazio = 1,
    combate = 2,
    tesouro = 3,
    taverna = 4,
    evento = 5,
    chefe_final = 6,
} tabuleiro_casas;

typedef enum {
   atacar = 1,
   usar_item,
   habilidades,
   fugir,
} acao_combate;

typedef struct {
   char nome[100];
   classes_personagens classe;
   int vida, vida_maxima;
   int forca, defesa, agilidade, maldicao, maldicao_maxima, pontos_medalha;
   total_habilidades lista_habilidades[4];
   medalhas medalhas_inventario[20];
   medalhas medalhas_utilizaveis[10];
   itens inventario[5];
   int xp, nivel;
   int medalha_almirante_ativada;
   int medalha_fome_ativada;
   int medalha_cracas_ativada;
} personagem;

typedef struct {
   char nome[100];
   char descricao[100];
   int vida, ataque, defesa, nivel;
   int xp_recompensa;
   int medalha_recompensa;
} inimigo;

typedef enum {
   molenga = 1,
   osso_duro,
   abissal,
   lenda,
} nivel_inimigo;

void enter() {
    int enter;
    while ((enter = getchar()) != '\n' && enter != EOF);
    printf("\n[Pressione ENTER para continuar...]");
    getchar();
}


void recompensa_aleatoria (personagem *p) {
   printf("você recebeu uma recompensa aleatoria!\n");
   printf("\n");
   int upgrade_aleatorio = (rand() % 6) + 1;
   switch (upgrade_aleatorio) {
      case 1:
         p->vida_maxima += 1;
         printf("você recebeu um ponto de vida maxima!\n");
         break;
      case 2:
         p->forca += 1;
         printf("você recebeu um ponto de força!\n");
         break;
      case 3:
         p->defesa += 1;
         printf("você recebeu um ponto de defesa!\n");
         break;
      case 4:
         p->agilidade += 1;
         printf("você recebeu um ponto de agilidade\n");
         break;
      case 5:
         p->maldicao_maxima += 1;
         printf("você recebeu um ponto de maldição maxima!\n");
         break;
      case 6:
         p->pontos_medalha += 1;
         printf("você recebeu 1 ponto de medalha a mais!\n");
         break;
      default:
         printf("\n");
   }
   printf("\n");
   enter();
}

void subir_nivel (personagem *p) {
   printf("Parabens! você subiu de Nivel!\n");
   p->vida_maxima += 1;
   p->forca += 1;
   p->defesa += 1;
   p->agilidade += 1;
   p->maldicao_maxima += 1;
   p->pontos_medalha += 1;
   p->nivel += 1;
   p->xp = 0;
   printf("\n");
   enter();
}

int inventario_cheio(personagem *p) {
   for (int i = 0; i < 5; i++) {
      if (p->inventario[i] == 0) {
         return 0;
      }
   }
   return 1;
}

int medalha_duplicada(personagem *p, int id_medalha) {
   for (int i = 0; i < 20; i++) {
      if (p->medalhas_inventario[i] == id_medalha) {
         return 1;
      }
   }
   return 0;
}

void adicionar_habilidade(personagem *p, int nova_habilidade) {
   int slot_livre = -1;
   for (int i = 0; i < 4; i++) {
      if (p->lista_habilidades[i] == 0) {
         slot_livre = i;
         break;
      }
   }
   if (slot_livre != -1) {
      p->lista_habilidades[slot_livre] = nova_habilidade;
      printf("Nova habilidade adquirida com sucesso!\n");
      return;
   }
   for (int i = 0; i < 4; i++) {
      printf(" [ %d ] Habilidade ID atual: %d\n", i + 1, p->lista_habilidades[i]);
   }
   printf(" Escolha qual slot deseja substituir (1-4) ou 0 para cancelar: ");
   int escolha_substituir;
   scanf("%d", &escolha_substituir);
   if (escolha_substituir == 0) {
      printf("Nova habilidade descartada! mantem-se as atuais!\n");
      return;
   }
   if (escolha_substituir < 1 || escolha_substituir > 4) {
      printf("opção invalida! nova habilidade foi descartada!\n");
      return;
   }
   int indice_substituir = escolha_substituir - 1;
   int velha = p->lista_habilidades[indice_substituir]; 
   p->lista_habilidades[indice_substituir] = nova_habilidade;
   printf("nova habilidade adicionada e antiga descartada!\n");
}



void casa_tesouro(personagem *p) {
   printf("\n==================================================\n");
   printf("          ACHADO BOM! UMA CASA DE TESOURO!        \n");
   printf("==================================================\n");
   printf(" O convés range enquanto você abre um baú empoeirado...\n");
   printf(" Vejamos o que o destino guardou para o capitão!\n");
   printf("--------------------------------------------------\n");
   enter();
   int tipo_tesouro = (rand() % 10) + 1;
   switch (tipo_tesouro) {
      case 1:
      case 2:
         printf("Dentro do Bau, havia uma carne de lula fresquinha! mas ainda nojenta..\n");
         if (inventario_cheio(p) == 0) {
            for (int i = 0; i < 5; i++) {
               if (p->inventario[i] == 0) {
                  p->inventario[i] = carne_lula;
                  break;
               }
            }
            printf("carne de lula foi adicionada ao seu inventario!\n");
            break;
         }
         else {
            printf("mas infelizmente, seu inventario está cheio demais para comportar esse item!\n");
            break;
         }
      case 3:
      case 4:
         printf("Dentro do baú, você encontrou uma essência amaldiçoada brilhando com uma luz sombria!\n");
         if (inventario_cheio(p) == 0) {
            for (int i = 0; i < 5; i++) {
               if (p->inventario[i] == 0) {
                  p->inventario[i] = essencia_amaldicoada;
                  break;
               }
            }
            printf("Essência Amaldiçoada foi adicionada ao seu inventário!\n");
            break;
         }
         else {
            printf("Mas infelizmente, seu inventário está cheio para carregar esse item!\n");
            break;
         }
      case 5:
         printf("Dentro do bau, você encontrou a medalha do velocista!\n");
         if (medalha_duplicada(p, medalha_velocista)) {
            printf("Mas você já possui esta medalha guardada no seu inventario!\n");
         } 
         else {
            int slot_bau = -1;
            for (int i = 0; i < 20; i++) {
               if (p->medalhas_inventario[i] == 0) {
                  slot_bau = i;
                  break;
               }
            }
            if (slot_bau != -1) {
               p->medalhas_inventario[slot_bau] = medalha_velocista;
               printf("medalha do velocista foi guardada no seu inventario!\n");
            } 
            else {
               printf("Seu baú de medalhas está totalmente lotado!\n");
            }
         }
         break;
      case 6:
         printf("Dentro do baú, você encontrou uma furia engarrafada brilhando em vermelho vivo!\n");
         if (inventario_cheio(p) == 0) {
            for (int i = 0; i < 5; i++) {
               if (p->inventario[i] == 0) {
                  p->inventario[i] = furia_engarrafada;
                  break;
               }
            }
            printf("Fúria Engarrafada adicionada ao inventário!\n");
         } 
         else {
            printf("Mas infelizmente, seu inventário está cheio para carregar esse item!\n");
         }
         break;
      case 7:
         printf("Dentro do baú, você encontrou a medalha do escudo blindado! com todo aquele peso que incomoda..\n");
         if (medalha_duplicada(p, medalha_escudo_blindado)) {
            printf("Mas você já possui esta medalha guardada no seu inventario.\n");
         } 
         else {
            int slot_bau = -1;
            for (int i = 0; i < 20; i++) {
               if (p->medalhas_inventario[i] == 0) {
                  slot_bau = i;
                  break;
               }
            }
            if (slot_bau != -1) {
               p->medalhas_inventario[slot_bau] = medalha_escudo_blindado;
               printf("medalha do escudo blindado foi guardada no seu inventario!\n");
            } 
            else {
               printf("Seu baú de medalhas está lotado!\n");
            }
         }
         break;
      case 8:
      case 9:
         printf("dentro do baú, você encontrou uma garrafa de rum velha, mas ainda bebivel!\n");
         if (inventario_cheio(p) == 0) {
            for (int i = 0; i < 5; i++) {
               if (p->inventario[i] == 0) {
                  p->inventario[i] = rum;
                  break;
               }
            }
            printf("Rum adicionado ao seu inventário!\n");
         } 
         else {
            printf("Mas infelizmente, seu inventário está cheio para carregar esse item!\n");
         }
         break;
      case 10:
         printf("dentro do bau, você encontrou a medalha do guerreiro! realmente uma medalha inspiradora!\n");
         if (medalha_duplicada(p, medalha_guerreiro)) {
            printf("Mas você já possui esta medalha guardada no seu baú.\n");
         } 
         else {
            int slot_bau = -1;
            for (int i = 0; i < 20; i++) {
               if (p->medalhas_inventario[i] == 0) {
                  slot_bau = i;
                  break;
               }
            }
            if (slot_bau != -1) {
               p->medalhas_inventario[slot_bau] = medalha_guerreiro;
               printf("medalha do guerreiro foi guardada no seu inventario!\n");
            } 
            else {
               printf("Seu baú de medalhas está lotado!\n");
            }
         }
         break;
      default:
         printf("O baú estava misteriosamente vazio... Que azar, capitão!\n");
         break;
   }
}
void casa_evento(personagem *p) {   
   printf("\n==================================================\n");
   printf("          ÁGUAS MISTERIOSAS: CASA DE EVENTO!      \n");
   printf("==================================================\n");
   printf(" O vento cessa de repente e uma névoa densa cobre o mar.\n");
   printf(" Algo inesperado está prestes a acontecer no horizonte!\n");
   printf("--------------------------------------------------\n");
   enter();
   int tipo_evento = (rand() % 10) + 1;
   int dado_evento = (rand() % 20) + 1;
   int pontuacao_total = 0;

   switch (tipo_evento) {
      case 1:
         printf("\nVocê atracou em uma ilha onde uma estátua colossal de pedra bloqueia a passagem!\n");
         printf("Você precisa usar sua força bruta para empurrar o monólito.\n");
         pontuacao_total = dado_evento + p->forca;
         printf("Rolagem do d20: %d + Sua Força (%d) = Pontuação Total: %d (Necessário: 15)\n", dado_evento, p->forca, pontuacao_total);
         if (pontuacao_total >= 15) {
            printf("Você empurrou a pedra com um berro feroz e encontrou os segredos de um batedor!\n");
            p->forca += 2;
            printf("Você ganhou +2 pontos de Força!\n");
            adicionar_habilidade(p, golpe_pesado);
         } else {
            printf("\nSeus músculos falharam e a pedra nem se moveu. Você perdeu a oportunidade... e se sente um fracassado\n");
         }
         break;

      case 2:
         printf("\nVocê é cercado por marinheiros rivais em uma taverna portuária!\n");
         printf("Você precisa usar sua lábia e agilidade para provocar e desviar dos copos voadores.\n");
         pontuacao_total = dado_evento + p->agilidade;
         printf("Rolagem do d20: %d + Sua Agilidade (%d) = Pontuação Total: %d (Necessário: 15)\n", dado_evento, p->agilidade, pontuacao_total);
         if (pontuacao_total >= 15) {
            printf("Seus insultos irritaram tanto os rivais que eles brigaram entre si!\n");
            p->vida_maxima += 2;
            p->vida += 2;
            printf("Você ganhou +2 pontos de Vida Máxima!\n");
            adicionar_habilidade(p, insulto);
         } else {
            printf("\nVocê tropeçou nas próprias palavras e saiu correndo de mãos abrevias.\n");
         }
         break;
      case 3: 
         printf("\nUma forte tempestade de correntes marítimas puxa seu navio contra os recifes.\n");
         printf("Você precisa resistir firme no timão para proteger o casco da embarcação.\n");
         pontuacao_total = dado_evento + p->defesa;
         printf("Rolagem do d20: %d + Sua Defesa (%d) = Pontuação Total: %d (Necessário: 15)\n", dado_evento, p->defesa, pontuacao_total);
         if (pontuacao_total >= 15) {
            printf("\nO navio resistiu bravamente! Nas águas calmas após o temporal, você recolhe uma relíquia.\n");
            p->vida_maxima += 1;
            p->vida += 1;
            printf("Você ganhou +1 ponto de Vida Máxima!\n");
            if (!medalha_duplicada(p, medalha_vida)) {
               int slot = -1;
               for (int i = 0; i < 20; i++) if (p->medalhas_inventario[i] == 0) { slot = i; break; }
               if (slot != -1) { p->medalhas_inventario[slot] = medalha_vida; printf("Medalha da Vida guardada no baú!\n"); }
            } else { printf("Você já possui a Medalha da Vida!\n"); }
         } else {
            printf("\n O casco sofreu danos severos na batida contra os recifes...\n");
         }
         break;
      case 4:
         printf("\nUm altar sombrio emerge das profundezas do oceano, pulsando energia maligna.\n");
         printf("Você precisa canalizar a maldição interna para dominar a entidade do altar.\n");
         pontuacao_total = dado_evento + p->maldicao;
         printf("Rolagem do d20: %d + Sua Maldição (%d) = Pontuação Total: %d (Necessário: 15)\n", dado_evento, p->maldicao, pontuacao_total);  
         if (pontuacao_total >= 15) {
            printf("O altar reconhece sua alma sombria e consagra você com poderes proibidos!\n");
            p->maldicao_maxima += 5;
            p->pontos_medalha += 10;
            printf("Você ganhou +5 de Maldição Máxima e +10 Pontos de Medalha!\n");
            
            if (!medalha_duplicada(p, medalha_amaldicoado)) {
               int slot = -1;
               for (int i = 0; i < 20; i++) if (p->medalhas_inventario[i] == 0) { slot = i; break; }
               if (slot != -1) { p->medalhas_inventario[slot] = medalha_amaldicoado; printf("Medalha do Amaldiçoado guardada no baú!\n"); }
            } else { printf("Você já possui a Medalha do Amaldiçoado!\n"); }
         } else {
            printf("\nA energia sombria te rejeita, drenando suas forças momentaneamente.\n");
         }
         break;

      case 5: 
         printf("\nEm um santuário abandonado em alto-mar, sussurros divinos testam sua fé.\n");
         printf("Você precisa focar sua essência amaldiçoada para purificar o ambiente.\n");
         pontuacao_total = dado_evento + p->maldicao;
         printf("Rolagem do d20: %d + Sua Maldição (%d) = Pontuação Total: %d (Necessário: 15)\n", dado_evento, p->maldicao, pontuacao_total);
         if (pontuacao_total >= 15) {
            printf("\n[SUCESSO!] Uma luz sagrada abençoa sua armadura e concede uma prece milagrosa!\n");
            p->defesa += 1;
            printf("Você ganhou +1 ponto de Defesa!\n");
            adicionar_habilidade(p, oracao);
         } else {
            printf("\nOs sussurros viram gritos ensurdecedores. Nada foi alcançado.\n");
         }
         break;

      case 6:
         printf("\nUm capitão fantasma desafia você para um duelo de braço de ferro nos mastros.\n");
         printf("Você usa sua força para impor respeito à alma penada.\n");
         pontuacao_total = dado_evento + p->forca;
         printf("Rolagem do d20: %d + Sua Força (%d) = Pontuação Total: %d (Necessário: 15)\n", dado_evento, p->forca, pontuacao_total);
         if (pontuacao_total >= 15) {
            printf("\nVocê esmaga a mão do fantasma na mesa, ganhando seu respeito e segredos sombrios!\n");
            p->maldicao += 5;
            if (p->maldicao > p->maldicao_maxima) p->maldicao = p->maldicao_maxima;
            p->pontos_medalha += 10;
            printf("Você ganhou +5 pontos de Maldição e +10 Pontos de Medalha!\n");
            adicionar_habilidade(p, maldicao_pirata);
         } else {
            printf("\nO capitão fantasma é mais forte do que aparentava e ri da sua fraqueza.\n");
         }
         break;

      case 7:
         printf("\nUm monstro marinho salta sobre o convés! Você precisa de reflexos rápidos para um bote fatal.\n");
         pontuacao_total = dado_evento + p->agilidade;
         printf("Rolagem do d20: %d + Sua Agilidade (%d) = Pontuação Total: %d (Necessário: 15)\n", dado_evento, p->agilidade, pontuacao_total);  
         if (pontuacao_total >= 15) {
            printf("\nCom um salto acrobático impecável, você desfere um golpe ensinado pelas lendas do mar!\n");
            p->forca += 2;
            printf("Você ganhou +2 pontos de Força!\n");
            adicionar_habilidade(p, golpe_lendario);
         } else {
            printf("\n O monstro escapa antes que você pudesse acertar o golpe decisivo.\n");
         }
         break;

      case 8:
         printf("\nO núcleo da maldição do oceano absorve seu corpo por completo em uma visão tridimensional.\n");
         printf("Você precisa canalizar sua maldição para dominar o ritual supremo.\n");
         pontuacao_total = dado_evento + p->maldicao;
         printf("Rolagem do d20: %d + Sua Maldição (%d) = Pontuação Total: %d (Necessário: 15)\n", dado_evento, p->maldicao, pontuacao_total);
         if (pontuacao_total >= 15) {
            printf("\nVocê domina o ritual definitivo e absorve a essência pura do terror marinho!\n");
            p->vida_maxima += 2;
            p->vida += 2;
            p->maldicao_maxima += 1;
            printf("Você ganhou +2 de Vida Máxima e +1 de Maldição Máxima!\n");
            adicionar_habilidade(p, corpus_maledictum);
         } else {
            printf("\n A maldição é forte demais e quase consome sua sanidade.\n");
         }
         break;

      case 9: 
         printf("\nUma armaria secreta naufragada é descoberta no fundo de uma gruta marinha.\n");
         printf("Você precisa testar e ajustar os ferrolhos e escudos antigos.\n");
         pontuacao_total = dado_evento + p->defesa;
         printf("Rolagem do d20: %d + Sua Defesa (%d) = Pontuação Total: %d (Necessário: 15)\n", dado_evento, p->defesa, pontuacao_total);
         
         if (pontuacao_total >= 15) {
            printf("\nVocê reforça sua blindagem pessoal com peças de aço e ligas navais lendárias!\n");
            p->forca += 1;
            p->defesa += 1;
            printf("Você ganhou +1 de Força e +1 de Defesa!\n");
         } else {
            printf("\nAs engrenagens estavam enferrujadas e nada pôde ser aproveitado.\n");
         }
         break;

      case 10:
         printf("Um contrabandista misterioso oferece relíquias raras em troca de uma corrida de fuga contra a marinha.\n");
         printf("Você precisa pilotar o navio com agilidade máxima pelos canais estreitos.\n");
         pontuacao_total = dado_evento + p->agilidade;
         printf("Rolagem do d20: %d + Sua Agilidade (%d) = Pontuação Total: %d (Necessário: 15)\n", dado_evento, p->agilidade, pontuacao_total);
         
         if (pontuacao_total >= 15) {
            printf("\nFuga perfeita! O contrabandista entrega os espólias prometidos com um sorriso.\n");
            p->pontos_medalha += 10;
            p->maldicao += 10;
            if (p->maldicao > p->maldicao_maxima) p->maldicao = p->maldicao_maxima;
            printf("Você ganhou +10 Pontos de Medalha e +10 pontos de Maldição!\n");
         } else {
            printf("\nA patrulha naval interceptou parte da carga. Você saiu de mãos vazias.\n");
         }
         break;
   }
   enter();
}         
         

  
void casa_taverna (personagem *p) {
   printf("\n========================================================================================\n");
   printf(" Você atraca em uma acolhedora taverna de uma ilha costeira! O cheiro de Rum e peixe assado \n");
   printf(" preenche o ar. Você descansa, bebe com os locais e lava a alma por um instante...\n");
   printf(" [!] Seus pontos de vida foram totalmente restaurados e sua maldição restaurada!\n");
   printf("========================================================================================\n");
   printf("\n");
   p->vida = p->vida_maxima;
   p->maldicao = p->maldicao_maxima;
   enter();
}

void casa_mar_vazio() {
   printf("\n");
   printf("\n==================================================\n");
   printf(" Literalmente um mar vazio... onde não acontece \n");
   printf(" nada... podemos dizer que é um momento onde \n");
   printf(" podes respirar por um tempo e pensar nas suas \n");
   printf(" decisões de vida... bem sem graça!\n");
   printf("==================================================\n");
   enter();
}


void acao_usar_item(personagem *p) {
   int escolha_item;
   int inventario_vazio = 1;
   for (int i = 0; i < 5; i++) {
       if (p->inventario[i] != 0) {
           inventario_vazio = 0;
           break;
       }
    }
    if (inventario_vazio) {
        printf("seu inventario está vazio! sem itens para se usar aqui...\n");
        return;
    }
    printf(" Tabela do ID de Itens \n");
    printf(" 1 - Carne de Lula\n");
    printf(" 2 - Essencia Amaldiçoada\n");
    printf(" 3 - Elixir da Vida\n");
    printf(" 4 - Furia Engarrafada\n");
    printf(" 5 - Rum\n");
    for (int i = 0; i < 5; i++) {
        if (p->inventario[i] != 0) {
            printf(" %d Item ID: %d\n", i + 1, p->inventario[i]);
        }
    }
    printf("Em caso de não querer usar nenhum item, escolha a opção 0\n");
    printf("Escolha o número do item que deseja usar (1-5): ");
    scanf("%d", &escolha_item);

    if (escolha_item == 0) {
        printf("você não usou nenhum item\n");
        return;
    }

    if (escolha_item < 1 || escolha_item > 5 || p->inventario[escolha_item - 1] == 0) {
        printf("opção invalida!\n");
        return;
    }

    int item_escolhido = p->inventario[escolha_item - 1];

    switch (item_escolhido) {
        case carne_lula:
            if (p->medalha_fome_ativada == 1) {
               printf("Você comeu a carne de lula... minimamente duvidosa\n");
               printf("recuperou 10 pontos de vida!\n");
               p->vida += 15;
               if (p->vida > p->vida_maxima) {
                  p->vida = p->vida_maxima;
                  break;
               }
               break;
            }
            else {
               printf("Você comeu a carne de lula... minimamente duvidosa\n");
               printf("recuperou 10 pontos de vida!\n");
               p->vida += 10;
               if (p->vida > p->vida_maxima) {
                  p->vida = p->vida_maxima;
                  break;
               }
               break;
            }
        case essencia_amaldicoada:
            if (p->medalha_fome_ativada == 1) {
               printf("você tomou a essencia amaldiçoada! o gosto é... doce?\n");
               printf("recuperou 10 pontos de maldição!\n");
               p->maldicao += 15;
               if (p->maldicao > p->maldicao_maxima) {
                  p->maldicao = p->maldicao_maxima;
                  break;
               }
               break;
            }
            else {
               printf("você tomou a essencia amaldiçoada! o gosto é... doce?\n");
               printf("recuperou 10 pontos de maldição!\n");
               p->maldicao += 10;
               if (p->maldicao > p->maldicao_maxima) {
                  p->maldicao = p->maldicao_maxima;
                  break;
               }
               break;
            }
        case elixir_vida:
            if (p->medalha_fome_ativada == 1) {
               printf("você tomou o elixir da vida!.. mas não é tão prazeroso assim\n");
               p->vida += 25;
               if (p->vida > p->vida_maxima) {
                  p->vida = p->vida_maxima;
                  break;
               }
               break;
            }
            else {
               printf("você tomou o elixir da vida!.. mas não é tão prazeroso assim\n");
               p->vida += 20;
               if (p->vida > p->vida_maxima) {
                  p->vida = p->vida_maxima;
                  break;
               }
               break;
            }
        case furia_engarrafada:
            if (p->medalha_fome_ativada == 1) {
               printf("Você bebeu uma furia engarrafada! você sente um odio em seu coração (se é que já não existia um)\n");
               printf("Seus pontos de força aumentaram em 2 pontos durante esse combate!");
               p->forca += 4;
               break;
            }
            else {
               printf("Você bebeu uma furia engarrafada! você sente um odio em seu coração (se é que já não existia um)\n");
               printf("Seus pontos de força aumentaram em 2 pontos durante esse combate!");
               p->forca += 2;
               break;
            }
        case rum:
            if (p->medalha_fome_ativada == 1) {
               printf("Você bebeu Rum! a bebida favorita de qualquer pirata!\n");
               p->vida += 25;
               p->maldicao += 15;
               if (p->vida > p->vida_maxima) {
                  p->vida = p->vida_maxima;
               }
               if (p->maldicao > p->maldicao_maxima) {
                  p->vida = p->maldicao_maxima;
                  break;
               }
               break;
            }
            else {
               printf("Você bebeu Rum! a bebida favorita de qualquer pirata!\n");
               p->vida += 25;
               p->maldicao += 15;
               if (p->vida > p->vida_maxima) {
                  p->vida = p->vida_maxima;
               }
               if (p->maldicao > p->maldicao_maxima) {
                  p->vida = p->maldicao_maxima;
                  break;
               }
               break;
            }
        default:
            printf("opção invalida!\n");
            return;
    }
    p->inventario[escolha_item - 1] = 0; 
    enter();
}
      

int fuga (int casa_chefe_final) {
   int chance_fuga = (rand() % 2);
   if (casa_chefe_final == 1) {
      printf("você não pode fugir dessa luta! agora, deixe de ser covarde e volte!\n");
      return 0;
   }
   else {
      if (chance_fuga == 0) {
         printf("você tentou fugir... mas escorregou. Continue a lutar, seu imbecil!\n");
         return 0;
      }
      else {
         printf("você tentou fugir... e por muita sorte conseguiu!\n");
         return 1;
      }
   }        
}


void equipar_medalha(personagem *p) {
   int tem_medalhas = 0;
   for (int i = 0; i < 20; i++) {
      if (p->medalhas_inventario[i] != 0) {
         tem_medalhas = 1; 
         break; 
      }
   }
   if (tem_medalhas == 0) {
      printf("Você não tem nenhuma medalha em seu inventário!\n");
      return;
   }
   for (int i = 0; i < 20; i++) {
      if (p->medalhas_inventario[i] != 0) {
         printf(" [ %d ] Medalha ID: %d\n", i + 1, p->medalhas_inventario[i]);
      }
   }
   printf("Em caso de não querer equipar nenhuma medalha, escolha a opção 0\n");
   printf("Escolha o número do slot da medalha que deseja equipar (1- 20): ");
   int escolha_medalha;
   scanf("%d", &escolha_medalha);
   if (escolha_medalha == 0) {
      printf("você decidiu não usar nennuma medalha\n");
      return;
   }
   if (escolha_medalha < 1 || escolha_medalha > 20 || p->medalhas_inventario[escolha_medalha - 1] == 0) {
      printf("Escolha inválida!\n");
      return;
   }
   int id_medalha_escolhida = p->medalhas_inventario[escolha_medalha - 1];
   int custo = 0;
   switch (id_medalha_escolhida) {
      case medalha_velocista:       custo = 7;  break;
      case medalha_amaldicoado:     custo = 7;  break;
      case medalha_escudo_blindado: custo = 7;  break;
      case medalha_vida:            custo = 10; break;
      case medalha_guerreiro:       custo = 10; break;
      case medalha_homens_peixes:   custo = 12; break;
      case medalha_almirante:       custo = 15; break;
      case medalha_fome:            custo = 15; break;
      case medalha_cracas:          custo = 15; break;
      case medalha_lendas:          custo = 20; break;
   } 
   if (custo > p->pontos_medalha) {
      printf("Pontos de medalha insuficientes!\n");
      return;
   }
   int slot_ativo_livre = -1;
   for (int i = 0; i < 10; i++) {
      if (p->medalhas_utilizaveis[i] == 0) {
        slot_ativo_livre = i;
         break;
      }
   }
   p->medalhas_utilizaveis[slot_ativo_livre] = id_medalha_escolhida;
   p->medalhas_inventario[escolha_medalha - 1] = 0;
   p->pontos_medalha -= custo;
   switch (id_medalha_escolhida) {
      case medalha_velocista:
         p->agilidade += 5;
         printf("Você equipou a medalha do velocista! mais 5 pontos de agilidade!\n");
         break;
      case medalha_amaldicoado:
         p->maldicao += 5;
         printf("Você equipou a medalha do amaldiçoado! mais 5 pontos de maldição!\n");
         break;
      case medalha_escudo_blindado:
         p->defesa += 1;
         printf("Você equipou a medalha do escudo blindade! mais 1 ponto de defesa!\n");
         break;
      case medalha_vida:
         p->vida_maxima += 5;
         printf("você equipou a medalha da vida! mais 5 pontos de vida maxima!\n");
         break;
      case medalha_guerreiro:
         p->forca += 2;
         printf("você equipou a medalha do guerreiro! mais 2 pontos de força!\n");
         break;
      case medalha_homens_peixes:
         p->forca += 2;
         p->defesa += 2;
         printf("você equipou a medalha dos homens peixes! +2 pontos de força e defesa!\n");
         break;
      case medalha_almirante:
         p->medalha_almirante_ativada = 1;
         printf("você equipou a medalha do almirante! todas as habilidades estão com metade do custo de maldição!\n");
         break;
      case medalha_fome:
         p->medalha_fome_ativada = 1;
         printf("você equipou a medalha da fome! todos os itens receberam um bonus ao usa-los!\n");
         break;
      case medalha_cracas:
         p->medalha_cracas_ativada = 1;
         printf("você equipou a medalha das cracas! o inimigo recebe 1 ponto de dano ao lhe atacar!\n");
         break;
      case medalha_lendas:
         p->forca += 2;
         p->defesa += 2;
         p->agilidade += 5;
         p->maldicao += 5;
         printf("Você equipou a medalha das lendas! +2 de força, +2 de defesa, +5 de agilidade, +5 de maldição!\n");
         break;
   }
   enter();
}

void desequipar_medalha(personagem *p) {
   int tem_alguma = 0;
   for (int i = 0; i < 10; i++) {
      if (p->medalhas_utilizaveis[i] != 0) {
         printf(" [ %d ] Medalha ID: %d\n", i + 1, p->medalhas_utilizaveis[i]);
         tem_alguma = 1;
      }
   }
   if (tem_alguma == 0) {
      printf("Você não possui nenhuma medalha equipada no momento.\n");
      return;
   }
    
    int escolha_slot;
    printf("Em caso de não querer desequipar nenhuma medalha, escolha a opção 0\n");
    printf("Escolha o número do slot da medalha que deseja desequipar (1- 20): ");
    scanf("%d", &escolha_slot);
    if (escolha_slot == 0) {
        return;
    }

    if (escolha_slot < 1 || escolha_slot > 10 || p->medalhas_utilizaveis[escolha_slot - 1] == 0) {
        printf("opção invalida!\n");
        return;
    }

    int indice_ativo = escolha_slot - 1;
    int id_medalha = p->medalhas_utilizaveis[indice_ativo];

    int slot_bau_livre = -1;
    for (int i = 0; i < 20; i++) {
        if (p->medalhas_inventario[i] == 0) {
            slot_bau_livre = i;
            break;
        }
    }

    if (slot_bau_livre == -1) {
        printf("você não possue mais espaço de medalhas sobrando!");
        return;
    }

    int custo_devolvido = 0;
    switch (id_medalha) {
        case medalha_velocista:
            p->agilidade -= 5;
            custo_devolvido = 7;
            printf("medalha do velocista desequipada. Agilidade -5\n");
            break;
        case medalha_amaldicoado:
            p->maldicao -= 5;
            if (p->maldicao < 0) {
               p->maldicao = 0;
            }
            custo_devolvido = 7;
            printf("medalha do Amaldiçoado desequipada. Maldição -5\n");
            break;
        case medalha_escudo_blindado:
            p->defesa -= 1;
            custo_devolvido = 7;
            printf("medalha do Escudo Blindado desequipada. Defesa -1.\n");
            break;
        case medalha_vida:
            p->vida_maxima -= 5;
            if (p->vida > p->vida_maxima) {
               p->vida = p->vida_maxima;
            }
            custo_devolvido = 10;
            printf("medalha da Vida desequipada. Vida Máxima -5\n");
            break;
        case medalha_guerreiro:
            p->forca -= 2;
            custo_devolvido = 10;
            printf("\n[-] Medalha do Guerreiro desequipada. Força -2.\n");
            break;
        case medalha_homens_peixes:
            p->forca -= 2;
            p->defesa -= 2;
            custo_devolvido = 12;
            printf("medalha dos Homens Peixes desequipada. Força e Defesa -3\n");
            break;
        case medalha_almirante:
            p->medalha_almirante_ativada = 0;
            custo_devolvido = 15;
            printf("medalha do Almirante desequipada. Custos de habilidade normalizados\n");
            break;
        case medalha_fome:
            p->medalha_fome_ativada = 0; 
            custo_devolvido = 15;
            printf("medalha da Fome desequipada. Efeitos dos itens normalizados.\n");
            break;
        case medalha_cracas:
            p->medalha_cracas_ativada = 0;
            custo_devolvido = 15;
            printf("medalha das Cracas desequipada. Dano de retaliação desativado\n");
            break;
        case medalha_lendas:
            p->forca -= 2;
            p->defesa -= 2;
            p->agilidade -= 5;
            p->maldicao -= 5;
            if (p->maldicao < 0) {
               p->maldicao = 0;
            }
            custo_devolvido = 20;
            printf("\n[-] Medalha das Lendas desequipada. Força e Defesa -2, Maldição e Agilidade -5.\n");
            break;
    }
    p->pontos_medalha += custo_devolvido;
    p->medalhas_inventario[slot_bau_livre] = id_medalha;
    p->medalhas_utilizaveis[indice_ativo] = 0;
}




void usar_habilidades (personagem *p, inimigo *i) {
   int escolha_habilidade = 0;
   printf("\n");
   printf(" Tabela de IDs de Habilidades \n");
   printf(" 0 - Sem Habilidade\n");
   printf(" 1 - Grito Pirata\n");
   printf(" 2 - Barreira Amaldiçoada\n");
   printf(" 3 - Comida de Bolso\n");
   printf(" 4 - Golpe Pesado\n");
   printf(" 5 - Insulto\n");
   printf(" 6 - Oração\n");
   printf(" 7 - Maldição Pirata\n");
   printf(" 8 - Golpe Lendário\n");
   printf(" 9 - Corpus Maledictum\n");
   printf("\n");
   for (int id = 0; id < 4; id++) {
      printf(" %d - habilidade ID: %d\n", id + 1, p->lista_habilidades[id]);
   }
   printf("\n");
   printf("Em caso de não querer usar nenhuma habilidade, escolha a opção 0\n");
   printf("Escolha o número da habilidade que deseja usar (1-4): ");
   scanf("%d", &escolha_habilidade);
   if (escolha_habilidade == 0) {
        printf("você decidiu utilizar nenhuma habilidade.\n");
        return;
   }
   if (escolha_habilidade < 1 || escolha_habilidade > 4) {
        printf("opção invalida! Perdeu o foco e o turno... (otario!)\n");
        return;
   }
   int hab_escolhida = p->lista_habilidades[escolha_habilidade - 1];
   switch (hab_escolhida) {
      case grito_pirata:
         if (p->medalha_almirante_ativada == 1) {
            if (p->maldicao < 2) {
               printf("Pontos de Maldição Insuficiente!\n");
               break;
            }
            else {
               p->maldicao -= 2;
               i->defesa -= 1;
               printf("Você usou Grito Pirata. A defesa do inimigo caiu em um ponto!\n");
               break;
            }
         }
         else {
            if (p->maldicao < 4) {
               printf("Pontos de Maldição Insuficiente!\n");
               break;
            }
            else {
               p->maldicao -= 4;
               i->defesa -= 1;
               printf("Você usou Grito Pirata. A defesa do inimigo caiu em um ponto!\n");
               break;
            }
         }
      case barreira_amaldicoada:
           if (p->medalha_almirante_ativada == 1) {
              if (p->maldicao < 2) {
                printf("Pontos de Maldição Insuficiente!\n");
                break;
              }
              else {
                 p->maldicao -= 2;
                 p->defesa += 1;
                 printf("Você usou Barreia Amaldiçoada! Sua defesa aumentou em 1 ponto!\n");
                 break;
              }
           }
           else {
              if (p->maldicao < 4) {
                 printf("Pontos de Maldição Insuficiente!\n");
                 break;
              }
              else {
                 p->maldicao -= 4;
                 p->defesa += 1;
                 printf("Você usou Barreia Amaldiçoada! Sua defesa aumentou em 1 ponto!\n");
                 break;
              }
           }
      case comida_bolso:
         if (p->medalha_almirante_ativada == 1) {
            if (p->maldicao < 1) {
               printf("Pontos de Maldição Insuficiente!\n");
               break;
            }
            else {
               p->maldicao -= 1;
               p->vida += 3; 
               if (p->vida > p->vida_maxima) {
                  p->vida = p->vida_maxima;
               }
               printf("Você usou Comida de Bolso! recuperou 3 pontos de vida!\n");
               break;
            }
         }
         else {
            if (p->maldicao < 3) {
               printf("Pontos de Maldição Insuficiente!\n");
               break;
            }
            else {
               p->maldicao -= 3;
               p->vida += 3; 
               if (p->vida > p->vida_maxima) {
                  p->vida = p->vida_maxima;
               }
               printf("Você usou Comida de Bolso! recuperou 3 pontos de vida!\n");
               break;
            }
         }
      case golpe_pesado:
         if (p->medalha_almirante_ativada == 1) {
            if (p->maldicao < 2) {
               printf("Pontos de Maldição Insuficiente!\n");
               break;
            }
            else {
               p->maldicao -= 2;
               int dano_pesado = (int)(p->forca * 1.5);
               dano_pesado = (dano_pesado - i->defesa);
               i->vida -= dano_pesado;
               printf("Você usou Golpe_pesado! causando %d pontos de dano!\n", dano_pesado);
               break;
            }
         }
         else {
            if (p->maldicao < 5) {
               printf("Pontos de Maldição Insuficiente!\n");
               break;
            }
            else {
               p->maldicao -= 5;
               int dano_pesado = (int)(p->forca * 1.5);
               dano_pesado = (dano_pesado - i->defesa);
               i->vida -= dano_pesado;
               printf("Você usou Golpe_pesado! causando %d pontos de dano!\n", dano_pesado);
               break;
            }
         }
      case insulto:
         if (p->medalha_almirante_ativada == 1) {
            if (p->maldicao < 1) {
               printf("Pontos de Maldição Insuficiente!\n");
               break;
            }
            else {
               p->maldicao -= 1;
               i->defesa -= 1;
               p->forca += 1;
               printf("\nVocê usa Insulto e xinga a mãe do inimigo! a defesa do oponente diminue 1 ponto, mas sua força aumenta também!\n");
               break;
            }
         }
         else {
            if (p->maldicao < 3) {
               printf("Pontos de Maldição Insuficiente!\n");
               break;
            }
            else {
               p->maldicao -= 3;
               i->defesa -= 1;
               p->forca += 1;
               printf("\nVocê usa Insulto e xinga a mãe do inimigo! a defesa do oponente diminue 1 ponto, mas sua força aumenta também!\n");
               break;
            }
         }
      case oracao:
         if (p->medalha_almirante_ativada == 1) {
            if (p->maldicao < 5) {
               printf("Pontos de Maldição Insuficiente!\n");
               break;
            }
            else {
               p->maldicao -= 5;
               int cura = p->vida_maxima / 2;
               p->vida += cura;
               printf("Você usa Oração! e poderes divinos devolvem metade de sua vida!\n");
               printf("você recuperou %d pontos de vida\n", cura);
               if (p->vida > p->vida_maxima) {
                  p->vida = p->vida_maxima;
                  break;
               }
               break;
            }
         }
         else {
            if (p->maldicao < 10) {
               printf("Pontos de Maldição Insuficiente!\n");
               break;
            }
            else {
               p->maldicao -= 10;
               int cura = p->vida_maxima / 2;
               p->vida += cura;
               printf("Você usa Oração! e poderes divinos devolvem metade de sua vida!\n");
               printf("você recuperou %d pontos de vida\n", cura);
               if (p->vida > p->vida_maxima) {
                  p->vida = p->vida_maxima;
                  break;
               }
               break;
            }
         }
      case maldicao_pirata:
         if (p->medalha_almirante_ativada == 1) {
            if (p->maldicao < 5) {
               printf("Pontos de Maldição Insuficiente!\n");
               break;
            }
            else {
               p->maldicao -= 5;
               p->defesa += 1;
               p->forca += 1;
               printf("Você usou a maldição pirata! e seus pontos de defesa e força aumentaram em 1 ponto!\n");
               break;
            }
         }
         else {
            if (p->maldicao < 10) {
               printf("Pontos de Maldição Insuficiente!\n");
               break;
            }
            else {
               p->maldicao -= 10;
               p->defesa += 1;
               p->forca += 1;
               printf("Você usou a maldição pirata! e seus pontos de defesa e força aumentaram em 1 ponto!\n");
               break;
            }
         }
      case golpe_lendario:
         if (p->medalha_almirante_ativada == 1) {
            if (p->maldicao < 7) {
              printf("Pontos de Maldição Insuficiente!\n");
              break;
            } 
            else {
               p->maldicao -= 7;
               int dano_lendario = p->forca * 2;
               dano_lendario = (dano_lendario - i->defesa);
               i->vida -= dano_lendario;
               printf("você usou Golpe Lendario! infligiu %d pontos de dano!\n", dano_lendario);
               break;
            }
         }
         else {
            if (p->maldicao < 15) {
               printf("Pontos de Maldição Insuficiente!\n");
               break;
            }
            else {
               p->maldicao -= 15;
               int dano_lendario = p->forca * 2;
               dano_lendario = (dano_lendario - i->defesa);
               i->vida -= dano_lendario;
               printf("você usou Golpe Lendario! infligiu %d pontos de dano!\n", dano_lendario);
               break;
            }
         }
      case corpus_maledictum:
         if (p->medalha_almirante_ativada == 1) {
            if (p->maldicao < 10) {
               printf("Pontos de Maldição Insuficiente!\n");
               break;
            }
            else {
               p->maldicao -= 10;
               p->vida = p->vida_maxima;
               printf("Você usou Corpus Maledictum! a maldição suprema que recupera toda a sua vida!\n");
               break;
            }
         }
         else {
            if (p->maldicao < 20) {
               printf("Pontos de Maldição Insuficiente!\n");
               break;
            }
            else {
               p->maldicao -= 20;
               p->vida = p->vida_maxima;
               printf("Você usou Corpus Maledictum! a maldição suprema que recupera toda a sua vida!\n");
               break;
            }
         }
      default:
         printf("Opção invalida! de brinde, você perdeu seu turno!\n");
   }
}


void ataque_jogador (personagem *p, inimigo *i) {
   int dano = (p->forca - i->defesa);
   if (p->forca <= i->defesa) {
      i->vida = i->vida - 1;
      printf("você deu 1 ponto de dano no seu inimigo!\n");
   }
   else {
      i->vida = i->vida - dano;
      printf("você deu %d pontos de dano no seu inimigo!\n", dano);
   }
}

void ataque_inimigo (personagem *p, inimigo *i) {
   int desviar = (rand() % 20) + 1;
   int dano = (i->ataque - (p->defesa)/2);
   if (p->agilidade > 10) {
      if (desviar >= 15) {
         printf("você desviou do golpe! tomou zero dano!\n");
         return;
      }
      else {
         p->vida = p->vida - dano;
         printf("o inimigo lhe deu %d de dano!\n", dano);
      }
   }
   else if (p->agilidade > 10) {
      if (desviar >= 15) {
         printf("você desviou do golpe! tomou zero dano!\n");
         return;
      }
      else {
         p->vida = p->vida - dano;
         printf("o inimigo lhe deu %d de dano!\n", dano);
      }
   }
   else {
      p->vida = p->vida - dano;
      printf("o inimigo lhe deu %d de dano!\n", dano);
   } 
}
      
void exibir_mapa(tabuleiro_casas fase[], int posicao) {
    printf("\n====================================================================\n");
    printf("                        MAPA DA NAVEGAÇÃO                           \n");
    printf("====================================================================\n");
    
    for (int i = 0; i < 16; i++) {
        printf(" Casa %02d: ", i + 1);
        switch (fase[i]) {
            case casa_inicial: printf("[ Casa Inicial    ]"); break;
            case mar_vazio:    printf("[ Mar Vazio       ]"); break;
            case combate:      printf("[ Combate         ]"); break;
            case tesouro:      printf("[ Tesouro         ]"); break;
            case taverna:      printf("[ Taverna         ]"); break;
            case evento:       printf("[ Evento          ]"); break;
            case chefe_final:  printf("[ Chefe Final     ]"); break;
        }
        if (i == posicao) {
            printf("  <-- [ SEU BARCO ]");
        }
        printf("\n");
    }
    printf("====================================================================\n");
}



void inimigo_molenga (inimigo *i) {
   int inimigo_aleatorio = (rand() % 5) + 1;
   switch (inimigo_aleatorio) {
      case 1:
         strcpy(i->nome, "Ratazana");
         strcpy(i->descricao, "Uma Ratazana suja aparece para roer as madeiras do seu barco!");
         i->vida = 4;
         i->ataque = 1;
         i->defesa = 0;
         i->nivel = molenga;
         i->xp_recompensa = 10;
         i->medalha_recompensa = 0;
         break;
      case 2:
         strcpy(i->nome, "Grupo de Corvos");
         strcpy(i->descricao, "Um grupo de Corvos famintos rodeiam o seu barco para devorar sua carne!");
         i->vida = 4;
         i->ataque = 1;
         i->defesa = 0;
         i->nivel = molenga;
         i->xp_recompensa = 10;
         i->medalha_recompensa = 0;
         break;
      case 3:
         strcpy(i->nome, "Espectro Fantasmagórico");
         strcpy(i->descricao, "Em meio a uma misteriosa nevoa, um Espectro Fantasmagorico aparece para lhe assombrar!");
         i->vida = 5;
         i->ataque = 1;
         i->defesa = 1;
         i->nivel = molenga;
         i->xp_recompensa = 10;
         i->medalha_recompensa = 0;
         break;
      case 4:
         strcpy(i->nome, "Marujo Ladrão");
         strcpy(i->descricao, "Um Marujo Ladrão invade seu barco para roubar seus tesouros! se é que existe um...");
         i->vida = 6;
         i->ataque = 2;
         i->defesa = 0;
         i->nivel = molenga;
         i->xp_recompensa = 10;
         i->medalha_recompensa = 0;
         break;
      case 5:
         strcpy(i->nome, "Morto Vivo Afogado");
         strcpy(i->descricao, "Submergendo das aguas e invadindo seu barco, um Morto Vivo Afogado surge para lhe atormentar!");
         i->vida = 6;
         i->ataque = 1;
         i->defesa = 0;
         i->nivel = molenga;
         i->xp_recompensa = 5;
         i->medalha_recompensa = 0;
         break;
      default:
         printf("\n");
   }
}

void inimigo_osso_duro (inimigo *i) {
   int inimigo_aleatorio = (rand() % 5) + 1;
   switch (inimigo_aleatorio) {
      case 1:
         strcpy(i->nome, "Escorpião dos Corais");
         strcpy(i->descricao, "Saindo dos corais, um Escorpião dos Corais surge para lhe incomodar!");
         i->vida = 5;
         i->ataque = 3;
         i->defesa = 1;
         i->nivel = osso_duro;
         i->xp_recompensa = 15;
         i->medalha_recompensa = 0;
         break;
      case 2:
         strcpy(i->nome, "Esqueleto");
         strcpy(i->descricao, "Ao passar por uma ilha mal assombrada, ossos emergem das aguas, formando um Esqueleto humano, Pronto para lhe fatiar com sua cimitarra!");
         i->vida = 7;
         i->ataque = 2;
         i->defesa = 1;
         i->nivel = osso_duro;
         i->xp_recompensa = 15;
         i->medalha_recompensa = 0;
         break;
      case 3:
         strcpy(i->nome, "Sereia");
         strcpy(i->descricao, "Em meio ao mar, uma sereia tenta lhe persuadir a fim de lhe afogar!");
         i->vida = 9;
         i->ataque = 1;
         i->defesa = 2;
         i->nivel = osso_duro;
         i->xp_recompensa = 15;
         i->medalha_recompensa = 0;
         break;
      case 4:
         strcpy(i->nome, "Guarda Real");
         strcpy(i->descricao, "Tranquilamente em seu barco, o barco da Guarda Real Britanica invade, e os Guardas estão prontos para lhe prender!");
         i->vida = 5;
         i->ataque = 2;
         i->defesa = 3;
         i->nivel = osso_duro;
         i->xp_recompensa = 15;
         i->medalha_recompensa = 0;
         break;
      case 5:
         strcpy(i->nome, "Chefe da Guarda Real");
         strcpy(i->descricao, "Como se já não fosse suficiente, agora o Chefe da Guarda Real aparece em pessoa para lhe prender!");
         i->vida = 12;
         i->ataque = 3;
         i->defesa = 2;
         i->nivel = osso_duro;
         i->xp_recompensa = 15;
         i->medalha_recompensa = 0;
         break;      
      default:
         printf("\n");
   }
}


void inimigo_abissal (inimigo *i) {
   int inimigo_aleatorio = (rand() % 5) + 1;
   switch (inimigo_aleatorio) {
      case 1:
         strcpy(i->nome, "Hibrido Tubarão Martelo");
         strcpy(i->descricao, "Das aguas, Um Hibrido Tubarão Martelo aparece para lhe desafiar!");
         i->vida = 10;
         i->ataque = 4;
         i->defesa = 3;
         i->nivel = abissal;
         i->xp_recompensa = 20;
         i->medalha_recompensa = medalha_homens_peixes;
         break;
      case 2:
         strcpy(i->nome, "Amirante sem Cabeça");
         strcpy(i->descricao, "Vagando pelo mar, o Almirante sem Cabeça o avista, e decide te assombrar!");
         i->vida = 15;
         i->ataque = 3;
         i->defesa = 3;
         i->nivel = abissal;
         i->xp_recompensa = 20;
         i->medalha_recompensa = medalha_almirante;
         break;
      case 3:
         strcpy(i->nome, "Serpente Marinha");
         strcpy(i->descricao, "Cheia de fome, uma Serpente Marinha rasteja para lhe devorar por inteiro!");
         i->vida = 17;
         i->ataque = 4;
         i->defesa = 2;
         i->nivel = abissal;
         i->xp_recompensa = 20;
         i->medalha_recompensa = medalha_fome;
         break;
      case 4:
         strcpy(i->nome, "Lorde das Cracas");
         strcpy(i->descricao, "Preso ao seu barco, o Lorde das Cracas irá lhe fazer o favor de te cortar inteiro!");
         i->vida = 15;
         i->ataque = 4;
         i->defesa = 4;
         i->nivel = abissal;
         i->xp_recompensa = 20;
         i->medalha_recompensa = medalha_cracas;
         break;
      case 5:
         strcpy(i->nome, "Behemoth, O Tirano");
         strcpy(i->descricao, "Behemoth, O Tirano, e sua tripulação, surgem para lhe desafiar e ver quem é o pirata mais forte!");
         i->vida = 20;
         i->ataque = 6;
         i->defesa = 4;
         i->nivel = abissal;
         i->xp_recompensa = 20;
         i->medalha_recompensa = medalha_lendas;
         break;     
      default:
         printf("\n");
   }
}

void inimigo_lendas_mares (inimigo *i) {
   int inimigo_aleatorio = (rand() % 3) + 1;
   switch (inimigo_aleatorio) {
      case 1:
         strcpy(i->nome, "Davy Jones");
         strcpy(i->descricao, "Um duelo final está prestes a começar! se encontras com o lendario pirata Davy Jones para disputar o tesouro do Caribe!");
         i->vida = 28;
         i->ataque = 8;
         i->defesa = 7;
         i->nivel = lenda;
         i->xp_recompensa = 0;
         i->medalha_recompensa = 0;
         break;
      case 2:
         strcpy(i->nome, "Kraken");
         strcpy(i->descricao, "Tão perto da Ilha do tesouro, Kraken emerge das profundezas do mar para afogar seu navio por inteiro!");
         i->vida = 35;
         i->ataque = 8;
         i->defesa = 5;
         i->nivel = lenda;
         i->xp_recompensa = 0;
         i->medalha_recompensa = 0;
         break;
      case 3:
         strcpy(i->nome, "BigBig, O Barco Fantasma");
         strcpy(i->descricao, "Um som de um grande tormento de aproxima, e BigBig, o barco fantasma, surge da vastidão do mar para lhe destruir!");
         i->vida = 30;
         i->ataque = 9;
         i->defesa = 6;
         i->nivel = lenda;
         i->xp_recompensa = 0;
         i->medalha_recompensa = 0;
         break;
      default:
         printf("\n");
   }
}


void casa_combate(personagem *p, int numero_fase, int casa_chefe_final, inimigo *i) {
   inimigo inimigo_combate;
   int resultado = 0;
   int forca_original = p->forca;
   int defesa_original = p->defesa;
   if (numero_fase == 1) {
      inimigo_molenga(&inimigo_combate);
   }
   if (numero_fase == 2) {
      inimigo_osso_duro(&inimigo_combate);
   }
   if (numero_fase == 3) {
      if (casa_chefe_final == 1) {
         inimigo_lendas_mares(&inimigo_combate);
      }
      else {
         inimigo_abissal(&inimigo_combate);
      }
   }
   printf("\n");
   printf("%s\n", inimigo_combate.descricao);
   printf("==================================================\n");
   printf(" Inimigo        : %s\n", inimigo_combate.nome);
   printf("--------------------------------------------------\n");
   printf(" Vida           : %d HP\n", inimigo_combate.vida);
   printf(" Força/Ataque   : %d\n", inimigo_combate.ataque);
   printf(" Defesa         : %d\n", inimigo_combate.defesa);
   printf(" Recompensa XP  : %d XP\n", inimigo_combate.xp_recompensa);
   printf("==================================================\n");
   enter();
   printf("\n---- Que o Combate se inicie! ----\n");
   printf("---- %s Vs %s ----\n", p->nome, inimigo_combate.nome);
   while (p->vida > 0 && inimigo_combate.vida > 0) {
     int acao = 0;
     printf("\n--------------------------------------------------\n");
     printf(" [STATUS DA BATALHA]\n");
     printf(" Você  : %s | Vida: %d | Maldição: %d\n", p->nome, p->vida, p->maldicao);
     printf(" Inimigo: %s | Vida: %d\n", inimigo_combate.nome, inimigo_combate.vida);
     printf("--------------------------------------------------\n");
     printf("\n");
     printf("\n");
     printf("\nO que deseja fazer?\n");
     printf(" [ 1 ] Atacar\n");
     printf(" [ 2 ] Usar Item\n");
     printf(" [ 3 ] Usar Habilidade\n");
     printf(" [ 4 ] Fugir\n");
     printf(" Escolha sua ação: ");
     scanf("%d", &acao);
     switch (acao) {
        case atacar:
           ataque_jogador(p, &inimigo_combate);
           break;
        case usar_item:
           acao_usar_item(p);
           break;
        case habilidades:
           usar_habilidades(p, i);
           break;
        case fugir:
           resultado = fuga(casa_chefe_final);
           break;
        default:
           printf("opção invalida! perdeu seu turno! otario!!\n");
           printf("\n");
     }
     if (resultado == 1) {
        p->forca = forca_original;
        p->defesa = defesa_original;
        enter();
        break;
     }
     if (inimigo_combate.vida <= 0) { 
        printf("%s derrotado! você ganhou!\n", inimigo_combate.nome);
        p->forca = forca_original;
        p->defesa = defesa_original;
        p->xp += inimigo_combate.xp_recompensa;
        if (inimigo_combate.medalha_recompensa != 0) {
            if (medalha_duplicada(p, inimigo_combate.medalha_recompensa)) {
               printf("o inimigo dropou uma medalha na qual você já tem!\n");
            }
            else {
               printf("Você ganhou uma medalha de recompensa também!\n");
               int slot_bau = -1;
               for (int j = 0; j < 20; j++) {
                  if (p->medalhas_inventario[j] == 0) {
                     slot_bau = j;
                     break;
                  } 
               }    
               if (slot_bau != -1) {
                  p->medalhas_inventario[slot_bau] = inimigo_combate.medalha_recompensa;
                  printf("A medalha foi guardada no seu inventario!\n");
               } 
               else {
                  printf("Seu baú de medalhas está lotado e a recompensa foi perdida...\n");
               }
            }
        }
        if (p->xp >= 20) {
           subir_nivel(p);
           break;
        }
        else {
           recompensa_aleatoria(p);
           break;
        }
        break;
     }
     printf("Turno do inimigo!\n");
     enter();
     ataque_inimigo(p, &inimigo_combate);
     if (p->medalha_cracas_ativada == 1) {
        inimigo_combate.vida -= 1;
     }
     if (p->vida <= 0) {
        printf("você foi derrotado!\n");
        break;
     }  
  }
}


void escolher_personagem (personagem *p, classes_personagens, total_habilidades) {
   int escolha;
   char nome_unico[100];
   scanf("%d", &escolha);
   while (escolha < 1 || escolha > 3) {
      printf("opção invalida! tente novamente!");
      printf("\n");
      printf("Digite o número da sua classe (1, 2 ou 3): ");
      scanf("%d", &escolha);
   }
   printf("digite o seu nome (nome unico!): ");
   scanf("%s", nome_unico);
   switch (escolha) {
      case marujo:
         strcpy(p->nome, "capitão");
         strcat(p->nome, " ");
         strcat(p->nome, nome_unico);
         p->vida = 25;
         p->vida_maxima = 25;
         p->forca = 3;
         p->defesa = 1;
         p->agilidade = 0;
         p->maldicao = 3;
         p->maldicao_maxima = 3;
         p->pontos_medalha = 10;
         p->nivel = 1;
         p->xp = 0;
         p->lista_habilidades[0] = grito_pirata;
         break;
      case xama:
         strcpy(p->nome, "capitão");
         strcat(p->nome, " ");
         strcat(p->nome, nome_unico);
         p->vida = 15;
         p->vida_maxima = 15;
         p->forca = 1;
         p->defesa = 2;
         p->agilidade = 1;
         p->maldicao = 6;
         p->maldicao_maxima = 6;
         p->pontos_medalha = 15;
         p->nivel = 1;
         p->xp = 0;
         p->lista_habilidades[0] = barreira_amaldicoada;
         break;
      case artilheiro:
         strcpy(p->nome, "capitão");
         strcat(p->nome, " ");
         strcat(p->nome, nome_unico);
         p->vida = 20;
         p->vida_maxima = 20;
         p->forca = 2;
         p->defesa = 1;
         p->agilidade = 4;
         p->maldicao = 3;
         p->maldicao_maxima = 3;
         p->pontos_medalha = 17;
         p->nivel = 1;
         p->xp = 0;
         p->lista_habilidades[0] = comida_bolso;
         break;
      default:
         printf("\n");
   }
}

void status(personagem *p) {
    printf("==================================================\n");
    printf("                     STATUS                       \n");
    printf("==================================================\n");
    printf(" Nome           : %s\n", p->nome);
    printf(" Nível          : %d  (XP: %d/20)\n", p->nivel, p->xp);
    printf("--------------------------------------------------\n");
    printf(" Vida           : %d / %d HP\n", p->vida, p->vida_maxima);
    printf(" Força          : %d\n", p->forca);
    printf(" Defesa         : %d\n", p->defesa);
    printf(" Agilidade      : %d\n", p->agilidade);
    printf(" Maldição       : %d \n", p->maldicao);
    printf(" Medalhas       : %d\n", p->pontos_medalha);
    printf("==================================================\n");
    printf("\n");
    enter();
}


void jogar_dado(personagem *p, tabuleiro_casas fase[], int *posicao, int casa_chefe_final, int numero_fase, inimigo *i) {
   int dado = (rand() % 6) + 1;
   *posicao += dado;
   if (*posicao >= 15) {
      *posicao = 15;
   }
   casa_chefe_final = (*posicao == 15) ? 1 : 0;
   tabuleiro_casas casa_atual = fase[*posicao];
   printf("\n");
   printf("você tirou %d no dado!\n", dado);
   enter();
   switch (casa_atual) {
      case mar_vazio:
         casa_mar_vazio();
         break;
      case combate:
         casa_combate(p, numero_fase, casa_chefe_final, i);
         break;
      case tesouro:
         casa_tesouro(p);
         break;
      case taverna:
         casa_taverna(p);
         break;
      case evento:
         casa_evento(p);
         break;
      case chefe_final:
         casa_combate(p, numero_fase, casa_chefe_final, i);
         break;
      default:
         printf("\n");
   }   
}

void tela_opcao(personagem *p, tabuleiro_casas fase[], int posicao, int casa_chefe_final, int numero_fase) {
   int opcao = 0;
   int equipar_desequipar = 0;
   while (opcao != 1) {
       printf(" Agora, Você está em seu barco, pronto para navergar\n ");
       printf("\n--------------------------------------------------\n");
       printf(" O QUE DESEJA FAZER AGORA, %s?\n", p->nome);
       printf("--------------------------------------------------\n");
       printf(" [ 1 ] Jogar o dado e navegar\n");
       printf(" [ 2 ] Ver os status\n");
       printf(" [ 3 ] Ver o mapa\n");
       printf(" [ 4 ] Equipar medalhas\n");
       printf("--------------------------------------------------\n");
       printf(" Escolha uma opção: ");
       scanf("%d", &opcao);
       switch (opcao) {
          case 1:
             break; 
          case 2:
             status(p);
             break;
          case 3:
             exibir_mapa(fase, posicao);
             break;
          case 4:
             printf("você deseja equipar ou desequipar uma medalha?(1 para equipar, 0 para desequipar: \n");
             scanf("%d", &equipar_desequipar);
             if (equipar_desequipar == 1) {
                equipar_medalha(p);
                break;
             }
             else {
                desequipar_medalha(p);
                break;
             }
          default:
             printf("\n[!] Opção inválida! Escolha entre 1 e 4.\n");
             break;
       }
   }
}


int jogar_fase (personagem *p, tabuleiro_casas fase[], const char* nome_fase, int casa_chefe_final, int numero_fase, inimigo *i) {
   int posicao = 0;
   printf("\n==================================================\n");
   printf("           Você está no %s\n", nome_fase);
   printf("==================================================\n");
   while (posicao < 15 && p->vida > 0) {
      tela_opcao(p, fase, posicao, casa_chefe_final, numero_fase);
      jogar_dado(p, fase, &posicao, casa_chefe_final, numero_fase, i);
      if (p->vida <= 0) {
         printf("Game Over! você virou isca de peixe!\n");
         return 0;
      }
   }
   if (p->vida > 0) {
      printf("você sobreviveu ao %s, agora navegue para a proxima fase!\n", nome_fase);
      return 1;
   }
   return 0;
}
	
   

void historia() {
    printf("==================================================================================\n");
    printf("                                   Historia                                       \n");
    printf("==================================================================================\n\n");

    printf("[ ANO DE 1699 ]\n");
    printf("Você era apenas um simples vendedor de peixes nas ruas da Inglaterra, cansado da\n");
    printf("rotina. Movido por pura curiosidade e sede de emoção, você abandonou tudo,\n");
    printf("roubou um barco de médio porte e decidiu se tornar um pirata.\n\n");

    printf("\n");
    printf("Sem tripulação, sem aliados e contando apenas com suas próprias habilidades,\n");
    printf("você navega sozinho em direção aos perigos mais mortais do oceano: feras colossais,\n");
    printf("piratas lendários e até mesmo a implacável Guarda Real Inglesa lhe perseguindo.\n\n");

    printf("\n");
    printf("Em uma parada numa taverna na França, você ouviu boatos sobre um tesouro místico\n");
    printf("escondido no Caribe. Agora, você está em alto-mar seguindo a pista, torcendo para\n");
    printf("fazer história... ou para não virar comida de peixe!\n");
    printf("==================================================================================\n");
    printf("\n");
    enter();
}


void menu_classes() {
    printf("==================================================================================\n");
    printf("                           ESCOLHA SUA CLASSE PIRATA                              \n");
    printf("==================================================================================\n\n");

    // 1. MARUJO DE CONVÉS
    printf("[ 1 ] MARUJO (Guerreiro)\n");
    printf("----------------------------------------------------------------------------------\n");
    printf("  • Vida              : 25 HP\n");
    printf("  • Força             : 3  | Defesa: 1  | Agilidade: 0\n");
    printf("  • Maldição          : 3  | Pontos de Medalhas Iniciais: 10\n");
    printf("  • Arma Equipada     : Sabre\n");
    printf("  • Habilidade Inicial: Grito Pirata\n");
    printf("    -> Efeito: Acovarda o oponente e reduz a defesa dele em 1 ponto durante toda a luta.\n");
    printf("    -> Custo : 2 Pontos de Maldição\n");
    printf("----------------------------------------------------------------------------------\n\n");

    // 2. XAMÃ DO MAR
    printf("[ 2 ] XAMÃ (Mago)\n");
    printf("----------------------------------------------------------------------------------\n");
    printf("  • Vida              : 15 HP\n");
    printf("  • Força             : 1  | Defesa: 2  | Agilidade: 1\n");
    printf("  • Maldição          : 6  | Pontos de Medalhas Iniciais: 15\n");
    printf("  • Arma Equipada     : Cajado de Pau Velho\n");
    printf("  • Habilidade Inicial: Barreira Amaldiçoada\n");
    printf("    -> Efeito: Cria um campo concentrado que aumenta a defesa em 1 ponto durante toda a luta.\n");
    printf("    -> Custo : 2 Pontos de Maldição\n");
    printf("----------------------------------------------------------------------------------\n\n");

    // 3. ARTILHEIRO DE ELITE
    printf("[ 3 ] ARTILHEIRO DE ELITE (Arqueiro/Atirador)\n");
    printf("----------------------------------------------------------------------------------\n");
    printf("  • Vida              : 20 HP\n");
    printf("  • Força             : 2  | Defesa: 1  | Agilidade: 4\n");
    printf("  • Maldição          : 3  | Pontos de Medalhas Iniciais: 17\n");
    printf("  • Arma Equipada     : Mosquete\n");
    printf("  • Habilidade Inicial: Comida de Bolso\n");
    printf("    -> Efeito: Recupera 3 pontos de vida (cuidado para não comer uma bala!).\n");
    printf("    -> Custo : 1 Ponto de Maldição\n");
    printf("==================================================================================\n");
    printf("\n");
    printf("\n");
    printf("Digite o número da sua classe (1, 2 ou 3): ");
}
        
void menu() {
   printf("========================================\n");
   printf("         PIRATAS DO BALACUBACO          \n");
   printf("========================================\n");
   printf("\n");
   enter();
}

void fim() {
   printf("\n========================================================================================\n");
   printf(" Você finalmente chega à tão aguardada ilha do tesouro perdido nas terras do Caribe!\n");
   printf(" Você escora seu barco e então, ali na areia, encontra-se um X marcado com tinta vermelha,\n");
   printf(" estranhamente muito óbvio por sinal!\n");
   printf("----------------------------------------------------------------------------------------\n");
   printf(" Você então começa a cavar ali, com afinco e suor, até que finalmente acha o baú!\n");
   printf(" Ao abrir a tampa pesada... você encontra... um papel... APENAS UM PAPEL?\n");
   printf("----------------------------------------------------------------------------------------\n");
   printf(" Ao pegar o pergaminho trêmulo, você percebe que há algo escrito nele:\n\n");
   printf("   \"O verdadeiro tesouro são os amigos que fazemos pelo caminho...\"\n\n");
   printf(" Um silêncio fúnebre toma conta da praia...\n");
   printf(" ...acho que definitivamente essa aventura não valeu a pena.\n");
   printf("========================================================================================\n");
}


int main() {
   srand(time(NULL));
   system("chcp 65001 > nul");
   tabuleiro_casas fase_mar_aberto[16] = {
      casa_inicial, mar_vazio, combate, tesouro, combate, taverna, mar_vazio, evento,
      evento, combate, combate, taverna, mar_vazio, combate, tesouro, combate
   };
   tabuleiro_casas fase_mar_agitado[16] = {
      casa_inicial, taverna, evento, combate, tesouro, combate, mar_vazio, mar_vazio, combate, tesouro, combate,
      mar_vazio, evento, combate, taverna, combate
   };
   tabuleiro_casas fase_mar_amaldiçoado[16] = {
      casa_inicial, combate, mar_vazio, taverna, combate, tesouro, tesouro, evento, combate, taverna, combate,
      evento, mar_vazio, mar_vazio, taverna, chefe_final
   };
   personagem pirata;
   inimigo oponente;
   classes_personagens classe;
   total_habilidades habilidade;
   pirata.medalha_almirante_ativada = 0;
   pirata.medalha_cracas_ativada = 0;
   pirata.medalha_fome_ativada = 0;
   int casa_chefe_final = 0;
   int numero_fase = 1;
   for (int i = 0; i < 20; i++) {
      pirata.medalhas_inventario[i] = 0;
   }
   for (int i = 0; i < 4; i++) {
      pirata.lista_habilidades[i] = 0;
   }
   for (int i = 0; i < 5; i++) {
      pirata.inventario[i] = 0;
   }
   for (int i = 0; i < 10; i++) {
      pirata.medalhas_utilizaveis[i] = 0;
   }
   menu();
   historia();
   menu_classes();
   escolher_personagem(&pirata, classe, habilidade);
   status(&pirata);
   if (jogar_fase(&pirata, fase_mar_aberto, "Mar Aberto", casa_chefe_final, numero_fase, &oponente) == 0) {
      return 0;
   }
   numero_fase++;
   if (jogar_fase(&pirata, fase_mar_agitado, "Mar Agitado", casa_chefe_final, numero_fase, &oponente) == 0) {
      return 0;
   }
   numero_fase++;
   if (jogar_fase(&pirata, fase_mar_agitado, "Mar Amaldiçoado", casa_chefe_final, numero_fase, &oponente) == 0) {
      return 0;
   }
   printf("\n");
   fim();
   return 0;
}
   

