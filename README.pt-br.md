# Biblioteca para conjuntos de dados numéricos em formato de arquivo WL

<p align="right"><a href="README.md">Read in English</a></p>

Um conjunto de funções em C++ para lidar com dados numéricos em três dimensões de arquivos em [Wolfram Language package source format (WL)](https://reference.wolfram.com/language/ref/format/WL.html).

Os arquivos WL têm uma estrutura na qual os dados são separados por vírgulas e as matrizes são agrupadas por chaves.

É muito útil para importar e exportar tensores, matrizes, tabelas, conjuntos de dados numéricos e assim por diante.

## Funções disponíveis

- Obter as dimensões de um arquivo WL contendo um conjunto 3D de dados numéricos
```
void wldat_getsize( std::string wldat_path, int &imax, int &jmax, int &kmax )
```

- Importar um arquivo WL contendo um conjunto 3D de dados numéricos e armazenar os valores em um array de uma dimensão
```
template<typename T>
void wldat_import( std::string wldat_path, T * data_array, int imax, int jmax, int kmax )
```

- Exportar um array de uma dimensão contendo um conjunto 3D de dados numéricos para um arquivo WL 
```
template<typename T>
void wldat_export( std::string wldat_path, T * data_array, int imax, int jmax, int kmax, int out_precision = 0, bool out_scientific = false )
```
```
template<typename T>
void wldat_export( std::string wldat_path, std::complex<T> * data_array, int imax, int jmax, int kmax, int out_precision = 0, bool out_scientific = false )
```

## Como usar

A bibliotea está em estilo *header-only* (apenas cabeçalho), ou seja, não é necessário compilá-la separadamente, você só precisa incluir o arquivo <a href="wldat-library.hpp">*wldat-library.hpp*</a> no seu projeto.

## Licença

Este projeto está protegido sob a licença <a href="LICENSE">MIT License</a> e tem [@jodesarro](https://github.com/jodesarro) como seu principal autor.
