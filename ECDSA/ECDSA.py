import random

def find_inverse(number, modulus):
	return pow(number, -1, modulus)

class Point:
    def __init__(self, x, y, curve_config):
        a = curve_config['a']
        b = curve_config['b']
        p = curve_config['p']

        if (y ** 2) % p != (x ** 3 + a * x + b) % p:
            raise Exception("Điểm không nằm trên đường cong")

        self.x = x
        self.y = y
        self.curve_config = curve_config

    def is_equal_to(self, point):
        return self.x == point.x and self.y == point.y
    
    def add(self, point):
        p = self.curve_config['p']
        if self.is_equal_to(point):
            slope = (3 * point.x ** 2) * find_inverse(2 * point.y, p) % p
        else:
            slope = (point.y - self.y) * find_inverse(point.x - self.x, p) % p

        x = (slope ** 2 - point.x - self.x) % p
        y = (slope * (self.x - x) - self.y) % p
        return Point(x, y, self.curve_config)

    # def multiply(self, times):
    #     point = self
    #     for i in range(times - 1):
    #         point = point.add(self)
    #     return point
    
    def multiply(self, times):
        current_point = self
        current_coefficient = 1

        pervious_points = []
        while current_coefficient < times:
            # lưu trữ current_point như previous_point
            pervious_points.append((current_coefficient, current_point))
            # nếu có thể nhân current_point với 2
            if 2 * current_coefficient <= times:
                current_point = current_point.add(current_point)
                current_coefficient = 2 * current_coefficient
            # ngược lại, tìm previous_point lớn nhất để thêm vào point
            else:
                next_point = self
                next_coefficient = 1
                for (previous_coefficient, previous_point) in pervious_points:
                    if previous_coefficient + current_coefficient <= times:
                        if previous_point.x != current_point.x:
                            next_coefficient = previous_coefficient
                            next_point = previous_point
                current_point = current_point.add(next_point)
                current_coefficient = current_coefficient + next_coefficient

        return current_point
    
secp256k1_curve_config = {
    'a': 0,
    'b': 7,
    'p': 115792089237316195423570985008687907853269984665640564039457584007908834671663
}
x = 55066263022277343669578718895168534326250603453777594175500187360389116729240
y = 32670510020758816978083085130507043184471273380659243275938904335757337482424
n = 115792089237316195423570985008687907852837564279074904382605163141518161494337
g_point = Point(x, y, secp256k1_curve_config)

def sign_message(message, private_key):
    k = random.randint(1, n)
    r_point = g_point.multiply(k)
    r = r_point.x % n
    if r == 0:
        return sign_message(message, private_key)
    k_inverse = find_inverse(k, n)
    s = k_inverse * (message + r * private_key) % n
    return r, s

def verify_signature(signature, message, public_key):
    (r, s) = signature
    s_inverse = find_inverse(s, n)
    u = message * s_inverse % n
    v = r * s_inverse % n
    c_point = g_point.multiply(u).add(public_key.multiply(v))
    return c_point.x == r

def ECC_encode(key_point, pub_ECC):
    k = random.randint(1, n)
    Cm1 = g_point.multiply(k)
    Cm2 = key_point.add(pub_ECC.multiply(k))
    return Cm1, Cm2

def ECC_decode(Cm, priv_ECC):
    (a,b) = Cm
    c = a.multiply(priv_ECC)
    c.y = - c.y
    return c.add(b)

private_key = 123456789012345  # số nguyên ngẫu nhiên
public_key = g_point.multiply(private_key)

message = 12345

signature = sign_message(message, private_key)
print('Signature: ', signature)
print('Is valid: ', verify_signature(signature, message, public_key))
