#!/usr/bin/env python3

# The Vigenère cipher works by encrypting each letter of the plaintext
# with a different Cesar cipher, whos shift value is determined by a
# corresponding letter of the key.
#
# https://en.wikipedia.org/wiki/Vigen%C3%A8re_cipher
# 

class Vigenere:
    def __init__(self, key):
        self.alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        self.alphabet_len = 26        
        self.key = key

    def _char2code(self, char):
        return ord(char) - ord('A')

    def _code2char(self, code):
        return chr(code + ord('A'))

    # ----------------------------

    def _encrypt_char(self, key_index, char):
        key_char = self.key[key_index % len(self.key)]
        key_code = self._char2code(key_char)

        code = self._char2code(char)
        if (code < 0 or code > 25) or (key_code < 0 or key_code > 25):
            return char

        encrypted_code = (code + key_code) % self.alphabet_len
        encrypted_char = self._code2char(encrypted_code)
        return encrypted_char

    def _decrypt_char(self, key_index, char):
        key_char = self.key[key_index % len(self.key)]
        key_code = self._char2code(key_char)

        code = self._char2code(char)
        if (code < 0 or code > 25) or (key_code < 0 or key_code > 25):
            return char

        decrypted_code = (code - key_code) % self.alphabet_len
        decrypted_char = self._code2char(decrypted_code)
        return decrypted_char

    # ----------------------------

    def encrypt(self, plaintext):
        ciphertext = [''] * len(plaintext)
        for i, c in enumerate(plaintext):
            ciphertext[i] = self._encrypt_char(i, plaintext[i])
        return "".join(ciphertext)

    def decrypt(self, ciphertext):
        plaintext = [''] * len(ciphertext)
        for i, c in enumerate(ciphertext):
            plaintext[i] = self._decrypt_char(i, ciphertext[i])        
        return "".join(plaintext)

# ------------------------------------------------------

def main():
    key = "ADC"
    cipher = Vigenere(key)
    plaintext = "HELLO WORLD"
    ciphertext = cipher.encrypt(plaintext)
    print(f"[key='{key}' Encryption] '{plaintext}' -> '{ciphertext}'")

    cipher = Vigenere("ADC")
    new_plaintext = cipher.decrypt(ciphertext)        

    print(f"[key='{key}' Decryption] '{ciphertext}' -> '{new_plaintext}'")    

if __name__ == "__main__":
    main()
