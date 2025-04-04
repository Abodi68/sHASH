/* * * * * * * * * * * * * * * * * * * * *
 * Title: sHASH
 *
 * Creator: Alexander Bodi
 *
 * Notes: Welcome to my capstone project!
 * This is my tool to hash a given file,
 * appropriately handle errors, and
 * follow secure hashing guidelines.
 * * * * * * * * * * * * * * * * *  * * */


#include <fstream>
#include <getopt.h>
#include <iomanip>
#include <iostream>
#include <openssl/md5.h>
//#include <openssl/sha.h>



using namespace std;

void helpMessage() {
        cout << "\nUsage: ./sHASH [options] <file_path> <algorithm> \n\n";
        cout << "Options:\n";
        cout << "  -h, --help       Show this help message and exit\n";
        cout << "  <file_path>      Path to the file to hash\n";
        cout << "  <algorithm>      NOT CURRENTLY SUPPORTED - Hashing algorithm to use (currently only supporting md5)\n";
}


void intro() {
        cout << "\n\n\nWelcome to..." << endl
        << "                 HHHHHHHHH     HHHHHHHHH               AAA                 SSSSSSSSSSSSSSS HHHHHHHHH     HHHHHHHHH" << endl
        << "                 H:::::::H     H:::::::H              A:::A              SS:::::::::::::::SH:::::::H     H:::::::H" << endl
        << "                 H:::::::H     H:::::::H             A:::::A            S:::::SSSSSS::::::SH:::::::H     H:::::::H" << endl
        << "                 H:::::::H     H:::::::H             A:::::A            S:::::SSSSSS::::::SH:::::::H     H:::::::H" << endl
        << "                 HH::::::H     H::::::HH            A:::::::A           S:::::S     SSSSSSSHH::::::H     H::::::HH" << endl
        << "    ssssssssss     H:::::H     H:::::H             A:::::::::A          S:::::S              H:::::H     H:::::H" << endl
        << "  ss::::::::::s    H:::::H     H:::::H            A:::::A:::::A         S:::::S              H:::::H     H:::::H" << endl
        << "ss:::::::::::::s   H::::::HHHHH::::::H           A:::::A A:::::A         S::::SSSS           H::::::HHHHH::::::H" << endl
        << "s::::::ssss:::::s  H:::::::::::::::::H          A:::::A   A:::::A         SS::::::SSSSS      H:::::::::::::::::H" << endl
        << " s:::::s  ssssss   H:::::::::::::::::H         A:::::A     A:::::A          SSS::::::::SS    H:::::::::::::::::H" << endl
        << "   s::::::s        H::::::HHHHH::::::H        A:::::AAAAAAAAA:::::A            SSSSSS::::S   H::::::HHHHH::::::H" << endl
        << "      s::::::s     H:::::H     H:::::H       A:::::::::::::::::::::A                S:::::S  H:::::H     H:::::H" << endl
        << "ssssss   s:::::s   H:::::H     H:::::H      A:::::AAAAAAAAAAAAA:::::A               S:::::S  H:::::H     H:::::H" << endl
        << "s:::::ssss::::::sHH::::::H     H::::::HH   A:::::A             A:::::A  SSSSSSS     S:::::SHH::::::H     H::::::HH" << endl
        << "s::::::::::::::s H:::::::H     H:::::::H  A:::::A               A:::::A S::::::SSSSSS:::::SH:::::::H     H:::::::H" << endl
        << " s:::::::::::ss  H:::::::H     H:::::::H A:::::A                 A:::::AS:::::::::::::::SS H:::::::H     H:::::::H" << endl
        << "  sssssssssss    HHHHHHHHH     HHHHHHHHHAAAAAAA                   AAAAAAASSSSSSSSSSSSSSS   HHHHHHHHH     HHHHHHHHH" << endl;
}



int main(int argc, char* argv[]) {
        intro();

        int optNum;
        struct option long_options[] ={
                {"help", no_argument, 0, 'h'},
                {0, 0, 0, 0}
        };

        while ((optNum = getopt_long(argc, argv, "h", long_options, nullptr)) != -1) {
                switch(optNum) {
                        case 'h':
                                helpMessage();
                                return 0;
                        default:
                                helpMessage();
                                return 1;
                }
        }

        if (argc - optind != 1) {
                cout << "\n!!! please give a filename in the same line as the run command! You can use ./sHash -h or --help for futher options !!! \n" << endl;
                return 1;
        }

        return 0;
}


