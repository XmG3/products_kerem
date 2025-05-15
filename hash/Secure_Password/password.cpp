#include <iostream>
#include <string>
#include <cstring>
#include <random>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <openssl/sha.h>
#include <openssl/evp.h>

class SecurePasswordStorage {
private:    

    struct PasswordEntry {
        std::string passwordHash;
        std::string salt;
        std::string username;
        bool isDeleted;
        bool isEmpty;
    
        PasswordEntry() : isDeleted(false), isEmpty(true) {}

    };

    PasswordEntry* entries;
    int size;

    //function to create salt
    std::string saltGen(size_t length = 16) {
        static const char alpha[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, sizeof(alpha) - 2);

        std::string salt;
        salt.reserve(length);

        for(size_t i =0; i < length; ++i) {
            salt += alpha[distrib(gen)];
        } 

        return salt;
    }

    //salting the password
    std::string hashPassword(const std::string& password, const std::string& salt) {
        EVP_MD_CTX* ctx = EVP_MD_CTX_new();
        if (ctx == nullptr) {
            throw std::runtime_error("Failed to generate hash context");
        }
        
        const EVP_MD* md = EVP_sha256();
        unsigned char hash[EVP_MAX_MD_SIZE];
        unsigned int lengthOfHash = 0;

        std::string saltedPassword = salt + password; //combine salt and password

        EVP_DigestInit_ex(ctx, md, nullptr);
        EVP_DigestUpdate(ctx, saltedPassword.c_str(), saltedPassword.size());
        EVP_DigestFinal_ex(ctx, hash, &lengthOfHash);
        EVP_MD_CTX_free(ctx);

        std::stringstream ss;
        for(unsigned int i = 0; i < lengthOfHash; ++i) {
            ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
        }

        return ss.str();
    }

    //hash function finding index
    int findIndex(const std::string& username) const {
        unsigned hash = 5381;
        for (char c : username) {
            hash = ((hash << 5) + hash) + c; //hash * 33 + c
        }
        return hash % size;
    }

public:

    SecurePasswordStorage(int tableSize) : size(tableSize) {
        entries = new PasswordEntry[size];
    }

    ~SecurePasswordStorage() {
        delete[] entries;
    }

    bool storePassword(const std::string& username, const std::string& password) {
        int index = findIndex(username);
        int i = 0;

        //linear probing
        while (i < size) {
            int currentIndex = (index + i) % size;

            if(entries[currentIndex].isEmpty || entries[currentIndex].isDeleted) { //empty or deleted
                std::string salt = saltGen();
                std::string hashedPassword = hashPassword(password, salt);
                
                entries[currentIndex].username = username;
                entries[currentIndex].passwordHash = hashedPassword;
                entries[currentIndex].salt = salt;
                entries[currentIndex].isEmpty = false;
                entries[currentIndex].isDeleted = false;
                return true;
            }
            else if(entries[currentIndex].username == username){ //update password
                std::string salt = saltGen();
                std::string hashedPassword = hashPassword(password, salt);

                entries[currentIndex].passwordHash = hashedPassword;
                entries[currentIndex].salt = salt;
                return true;
            }
            i++;
        }
        return false; //table full
    }

    bool verifyPassword(const std::string& username, const std::string& password) {
        int index = findIndex(username);
        int i = 0;
        while (i < size) {
            int currentIndex = (index + i) % size;
            if(entries[currentIndex].isEmpty) {
                return false; //user not found
            }
            if(!entries[currentIndex].isDeleted && entries[currentIndex].username == username) {
                std::string hashAttempt = hashPassword(password, entries[currentIndex].salt);
                return hashAttempt == entries[currentIndex].passwordHash;
            }
            ++i;
        }
        return false;
    }

    bool removeUser(const std::string& username) {
        int index = findIndex(username);
        int i = 0;

        while (i < size) {
            int currentIndex = (index + i) % size;
            
            if (entries[currentIndex].isEmpty) {
                // User doesn't exist
                return false;
            }
            
            if (!entries[currentIndex].isDeleted && entries[currentIndex].username == username) {
                // Found the user, mark as deleted
                entries[currentIndex].isDeleted = true;
                return true;
            }
            
            i++;
        }
        return false;
    }

    void printTable() {
        for (int i = 0; i < size; i++) {
            std::cout << "[" << i << "]: ";
            
            if (entries[i].isEmpty) {
                std::cout << "EMPTY";
            } else if (entries[i].isDeleted) {
                std::cout << "DELETED";
            } else {
                std::cout << "Username: " << entries[i].username 
                          << ", Password Hash: " << entries[i].passwordHash.substr(0, 10) << "..." 
                          << ", Salt: " << entries[i].salt;
            }
            
            std::cout << std::endl;
        }
    }

    void createFile() {
        std::ofstream outFile("passwords.txt");
        if(!outFile) {
            std::cerr << "Cannot open file" << std::endl;
            return;
        }

        for (int i = 0; i < size; i++) {
            outFile << "[" << i << "]: ";
            
            if (entries[i].isEmpty) {
                outFile << "EMPTY";
            } else if (entries[i].isDeleted) {
                outFile << "DELETED";
            } else {
                outFile << "Username: " << entries[i].username 
                          << ", Password Hash: " << entries[i].passwordHash.substr(0, 10) << "..." 
                          << ", Salt: " << entries[i].salt;
            }
            
            outFile << std::endl;
        }
        outFile.close();
    }

};

int main() {

    SecurePasswordStorage storage(5);
    std::string username, password;
    std::cout << "Enter username: ";
    std::cin >> username;
    std::cout << "Enter password: ";
    std::cin >> password;
    storage.storePassword(username, password);

    std::cout << "Password storage table: " << std::endl;
    storage.printTable();
    storage.createFile();
    std::cout << username <<" verification is: "<< (storage.verifyPassword(username, password) ? "Valid" : "Invalid") << std::endl;
    return 0;
}