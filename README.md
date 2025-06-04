# WLdat library: A set of C ++ functions to handle numeric data of files in Wolfram Language package source format

<p align="right"><a href="README.pt-br.md">Leia em português (br)</a></p>

A set of C++ functions for handling numeric data in three dimensions from files in [Wolfram Language package source format (WL)](https://reference.wolfram.com/language/ref/format/WL.html).

WL files have a structure in which data are separated by commas and arrays are grouped by braces. Such format is very useful for importing and exporting tensors, matrices, tables, numeric datasets and so on.

## Available functions

### Reading and importing data

- Get the dimensions of a WL file containing a 3D set of numerical data
```
void wldat_getsize( std::string wldat_path, int &imax, int &jmax, int &kmax )
```

- Import a WL file containing a 3D set of numeric data and store the values in an one-dimensional array
```
template<typename T>
void wldat_import( std::string wldat_path, T * data_array, int imax, int jmax, int kmax )
```

- Import a WL file containing a 3D set of numeric complex data and store the values in an one-dimensional complex array
```
template<typename T>
void wldat_import( std::string wldat_path, std::complex<T> * data_array, int imax, int jmax, int kmax )
```

- Import a WL file containing a 3D set of numeric data and store the values in an three-dimensional vector
```
template<typename T>
void wldat_import( std::string wldat_path, std::vector<std::vector<std::vector<T>>> &data_vector, int imax, int jmax, int kmax )
```

- Import a WL file containing a 3D set of numeric complex data and store the values in an three-dimensional complex vector
```
template<typename T>
void wldat_import( std::string wldat_path, std::vector<std::vector<std::vector<std::complex<T>>>> &data_vector, int imax, int jmax, int kmax )
```

### Exporting data

- Export an one-dimensional array containing a 3D set of numeric data to a WL file
```
template<typename T>
void wldat_export( std::string wldat_path, T * data_array, int imax, int jmax, int kmax, int out_precision = 0, bool out_scientific = false )
```

- Export an one-dimensional complex array containing a 3D set of numeric complex data to a WL file
```
template<typename T>
void wldat_export( std::string wldat_path, std::complex<T> * data_array, int imax, int jmax, int kmax, int out_precision = 0, bool out_scientific = false )
```

- Export a three-dimensional vector containing a 3D set of numeric data to a WL file
```
template<typename T>
void wldat_export( std::string wldat_path, std::vector<std::vector<std::vector<T>>> &data_vector, int imax, int jmax, int kmax, int out_precision = 0, bool out_scientific = false )
```

- Export a three-dimensional complex vector containing a 3D set of numeric complex data to a WL file
```
template<typename T>
void wldat_export( std::string wldat_path, std::vector<std::vector<std::vector<std::complex<T>>>> &data_vector, int imax, int jmax, int kmax, int out_precision = 0, bool out_scientific = false )
```

## How to use

The library is in a header-only library style, i.e., there is nothing to build, you only have to include the <a href="wldat-library.hpp">*wldat-library.hpp*</a> file into your project.

## Authorship

The codes and routines were developed and are updated by <a href="https://www.researchgate.net/profile/Jhonas-de-Sarro">Jhonas O. de Sarro</a> ([@jodesarro]( https://github.com/jodesarro )).

## Licensing

This project is protected under <a href="LICENSE">MIT License</a>. 