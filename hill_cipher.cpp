// Author: Kalki Eshwar D
// BCSE309P: Cryptography and Network Security Lab
// Vellore Institute of Technology, Vellore
// Hill Cipher

// Note: Supports only 2x2 and 3x3 matrice keys

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class HillCipher {
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
            if (HillCipher::isAlpha(c) == false) return false;

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
            return (int)(HillCipher::toCaps(c) - 'A');
        }

        // Create a function to find the alphabet given it's corresponding index.
        static char findChar(int i) {
            i = i % 26;
            return (char)('A' + i);
        }

        // A function to multiply two matrices
        static vector<vector<int>> multiplyMatrix(vector<vector<int>> matrixA, vector<vector<int>> matrixB) {
            if (matrixA[0].size() != matrixB.size() || matrixA.size() == 0 || matrixB.size() == 0) {
                cout << "ERROR: Matrices cannot be multiplied" << endl;
                cout << "Matrix A: [" << matrixA.size() << "][" << matrixA[0].size() << "]" << endl;
                cout << "Matrix B: [" << matrixB.size() << "][" << matrixB[0].size() << "]" << endl;
                return vector<vector<int>> ( { { -1 } } ); // Return incase the dimensional requirements don't match.
            }
            
            int resultRows = matrixA.size();
            int resultColumns = matrixB[0].size();

            vector<vector<int>> resultMatrix = { };

            // Initialize the vector values with 0s.
            for (int i = 0; i < resultRows; i++) {
                resultMatrix.push_back({ });
                for (int j = 0; j < resultColumns; j++) {
                    resultMatrix[i].push_back( 0 );
                }
            }

            // Iterate through the rows of the result matrix
            for (int i = 0; i < resultRows; i++) {
                // Iterate through the columns of the result matrix
                for (int j = 0; j < resultColumns; j++) {

                    int sum = 0;
                    // Calculate sum
                    for (int l = 0; l < matrixA[0].size(); l++) {
                        sum += matrixA[i][l] * matrixB[l][j];
                    }
                    resultMatrix[i][j] = sum;
                    
                }
            }

            // Mod the values by 26
            for (int i = 0; i < resultMatrix.size(); i++) {
                for (int j = 0; j < resultMatrix[0].size(); j++) {
                    resultMatrix[i][j] = resultMatrix[i][j] % 26;
                }
            }

            return resultMatrix;
        }

        // A function to calculate the determinant of a matrix
        static int calculateDeterminant(vector<vector<int>> matrix) {
            int determinant = -1;
            if (matrix.size() == 2 && matrix[0].size() == 2) {
                determinant = ((matrix[0][0] * matrix[1][1]) - (matrix[1][0] * matrix[0][1]));
            } else if (matrix.size() == 3 && matrix[0].size() == 3) {
                determinant = (((matrix[0][0] * matrix[1][1] * matrix[2][2]) + (matrix[1][0] * matrix[2][1] * matrix[0][2])+ (matrix[2][0] * matrix[0][1] * matrix[1][2])) - ((matrix[0][2] * matrix[1][1] * matrix[2][0]) + (matrix[1][2] * matrix[2][1] * matrix[0][0]) + (matrix[2][2] * matrix[0][1] * matrix[1][0])));
            } else {
                cout << "ERROR: Matrix dimensions not supported. Determinant cannot be calculated." << endl;
                throw exception();
                return -1;
            }
            return (determinant >= 0) ? determinant : -determinant; // Return only positive values
        }

        // A function to calculate the multiplicative inverse of a number
        static int calculateMultiplicativeInverse(int a) {
            // Use extended euclidean algorithm if need be. Since n is small (26), I will be brute forcing.
            for (int i = 1; i < 26; i++) {
                if (((a * i) % 26 ) == 1) return i;
            }

            cout << "ERROR: Multiplicative Inverse not found." << endl;
            return -1;
        }

        // A function to calculate the transpose of a matrix
        static vector<vector<int>> calculateTranspose(vector<vector<int>> matrix) {
            vector<vector<int>> transposeMatrix;

            for (int i = 0; i < matrix[0].size(); i++) {
                transposeMatrix.push_back({});
                for (int j = 0; j < matrix.size(); j++) {
                    transposeMatrix[i].push_back(matrix[j][i]);
                }
            }

            return transposeMatrix;
        }

        // A function to calculate the adjoint of a matrix
        static vector<vector<int>> calculateAdjointMatrix(vector<vector<int>> matrix) {
            vector<vector<int>> adjointMatrix = {};
            vector<vector<int>> cofactorMatrix = {};

            if (matrix.size() == 2 && matrix[0].size() == 2) {
                adjointMatrix.push_back({ matrix[1][1], -matrix[0][1] });
                adjointMatrix.push_back({ -matrix[1][0], matrix[0][0] });

                return adjointMatrix;
            } else if (matrix.size() == 3 && matrix[0].size() == 3) {
                // Calculate minors of first row
                vector<vector<int>> minor00 = { { matrix[1][1], matrix[1][2] }, { matrix[2][1], matrix[2][2] } };
                vector<vector<int>> minor01 = { { matrix[1][0], matrix[1][2] }, { matrix[2][0], matrix[2][2] } };
                vector<vector<int>> minor02 = { { matrix[1][0], matrix[1][1] }, { matrix[2][0], matrix[2][1] } };
                
                // Calculate minors of second row
                vector<vector<int>> minor10 = { { matrix[0][1], matrix[0][2] }, { matrix[2][1], matrix[2][2] } };
                vector<vector<int>> minor11 = { { matrix[0][0], matrix[0][2] }, { matrix[2][0], matrix[2][2] } };
                vector<vector<int>> minor12 = { { matrix[0][0], matrix[0][1] }, { matrix[2][0], matrix[2][1] } };
                
                // Calculate minors of third row
                vector<vector<int>> minor20 = { { matrix[0][1], matrix[0][2] }, { matrix[1][1], matrix[1][2] } };
                vector<vector<int>> minor21 = { { matrix[0][0], matrix[0][2] }, { matrix[1][0], matrix[1][2] } };
                vector<vector<int>> minor22 = { { matrix[0][0], matrix[0][1] }, { matrix[1][0], matrix[1][1] } };
                
                // Calculate the cofactors of each element
                cofactorMatrix.push_back({ (int)(pow(-1, ( 1 + 1 )) * (HillCipher::calculateDeterminant(minor00))), (int)((pow(-1, ( 1 + 2 )) * (HillCipher::calculateDeterminant(minor01)))), (int)((pow(-1, ( 1 + 3 )) * (HillCipher::calculateDeterminant(minor02)))) });
                cofactorMatrix.push_back({ (int)(pow(-1, ( 2 + 1 )) * (HillCipher::calculateDeterminant(minor10))), (int)((pow(-1, ( 2 + 2 )) * (HillCipher::calculateDeterminant(minor11)))), (int)((pow(-1, ( 2 + 3 )) * (HillCipher::calculateDeterminant(minor12)))) });
                cofactorMatrix.push_back({ (int)(pow(-1, ( 3 + 1 )) * (HillCipher::calculateDeterminant(minor20))), (int)((pow(-1, ( 3 + 2 )) * (HillCipher::calculateDeterminant(minor21)))), (int)((pow(-1, ( 3 + 3 )) * (HillCipher::calculateDeterminant(minor22)))) });

                // Find the transpose of the cofactor matrix and that will be the adjoint
                adjointMatrix = HillCipher::calculateTranspose(cofactorMatrix);

                // Mod the values by 26
                for (int i = 0; i < adjointMatrix.size(); i++) {
                    for (int j = 0; j < adjointMatrix[0].size(); j++) {
                        adjointMatrix[i][j] = adjointMatrix[i][j] % 26;
                    }
                }


                return adjointMatrix;
            } else {
                cout << "ERROR: Matrix dimensions not supported. Adjoint cannot be calculated." << endl;
                throw exception();
                return vector<vector<int>> ({ { -1 } });
            }
        }

    public:

        static string encrypt (string data, vector<vector<int>> key) {
            string encryptedString = "";
            // Create the vector of vector characters with each char replaced with their respective value.
            int charPerMatrix = key[0].size();
            if (!(charPerMatrix == 2 || charPerMatrix == 3)) {
                cout << "ERROR: Key matrix size not supported. Enter only 3x3 or 2x2 matrix." << endl;
            }

            while (data.size() % charPerMatrix != 0) data += 'X'; // Pad the data as long as there is less than needed elements.

            vector<vector<int>> plainTextMatrix;

            int index = 0;
            // Iterate through columns
            for (int i = 0; i < data.size() / charPerMatrix; i++) {
                // Iterate through rows
                for (int j = 0; j < charPerMatrix; j++) {
                    if (i == 0) { plainTextMatrix.push_back({ });}
                    plainTextMatrix[j].push_back(HillCipher::findVal(data[index]));
                    index++;
                }
            }

            // Multiply the key and each vector and mod each value
            vector<vector<int>> encryptedMatrix;
            vector<vector<int>> plainTextVec;
            for (int i = 0; i < plainTextMatrix[0].size(); i++) {
                encryptedMatrix.push_back({ });
                plainTextVec = {};
                for (int j = 0; j < plainTextMatrix.size(); j++) {
                    plainTextVec.push_back({ plainTextMatrix[j][i] });
                }
                vector<vector<int>> cipherVec = HillCipher::multiplyMatrix(key, plainTextVec); // Multiply both the matrices
                
                for (int j = 0; j < plainTextMatrix.size(); j++) {
                    encryptedMatrix[i].push_back(cipherVec[j][0]);
                }
            }

            for (int i = 0; i < encryptedMatrix.size(); i++) {
                for (int j = 0; j < encryptedMatrix[0].size(); j++) {
                    encryptedMatrix[i][j] = encryptedMatrix[i][j] % 26;
                }
            }

            // For every element present in the matrix, mod it
            for (int i = 0; i < encryptedMatrix.size(); i++) {
                for (int j = 0; j < encryptedMatrix[0].size(); j++) {
                    encryptedMatrix[i][j] = encryptedMatrix[i][j] % 26;
                }
            }

            // Extract the character from the values
            // Iterate through all the columns
            for (int i = 0; i < encryptedMatrix.size(); i++) {
                // Iterate through rows
                for (int j = 0; j < encryptedMatrix[0].size(); j++) {
                    encryptedString += HillCipher::findChar(encryptedMatrix[i][j]); // Append it to the encrypted string
                }
            }

            return encryptedString;
        }

        static string decrypt(string data, vector<vector<int>> key) {
            string decryptedString = "";

            // Calculate the derminant of the key matrix
            int determinant = HillCipher::calculateDeterminant(key);
            // Calculate the multiplicative inverse of the determinant of the key matrix
            int multiplicativeInverse = HillCipher::calculateMultiplicativeInverse(determinant);
            // Calculate the adjoint of the matrix
            vector<vector<int>> adjointMatrix = HillCipher::calculateAdjointMatrix(key);
            
            // Calculate the encrypted matrix
            vector<vector<int>> encryptedMatrix;
            int charPerMatrix = key.size();
            int index = 0;
            // Iterate through columns
            for (int i = 0; i < data.size() / charPerMatrix; i++) {
                // Iterate through rows
                for (int j = 0; j < charPerMatrix; j++) {
                    if (i == 0) {
                        encryptedMatrix.push_back({ });
                    }
                    encryptedMatrix[j].push_back( HillCipher::findVal(data[index]) % 26 );
                    index++;
                }
            }

            vector<vector<int>> inverseKeyMatrix;

            // Find the inverse of a key matrix mod 26
            for (int i = 0; i < adjointMatrix.size(); i++) {
                inverseKeyMatrix.push_back({ });
                for (int j = 0; j < adjointMatrix[0].size(); j++) {
                    inverseKeyMatrix[i].push_back((adjointMatrix[i][j] * multiplicativeInverse) % 26);
                }
            }

            // Multiply the inverse key matrix with the encrypted text vector
            vector<vector<int>> decryptedMatrix;
            vector<vector<int>> encryptedTextVec;
            vector<vector<int>> decrypedTextVec;
  
            // Iterate through the columns
            for (int i = 0; i < encryptedMatrix[0].size(); i++) {
                encryptedTextVec = { };
                // Iterate through the rows
                for (int j = 0; j < encryptedMatrix.size(); j++) {
                    encryptedTextVec.push_back({ encryptedMatrix[j][i] });
                }

                decrypedTextVec = HillCipher::multiplyMatrix(inverseKeyMatrix, encryptedTextVec);
                
                // Mod the value by 26
                for (int j = 0; j < decrypedTextVec.size(); j++) {
                    decrypedTextVec[j][0] = decrypedTextVec[j][0] % 26;
                    if (decrypedTextVec[j][0] < 0) {
                        decrypedTextVec[j][0] += 26; // Incase the output is negative, then add it with 26 to make it positive.
                    }
                }



                for (int j = 0; j < decrypedTextVec.size(); j++) {
                    decryptedString += HillCipher::findChar(decrypedTextVec[j][0] % 26);
                }
            }

            return decryptedString;
        }
};

int main() {
    vector<vector<int>> keyMatrix = { { 1, 3, 41 }, { 3, 5, 6 } , { 6, 1, 1 }};
    string encryptedString = HillCipher::encrypt("attack", keyMatrix);
    cout << "Encrypted String: " << encryptedString << endl;

    string decryptedString = HillCipher::decrypt(encryptedString, keyMatrix);
    cout << "Decrypted String: " << decryptedString << endl;

    return 0;
}
