#include <fstream>
#include <complex>
#include <regex>
#include <string>

void _string_find_replace(std::string &_the_string, const std::string &_string_to_search, const std::string &_string_to_replace)
   {
    size_t pos = 0;
    while ((pos = _the_string.find(_string_to_search, pos)) != std::string::npos) {
        _the_string.replace(pos, _string_to_search.length(), _string_to_replace);
        pos += _string_to_replace.length();
    }
}

template<typename T>
std::complex<T> _wldata_to_cpp(std::string _wldata, T _nothing)
{
    std::string wldata = _wldata;
    _string_find_replace(wldata, "*^-", "em");
    _string_find_replace(wldata, "*^+", "ep");
    _string_find_replace(wldata, "*^", "ep");
    _string_find_replace(wldata, " ", "");
    
    std::string real_part = "";
    std::string imag_part = "";
    bool is_the_imag_part = false;
    
    for (char ch : wldata)
    {
        if ( ( ch == '+' || ch == '-' ) && real_part != "" )
        {
            is_the_imag_part = true;
        }
        else if (ch == '*')
        {
            if ( !is_the_imag_part )
            {
                imag_part = real_part;
                real_part = "";
            }
            break; // Skip '*I'
        }

        if ( is_the_imag_part )
        {
            imag_part += std::string(1, ch);
        }
        else
        {
            real_part += std::string(1, ch);
        }
    }

    _string_find_replace(real_part, "em", "e-");
    _string_find_replace(imag_part, "em", "e-");
    _string_find_replace(real_part, "ep", "e+");
    _string_find_replace(imag_part, "ep", "e+");

    if ( real_part == "" )
    {
        real_part = "0";
    }
    if ( imag_part == "" )
    {
        imag_part = "0";
    }

    return std::complex<T>( std::stod(real_part), std::stod(imag_part) );
}

template<typename T>
void _wldat_import( std::string _wldat_path, T * _real_array, std::complex<T> * _complex_array,
    std::vector<std::vector<std::vector<T>>> &_real_vector, std::vector<std::vector<std::vector<std::complex<T>>>> &_complex_vector,
    bool _is_complex, bool _is_vector, int _imax, int _jmax, int _kmax )
{    
    std::fstream wldat_file;
    wldat_file.open( _wldat_path, std::fstream::in );
    char ch;
    int i = 0; int j = 0; int k = 0;
    std::string the_data = "";
    bool ignore_char = false;
    while ( wldat_file.get(ch) )
    {
        // Dealing with the char.
        if ( ch == '{' || ch == '}' || ch == ' ' || ch == '\n' || ch == '\t' )
        {
            ignore_char = true;
        }
        else if ( ch == '(' )
        {
            // A possible start of a comment.
            wldat_file.get(ch);
            if ( ch == '*' )
            {
                // It is the start of a comment.
                bool the_end_of_the_comments = false;
                int comments = 1;
                // Finding the end of the comment.
                while ( wldat_file.get(ch) && !the_end_of_the_comments )
                {
                    if ( ch == '(' )
                    {
                        // A possible nested comment.
                        wldat_file.get(ch);
                        if ( ch == '*' )
                        {
                            // A nested comment.
                            comments += 1;
                        }
                        //wldat_file.unget();
                        //ch == '(';
                    }
                    else if ( ch == '*' )
                    {
                        // A possible end of a (nested or not) comment.
                        wldat_file.get(ch);
                        if ( ch == ')' )
                        {
                            // End of a (nested or not) comment.
                            comments -= 1 ;
                            if ( comments == 0 )
                            {
                                the_end_of_the_comments = true;
                            }
                        }
                    }
                }
                wldat_file.unget();
                ch == ')';
                ignore_char = true;
            }
            else
            {
                // It is not the start of a comment,
                //  treating '(' as any character.
                wldat_file.unget();
                ch == '(';
            }
        }
        else if ( ch == ',' )
        {
            // Storing the the_data.
            if ( _is_complex )
            {
                if ( _is_vector )
                {
                    _complex_vector.at(i).at(j).at(k) = _wldata_to_cpp( the_data, T(0) );
                }
                else
                {
                    _complex_array[i + _imax*(j + _jmax*k)] = _wldata_to_cpp( the_data, T(0) );
                }
            }
            else
            {
                if ( _is_vector )
                {
                    _real_vector.at(i).at(j).at(k) = std::real( _wldata_to_cpp( the_data, T(0) ) );
                }
                else
                {
                    _real_array[i + _imax*(j + _jmax*k)] = std::real( _wldata_to_cpp( the_data, T(0) ) );
                }
            }
            the_data = "";

            // Iterating the indices
            k++;
            if ( k == _kmax )
            {
                k = 0;
                j++;
                if ( j == _jmax )
                {
                    j = 0;
                    i++;
                    if ( i == _imax )
                    {
                        // It shoud be an error.
                        i = j = k = 0;
                    }
                }
            }
            ignore_char = true;
        }
        
        if ( !ignore_char )
        {
            // Adding char to the_data string.
            the_data += std::string(1, ch);
        }
        else
        {
            ignore_char = false;
        }
    }

    // Storing the last the_data of the file.
    if ( _is_complex )
    {
        if ( _is_vector )
        {
            _complex_vector.at(i).at(j).at(k) = _wldata_to_cpp( the_data, T(0) );
        }
        else
        {
            _complex_array[i + _imax*(j + _jmax*k)] = _wldata_to_cpp( the_data, T(0) );
        }
    }
    else
    {
        if ( _is_vector )
        {
            _real_vector.at(i).at(j).at(k) = std::real( _wldata_to_cpp( the_data, T(0) ) );
        }
        else
        {
            _real_array[i + _imax*(j + _jmax*k)] = std::real( _wldata_to_cpp( the_data, T(0) ) );
        }
    }

    wldat_file.close();
}

template<typename T>
void _wldat_export( std::string _wldat_path, T * _real_array, std::complex<T> * _complex_array,
    std::vector<std::vector<std::vector<T>>> &_real_vector, std::vector<std::vector<std::vector<std::complex<T>>>> &_complex_vector,
    bool _is_complex, bool _is_vector, int _imax, int _jmax, int _kmax, int _out_precision = 0, bool _out_scientific = false )
{
    std::fstream wldat_file;
    wldat_file.open( _wldat_path, std::fstream::out );
    wldat_file << "(*" << " Created with C/C++ using the WLdat library: https://github.com/jodesarro/wldat-library " << "*)" << std::endl;
    wldat_file << "{";
    for ( int i=0; i<_imax; i++ )
    {
        wldat_file << "{";
        for ( int j=0; j<_jmax; j++ )
        {
            wldat_file << "{";
            for ( int k=0; k<_kmax; k++ )
            {
                std::stringstream the_data;
                if ( _out_precision != 0 )
                {
                    the_data.precision(_out_precision);
                }
                if ( _out_scientific )
                {
                    the_data << std::scientific;
                }
                the_data << std::showpos;
                if ( _is_complex )
                    {
                    if ( _is_vector )
                    {
                        the_data << std::real(_complex_vector.at(i).at(j).at(k)) << std::imag(_complex_vector.at(i).at(j).at(k)) << "*I";
                    }
                    else
                    {
                        the_data << std::real(_complex_array[i + _imax*(j + _jmax*k)]) << std::imag(_complex_array[i + _imax*(j + _jmax*k)]) << "*I";
                    }
                }
                else
                {
                    if ( _is_vector )
                    {
                        the_data << _real_vector.at(i).at(j).at(k);
                    }
                    else
                    {
                        the_data << _real_array[i + _imax*(j + _jmax*k)];
                    }
                }  
                wldat_file << std::regex_replace(the_data.str(), std::regex{"e"}, "*^");
                ( k == _kmax-1 ) ? ( wldat_file << "}") : ( wldat_file << ",");
            }
            ( j == _jmax-1 ) ? ( wldat_file << "}") : ( wldat_file << ",") ;
        }
        ( i == _imax-1 ) ? ( wldat_file << "}") : ( wldat_file << ",") ;
    }
    wldat_file.close();
}

template<typename T>
void wldat_import( std::string wldat_path, T * data_array, int imax, int jmax, int kmax )
{
    std::complex<T> empty_complex_array [1];
    std::vector<std::vector<std::vector<T>>> empty_real_vector(1, std::vector<std::vector<T>>(1, std::vector<T>(1, 0) ) );
    std::vector<std::vector<std::vector<std::complex<T>>>> empty_complex_vector(1, std::vector<std::vector<std::complex<T>>>(1, std::vector<std::complex<T>>(1, 0) ) );
    _wldat_import( wldat_path, data_array, empty_complex_array, empty_real_vector, empty_complex_vector, false, false, imax, jmax, kmax );
}

template<typename T>
void wldat_import( std::string wldat_path, std::complex<T> * data_array, int imax, int jmax, int kmax )
{
    T empty_real_array [1];
    std::vector<std::vector<std::vector<T>>> empty_real_vector(1, std::vector<std::vector<T>>(1, std::vector<T>(1, 0) ) );
    std::vector<std::vector<std::vector<std::complex<T>>>> empty_complex_vector(1, std::vector<std::vector<std::complex<T>>>(1, std::vector<std::complex<T>>(1, 0) ) );
    _wldat_import( wldat_path, empty_real_array, data_array, empty_real_vector, empty_complex_vector, true, false, imax, jmax, kmax );
}

template<typename T>
void wldat_import( std::string wldat_path, std::vector<std::vector<std::vector<T>>> &data_vector, int imax, int jmax, int kmax )
{
    T empty_real_array [1];
    std::complex<T> empty_complex_array [1];
    std::vector<std::vector<std::vector<std::complex<T>>>> empty_complex_vector(1, std::vector<std::vector<std::complex<T>>>(1, std::vector<std::complex<T>>(1, 0) ) );
    _wldat_import( wldat_path, empty_real_array, empty_complex_array, data_vector, empty_complex_vector, false, true, imax, jmax, kmax );
}

template<typename T>
void wldat_import( std::string wldat_path, std::vector<std::vector<std::vector<std::complex<T>>>> &data_vector, int imax, int jmax, int kmax )
{
    T empty_real_array [1];
    std::complex<T> empty_complex_array [1];
    std::vector<std::vector<std::vector<T>>> empty_real_vector(1, std::vector<std::vector<T>>(1, std::vector<T>(1, 0) ) );
    _wldat_import( wldat_path, empty_real_array, empty_complex_array, empty_real_vector, data_vector, true, true, imax, jmax, kmax );
}

template<typename T>
void wldat_export( std::string wldat_path, T * data_array, int imax, int jmax, int kmax, int out_precision = 0, bool out_scientific = false )
{
    std::complex<T> empty_complex_array [1];
    std::vector<std::vector<std::vector<T>>> empty_real_vector(1, std::vector<std::vector<T>>(1, std::vector<T>(1, 0) ) );
    std::vector<std::vector<std::vector<std::complex<T>>>> empty_complex_vector(1, std::vector<std::vector<std::complex<T>>>(1, std::vector<std::complex<T>>(1, 0) ) );
    _wldat_export( wldat_path, data_array, empty_complex_array, empty_real_vector, empty_complex_vector, false, false, imax, jmax, kmax, out_precision, out_scientific );
}

template<typename T>
void wldat_export( std::string wldat_path, std::complex<T> * data_array, int imax, int jmax, int kmax, int out_precision = 0, bool out_scientific = false )
{
    T empty_real_array [1];
    std::vector<std::vector<std::vector<T>>> empty_real_vector(1, std::vector<std::vector<T>>(1, std::vector<T>(1, 0) ) );
    std::vector<std::vector<std::vector<std::complex<T>>>> empty_complex_vector(1, std::vector<std::vector<std::complex<T>>>(1, std::vector<std::complex<T>>(1, 0) ) );
    _wldat_export( wldat_path, empty_real_array, data_array, empty_real_vector, empty_complex_vector, true, false, imax, jmax, kmax, out_precision, out_scientific );
}

template<typename T>
void wldat_export( std::string wldat_path, std::vector<std::vector<std::vector<T>>> &data_vector, int imax, int jmax, int kmax, int out_precision = 0, bool out_scientific = false )
{
    T empty_real_array [1];
    std::complex<T> empty_complex_array [1];
    std::vector<std::vector<std::vector<std::complex<T>>>> empty_complex_vector(1, std::vector<std::vector<std::complex<T>>>(1, std::vector<std::complex<T>>(1, 0) ) );
    _wldat_export( wldat_path, empty_real_array, empty_complex_array, data_vector, empty_complex_vector, false, true, imax, jmax, kmax, out_precision, out_scientific );
}

template<typename T>
void wldat_export( std::string wldat_path, std::vector<std::vector<std::vector<std::complex<T>>>> &data_vector, int imax, int jmax, int kmax, int out_precision = 0, bool out_scientific = false )
{
    T empty_real_array [1];
    std::complex<T> empty_complex_array [1];
    std::vector<std::vector<std::vector<T>>> empty_real_vector(1, std::vector<std::vector<T>>(1, std::vector<T>(1, 0) ) );
    _wldat_export( wldat_path, empty_real_array, empty_complex_array, empty_real_vector, data_vector, true, true, imax, jmax, kmax, out_precision, out_scientific );
}

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