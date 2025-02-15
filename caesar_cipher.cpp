// Author: Kalki Eshwar D
// BCSE309P: Cryptography and Network Security Lab
// Vellore Institute of Technology, Vellore
// Caesar Cipher

#include <iostream>
#include <vector>

using namespace std;

class CaesarCipher {
    private:
        // Declare the 'a' and 'A' as the base as we will be considering them as the first index.
        static const char smallBase = 'a';
        static const char bigBase = 'A';

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
            if (!CaesarCipher::isAlpha(c)) return false;

            if (c <= 'z' && c >= 'a') {
                return false;
            } else {
                return true;
            }
        }
    
    public:
        static string decrypt(string data, int key) {
            string decryptedString = "";
            key = key % 26; // If the key is larger than 26, then mod it with 26.

            for (int i = 0; i < data.size(); i++) {
                if (CaesarCipher::isAlpha(data[i])) {
                    if (CaesarCipher::isCaps(data[i])) {
                        // If a character is uppercase, then create a variable temp that stores the character's index value and shifting it back.
                        int temp = ((data[i] - CaesarCipher::bigBase) - key);
                        if (temp <= 0) {
                            // Modify the index value if it is negative or zero (when the key is larger than or equal to the index value of the character).
                            temp += 26;
                        }
                        decryptedString += (char)(CaesarCipher::bigBase + temp); // Add the character to the decrypted string after typecasting it.
                    } else {
                        // Perform the same operations as above with the minor change being the base is of smaller case 'a'.
                        int temp = ((data[i] - CaesarCipher::smallBase) - key);
                        if (temp <= 0) {
                            temp += 26;
                        }
                        decryptedString += (char)(CaesarCipher::smallBase + temp);
                    }
                } else {
                    // If the character is not of type alphabet, then simply append the character to the decrypted string.
                    decryptedString += data[i];
                }
            }

            return decryptedString;
        }



        static string encrypt(string data, int key) {
            // The opeartions are same as decryption, the only difference being that this function returns the encrypted character.
            string encryptedString = "";
            key = key % 26;

            for (int i = 0; i < data.length(); i++) {
                if (isAlpha(data[i])) {
                    if (isCaps(data[i])) {
                        encryptedString += (char)(CaesarCipher::bigBase + (((data[i] - CaesarCipher::bigBase) + key) % 26));      
                    } else {
                        encryptedString += (char)(CaesarCipher::smallBase + (((data[i] - CaesarCipher::smallBase) + key) % 26)); 
                    }
                } else {
                    encryptedString += data[i];
                }
            }

            return encryptedString;
        }
};

int main() {
    cout << "Result: " << CaesarCipher::encrypt("Z 12", 1) << endl;
    cout << "Result: " << CaesarCipher::decrypt(CaesarCipher::encrypt("Z 12", 1), 1) << endl;
    return 0;
}