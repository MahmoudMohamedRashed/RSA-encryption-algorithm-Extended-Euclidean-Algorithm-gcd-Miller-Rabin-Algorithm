#include <iostream>
#include <string>
#include <ctime>
#include <fstream>

using namespace std;

/*************Variable*************/

unsigned long long p ;
unsigned long long q ;
unsigned long long n ;
unsigned long long Q ;
unsigned long long e ;
unsigned long long d ;

/*********gcd_algorithm*******/

unsigned long long gcd ( unsigned long long num, unsigned long long mod )
{
    if ( mod == 0 ) return num ;
    return gcd ( mod , num % mod ) ;
}

/****Extended_Euclidean_Algorithm****/
long long Inverse_number ( long long num, long long mod )
{
    if ( gcd ( abs(num), abs(mod) ) != 1 )
    {
        cout << "\nError ... Two Number not Relatively Prime \n" ;
        return 0 ;
    }
    long long x[2] = { 1, 0 } ;
    long long quotient;
    long long R[2] ;
    long long temp ;
    R[0] = num ;
    R[1] = mod ;
    while ( R[1] != 1 )
    {
        quotient = R[0] / R [1] ;
        temp = R[0] % R[1] ;
        R[0] = R[1] ;
        R[1] = temp ;
        temp = x[0] - ( x[1] * quotient ) ;
        x[0] = x[1] ;
        x[1] = temp ;
    }
    if ( x[1] < 0 )
    {
        return mod+x[1] ;
    }
    else return x[1] ;
}

/****Modular Exponentiation****/
unsigned long long exp_mod ( unsigned long long base, unsigned long long exp, unsigned long long mod )
{
    unsigned long long result = 1 ;  //unsigned long long to avoid any overflow
    base = base % mod ;             // while (n * n) << long long no , then no over flow
    while ( exp > 0 )
    {
        if ( exp & 1 ) result = (result * base) % mod ;     //if and bitwise is true
        base = (base * base) % mod ;
        exp >>= 1 ;     //shift right to represent number in binary
    }
    return result ;
}

/********Test Prime*********/
bool test ( unsigned long long prime, int iter = 10 )
{
    if ( prime <  2 || prime % 2 == 0 ) return false ;
    if ( prime == 2 || prime == 3 ) return true ;
    unsigned long long number = prime -1 ;
    unsigned int k = 0 ;
    unsigned long long temp ;
    while ( number % 2 == 0 )
    {
        number /= 2 ;
        k++ ;
    }
    bool out = 1 ; // if out is zero that main one of test is failure
    for ( int j = 0 ; j < iter ; j++ )  // check 10 time to sure that is prime
    {
        unsigned long long a = 2 + rand() % (prime - 5 ) ; // 2 < a < prime-3
        if ( exp_mod( a, number, prime ) == 1 ) continue ;
        temp = 1 ;
        out  = 0 ;
        for ( int i = 0 ; i < k ; i++ )
        {
            if ( exp_mod( a, temp*number, prime ) == (prime -1 ))
            {
                out = 1 ;
                break ;
            }
            temp *= 2 ;
        }
        if ( !out ) return false ;
    }
    return true ;
}

/**********Initial key***********/
bool init_key ( unsigned long long minn = 10000 , unsigned long long maxx = 100000)
{
    /********select p , q ***********/
    unsigned long long prime_num = minn ;
    int counter = 0 ;
    while ( counter < 2 )
    {
        prime_num = minn + ( rand() % (maxx - minn) ) ; // to get prime number in range (10k - 100k)
        if ( test ( prime_num ) )
        {
            if ( counter == 0 )
            {
                p = prime_num ;
            }
            else
            {
                q = prime_num ;
            }
            ++counter ;
        }
    }
    if ( p == q ) return false ;  // as if (p == q) chiper is same textciper

    /*********get n , Q ************/
    n = p * q ;
    Q = (p-1) * (q-1) ;
    /*********select n , d ***********/
    while ( (e < 2 ) || ( gcd ( e , Q ) != 1 ) )
    {
        e = 2 + rand() % Q ;
    }
    d = Inverse_number( e , Q ) ;
    if ( d == 0 ) return false ;
    return true ;
}

/**********Encryption************/
string encry ( string & inp )
{
    string out ;
    string help;
    unsigned long long num ;
    for ( int i = 0 ; i < inp.size() ; i++ )
    {
        num = inp[i] ;
        num = exp_mod(num , e , n ) ;
        help= to_string(num) ;
        help +="!" ;  // ! to separates each letter
        out += help ;
    }
    return out ;
}

/*********decryption**********/
string decry ( string &inp )
{
    string out ;
    char ch ;
    unsigned long long num ;
    for ( int i = 0 ; i < inp.size() ; i++ ) // to get all message
    {
        ch = inp[i] ;
        num= 0 ;
        while ( ch != '!' ) // to get one char
        {
            num *= 10 ;
            num += (ch - '0') ;
            ch = inp[++i] ;
        }
        num = exp_mod( num , d , n ) ;
        ch = num ;
        out += ch ;
    }
    return out ;
}
int main()
{
    srand(time(0)) ;
    init_key() ;
    while (1)
    {
        char state ;
        cout << "Do you want to do \"E( Encrypt ) \" or \"D( Decrypt )\" \n" ;
        cin  >> state ;
        if ( (state != 'E' ) && (state != 'D') ) break ;
        string file_input, file_output ;
        cout << "Enter name of input file \n" ;
        cin  >> file_input ;
        fstream input ( file_input ) ;
        input.close() ;
        input.open(file_input , ios::out | ios::in ) ;
        string text ;
        char ch ;
        while ( input.get(ch) ) text += ch ;
        cout << "Enter name of output file \n" ;
        cin  >> file_output ;
        string result ;
        cout << endl ;
        switch (state)
        {
        case 'E' :
            result = encry( text ) ;
            break ;
        case 'D' :
            result = decry( text ) ;
            break ;
        }
        fstream output ( file_output ) ;
        output.close() ;
        output.open(file_output , ios::out | ios::in) ;
        for( int i = 0 ; i < result.size() ; i++ )
            output.put( result[ i ] ) ;
    }
    return 0;
}
