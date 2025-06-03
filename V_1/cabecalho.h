#ifndef CABECALHO_H
#define CABECALHO_H

// Estruturas opacas para "usuário"
// ------------------------------------------------------------------------
typedef struct Usuarios Usuarios;
typedef struct Estudante Estudante;
typedef struct LivrosSobPosse LivrosSobPosse;
// ------------------------------------------------------------------------

// Estruturas opacas para "atendente"
// ------------------------------------------------------------------------
typedef struct Funcionarios Funcionarios;
typedef struct Atendente Atendente;
typedef struct Acervo Acervo;
typedef struct Livro Livro;
// ------------------------------------------------------------------------

// Definição das funções utilizadas em "usuário"
// ------------------------------------------------------------------------
Usuarios* CriarUsuarios();
Usuarios* CarregarUsuarios(const char* nomeArquivo);
Estudante* CadastrarEstudante(Usuarios* usuarios, char nome[], char matricula[], char senha[]);
Estudante* LoginEstudante(const Usuarios* usuarios, char matricula[], char senha[]);
void DestruirUsuarios(Usuarios* usuarios);
void EmprestarLivro(Acervo* acervo, Estudante* estudanteAtual, Atendente* atendenteAtual, char codigo[], int diasPrazo);
void DevolverLivro(Acervo* acervo, Usuarios* usuarios, const char* matricula, char* codigo);
void RenovarLivro(Estudante* estudante, const char* codigo, int diasAdicionais);
void LivrosEmMaos(const Estudante* estudanteAtual);
Estudante* BuscarEstudantePorMatricula(Usuarios* usuarios, const char* matricula);
// ------------------------------------------------------------------------

// Definição das funções utilizadas em "atendente"
// ------------------------------------------------------------------------
Funcionarios* CriarAtendentes();
Atendente* CadastrarAtendente(Funcionarios* funcionarios, char nome[], char cpf[], char senha[]);
Funcionarios* CarregarAtendentes(const char* nomeArquivo);
void SalvarFuncionarios(const Funcionarios* funcionarios, const char* nomeArquivo);
Atendente* LoginAtendente(const Funcionarios* funcionarios, char cpf[], char senha[]);
void ImprimirFuncionarios(const Funcionarios* funcionarios);
void DestruirFuncionarios(Funcionarios* funcionarios);

Acervo* CriarAcervo();
Acervo* CarregarAcervo(const char* nomeArquivo);
void SalvarAcervo(const Acervo* acervo, const char* nomeArquivo);
Livro* CadastrarLivro(Acervo* acervo, char titulo[], char autor[], int ano, int exemplares, char codigo[]);
Livro* BuscarLivro(const Acervo* acervo, char codigo[]);
void ExcluirLivro(Acervo* acervo, char codigo[]);
void ImprimirAcervo(const Acervo* acervo);
void ImprimirUsuarios(const Usuarios* usuarios);
void SalvarUsuarios(const Usuarios* usuarios, const char* nomeArquivo);
void ImprimirLivrosEmprestados(const char* nomeArquivo);
void LimparBuffer();
void DestruirBiblioteca(Acervo* acervo);
// ------------------------------------------------------------------------

void OrdenarAcervoAlfabeticamente(Acervo* acervo);

#endif