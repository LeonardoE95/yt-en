#!/usr/bin/env python3

class Caesar:
    def __init__(self, shift=3):
        self.alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        self.alphabet_len = 26
        self.shift_value = shift

    def _char2code(self, char):
        return ord(char) - ord('A')

    def _code2char(self, code):
        return chr(code + ord('A'))

    # ----------------------------    

    def _encrypt_char(self, char):
        code = self._char2code(char)

        if code < 0 or code > 25:
            return char
        
        encrypted_code = (code + self.shift_value) % self.alphabet_len
        encrypted_char = self._code2char(encrypted_code)
        return encrypted_char

    def _decrypt_char(self, char):
        code = self._char2code(char)

        if code < 0 or code > 25:
            return char
        
        decrypted_code = (code - self.shift_value) % self.alphabet_len
        decrypted_char = self._code2char(decrypted_code)
        return decrypted_char
    
    # ----------------------------
    
    def encrypt(self, plaintext):
        ciphertext = [''] * len(plaintext)
        for i, c in enumerate(plaintext):
            ciphertext[i] = self._encrypt_char(plaintext[i])
        return "".join(ciphertext)

    def decrypt(self, ciphertext):
        plaintext = [''] * len(ciphertext)
        for i, c in enumerate(ciphertext):
            plaintext[i] = self._decrypt_char(ciphertext[i])
        return "".join(plaintext)

# ------------------------------------------------------

def main(): 
    shift_value = 3
    cipher = Caesar(shift=shift_value)
    plaintext = "HELLO WORLD"
    ciphertext = cipher.encrypt(plaintext)
    new_plaintext = cipher.decrypt(ciphertext)        
    print(f"[c={shift_value}] '{plaintext}' -> '{ciphertext}'")
    print(f"[c={shift_value}] '{ciphertext}' -> '{new_plaintext}'")    
        
if __name__ == "__main__":
    main()
