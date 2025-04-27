/* * * * * * * * * * * * * * * * * * * * *
 * Title: sHASH
 *
 * Creator: Alexander Bodi
 *
 * IDE: Kate - Ubuntu Linux
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
#include <openssl/evp.h>
#include <vector>
#include <chrono>
#include <thread>

using namespace std;

void intro() {
        cout << "\n\n\nWelcome to..." << endl
        << "                 HHHHHHHHH     HHHHHHHHH               AAA                 SSSSSSSSSSSSSSS HHHHHHHHH     HHHHHHHHH" << endl
        << "                 H:::::::H     H:::::::H              A:::A              SS:::::::::::::::SH:::::::H     H:::::::H" << endl
        << "                 H:::::::H     H:::::::H             A:::::A            S:::::SSSSSS::::::SH:::::::H     H:::::::H" << endl
        << "                 H:::::::H     H:::::::H             A:::::A            S:::::SSSSSS::::::SH:::::::H     H:::::::H" << endl
        << "                 HH::::::H     H::::::HH            A:::::::A           S:::::S     SSSSSSSHH::::::H     H::::::HH" << endl
        << "    ssssssssss     H:::::H     H:::::H             A:::::::::A          S:::::S              H:::::H     H:::::H"   << endl
        << "  ss::::::::::s    H:::::H     H:::::H            A:::::A:::::A         S:::::S              H:::::H     H:::::H"   << endl
        << "ss:::::::::::::s   H::::::HHHHH::::::H           A:::::A A:::::A         S::::SSSS           H::::::HHHHH::::::H"   << endl
        << "s::::::ssss:::::s  H:::::::::::::::::H          A:::::A   A:::::A         SS::::::SSSSS      H:::::::::::::::::H"   << endl
        << " s:::::s  ssssss   H:::::::::::::::::H         A:::::A     A:::::A          SSS::::::::SS    H:::::::::::::::::H"   << endl
        << "   s::::::s        H::::::HHHHH::::::H        A:::::AAAAAAAAA:::::A            SSSSSS::::S   H::::::HHHHH::::::H"   << endl
        << "      s::::::s     H:::::H     H:::::H       A:::::::::::::::::::::A                S:::::S  H:::::H     H:::::H"   << endl
        << "ssssss   s:::::s   H:::::H     H:::::H      A:::::AAAAAAAAAAAAA:::::A               S:::::S  H:::::H     H:::::H"   << endl
        << "s:::::ssss::::::sHH::::::H     H::::::HH   A:::::A             A:::::A  SSSSSSS     S:::::SHH::::::H     H::::::HH" << endl
        << "s::::::::::::::s H:::::::H     H:::::::H  A:::::A               A:::::A S::::::SSSSSS:::::SH:::::::H     H:::::::H" << endl
        << " s:::::::::::ss  H:::::::H     H:::::::H A:::::A                 A:::::AS:::::::::::::::SS H:::::::H     H:::::::H" << endl
        << "  sssssssssss    HHHHHHHHH     HHHHHHHHHAAAAAAA                   AAAAAAASSSSSSSSSSSSSSS   HHHHHHHHH     HHHHHHHHH" << endl;
}

void helpMessage() {
        cout << "\nUsage: ./sHASH [options] <file_path> \n\n";
        cout << "Options:\n";
        cout << " -h, --help Show this help message and exit\n";
        cout << " <file_path> Path to the file to hash\n";
}

void displayLoadingBar(int progress, int total) {
        int barWidth = 70;
        float ratio = progress / (float)total;
        int pos = barWidth * ratio;

        cout << "[";
        for (int i = 0; i < barWidth; ++i) {
                if (i < pos) cout << "=";
                else if (i == pos) cout << ">";
                else cout << " ";
        }
        cout << "] " << int(ratio * 100.0) << " %\r";
        cout.flush();
}

string calculateMD5(const string& filePath) {
        ifstream file(filePath, ios::binary);
        if (!file) {
                cerr << "Error: Cannot open file " << filePath << endl;
                exit(1);
        }

        EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
        const EVP_MD* md = EVP_md5();
        EVP_DigestInit_ex(mdctx, md, nullptr);

        char buffer[1024];
        while (file.read(buffer, sizeof(buffer))) {
                EVP_DigestUpdate(mdctx, buffer, file.gcount());
                displayLoadingBar(file.tellg(), file.seekg(0, ios::end).tellg());
                file.seekg(file.tellg(), ios::beg);
                this_thread::sleep_for(chrono::milliseconds(100)); // Simulate loading
        }
        EVP_DigestUpdate(mdctx, buffer, file.gcount());

        unsigned char result[EVP_MAX_MD_SIZE];
        unsigned int result_len;
        EVP_DigestFinal_ex(mdctx, result, &result_len);
        EVP_MD_CTX_free(mdctx);

        stringstream md5String;
        for (unsigned int i = 0; i < result_len; ++i) {
                md5String << hex << setw(2) << setfill('0') << (int)result[i];
        }

        return md5String.str();
}

int main(int argc, char* argv[]) {
        intro();
        int optNum;
        struct option long_options[] = {
                {"help", no_argument, 0, 'h'},
                {0, 0, 0, 0}
        };

        while ((optNum = getopt_long(argc, argv, "h", long_options, nullptr)) != -1) {
                switch (optNum) {
                        case 'h':
                                helpMessage();
                                return 0;
                        default:
                                helpMessage();
                                return 1;
                }
        }

        if (argc - optind != 1) {
                cout << "\n!!! Please provide a filename in the same line as the run command! You can use ./sHASH -h or --help for further options !!! \n" << endl;
                return 1;
        }

        string filePath = argv[optind];
        cout << "\nCalculating MD5 hash for file: " << filePath << endl;
        string md5Hash = calculateMD5(filePath);
        cout << "\nMD5 Hash: " << md5Hash << endl;

        return 0;
}
