#include <stdio.h>
#include <string.h>
#include "biblioteca.c"

#define DIAS_PRAZO 15

int main()
{
    Funcionarios* funcionarios = CarregarAtendentes("funcionarios.csv");
    Usuarios* usuarios = CarregarUsuarios("usuarios.csv");
    Acervo* acervo = CarregarAcervo("acervo.csv");
    CarregarEmprestimos(usuarios, acervo);

    Atendente* atendenteAtual;
    Estudante* estudanteAtual;

    int opcaoMenuPrincipal;
    bool menuPrincipal = true;

    do
    {
        printf("\n===== BIBLIOTECA VIRTUAL =====\n");
        printf("1.........Atendente\n");
        printf("2.........Estudante\n");
        printf("0.........Sair\n");
        printf("\nQuem está acessando: ");
        scanf("%d", &opcaoMenuPrincipal);
        limparBuffer();

        switch (opcaoMenuPrincipal)
        {
            case 1:
                int opcaoPainelDoAtendente;
                char cpf[50], senha[20];
                bool loginAtendente = true, painelDoAtendente = true;

                while (loginAtendente)
                {
                    printf("\nCPF: ");
                    fgets(cpf, sizeof(cpf), stdin);
                    cpf[strcspn(cpf, "\n")] = '\0';
                
                    printf("Senha: ");
                    fgets(senha, sizeof(senha), stdin);
                    senha[strcspn(senha, "\n")] = '\0';

                    atendenteAtual = LoginAtendente(funcionarios, cpf, senha);

                    if (atendenteAtual)
                    {
                        loginAtendente = false;
                    }
                }
                
                while (painelDoAtendente)
                {
                    printf("\n=== PAINEL DO ATENDENTE ===\n");
                    printf("1.........Cadastrar Livro\n");
                    printf("2.........Excluir Livro\n");
                    printf("3.........Consultar Acervo\n");
                    printf("4.........Usuários Cadastrados\n");
                    printf("5.........Funcionários Cadastrados\n");
                    printf("6.........Voltar a Menu Principal\n");
                    printf("0.........Sair\n");
                    printf("\nEscolha uma opção: ");
                    scanf("%d", &opcaoPainelDoAtendente);
                    limparBuffer();

                    switch (opcaoPainelDoAtendente)
                    {
                        case 1:
                            int ano, exemplares;    
                            char titulo[50], autor[50], codigo[20];

                            printf("\nForneça o título do livro: ");
                            fgets(titulo, sizeof(titulo), stdin);
                            titulo[strcspn(titulo, "\n")] = '\0';

                            printf("\nForneça o autor: ");
                            fgets(autor, sizeof(autor), stdin);
                            autor[strcspn(autor, "\n")] = '\0';

                            printf("\nForneça o ano: ");
                            scanf("%d", &ano);

                            printf("\nForneça a quantidade de exemplares: ");
                            scanf("%d", &exemplares);

                            limparBuffer();

                            printf("\nForneça o código: ");
                            fgets(codigo, sizeof(codigo), stdin);
                            codigo[strcspn(codigo, "\n")] = '\0';

                            CadastrarLivro(acervo, titulo, autor, ano, exemplares, codigo);

                            break;

                        case 2:
                            printf("\nForneça o código: ");
                            fgets(codigo, sizeof(codigo), stdin);
                            codigo[strcspn(codigo, "\n")] = '\0';

                            ExcluirLivro(acervo, codigo);

                            break;

                        case 3:
                            OrdenarAcervoAlfabeticamente(acervo);
                            ImprimirAcervo(acervo);
                
                            break;

                        case 4:
                            ImprimirUsuarios(usuarios);

                            break;

                        case 5:
                            ImprimirFuncionarios(funcionarios);

                            break;

                        case 6:
                            painelDoAtendente = false;
                            break;

                        case 0:
                            painelDoAtendente = false;
                            menuPrincipal = false;
                            break;

                        default:
                            printf("\nOpção inválida! Escolha entre 0 e 8!\n");
                            break;
                    }
                }

                break;
        
            case 2:
                if (atendenteAtual == NULL)
                {
                    printf("\nNenhum atendente disponível!\n");
                    break;
                }
                
                int opcaoPainelDoEstudante;
                char matricula[50], senhaTemp[20], codigo[20];;
                bool loginUsuario = true, painelDoEstudante = true;
                Estudante* estudanteAtual = NULL;

                while (loginUsuario)
                {
                    printf("\nMatrícula: ");
                    fgets(matricula, sizeof(matricula), stdin);
                    matricula[strcspn(matricula, "\n")] = '\0';
                
                    printf("Senha: ");
                    fgets(senhaTemp, sizeof(senhaTemp), stdin);
                    senhaTemp[strcspn(senhaTemp, "\n")] = '\0';

                    estudanteAtual = LoginEstudante(usuarios, matricula, senhaTemp);

                    if (estudanteAtual)
                    {
                        loginUsuario = false;
                    }
                }

                while (painelDoEstudante)
                {
                    printf("\n=== PAINEL DO ESTUDANTE ===\n");
                    printf("1.........Empréstimo de Livros\n");
                    printf("2.........Devolução de Livros\n");
                    printf("3.........Renovação de Livros\n");
                    printf("4.........Livros Emprestados\n");
                    printf("5.........Consultar Acervo\n");
                    printf("6.........Pesquisar por Livro\n");
                    printf("7.........Voltar a Menu Principal\n");
                    printf("0.........Sair\n");
                    printf("\nEscolha uma opção: ");
                    scanf("%d", &opcaoPainelDoEstudante);
                    limparBuffer();

                    switch (opcaoPainelDoEstudante)
                    {
                        case 1:
                            printf("\nForneça o código: ");
                            fgets(codigo, sizeof(codigo), stdin);
                            codigo[strcspn(codigo, "\n")] = '\0';

                            EmprestarLivro(acervo, estudanteAtual, atendenteAtual, codigo, DIAS_PRAZO);
                            break;
                    
                        case 2:
                            printf("\nForneça o código: ");
                            fgets(codigo, sizeof(codigo), stdin);
                            codigo[strcspn(codigo, "\n")] = '\0';

                            DevolverLivro(acervo, usuarios, estudanteAtual->matricula, codigo);
                            break;

                        case 3:
                            printf("\nForneça o código: ");
                            fgets(codigo, sizeof(codigo), stdin);
                            codigo[strcspn(codigo, "\n")] = '\0';

                            RenovarLivro(estudanteAtual, codigo, DIAS_PRAZO);
                            break;

                        case 4:
                            LivrosEmMaos(estudanteAtual);

                            break;

                        case 5:
                            OrdenarAcervoAlfabeticamente(acervo);
                            ImprimirAcervo(acervo);

                            break;

                        case 6:
                            printf("\nForneça o código: ");
                            fgets(codigo, sizeof(codigo), stdin);
                            codigo[strcspn(codigo, "\n")] = '\0';

                            BuscarLivro(acervo, codigo);

                            break;

                        case 7:
                            painelDoEstudante = false;

                            break;
                        
                        case 0:
                            painelDoEstudante = false;
                            menuPrincipal = false;

                            break;

                        default:
                        printf("\nOpção inválida! Escolha entre 0 e 8!\n");
                            break;
                    }
                }
                
                break;

            case 0:
                menuPrincipal = false;
                break;

            default:
                printf("\nOpção inválida! Escolha entre 0 e 2!\n");
                break;
        }

    } while (menuPrincipal);
    
    SalvarEmprestimos(usuarios);
    SalvarAcervo(acervo, "acervo.csv");
    SalvarUsuarios(usuarios, "usuarios.csv");
    SalvarFuncionarios(funcionarios, "funcionarios.csv");

    DestruirFuncionarios(funcionarios);
    DestruirUsuarios(usuarios);
    DestruirBiblioteca(acervo);

    return 0;
}