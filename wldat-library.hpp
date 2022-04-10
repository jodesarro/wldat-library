#pragma once

#include <fstream>
#include <complex>
#include <regex>

void wldat_getsize( std::string wldat_path, int &imax, int &jmax, int &kmax )
{
    std::fstream wldat_file;
    wldat_file.open( wldat_path, std::fstream::in );

    char ch;
    char last_ch = '\0';
    int comma = 0;
    int brace1 = 0;
    int brace2 = 0;

    while ( wldat_file >> ch )
    {
        if ( ch == ',' )
        {
            comma++;
        }
        else if ( ch == '{' )
        {
            brace1++;
            if ( last_ch == '{' )
            {
                brace2++;
            }
        }
        last_ch = ch;
    }

    wldat_file.close();

    imax = brace2 - 1;
    jmax = int( (brace1 -  brace2)/imax );
    kmax = int( (comma + 1)/(imax*jmax) );
}

template<typename T>
void wldat_export( std::string wldat_path, T * data_array, int imax, int jmax, int kmax, int out_precision = 0, bool out_scientific = false )
{    
    std::fstream wldat_file;
    wldat_file.open( wldat_path, std::fstream::out );
    wldat_file << "(*" << " Created with the https://github.com/jodesarro/wldat-library " << "*)" << std::endl;
    wldat_file << "{";
    for ( int i=0; i<imax; i++ )
    {
        wldat_file << "{";
        for ( int j=0; j<jmax; j++ )
        {
            wldat_file << "{";
            for ( int k=0; k<kmax; k++ )
            {
                std::stringstream number;
                if ( out_precision != 0 )
                {
                    number.precision(out_precision);
                }
                if ( out_scientific )
                {
                    number << std::scientific;
                }
                number << data_array[i + imax*(j + jmax*k)];
                wldat_file << std::regex_replace(number.str(), std::regex{"e"}, "*^");
                ( k == kmax-1 ) ? ( wldat_file << "}") : ( wldat_file << ",");
            }
            ( j == jmax-1 ) ? ( wldat_file << "}") : ( wldat_file << ",") ;
        }
        ( i == imax-1 ) ? ( wldat_file << "}") : ( wldat_file << ",") ;
    }
    wldat_file.close();
}

template<typename T>
void wldat_export( std::string wldat_path, std::complex<T> * data_array, int imax, int jmax, int kmax, int out_precision = 0, bool out_scientific = false )
{    
    std::fstream wldat_file;
    wldat_file.open( wldat_path, std::fstream::out );
    wldat_file << "(*" << " Created with the https://github.com/jodesarro/wldat-library " << "*)" << std::endl;
    wldat_file << "{";
    for ( int i=0; i<imax; i++ )
    {
        wldat_file << "{";
        for ( int j=0; j<jmax; j++ )
        {
            wldat_file << "{";
            for ( int k=0; k<kmax; k++ )
            {
                std::stringstream number;
                number << std::showpos;
                if ( out_precision != 0 )
                {
                    number.precision(out_precision);
                }
                if ( out_scientific )
                {
                    number << std::scientific;
                }
                number << std::real(data_array[i + imax*(j + jmax*k)]) << std::imag(data_array[i + imax*(j + jmax*k)]) << "*I";
                wldat_file << std::regex_replace(number.str(), std::regex{"e"}, "*^");
                ( k == kmax-1 ) ? ( wldat_file << "}") : ( wldat_file << ",");
            }
            ( j == jmax-1 ) ? ( wldat_file << "}") : ( wldat_file << ",") ;
        }
        ( i == imax-1 ) ? ( wldat_file << "}") : ( wldat_file << ",") ;
    }
    wldat_file.close();
}

template<typename T>
void wldat_import( std::string wldat_path, T * data_array, int imax, int jmax, int kmax )
{    
    std::fstream wldat_file;
    wldat_file.open( wldat_path, std::fstream::in );

    char ch;
    int i = 0;
    int j = 0;
    int k = 0;
    std::string number;
    bool ignore_char = false;
    bool ignore_comment = false;

    while ( wldat_file.get(ch) )
    {
        
        // Dealing with the char.
        if ( ch == '(' )
        {
            // A possible start of a comment.
            wldat_file.get(ch);
            if ( ch == '*' )
            {
                // It is a start of a comment,
                //  ignoring chars inside it.
                ignore_comment = true;
            }
            else
            {
                // It was not the start of a comment,
                //  treating '(' as any character.
                ch == '(';
                wldat_file.unget();
            }
        }
        else if ( ch == '*' )
        {
            // A possible end of a comment or
            //  a scientific form.
            wldat_file.get(ch);
            if ( ch == ')' )
            {
                // The end of a comment.
                ignore_comment = false;
                wldat_file.get(ch);
            }
            else if ( ch == '^' )
            {
                // Scientific form.
                ch = 'e';
            }
        }
        else if ( ch == '{' || ch == ' ' || ch == '}' )
        {
            // It is an useless char in this algorithm,
            //  ignoring it.
            ignore_char = true;
        }
        else if ( ch == ',' )
        {
            // It is an useless char for the data in this algotithm,
            //  ignoring it.
            ignore_char = true;

            // Storing the number.
            data_array[i + imax*(j + jmax*k)] = std::stod( number );
            number.clear();

            // Iterating the indexes
            k++;
            if ( k == kmax )
            {
                k = 0;
                j++;
                if ( j == jmax )
                {
                    j = 0;
                    i++;
                    if ( i == imax )
                    {
                        // It shoud be an error.
                        i = j = k = 0;
                    }
                }
            }
        }

        if ( !ignore_comment && !ignore_char )
        {
            // Adding char to number string.
            number += std::string(1, ch);
        }

        ignore_char = false;
    }

    // Storing last number.
    data_array[i + imax*(j + jmax*k)] = std::stod( number );
    number.clear();

    wldat_file.close();
}
