import sys

print("Hello world!!!!")
print("Hello Warning!!!", file=sys.stderr)
with open('hello.txt', 'w') as f:
    f.write("I am the contant of the file! WOW!")
