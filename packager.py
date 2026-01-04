import sys

def pack(file_path, key):
    with open(file_path, "rb") as f:
        data = bytearray(f.read())
    for i in range(len(data)):
        data[i] = (data[i] ^ key) & 0xFF
        data[i] = (~data[i]) & 0xFF
    with open("payload.bin", "wb") as f:
        f.write(data)
    print(f"Fichier payload.bin généré avec la clé {key}")

if __name__ == "__main__":
    pack(sys.argv[1], int(sys.argv[2]))
