#!/usr/bin/env python3

# Implementation of the caesar cipher for teaching purposes.
# 
# Youtube Channel: @hexdump1337
# Author: Leonardo Tamiano
# 
class Caesar:
    # NOTE: Assume plaintext and ciphertext are given already in
    # uppercase. Otherwise we can add an upper() method with some error
    # handling.

    def __init__(self, shift=3):
        self.alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        self.alphabet_len = 26
        self.shift_value = shift

    def _char2code(self, char):
        return ord(char) - ord('A')

    def _code2char(self, code):
        return chr(code + ord('A'))

    def _encrypt_char(self, char):
        code = self._char2code(char)

        if code < 0 or code > 25:
            return char

        encrypted_code = (code + self.shift_value) % self.alphabet_len        
        return self._code2char(encrypted_code)

    def _decrypt_char(self, char):
        code = self._char2code(char)

        if code < 0 or code > 25:
            return char

        plaintext_code = (code - self.shift_value) % self.alphabet_len
        return self._code2char(plaintext_code)

    # -------------------

    def encrypt(self, plaintext):
        ciphertext = [0] * len(plaintext)

        for i, c in enumerate(plaintext):
            ciphertext[i] = self._encrypt_char(plaintext[i])

        return "".join(ciphertext) 

    def decrypt(self, ciphertext):
        plaintext = [0] * len(ciphertext)

        for i, c in enumerate(plaintext):
            plaintext[i] = self._decrypt_char(ciphertext[i])

        return "".join(plaintext) 

# --------------------------------

def main():
    shift_value = 3
    cipher = Caesar(shift=shift_value)
    
    plaintext = "HELLO WORLD"
    ciphertext = cipher.encrypt(plaintext)
    plaintext2 = cipher.decrypt(ciphertext)

    print(f"[c={shift_value}] '{plaintext}' -> '{ciphertext}'")
    print(f"[c={shift_value}] '{ciphertext}' -> '{plaintext2}'")

if __name__ == "__main__":
    main()
