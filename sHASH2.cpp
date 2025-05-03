/* * * * * * * * * * * * * * * * * * * * *
 * Title: sHASH
 *
 * Creator: A. Bodi
 *
 * IDE: Kate - Ubuntu Linux
 *
 * Notes: Welcome to my capstone project!
 * This is my tool to hash a given file,
 * appropriately handle errors, and
 * follow secure hashing guidelines.
 * * * * * * * * * * * * * * * * *  * * */

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <thread>
#include <mutex>
#include <openssl/evp.h>
#include <filesystem>
#include <chrono>
#include <ctime>
#include <algorithm> // Include for std::remove

using namespace std;

struct HashResult {
    string sha256;
    string sha512;
};

mutex progressMutex;

void displayLoadingBar(size_t progress, size_t total) {
    lock_guard<mutex> lock(progressMutex);
    int barWidth = 50; // Width of the loading bar
    float ratio = static_cast<float>(progress) / total;
    int pos = static_cast<int>(barWidth * ratio);

    cout << "[";
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) cout << "=";
        else if (i == pos) cout << ">";
        else cout << " ";
    }
    cout << "] " << fixed << setprecision(1) << (ratio * 100.0) << "%\r";
    cout.flush();
}

string calculateSingleHash(const string& filePath, const EVP_MD* md, size_t totalSize, size_t& bytesRead) {
    ifstream file(filePath, ios::binary);
    if (!file) {
        cerr << "Error: Cannot open file " << filePath << endl;
        return "";
    }

    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(mdctx, md, nullptr);

    const size_t bufferSize = 1024;
    char buffer[bufferSize];

    while (file.read(buffer, bufferSize)) {
        size_t count = file.gcount();
        EVP_DigestUpdate(mdctx, buffer, count);
        bytesRead += count;

        // Update the loading bar
        displayLoadingBar(bytesRead, totalSize);
    }

    // Process any remaining bytes
    size_t count = file.gcount();
    if (count > 0) {
        EVP_DigestUpdate(mdctx, buffer, count);
        bytesRead += count;

        // Update the loading bar
        displayLoadingBar(bytesRead, totalSize);
    }

    // Finalize the hash
    unsigned char result[EVP_MAX_MD_SIZE];
    unsigned int result_len;
    EVP_DigestFinal_ex(mdctx, result, &result_len);
    EVP_MD_CTX_free(mdctx);

    // Convert the hash to a hexadecimal string
    stringstream hashString;
    for (unsigned int i = 0; i < result_len; ++i) {
        hashString << hex << setw(2) << setfill('0') << (int)result[i];
    }

    // Ensure the loading bar reaches 100%
    displayLoadingBar(totalSize, totalSize);
    cout << endl;

    return hashString.str();
}

void calculateHash(const string& filePath, const EVP_MD* md, string& hashResult, size_t& totalSize) {
    size_t bytesRead = 0;
    hashResult = calculateSingleHash(filePath, md, totalSize, bytesRead);
}

void fileOut(const string& filePath, const HashResult& hashResult) {
    ofstream someFile("hash_results.txt", ios::app);
    if (!someFile) {
        cerr << "Error: Cannot open a file for writing." << endl;
        return;
    }

    // Get the current time and convert it to a string
    auto now = chrono::system_clock::now();
    time_t now_c = chrono::system_clock::to_time_t(now);
    string timestamp = ctime(&now_c); // Convert to string

    // Remove the newline character from the timestamp
    timestamp.erase(remove(timestamp.begin(), timestamp.end(), '\n'), timestamp.end());

    string fileName = filesystem::path(filePath).filename().string();

    someFile << timestamp << endl;
    someFile << "File Name: " << fileName << endl;
    someFile << "File Path: " << filesystem::absolute(filePath) << endl;
    someFile << "SHA-256 Hash: " << hashResult.sha256 << endl;
    someFile << "SHA-512 Hash: " << hashResult.sha512 << endl;
    someFile << "----------------------------------------------------------------\n" << endl;

    someFile.close();
}

void intro() {
    cout << "\n\n\nWelcome to..." << endl
    << "                 HHHHHHHHH     HHHHHHHHH               AAA                 SSSSSSSSSSSSSSS HHHHHHHHH     HHHHHHHHH         222222222222222" << endl
    << "                 H:::::::H     H:::::::H              A:::A              SS:::::::::::::::SH:::::::H     H:::::::H        2:::::::::::::::22" << endl
    << "                 H:::::::H     H:::::::H             A:::::A            S:::::SSSSSS::::::SH:::::::H     H:::::::H        2::::::222222:::::2" << endl
    << "                 H:::::::H     H:::::::H            A:::::::A           S:::::S     SSSSSSSHH::::::H     H::::::HH        2222222     2:::::2" << endl
    << "    ssssssssss     H:::::H     H:::::H             A:::::::::A          S:::::S              H:::::H     H:::::H                      2:::::2" << endl
    << "  ss::::::::::s    H:::::H     H:::::H            A:::::A:::::A         S:::::S              H:::::H     H:::::H                      2:::::2" << endl
    << "ss:::::::::::::s   H::::::HHHHH::::::H           A:::::A A:::::A         S::::SSSS           H::::::HHHHH::::::H                   2222::::2" << endl
    << "s::::::ssss:::::s  H:::::::::::::::::H          A:::::A   A:::::A         SS::::::SSSSS      H:::::::::::::::::H              22222::::::22" << endl
    << " s:::::s  ssssss   H:::::::::::::::::H         A:::::A     A:::::A          SSS::::::::SS    H:::::::::::::::::H            22::::::::222" << endl
    << "   s::::::s        H::::::HHHHH::::::H        A:::::AAAAAAAAA:::::A            SSSSSS::::S   H::::::HHHHH::::::H           2:::::22222" << endl
    << "      s::::::s     H:::::H     H:::::H       A:::::::::::::::::::::A                S:::::S  H:::::H     H:::::H          2:::::2" << endl
    << "ssssss   s:::::s   H:::::H     H:::::H      A:::::AAAAAAAAAAAAA:::::A               S:::::S  H:::::H     H:::::H          2:::::2" << endl
    << "s:::::ssss::::::sHH::::::H     H::::::HH   A:::::A             A:::::A  SSSSSSS     S:::::SHH::::::H     H::::::HH        2:::::2       222222" << endl
    << "s::::::::::::::s H:::::::H     H:::::::H  A:::::A               A:::::A S::::::SSSSSS:::::SH:::::::H     H:::::::H ...... 2::::::2222222:::::2" << endl
    << " s:::::::::::ss  H:::::::H     H:::::::H A:::::A                 A:::::AS:::::::::::::::SS H:::::::H     H:::::::H .::::. 2::::::::::::::::::2" << endl
    << "  sssssssssss    HHHHHHHHH     HHHHHHHHHAAAAAAA                   AAAAAAASSSSSSSSSSSSSSS   HHHHHHHHH     HHHHHHHHH ...... 22222222222222222222\n" << endl;
}


int main(int argc, char* argv[]) {
    intro();
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <file_path>" << endl;
        return 1;
    }

    string filePath = argv[1];
    if (!filesystem::exists(filePath)) {
        cerr << "Error: File does not exist: " << filePath << endl;
        return 1;
    }

    cout << "Calculating hashes for file: " << filePath << endl;

    // Initialize hash results
    HashResult hashResult;
    size_t totalSize = filesystem::file_size(filePath);

    // Create threads for parallel hashing
    thread sha256Thread(calculateHash, filePath, EVP_sha256(), ref(hashResult.sha256), ref(totalSize));
    thread sha512Thread(calculateHash, filePath, EVP_sha512(), ref(hashResult.sha512), ref(totalSize));

    // Wait for both threads to finish
    sha256Thread.join();
    sha512Thread.join();

    // Output the results
    cout << "SHA-256 Hash: " << hashResult.sha256 << endl;
    cout << "SHA-512 Hash: " << hashResult.sha512 << endl;
    cout << "Log updated! You can find the result in 'hash_results.txt' " << endl;

    // Save the results to a file
    fileOut(filePath, hashResult);

    return 0;
}
