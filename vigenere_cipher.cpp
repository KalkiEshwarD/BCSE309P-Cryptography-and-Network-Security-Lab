// Author: Kalki Eshwar D
// BCSE309P: Cryptography and Network Security Lab
// Vellore Institutution of Technology, Vellore
// Vigenere Cipher

#include <iostream>
#include <vector>

using namespace std;

class VigenereCipher {
    private:
        // Creating a function that returns true if the provided input character is an alphabet and false if not.
        static bool isAlpha(char c) {
            if ((c <= 'z' && c >= 'a') || (c <= 'Z' && c >= 'A')) {
                return true;
            } else {
                return false;
            }
        }

        // Creating a function that returns true if a character is an uppercase alphabet and false if the character is not uppercase alphabet or an alphabet.
        static bool isCaps(char c) {
            if (VigenereCipher::isAlpha(c) == false) return false;

            if (c <= 'z' && c >= 'a') {
                return false;
            } else {
                return true;
            }
        }
        
        // A function to convert the character provided to upper case alphabet if it is in lowercase.
        static char toCaps(char c) {
            if (c <= 'z' && c >= 'a') {
                return ((char)(c - 'a' + 'A'));
            } else {
                return c;
            }
        }

        // Create a function to find the index of an alphabet
        static int findVal(char c) {
            return (int)(VigenereCipher::toCaps(c) - 'A');
        }

        // Create a function to find the alphabet given it's corresponding index.
        static char findChar(int i) {
            return (char)('A' + i);
        }
        
        // Function to extend the key to the desired limit (The size of the data)
        static string extendKey(string key, int length) {
            string extendedKey = "";

            for (int i = 0; i < length; i++) {
                extendedKey += key[i % key.size()];
            }

            return extendedKey;
        }

    public:
        // Function to encrypted the data given a key
        static string encrypt(string data, string key) {
            string encryptedString = "";
            string extendedKey = VigenereCipher::extendKey(key, data.size()); // Extend the key to the size of the data

            // Iterate through all the characters
            for (int i = 0; i < data.size(); i++) {
                if (VigenereCipher::isAlpha(data[i])) {
                    // If the character happens to be an alphabet, then modify it.
                    int dataCharVal = VigenereCipher::findVal(data[i]);
                    int keyCharVal = VigenereCipher::findVal(extendedKey[i]);

                    char cipherChar = VigenereCipher::findChar((dataCharVal + keyCharVal) % 26);
                    encryptedString += cipherChar; 
                } else {
                    // Else append it to the encrypted string as is.
                    encryptedString += data[i];
                }
            }

            return encryptedString;
        }

        static string decrypt(string data, string key) {
            string decryptedString = "";
            string extendedKey = extendKey(key, data.size());

            // Iterate through all the characters
            for (int i = 0; i < data.size(); i++) {
                if (VigenereCipher::isAlpha(data[i])) {
                    // If the character is an alphabet, then reverse the encryption.
                    int cipherCharVal = findVal(data[i]);
                    int keyCharVal = findVal(extendedKey[i]);

                    decryptedString += VigenereCipher::findChar((26 + (cipherCharVal - keyCharVal)) % 26);
                } else {
                    // If the character is not an alphabet, then append the character to the decypted string as is.
                    decryptedString += data[i];
                }
            }

            return decryptedString;
        }
};

int main() {
    string encryptedString = VigenereCipher::encrypt("wearediscoveredsaveyourself", "deceptive");
    cout << "Encrypted String: " << encryptedString << endl;
    string decryptedString = VigenereCipher::decrypt(encryptedString, "deceptive");
    cout << "Decrypted String: " << decryptedString << endl;
    return 0;
}
