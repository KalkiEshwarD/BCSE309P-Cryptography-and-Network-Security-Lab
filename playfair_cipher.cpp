// Author: Kalki Eshwar D
// BCSE309P: Cryptogrpahy and Network Security Lab
// Vellore Institute of Technology, Vellore
// Playfair Cipher

// Note: This code can take any input, but deletes all numbers and spaces.

#include <iostream>
#include <vector>
#include <set>
#include <utility>

using namespace std;

class PlayfairCipher {
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
            if (PlayfairCipher::isAlpha(c) == false) return false;

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

        // A function to return the index of the character in the key matrix.
        static pair<int, int> findIndex(vector<vector<char>> keyMatrix, char c) {
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 5; j++) {
                    if (c == keyMatrix[i][j]) {
                         return make_pair(i, j);
                    }
                }
            }

            return make_pair(-1, -1);
        }

        // Creating a function that constructs the key matrix for the playfair cipher.
        static vector<vector<char>> constuctKeyMatrix(string key) {
            set<char> usedCharsSet; // Initialize a vector to maintain all unique values in the key.
            string capsKey = "";
            
            vector<vector<char>> keyMatrix = { };
            keyMatrix.push_back(vector<char> ({ }));
            
            int row = 0;
            int column = 0;

            for (int i = 0; i < key.size(); i++) {
                capsKey += PlayfairCipher::toCaps(key[i]);
            }

            for (int i = 0; i < capsKey.size(); i++) {

                if (PlayfairCipher::isAlpha(capsKey[i]) == false) continue; // If the key contains non-alphabetic characters, ignore it.

                // First insert all the elements present in the key into the matrix.
                if (capsKey[i] == 'J') capsKey[i] = 'I'; // If the key happens to contain J, then replace it with I.
                
                if (usedCharsSet.find(capsKey[i]) == usedCharsSet.end()) {
                    // If the character is not present in the matrix.
                    keyMatrix[row].push_back(capsKey[i]);
                    if (column == 4) {
                        // If you reach the end of the matrix, then change go to the first column of the next row.
                        row++;
                        keyMatrix.push_back(vector<char> ({ }));
                        column = 0;
                    } else {
                        // If not, then just go to the next column.
                        column++;
                    }
                    usedCharsSet.insert(capsKey[i]);
                } else {
                    // If the character is already present in the matrix.
                    continue;
                }
            }

            // After inserting all the keys in the matrix, then insert all the other values.

            char base = 'A';

            for (int i = 0; i < 26; i++) {
                // Iterate through all the alphabets and start inserting them into the matrix.
                if ((char)(base + i) == 'J') continue; // If the character which we are checking is 'J', then skip it.

                if (usedCharsSet.find((char)(base + i)) == usedCharsSet.end()) {
                    // If the character was not part of the key, then insert it.
                    keyMatrix[row].push_back((char)(base + i));
                    if (column == 4) {
                        // If we have reached the end of the matrix, go to the first column of the next row.
                        row++;
                        keyMatrix.push_back(vector<char> ({ }));
                        column = 0; 
                    } else {
                        // If not, just go to the next column.
                        column++;
                    }
                } else {
                    // If the character was part of the key, then skip it.
                    continue;
                }
                usedCharsSet.insert(key[i]);
            }

            return keyMatrix;
        }

    public:
        static string encrypt(string data, string key) {
            string encryptedString = "";
            string capsString = "";
 
            vector<vector<char> > keyMatrix = constuctKeyMatrix(key);
            vector<string> splitStrings = {};
            

            // Convert all the characters to uppercase
            for (int i = 0; i < data.size(); i++) {
                if (PlayfairCipher::isAlpha(data[i]) == false) continue;
                capsString += PlayfairCipher::toCaps(data[i]);
            }

            // Split the string
            for (int i = 0; i < capsString.size(); i++) {
                string temp = "";
                temp += capsString[i];

                if (i == capsString.size() - 1) {
                    temp += "X";
                } else {
                    if (capsString[i + 1] == temp[0]) {
                        temp += "X";
                    } else {
                        i++;
                        temp += capsString[i];
                    }
                }

                splitStrings.push_back(temp);
            }

            // Perform replacements
            for (int i = 0; i < splitStrings.size(); i++) {
                char first = splitStrings[i][0];
                char second = splitStrings[i][1];

                pair<int, int> firstIndex;
                pair<int, int> secondIndex;

                // Find out their index.
                firstIndex = PlayfairCipher::findIndex(keyMatrix, first);
                secondIndex = PlayfairCipher::findIndex(keyMatrix, second);

                // Check if they belong to the same row
                if (firstIndex.first == secondIndex.first) {
                    encryptedString += keyMatrix[firstIndex.first][(firstIndex.second + 1) % 5];
                    encryptedString += keyMatrix[firstIndex.first][(secondIndex.second + 1) % 5];
                } else if (firstIndex.second == secondIndex.second) {
                    encryptedString += keyMatrix[(firstIndex.first + 1) % 5][firstIndex.second];
                    encryptedString += keyMatrix[(secondIndex.first + 1) % 5][firstIndex.second];
                } else {
                    encryptedString += keyMatrix[firstIndex.first][secondIndex.second];
                    encryptedString += keyMatrix[secondIndex.first][firstIndex.second];
                }
            }

            return encryptedString;
        }

        static string decrypt(string data, string key) {
            string decryptedString = "";
            string capsString = "";

            vector<string> splitStrings;

            vector<vector<char>> keyMatrix = PlayfairCipher::constuctKeyMatrix(key);

            // Convert each character to uppercase.
            for (int i = 0; i < data.size(); i++) {
                capsString += PlayfairCipher::toCaps(data[i]);
            }

            // Split the string
            for (int i = 0; i < capsString.size(); i++) {
                string temp = "";
                temp += capsString[i];
                i++;
                temp += capsString[i];
                splitStrings.push_back(temp);
            }

            // Perform replacements
            for (int i = 0; i < splitStrings.size(); i++) {
                char first = splitStrings[i][0];
                char second = splitStrings[i][1];

                pair<int, int> firstIndex;
                pair<int, int> secondIndex;

                // Find out their index.
                firstIndex = PlayfairCipher::findIndex(keyMatrix, first);
                secondIndex = PlayfairCipher::findIndex(keyMatrix, second);

                // Check if they belong to the same row
                if (firstIndex.first == secondIndex.first) {
                    decryptedString += keyMatrix[firstIndex.first][(5 + (firstIndex.second - 1)) % 5];
                    decryptedString += keyMatrix[firstIndex.first][(5 + (secondIndex.second - 1)) % 5];
                } else if (firstIndex.second == secondIndex.second) {
                    decryptedString += keyMatrix[(5 + (firstIndex.first - 1)) % 5][firstIndex.second];
                    decryptedString += keyMatrix[(5 + (secondIndex.first - 1)) % 5][firstIndex.second];
                } else {
                    decryptedString += keyMatrix[firstIndex.first][secondIndex.second];
                    decryptedString += keyMatrix[secondIndex.first][firstIndex.second];
                }

            }

            return decryptedString;
        }
};

int main() {
    string encryptedString = PlayfairCipher::encrypt("kill shepard", "monarchy");
    cout << "Encrypted String: " << encryptedString << endl;
    string decryptedString = PlayfairCipher::decrypt(encryptedString, "monarchy");
    cout << "Decrypted String: " << decryptedString << endl;
    return 0;
}
