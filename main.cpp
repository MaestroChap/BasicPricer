#include <iostream>
#include "MonteCarlo.hpp"
#include "BlackScholes.hpp"

int main()
{

    BlackScholes::PutEuro Call1(100, 100, 0.05, 1, 0.30);

    MonteCarlo::PutEuro Call2(100, 100, 0.05, 1, 0.30);


    std::cout << "Prix de l'option avec BlackScholes : " <<Call1.optionPrice() << std::endl;
    std::cout << "Prix de l'option avec MC : " << Call2.optionPrice() << std::endl;

    std::unordered_map<std::string, double> vec;
    vec = Call2.optionGrecques(0.01);

    std::cout << std::endl;

    std::cout << std::endl;



    for (std::unordered_map<std::string, double>::iterator it = vec.begin(); it != vec.end(); it++)
    {
        std::cout << it->first << " avec MC : " << it->second << std::endl;
    }

    std::cout << std::endl;

    std::cout << std::endl;

    std::unordered_map<std::string, double> vec1;

    vec1 = Call1.optionGrecques();

    for (std::unordered_map<std::string, double>::iterator it = vec1.begin(); it != vec1.end(); it++)
    {
        std::cout << it->first << " avec BS : " << it->second << std::endl;
    }
    return 0;
}