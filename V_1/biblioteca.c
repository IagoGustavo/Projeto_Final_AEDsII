#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iso646.h>
#include <stdbool.h>
#include "cabecalho.h"

struct Atendente
{
    char nome[50], cpf[50], senha[20];
    Atendente* prox;
};

struct Funcionarios
{
    Atendente* inicio;
};

struct Livro
{
    char titulo[50], autor[50], codigo[20]; 
    int ano, exemplares;
    Livro* prox;
};

struct Acervo
{
    Livro* inicio;
};

struct Estudante
{
    char nome[50], matricula[50], senha[20];
    LivrosSobPosse* livrosSobPosse;
    Estudante* prox;
};

struct Usuarios
{
    Estudante* inicio;
};

struct LivrosSobPosse
{
    char codigo[20];
    time_t dataEmprestimo;
    time_t dataDevolucao;
    LivrosSobPosse* prox;
};

Funcionarios* CriarAtendente()
{
    Funcionarios* funcionarios = (Funcionarios*)malloc(sizeof(Funcionarios));

    if (not funcionarios)
    {
        printf("\nErro ao alocar memória!\n");
        return NULL;
    }
    
    funcionarios->inicio = NULL;

    return funcionarios;
}

Atendente* CadastrarAtendente(Funcionarios* funcionarios, char nome[], char cpf[], char senha[])
{
    Atendente* atendente = (Atendente*)malloc(sizeof(Atendente));

    if (not atendente)
    {
        printf("\nErro ao alocar memória!\n");
        return NULL;
    }

    strcpy(atendente->nome, nome);
    strcpy(atendente->cpf, cpf);
    strcpy(atendente->senha, senha);

    atendente->prox = funcionarios->inicio;
    funcionarios->inicio = atendente;

    return atendente;
}

Funcionarios* CarregarAtendentes(const char* nomeArquivo)
{
    FILE* arquivo = fopen(nomeArquivo, "r");
    
    if (not arquivo) 
    {
        perror("\nErro ao abrir arquivo");
        return NULL;
    }

    Funcionarios* funcionarios = CriarAtendente();

    if (not funcionarios) 
    {
        fclose(arquivo);
        return NULL;
    }

    char linha[256];

    while (fgets(linha, sizeof(linha), arquivo)) 
    {
        char* nome = strtok(linha, ",");
        char* cpf = strtok(NULL, ",");
        char* senha = strtok(NULL, "\n");

        if (nome and cpf and senha) 
        {
            Atendente* atendente = CadastrarAtendente(funcionarios, nome, cpf, senha);
        }
    }

    fclose(arquivo);
    return funcionarios;
} 

void SalvarFuncionarios(const Funcionarios* funcionarios, const char* nomeArquivo)
{
    if (!funcionarios || !nomeArquivo) 
    {
        printf("\nErro: Parâmetros inválidos para salvar funcionários!\n");
        return;
    }

    FILE* arquivo = fopen(nomeArquivo, "w");

    if (!arquivo) 
    {
        perror("\nErro ao abrir arquivo para salvar funcionários");
        return;
    }

    Atendente* atual = funcionarios->inicio;

    while (atual) 
    {
        fprintf(arquivo, "%s,%s,%s\n", atual->nome, atual->cpf, atual->senha);
        
        atual = atual->prox;
    }

    fclose(arquivo);
}

Atendente* LoginAtendente(const Funcionarios* funcionarios, char cpf[], char senha[])
{
    struct Atendente* atendente = funcionarios->inicio;
    bool atendenteEncontrado = false;

    if (not atendente)
    {
        printf("\nNenhum funcionário cadastrado!\n");
        exit(1);
    }

    while (atendente)
    {
        if (strcmp(atendente->cpf, cpf) == 0 and strcmp(atendente->senha, senha) == 0)
        {
            atendenteEncontrado = true;
            
            printf("\nSeja bem-vindo(a) %s!\n", atendente->nome);

            return atendente;
        }

        atendente = atendente->prox;
    }
    
    if (not atendenteEncontrado)
    {
        printf("\nCPF e/ou senha incorretos! Tente novamente!\n");
        return NULL;
    }
}

void ImprimirFuncionarios(const Funcionarios* funcionarios)
{
    struct Atendente* atendente = funcionarios->inicio;

    if (not atendente)
    {
        printf("\nNenhum funcionário cadastrado!\n");
        return;
    }
    
    printf("\n===== FUNCIONÁRIOS CADASTRADOS =====\n");

    while (atendente)
    {
        printf("Nome: %s\nCPF: %s\n\n", atendente->nome, atendente->cpf);

        atendente = atendente->prox;
    }
    
    printf("============================\n");
}

void DestruirFuncionarios(Funcionarios* funcionarios)
{
    struct Atendente* atendente = funcionarios->inicio;

    while (atendente)
    {
        struct Atendente* temp = atendente;
        atendente = atendente->prox;

        free(temp);
    }
    
    free(funcionarios);
}

Acervo* CriarAcervo()
{
    Acervo* acervo = (Acervo*)malloc(sizeof(Acervo));

    if (not acervo)
    {
        printf("\nErro ao alocar memória!\n");
        return NULL;
    }
    
    acervo->inicio = NULL;

    return acervo;
}

Acervo* CarregarAcervo(const char* nomeArquivo)
{
    FILE* arquivo = fopen(nomeArquivo, "r");
    
    if (not arquivo) 
    {
        perror("\nErro ao abrir arquivo");
        return NULL;
    }

    Acervo* acervo = CriarAcervo();

    if (not acervo) 
    {
        fclose(arquivo);
        return NULL;
    }

    char linha[256];

    while (fgets(linha, sizeof(linha), arquivo)) 
    {
        char* titulo = strtok(linha, ",");
        char* autor = strtok(NULL, ",");
        char* anoString = strtok(NULL, ",");
        char* exemplaresString = strtok(NULL, ",");
        char* codigo = strtok(NULL, "\n");

        int ano = atoi(anoString);
        int exemplares = atoi(exemplaresString);

        if (titulo and autor and ano and exemplares and codigo) 
        {
            Livro* livro = CadastrarLivro(acervo, titulo, autor, ano, exemplares, codigo);
        }
    }

    fclose(arquivo);

    return acervo;
}

void SalvarAcervo(const Acervo* acervo, const char* nomeArquivo)
{
    if (!acervo) 
    {
        printf("\nErro: Acervo inválido!\n");
        return;
    }

    FILE* arquivo = fopen(nomeArquivo, "w");

    if (!arquivo) 
    {
        perror("\nErro ao abrir arquivo para salvar");
        return;
    }

    Livro* livroAtual = acervo->inicio;

    while (livroAtual) 
    {
        fprintf(arquivo, "%s,%s,%d,%d,%s\n", livroAtual->titulo, livroAtual->autor, livroAtual->ano, livroAtual->exemplares, livroAtual->codigo);
        
        livroAtual = livroAtual->prox;
    }

    fclose(arquivo);
}

Livro* CadastrarLivro(Acervo* acervo, char titulo[], char autor[], int ano, int exemplares, char codigo[])
{
    Livro* livro = (Livro*)malloc(sizeof(Livro));

    if (not livro)
    {
        printf("\nErro ao alocar memória!\n");
        exit(1);
    }
    
    strcpy(livro->titulo, titulo);
    strcpy(livro->autor, autor);
    livro->ano = ano;
    livro->exemplares = exemplares;
    strcpy(livro->codigo, codigo);

    livro->prox = acervo->inicio;
    acervo->inicio = livro;

    return livro;
}

Livro* BuscarLivro(const Acervo* acervo, char codigo[])
{
    struct Livro* livro = acervo->inicio;
    bool livroEncontrado = false;

    if (not livro)
    {
        printf("\nNenhum livro cadastrado!\n");
        exit(1);
    }

    while (livro)
    {
        if (strcmp(livro->codigo, codigo) == 0)
        {
            livroEncontrado = true;
            
            printf("\n===== LIVRO ENCONTRADO =====\n");
            printf("Título: %s\nAutor: %s\nAno: %d\nExemplares: %d\nCódigo: %s\n", livro->titulo, livro->autor, livro->ano, livro->exemplares, livro->codigo);

            return livro;
        }

        livro = livro->prox;
    }
    
    if (not livroEncontrado)
    {
        printf("\nLivro de código \"%s\" não encontrado!\n", codigo);
    }
}

void ExcluirLivro(Acervo* acervo, char codigo[])
{
    struct Livro* livro = acervo->inicio, *anterior = NULL;

    if (not livro)
    {
        printf("\nNenhum livro cadastrado!\n");
        return;
    }

    while (livro != NULL and strcmp(livro->codigo, codigo) != 0)
    {
        anterior = livro;
        livro = livro->prox;
    }
    
    if (not livro)
    {
        printf("\nNenhum livro encontrado com o código \"%s\"\n", codigo);
        return;
    }
    
    if (not anterior)
    {
        acervo->inicio = livro->prox;
    }

    else
    {
        anterior->prox = livro->prox;
    }
    
    free(livro);

    printf("\nLivro \"%s\" removido com sucesso!\n", codigo);
}

void ImprimirAcervo(const Acervo* acervo)
{
    struct Livro* livro = acervo->inicio;

    if (not livro)
    {
        printf("\nNenhum livro cadastrado!\n");
        return;
    }
    
    printf("\n===== ACERVO DE LIVROS =====\n");

    while (livro)
    {
        printf("Título: %s\nAutor: %s\nAno: %d\nExemplares: %d\nCódigo: %s\n\n", livro->titulo, livro->autor, livro->ano, livro->exemplares, livro->codigo);

        livro = livro->prox;
    }
    
    printf("============================\n");
}

void DestruirBiblioteca(Acervo* acervo)
{
    struct Livro* livro = acervo->inicio;

    while (livro)
    {
        struct Livro* temp = livro;
        livro = livro->prox;

        free(temp);
    }
    
    free(acervo);
}

void EmprestarLivro(Acervo* acervo, Estudante* estudanteAtual, Atendente* atendenteAtual, char codigo[], int diasPrazo)
{
    if (!acervo || !estudanteAtual || !atendenteAtual || !codigo) 
    {
        printf("\nErro: Parâmetros inválidos para empréstimo!\n");
        return;
    }

    Livro* livroEscolhido = BuscarLivro(acervo, codigo);
    
    if (livroEscolhido == NULL) 
    {
        printf("\nLivro com código %s não encontrado no acervo!\n", codigo);
        return;
    }

    if (livroEscolhido->exemplares <= 0) 
    {
        printf("\nNão há exemplares disponíveis do livro \"%s\"!\n", livroEscolhido->titulo);
        return;
    }

    LivrosSobPosse* livroEmprestado = (LivrosSobPosse*)malloc(sizeof(LivrosSobPosse));

    if (livroEmprestado == NULL) 
    {
        perror("\nErro ao alocar memória para registro de empréstimo");
        return;
    }

    livroEscolhido->exemplares--;

    strncpy(livroEmprestado->codigo, codigo, sizeof(livroEmprestado->codigo) - 1);
    livroEmprestado->codigo[sizeof(livroEmprestado->codigo) - 1] = '\0';
    
    time_t agora = time(NULL);
    livroEmprestado->dataEmprestimo = agora;
    livroEmprestado->dataDevolucao = agora + (diasPrazo * 86400);

    livroEmprestado->prox = estudanteAtual->livrosSobPosse;
    estudanteAtual->livrosSobPosse = livroEmprestado;

    printf("\nEmpréstimo realizado com sucesso:\n");
    printf("Livro: %s (Código: %s)\n", livroEscolhido->titulo, codigo);
    printf("Estudante: %s\n", estudanteAtual->nome);
    printf("Atendente responsável: %s\n", atendenteAtual->nome);
    
    // Formata data de devolução
    char dataStr[30];
    strftime(dataStr, sizeof(dataStr), "%d/%m/%Y", localtime(&livroEmprestado->dataDevolucao));
    printf("Data de devolução: %s\n", dataStr);
}

Usuarios* CriarUsuarios()
{
    Usuarios* usuarios = (Usuarios*)malloc(sizeof(Usuarios));

    if (not usuarios)
    {
        printf("\nErro ao alocar memória!\n");
        return NULL;
    }
    
    usuarios->inicio = NULL;

    return usuarios;
}

Estudante* CadastrarEstudante(Usuarios* usuarios, char nome[], char matricula[], char senha[])
{
    Estudante* estudante = (Estudante*)malloc(sizeof(Estudante));

    if (not estudante)
    {
        printf("\nErro ao alocar memória!\n");
        return NULL;
    }

    strcpy(estudante->nome, nome);
    strcpy(estudante->matricula, matricula);
    strcpy(estudante->senha, senha);
    estudante->livrosSobPosse = NULL;

    estudante->prox = usuarios->inicio;
    usuarios->inicio = estudante;

    return estudante;
}

Usuarios* CarregarUsuarios(const char* nomeArquivo)
{
    FILE* arquivo = fopen(nomeArquivo, "r");
    
    if (not arquivo) 
    {
        perror("\nErro ao abrir arquivo");
        return NULL;
    }

    Usuarios* usuarios = CriarUsuarios();

    if (not usuarios) 
    {
        fclose(arquivo);
        return NULL;
    }

    char linha[256];

    while (fgets(linha, sizeof(linha), arquivo)) 
    {
        char* nome = strtok(linha, ",");
        char* matricula = strtok(NULL, ",");
        char* senha = strtok(NULL, "\n");

        if (nome and matricula and senha) 
        {
            Estudante* estudante = CadastrarEstudante(usuarios, nome, matricula, senha);
        }
    }

    fclose(arquivo);

    return usuarios;
}

void SalvarUsuarios(const Usuarios* usuarios, const char* nomeArquivo)
{
    if (!usuarios || !nomeArquivo) 
    {
        printf("\nErro: Parâmetros inválidos para salvar usuários!\n");
        return;
    }

    FILE* arquivo = fopen(nomeArquivo, "w");

    if (!arquivo) 
    {
        perror("\nErro ao abrir arquivo para salvar usuários");
        return;
    }

    Estudante* atual = usuarios->inicio;

    while (atual) 
    {
        fprintf(arquivo, "%s,%s,%s\n", atual->nome, atual->matricula, atual->senha);
        
        atual = atual->prox;
    }

    fclose(arquivo);
}

Estudante* LoginEstudante(const Usuarios* usuarios, char matricula[], char senha[])
{
    struct Estudante* estudante = usuarios->inicio;
    bool estudanteEncontrado = false;

    if (not estudante)
    {
        printf("\nNenhum estudante cadastrado!\n");
        exit(1);
    }

    while (estudante)
    {
        if (strcmp(estudante->matricula, matricula) == 0 and strcmp(estudante->senha, senha) == 0)
        {
            estudanteEncontrado = true;
            
            printf("\nSeja bem-vindo(a) %s!\n", estudante->nome);

            return estudante;
        }

        estudante = estudante->prox;
    }
    
    if (not estudanteEncontrado)
    {
        printf("\nNúmero de matrícula e/ou senha incorretos! Tente novamente!\n");
        return NULL;
    }
}

void ExcluirEstudante(Usuarios* usuarios, char matricula[])
{
    struct Estudante* estudante = usuarios->inicio, *anterior = NULL;

    if (not estudante)
    {
        printf("\nNenhum livro cadastrado!\n");
        return;
    }

    while (estudante != NULL and strcmp(estudante->matricula, matricula) != 0)
    {
        anterior = estudante;
        estudante = estudante->prox;
    }
    
    if (not estudante)
    {
        printf("\nNenhum estudante de matrícula \"%s\" encontrado!\n", matricula);
        return;
    }
    
    if (not anterior)
    {
        usuarios->inicio = estudante->prox;
    }

    else
    {
        anterior->prox = estudante->prox;
    }
    
    free(estudante);

    printf("\nEstudante \"%s\" excluído com sucesso!\n", matricula);
}

void ImprimirUsuarios(const Usuarios* usuarios)
{
    struct Estudante* estudante = usuarios->inicio;

    if (not estudante)
    {
        printf("\nNenhum usuário cadastrado!\n");
        return;
    }
    
    printf("\n===== USUÁRIOS CADASTRADOS =====\n");

    while (estudante)
    {
        printf("Nome: %s\nMatrícula: %s\n\n", estudante->nome, estudante->matricula);

        estudante = estudante->prox;
    }
    
    printf("============================\n");
}

void DestruirUsuarios(Usuarios* usuarios)
{
    struct Estudante* estudante = usuarios->inicio;

    while (estudante)
    {
        struct Estudante* temp = estudante;
        estudante = estudante->prox;

        free(temp);
    }
    
    free(usuarios);
}

void VerificarPrazos(const Estudante* estudante) 
{
    time_t horaAtual = time(NULL);
    LivrosSobPosse* livrosSobPosse = estudante->livrosSobPosse;
    
    while(livrosSobPosse) 
    {
        double diasRestantes = difftime(livrosSobPosse->dataDevolucao, horaAtual) / 86400;
        
        if(diasRestantes < 0) 
        {
            printf("[ATRASADO] Livro %s: %.0f dias de atraso\n", livrosSobPosse->codigo, -diasRestantes);
        }

        else 
        {
            printf("Livro %s: %.0f dias restantes\n", livrosSobPosse->codigo, diasRestantes);
        }
        
        livrosSobPosse = livrosSobPosse->prox;
    }
}

void ImprimirData(time_t dataDevolucao) 
{
    struct tm *data = localtime(&dataDevolucao);

    printf("%02d/%02d/%04d", data->tm_mday, data->tm_mon+1, data->tm_year+1900);
}

void DevolverLivro(Acervo* acervo, Usuarios* usuarios, const char* matricula, char* codigo) 
{

    if (!acervo || !usuarios || !matricula || !codigo) 
    {
        printf("\nErro: Parâmetros inválidos para devolução!\n");
        return;
    }

    Estudante* estudante = BuscarEstudantePorMatricula(usuarios, matricula);

    if (!estudante) 
    {
        printf("\nEstudante com matrícula %s não encontrado!\n", matricula);
        return;
    }

    Livro* livro = BuscarLivro(acervo, codigo);

    if (!livro) 
    {
        printf("\nLivro com código %s não encontrado no acervo!\n", codigo);
        return;
    }

    LivrosSobPosse* anterior = NULL;
    LivrosSobPosse* atual = estudante->livrosSobPosse;
    bool livroEncontrado = false;

    while (atual) 
    {
        if (strcmp(atual->codigo, codigo) == 0) 
        {
            if (anterior) 
            {
                anterior->prox = atual->prox;
            } 
            
            else 
            {
                estudante->livrosSobPosse = atual->prox;
            }

            livro->exemplares++;
            free(atual);
            livroEncontrado = true;
            break;
        }

        anterior = atual;
        atual = atual->prox;
    }

    if (livroEncontrado) 
    {
        printf("\nLivro \"%s\" devolvido com sucesso por %s!\n", livro->titulo, estudante->nome);
        
        time_t agora = time(NULL);

        if (agora > atual->dataDevolucao) 
        {
            double diasAtraso = difftime(agora, atual->dataDevolucao) / 86400;
            printf("ATENÇÃO: Devolução com atraso de %.0f dias!\n", diasAtraso);
        }

    } 
    
    else 
    {
        printf("\nO estudante %s não possui o livro %s emprestado!\n", estudante->nome, livro->titulo);
    }
}

void RenovarLivro(Estudante* estudante, const char* codigo, int diasAdicionais) 
{

    if (!estudante || !codigo || diasAdicionais <= 0) 
    {
        printf("\nErro: Parâmetros inválidos para renovação!\n");
        return;
    }

    LivrosSobPosse* livro = estudante->livrosSobPosse;
    bool livroEncontrado = false;

    while (livro) 
    {
        if (strcmp(livro->codigo, codigo) == 0) 
        {
            time_t agora = time(NULL);

            if (agora > livro->dataDevolucao) 
            {
                printf("\nNão é possível renovar livro com devolução atrasada!\n");
                return;
            }

            livro->dataDevolucao += (diasAdicionais * 86400);
            
            char dataAntiga[20], dataNova[20];
            strftime(dataAntiga, 20, "%d/%m/%Y", localtime(&agora));
            strftime(dataNova, 20, "%d/%m/%Y", localtime(&livro->dataDevolucao));
            
            printf("\nLivro renovado com sucesso!\n");
            printf("Novo prazo de devolução: %s (Adicionados %d dias)\n", dataNova, diasAdicionais);
            livroEncontrado = true;
            break;
        }

        livro = livro->prox;
    }

    if (!livroEncontrado) 
    {
        printf("\nO estudante não possui o livro com código %s emprestado!\n", codigo);
    }
}

void LivrosEmMaos(const Estudante* estudanteAtual) 
{
    LivrosSobPosse* livroAtual = estudanteAtual->livrosSobPosse;

    if (not livroAtual) 
    {
        printf("\nO estudante \"%s\" não possui livros em mãos.\n", estudanteAtual->nome);
        return;
    }

    printf("\n=== Livros em posse de \"%s\" ===\n", estudanteAtual->nome);

    while(livroAtual) 
    {
        printf("Código: %s | Emprestado em: ", livroAtual->codigo);
        ImprimirData(livroAtual->dataEmprestimo);
        printf(" | Devolução até: ");
        ImprimirData(livroAtual->dataDevolucao);
        
        double dias = difftime(livroAtual->dataDevolucao, time(NULL)) / 86400;

        if(dias < 0) printf(" (ATRASADO)");
        
        printf("\n");
        livroAtual = livroAtual->prox;
    }
}

void SalvarEmprestimos(const Usuarios* usuarios) 
{
    FILE* arquivo = fopen("emprestimos.csv", "w");
    
    if (not arquivo) 
    {
        perror("\nErro ao abrir arquivo de empréstimos");
        return;
    }

    Estudante* estudante = usuarios->inicio;

    while (estudante) 
    {
        LivrosSobPosse* livro = estudante->livrosSobPosse;

        while (livro) 
        {
            fprintf(arquivo, "%s,%s,%ld,%ld\n", estudante->matricula, livro->codigo, livro->dataEmprestimo, livro->dataDevolucao);
            livro = livro->prox;
        }

        estudante = estudante->prox;
    }

    fclose(arquivo);
}

void CarregarEmprestimos(Usuarios* usuarios, Acervo* acervo) 
{
    if (!usuarios || !acervo) 
    {
        printf("Erro: Usuários ou acervo inválidos\n");
        return;
    }

    FILE* arquivo = fopen("emprestimos.csv", "r");
    if (!arquivo) 
    {
        return;
    }

    char linha[256];
    int contador = 0;

    while (fgets(linha, sizeof(linha), arquivo)) 
    {
        char* matricula = strtok(linha, ",");
        char* codigo = strtok(NULL, ",");
        char* strEmprestimo = strtok(NULL, ",");
        char* strDevolucao = strtok(NULL, "\n");

        if (!matricula || !codigo || !strEmprestimo || !strDevolucao) 
        {
            printf("Formato inválido na linha: %s\n", linha);
            continue;
        }

        time_t dataEmprestimo = (time_t)atol(strEmprestimo);
        time_t dataDevolucao = (time_t)atol(strDevolucao);

        Estudante* estudante = BuscarEstudantePorMatricula(usuarios, matricula);
        if (!estudante) 
        {
            printf("Estudante não encontrado: %s\n", matricula);
            continue;
        }

        LivrosSobPosse* novo = (LivrosSobPosse*)malloc(sizeof(LivrosSobPosse));

        if (!novo) 
        {
            perror("Erro ao alocar memória");
            continue;
        }

        strncpy(novo->codigo, codigo, sizeof(novo->codigo) - 1);
        novo->codigo[sizeof(novo->codigo) - 1] = '\0';
        novo->dataEmprestimo = dataEmprestimo;
        novo->dataDevolucao = dataDevolucao;
        novo->prox = estudante->livrosSobPosse;
        estudante->livrosSobPosse = novo;
        contador++;
    }

    fclose(arquivo);
}

Estudante* BuscarEstudantePorMatricula(Usuarios* usuarios, const char* matricula) 
{
    Estudante* atual = usuarios->inicio;

    while (atual) 
    {
        if (strcmp(atual->matricula, matricula) == 0) 
        {
            return atual;
        }

        atual = atual->prox;
    }

    return NULL;
}

void OrdenarAcervoAlfabeticamente(Acervo* acervo) {
    // Verifica se a lista está vazia ou tem apenas um elemento
    if (!acervo || !acervo->inicio || !acervo->inicio->prox) {
        return;
    }

    int trocado;
    Livro* atual;
    Livro* anterior = NULL;
    
    do {
        trocado = 0;
        atual = acervo->inicio;
        
        while (atual->prox != anterior) {
            // Compara os títulos dos livros adjacentes
            if (strcmp(atual->titulo, atual->prox->titulo) > 0) {
                // Troca os nós
                Livro* temp = atual->prox;
                atual->prox = temp->prox;
                temp->prox = atual;
                
                if (atual == acervo->inicio) {
                    acervo->inicio = temp;
                } else {
                    // Precisa encontrar o nó anterior para ajustar seu ponteiro
                    Livro* prev = acervo->inicio;
                    while (prev->prox != atual) {
                        prev = prev->prox;
                    }
                    prev->prox = temp;
                }
                
                atual = temp;
                trocado = 1;
            }
            
            atual = atual->prox;
        }
        
        anterior = atual;
    } while (trocado);
}

void limparBuffer()
{   
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
