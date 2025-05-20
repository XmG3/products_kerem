#include <iostream>
#include <string>
#include <cstring>
#include <random>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <limits>

//cl.exe compiler command
//cl /EHsc password.cpp /I "C:\Program Files\OpenSSL-Win64\include" /Fe:password.exe /link /LIBPATH:"C:\Program Files\OpenSSL-Win64\lib\VC\x64\MD" libcrypto.lib
//to-dos for next time
  // MOST IMPORTANT: add presistent login support
  // password confirmation during registration
  // old password cant be the same as new password
  // username already exists

#include <conio.h> // Windows-specific header for _getch()

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

    //function to check password strength, password must be longer than 8 chars, include at least one int and special char
    bool strengthChecker(const std::string& input) {
        if(input.length() < 8) {
            std::cout << "Must be more than 8 characters. Not strong enough." << std::endl;
            return false;
        }
        if(input.find_first_of("0123456789") == std::string::npos) {
            std::cout << "Must contain at least one digit. Not strong enough." << std::endl;
            return false;
        }
        if(input.find_first_of("!@#$%^&*()_+/") == std::string::npos) {
            std::cout << "Must contain at least one special character. Not strong enough." << std::endl;
            return false;
        }
        else {
            return true;
        }
    }

    //function to check if a username is already in use
    bool checkUsername(const std::string& username) {
        int index = findIndex(username);
        int i = 0;

        while (i < size) {
            int currentIndex = (index + i) % size;
            if(entries[currentIndex].isEmpty) {
                return false; //user not found
            }
            if(!entries[currentIndex].isDeleted && entries[currentIndex].username == username) {
                return true; //username already exists
            }
            ++i;
        }
        return false;
    }


};

//function for global hidden password input
std::string getHiddenPassword(const std::string& prompt = "") {
    std::string password;
    char ch;

    std::cout << prompt;

    while ((ch = _getch()) != '\r') { // carriage return means Enter key
        if (ch == '\b') { // backspace
            if (!password.empty()) {
                password.pop_back();
                std::cout << "\b \b";
            }
        } else if (isprint(ch)) {
            password += ch;
            std::cout << '*';
        }
    }

    std::cout << std::endl;
    return password;
    }

//main function

int main() {

    SecurePasswordStorage storage(10); //size of table, depending on number of users
    std::string username, password, confirmpassword;
    std::string input;
    int choice;
    bool loggedIn = false;
    std::string currentuser;
     std::cout << "Welcome to the login system!" << std::endl;

    while(true) {
        if(!loggedIn){
           
            std::cout << "1. Login\n";
            std::cout << "2. Register\n";
            std::cout << "3. Exit\n";
            std::cout << "Enter choice: ";
            std::getline(std::cin, input);
        
    if(input == "Exit" || input == "exit") {
        std::cout << "Exiting program." << std::endl;
        return 0;
    }

    std::stringstream ss(input);
    if(!(ss >> choice)) {
        std::cout << "Invalid input. Please enter an expected input." << std::endl;
        continue;
    }


    switch(choice) {
        case 1: //login
            std::cout << "Enter username: ";
            std::cin >> username;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Enter password: ";
            password = getHiddenPassword();
            if(storage.verifyPassword(username, password)) {
                std::cout << "Login successful!" << std::endl;
                loggedIn = true;
                currentuser = username;
            } else {
                std::cout << "Such a login doesn't exist." << std::endl;
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;

        case 2: { //register
            bool usernameValid = false;
            while (!usernameValid) {
                std::cout << "Enter username: ";
                std::cin >> username;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                if(username.length() < 5 || username.length() > 12) {
                    std::cout << "Username must be between 5 and 12 characters." << std::endl;
                    continue;
                }
                if(storage.checkUsername(username)) {
                    std::cout << "Username already exists. Please use another one." << std::endl;
                    continue;
                }

                usernameValid = true;
            }

            bool passwordValid = false;
            while(!passwordValid) {
                std::cout << "Enter password: ";
                password = getHiddenPassword();

                //checking password strength
                if(storage.strengthChecker(password) == false)  {
                    continue;;
                }  
                passwordValid = true;
            }

            if(storage.storePassword(username, password)) {
                std::cout << "Registration successful!" << std::endl;
                loggedIn = true;
                currentuser = username;
                break;
            } else {
                std::cout << "Registration failed." << std::endl;
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }

        case 3: //exit
            std::cout << "Exiting program." << std::endl;
            return 0;
            break;

        default: //invalid input
            std::cout << "Please enter an expected input." << std::endl;
            break;
       }
    }


    else{ //user logged in
        std::cout << "Welcome " << currentuser << "!" << std::endl;
            std::cout << "1. Change password\n";
            std::cout << "2. Delete account\n";
            std::cout << "3. Logout\n";
            std::cout << "4. Exit\n";
            std::cout << "Enter choice: ";
            std::getline(std::cin, input);

        if(input == "exit") {
            std::cout << "Exiting program." << std::endl;
            storage.createFile(); // save data before exit
            break;
        }

        std::stringstream ss(input);
        if (!(ss >> choice)) {
            std::cout << "Please enter a valid number." << std::endl;
            continue;
            }
        
        switch(choice) {
        case 1: //change password
            std::cout << "Enter new password: ";
            password = getHiddenPassword();
            if(storage.storePassword(currentuser, password)) {
                std::cout << "Password changed successfully!" << std::endl;
            } else {
                std::cout << "Failed to change password." << std::endl;
            }
            break;

        case 2: //delete account
            if(storage.removeUser(currentuser)) {
                std::cout << "Account deleted successfully!" << std::endl;
                loggedIn = false;
            } else {
                std::cout << "Failed to delete account." << std::endl;
            }
            break;
        
        case 3: //logout
            loggedIn = false;
            std::cout << "Logged out successfully!" << std::endl;
            break;

        case 4: //exit program
            std::cout << "Exiting program." << std::endl;
            storage.createFile(); //save to file
            return 0;
            

        default:
            std::cout << "Invalid input. Please try again." << std::endl;
            break;
        }
    }
}
    return 0;
}