/*A strong pseudo-prime to a base 'a' is an odd composite number 'n' with n-1=d·2^s (for d odd) for which either
        a^d(mod n) = 1(mod n)
            or
        a^(d·2^r)(mod n) = -1(mod n), for some r=0, 1, ..., s-1 (this can be interpreted as a^(d.2^r) (mod n) + 1 = n
Take note of the fact that n should not be a prime number, it has to be a composite number
*/
//I recommend reading the main function before the rest of the program, and then read the corresponding functions to know the
//implementation details
#include <bits/stdc++.h>
#include <ttmath/ttmath.h>
using namespace std;

//for a given value n, this function finds the corresponding d and s
//since two values cannot be returned, I am performing call by reference which changes the two values accordingly
typedef long long BigInt;
//this allows me to fetch the values I need to

void find_d_and_s(BigInt n, BigInt& d, BigInt& s)
{
    int q = 1;
    double numerator = n-1, d_val = 0, divisor = 0;
    while(1){
        divisor = pow(2,q);
        q++;
        //make use of the notation n = d.2^s + 1
        d_val = numerator/divisor;
        //checking whether d is an odd integer or not
        if((int)d_val==d_val && ((int)d_val)%2!=0){
            d = d_val;
            s = q;
            return;
        }
    }
}


//finding the LHS side of the primality test
//the following function can be used for finding a^d % n, the function works by generating the pattern which modulus operation
//generates with the given number and then selects the appropriate element from the pattern
BigInt find_LHS(BigInt n, BigInt d, BigInt a)
{
    int power = 0;
    vector<BigInt> pattern;
    BigInt use = pow(a,power);
    while(use<n && power<=d){
        pattern.push_back(use);
        power++;
        use = pow(a,power);
    }
    if(power==d)
        return use%n;
    if(power>d)
        return pattern[d];
    BigInt use_now = use%n;
    while(power<=d){
        if(use_now==1)
            break;
        pattern.push_back(use_now);
        power++;
        use_now = (use_now*a)%n;
    }
    if(power==d)
        return use_now;
    int index = d%(pattern.size());
    return pattern[index];
}

//the following function performs the first test
bool primality_test(BigInt n, BigInt d, BigInt a)
{
    BigInt lhs = find_LHS(n, d, a);
    if(lhs==1)
        return true;
    return false;
}

//the following function performs the second test
bool primality_test_2(BigInt n, BigInt d, BigInt s, BigInt a)
{
    //the loop ranges from 0 to s-1, to account for various r in a^(d.2^r)
    for(BigInt q=0; q!=s; q++){
        BigInt lhs = find_LHS(n, d*pow(2,q), a);
        if(lhs+1==n)
            return true;
    }
    return false;
}

//function for checking whether a number is prime or not
bool isprime(BigInt n)
{
    if(n==1)
        return false;
    double limit = sqrt(n);
    //checking if square root is an integer or not, changing values accordingly
    limit = (int)limit==limit? limit: limit+1;
    //division check is performed from 2 till n^1/2
    for(BigInt q=2; q<=limit; q++)
        if(n%q==0) return false;
    return true;
}

int main()
{
    //taking input
    cout<<"Enter the number of bases: ";
    int number_of_bases;
    cin>>number_of_bases;
    vector<int> bases;
    cout<<"Enter the bases:"<<endl;
    int a = 0;
    int base_temp;
    for(int q=0; q!=number_of_bases; q++){
        cin>>base_temp;
        if(base_temp>a)
            a = base_temp;
        bases.push_back(base_temp);
    }
    BigInt n = 9, d, s;
    cout<<"Currently checking:"<<endl;
    while(1){
        //since n should be composite, all primes are excluded from the test
        if(isprime(n)){
            n+=2;
            continue;
        }
        //finding d and s values for corresponding n
        find_d_and_s(n, d, s);
        //if n passes any of the two tests
        if(primality_test(n, d, a) || primality_test_2(n, d, s, a))
        {
            cout<<n<<endl;
            int counter = 1;
            for(vector<int>::iterator iter = bases.begin(); iter!=bases.end(); iter++){
                if(*iter!=a){
                if(primality_test(n, d, *iter) || primality_test_2(n, d, s, *iter))
                    counter++;
                else
                    break;
                }
            }
            if(counter==bases.size()){
                cout<<"Common found:"<<endl;
                cout<<n<<endl;
                return 0;
            }
        }
        //since n should only be odd, it is incremented by 2 after every iteration
        n+=2;
    }
}
