from cs50 import get_string
import re


number = get_string("Number: ")
n = int(number)
s1, s2, S, r, a = 0, 0, 0, 0, 0

while n > 0:
    r = n % 10
    s1 += r
    n = (n - r) // 10
    r = n % 10
    a = 2 * r

    while a > 0:
        b = a % 10
        s2 += b
        a = (a - b) // 10
        b = a % 10
        s2 += b
        a = a // 10

    S = s1 + s2
    n = n // 10

if S % 10 == 0:
    if len(number) == 15 and (re.match("34", number) or re.match("37", number)):
        print("AMEX")
    elif len(number) == 16 and (re.match("51", number) or re.match("52", number) or re.match("53", number) or re.match("54", number) or re.match("55", number)):
        print("MASTERCARD")
    elif (len(number) == 13 or len(number) == 16) and re.match("4", number):
        print("VISA")
    else:
        print("INVALID")
else:
    print("INVALID")
