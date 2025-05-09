#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

//--------- Estrutura tabela de símbolos ----------
struct tabSimb{
      string simbolo;
      int end;
      int valor;
};

//--------- Estrutura tabela de instrução e montagem ----------
struct instrucao{
      string instrucao;
      string op1, op2, op3;
      int opcode;
};

//--------- Declaração variáveis globais ----------
struct tabSimb tabelaSimb[100];
struct instrucao montagem[100];
struct instrucao tabelaInst[13];
int endAtual = 0;
int PC = -1;
int qtdSimb = 0;
int qtdInst = 0;

//--------- Protótipo das funções ----------
void erro(string mensagem, int linha);
string remover_espaco(string s);
void primeira_passagem();
void criar_tabSimbolos(string pal1, string pal2, string pal3);
void criar_montagem(string pal1, string pal2, string pal3, string pal4);
string converter_minusculo(string pal);
void tabela_instrucoes();
int buscar_opcode(string mnem);
void ler_arquivo();
int buscar_endereco(string simbolo);
void criar_codigoDecimal();
void segunda_passagem();
void imprime_tabSimb();
void imprime_montagem();


//--------- Função para tratamento de erros ---------
void erro(string mensagem, int linha = -1) {
    if (linha >= 0)
        printf("Erro na linha %d: %s\n", linha, mensagem.c_str());
    else
        printf("Erro: %s\n", mensagem.c_str());

    exit(1); 
}

//--------- Função para remoção de espaço, quebra de linha e etc ----------
string remover_espaco(string s) 
{
    s.erase(0, s.find_first_not_of(" \n\r\t"));  
    s.erase(s.find_last_not_of(" \n\r\t") + 1);  
    return s;
}
  
//--------- Função que converte palavra para minusculo ---------- 
string converter_minusculo(string pal) {
    string palavraMin = pal;
    for (char& c : palavraMin) {
        c = tolower(c);
    }
    return palavraMin;
}

//--------- Função tabela de instruções ---------- 
void tabela_instrucoes()
{
    tabelaInst[0] = {"add", "", "", "", 0};
    tabelaInst[1] = {"sub", "", "", "", 1};
    tabelaInst[2] = {"mul", "", "", "", 2};
    tabelaInst[3] = {"div", "", "", "", 3};
    tabelaInst[4] = {"mv", "", "", "", 4};
    tabelaInst[5] = {"st", "", "", "", 5};
    tabelaInst[6] = {"jmp", "", "", "", 6};
    tabelaInst[7] = {"jeq", "", "", "", 7};
    tabelaInst[8] = {"jgt", "", "", "", 8};
    tabelaInst[9] = {"jlt", "", "", "", 9};
    tabelaInst[10] = {"w", "", "", "", 10};
    tabelaInst[11] = {"r", "", "", "", 11};
    tabelaInst[12] = {"stp", "", "", "", 12};
}

//--------- Função que busca o opcode da instrução ---------- 
int buscar_opcode(string mnem) 
{     
    int i;

    for (i = 0; i < 13; i++) {
        if (tabelaInst[i].instrucao == mnem)
            return tabelaInst[i].opcode;
    }
    erro("Instrução desconhecida: " + montagem[i].instrucao, i + PC);
    return -1; 
}
  
//--------- Função que cria a tabela de simbolos ----------
void criar_tabSimbolos(string pal1, string pal2, string pal3)
{
    int i;

    for (i = 0; i < 100; i++) {
        if (tabelaSimb[i].simbolo == pal1) {
            erro("Símbolo duplicado: " + pal1);
            return;
        }
    }

    for (i = 0; i < 100; i++) {
        if (tabelaSimb[i].simbolo == "") {
            tabelaSimb[i].simbolo = pal1;
            tabelaSimb[i].end = endAtual;
            tabelaSimb[i].valor = stoi(pal3);
            break;
        }
    }

    if (i == 100) {
        printf("\nTabela de símbolos cheia!\n");
    }

    qtdSimb++;
}

//--------- Função que cria a montagem do código ----------
void criar_montagem(string pal1, string pal2, string pal3, string pal4)
{
    int i;

    if (PC == -1) {
        PC = endAtual; 
    }

    for (i = 0; i < 100; i++) {
        if (montagem[i].instrucao == "") {
            montagem[i].instrucao = pal1;
            montagem[i].opcode = buscar_opcode(pal1);

            if (pal1 == "stp") {
                montagem[i].op1 = "0";
                montagem[i].op2 = "0";
                montagem[i].op3 = "0";
            } else if (pal1 == "mv" || pal1 == "st") {
                montagem[i].op1 = pal2;
                montagem[i].op2 = pal3;
                montagem[i].op3 = "0";
            } else if (pal1 == "r" || pal1 == "w" || pal1 == "jmp"){
                montagem[i].op1 = pal2;
                montagem[i].op2 = "0";
                montagem[i].op3 = "0";
            } else {
                montagem[i].op1 = pal2;
                montagem[i].op2 = pal3;
                montagem[i].op3 = pal4;
            }
            break;
        }
    }

    if (i == 100) {
        printf("\nTabela de instruções cheia!\n");
    }

    qtdInst++;
}

//--------- Função de ler o arquivo assembly (.txt)----------
void ler_arquivo() 
{
    FILE *f;
    char nomeArq[30], linha[100];
    char *palavra;
    string palavra1, palavra2, palavra3, palavra4;

    printf("\n\nDigite o nome do arquivo (.txt): ");
    scanf("%s", nomeArq);

    f = fopen(nomeArq, "r");

    if (!f) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    while (fgets(linha, sizeof(linha), f)) {
        linha[strcspn(linha, "\n")] = '\0';  
      
        palavra = strtok(linha, " ");

        if (palavra != NULL){
            palavra1 = remover_espaco(converter_minusculo(palavra));
            palavra = strtok(NULL, " ");
        } 
        if (palavra != NULL){
            palavra2 = remover_espaco(converter_minusculo(palavra));
            palavra = strtok(NULL, " ");
        }
        if (palavra != NULL){
            palavra3 = remover_espaco(converter_minusculo(palavra));
            palavra = strtok(NULL, " ");
        }
        if (palavra != NULL){
            palavra4 = remover_espaco(converter_minusculo(palavra));
        } 
        if(palavra2 == "word"){
            criar_tabSimbolos(palavra1, palavra2, palavra3);
        } else {
            criar_montagem(palavra1, palavra2, palavra3, palavra4);
        }
        
        endAtual++; 
    }
    fclose(f);
}

//--------- Função que busca o endereço da variável ----------
int buscar_endereco(string simbolo)
{
    int i;

    for (i = 0; i < qtdSimb; i++) {
        if (tabelaSimb[i].simbolo == simbolo) {
            return tabelaSimb[i].end;
        }
    }
    return -1;
}

//--------- Função gera o código decimal (.txt) ----------
void criar_codigoDecimal()
{
    FILE *f_saida;
    int i;
    char nomeArq[30];

    printf("\n\nDigite o nome do código decimal (.txt): ");
    scanf("%s", nomeArq);

    f_saida = fopen(nomeArq, "w");

    if (!f_saida) {
        printf("Erro ao criar o arquivo.\n");
        exit(1);
    }

    fprintf(f_saida, "%d %d %d %d\n", PC, 0, 0, 0); //PC

    // Processa as variáveis
    for (i = 0; i < qtdSimb; i++) {
        fprintf(f_saida, "%d %d %d %d\n", tabelaSimb[i].valor, 0, 0, 0); 
    }
        // Processa as instruções
        for (i = 0; i < qtdInst; i++) //COLOCAR NUYMA FUNÇÃOOOO
        {
            switch (montagem[i].opcode) {
                case 0: // "add"
                case 1: // "sub"
                case 2: // "mul"
                case 3: // "div":
                    fprintf(f_saida, "%d %s %s %s\n", montagem[i].opcode, montagem[i].op1.c_str(), montagem[i].op2.c_str(), montagem[i].op3.c_str()); 
                break;

                case 4: // "mv"
                case 5: // "st"
                {
                    int end = buscar_endereco(remover_espaco(montagem[i].op2));
                    if (end == -1) {
                        erro("Variável '" + montagem[i].op2 + "' não declarada", i + PC);
                    }
                    fprintf(f_saida, "%d %s %d %d\n", montagem[i].opcode, montagem[i].op1.c_str(), end, 0);
                }
                break;

                case 6: // "jmp"
                {
                    int destino = stoi(montagem[i].op1);
                    if (destino < PC || destino >= PC + qtdInst) {
                        erro("Endereço de salto inválido: " + to_string(destino), i + PC);
                    }
                    fprintf(f_saida, "%d %d %d %d\n", montagem[i].opcode, destino, 0, 0); 
                }
                break;

                case 7: // "jeq"
                case 8: // "jgt"
                case 9: // "jlt"
                {
                    int destino = stoi(montagem[i].op3);
                    if (destino < PC || destino >= PC + qtdInst) {
                        erro("Endereço de salto inválido: " + to_string(destino), i + PC);
                    }
                    fprintf(f_saida, "%d %s %s %d\n", montagem[i].opcode, montagem[i].op1.c_str(), montagem[i].op2.c_str(), destino);     
                }
                break;

                case 10: // "w"
                case 11: // "r"
                {
                    int end = buscar_endereco(remover_espaco(montagem[i].op1));
                    if (end == -1) {
                        erro("Variável '" + montagem[i].op1 + "' não declarada", i + PC);
                    }
                    fprintf(f_saida, "%d %d %d %d\n", montagem[i].opcode, end, 0, 0);
                }
                break;

                case 12: // "stp"
                    fprintf(f_saida, "%d %d %d %d\n", montagem[i].opcode, 0, 0, 0);
                break;

                default:
                    printf("Instrução desconhecida!");
                break;
            }
        }

    fclose(f_saida);
}

void primeira_passagem()
{
    ler_arquivo();
}

void segunda_passagem()
{
    criar_codigoDecimal();
}

void imprime_tabSimb()
{
    int i;

    printf("\n-------- TABELA DE SÍMBOLOS --------\n");
    printf("%-15s %-10s %-10s\n", "Símbolo", "Endereço", "Valor");
 
    for (i = 0; i < 100 && tabelaSimb[i].simbolo != ""; i++) {
         printf("%-15s %-10d %-10d\n",
             tabelaSimb[i].simbolo.c_str(),
             tabelaSimb[i].end,
             tabelaSimb[i].valor);
     }
     printf("------------------------------------\n");
}

void imprime_montagem()
{
    int i;

    printf("\n----------------- TABELA DE MONTAGEM ------------------\n");
    printf("%-5s %-10s %-10s %-10s %-10s\n", "OPC", "Instr", "OP1", "OP2", "OP3");
 
    for (int i = 0; i < 100 && !montagem[i].instrucao.empty(); i++) {
         printf("%-5d %-10s %-10s %-10s %-10s\n",
             montagem[i].opcode,
             montagem[i].instrucao.c_str(),
             montagem[i].op1.c_str(),
             montagem[i].op2.c_str(),
             montagem[i].op3.c_str());
    }
    printf("---------------------------------------------------------\n");
}

int main()
{
    tabela_instrucoes();
    primeira_passagem();
    imprime_tabSimb();
    imprime_montagem();
    segunda_passagem();
    return 0;
}