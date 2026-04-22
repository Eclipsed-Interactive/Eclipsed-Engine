FNV_OFFSET_BASIS = 0xcbf29ce484222325
FNV_PRIME = 0x100000001b3

def fnv1a_64(data: bytes) -> int:
    hash_ = FNV_OFFSET_BASIS
    for byte in data:
        hash_ ^= byte
        hash_ = (hash_ * FNV_PRIME) & 0xFFFFFFFFFFFFFFFF
    return hash_