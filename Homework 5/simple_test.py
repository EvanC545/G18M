import pytest
import math
## functions to be tested
def openFile(filename):
    infile = open(filename, "r")

    print("File opened.")

def numbers(num1, num2):
    return num1 / num2

def dist(x1, y1, x2, y2):
    dist = (x2 - x1) ** 2 + (y2 - y1) ** 2
    dist = math.sqrt(dist)

    return dist

def isPalindrome(temp):
    test = temp[::-1]

    if(test == temp):
        return True

    else:
        return False

def divide():
    num1 = int(input("Enter a number: "))
    num2 = int(input("Enter another number: "))

    div = num1 / num2

    print("Your numbers divided is:", div)

def sq(num):
    return math.sqrt(num)

def greetUser(first, middle, last):
    print("Hello!")
    print("Welcome to the program", first, middle, last)
    print("Glad to have you!")

def displayItem(numbers, index):
    print("Your item at", index, "index is", numbers[index])

## functions to test functions

@pytest.mark.parametrize("filename", [1, "Stuff", "%$#"])
def test_openFile(filename):
    assert openFile(filename) == "File opened."

@pytest.mark.parametrize("num1, num2", [(0.001, 0.001), ("num1", 2), (True, False)])
def test_numbers(num1, num2):
    assert numbers(num1, num2) == 1


@pytest.mark.parametrize("x1, y1, x2, y2", [(0, 0, math.sqrt(7), math.sqrt(2)), ("x1", "y1", "x2", "y2"), (True, False, 3, 4)])
def test_dist(x1, y1, x2, y2):
    assert dist(x1, y1, x2, y2) == 3 
    ##first test is supposed to equal 3, but instead python says it's equal to 3.0000000000000004
    ##third test returns an actual number too, somehow.

@pytest.mark.parametrize("temp", ["racecar", True, 1.7171])
def test_isPalindrome(temp):
    assert isPalindrome(temp) == True
    ##first test succeeds
    ##second and third tests obviously fail because incorrect object

def test_divide(monkeypatch, capsys):
    divide()
    monkeypatch.setattr('builtins.input', lambda _: "8")
    #monkeypatch.setattr('builtins.input', lambda _: 4)
    
    captured_stdout, captured_stderr = capsys.readouterr()
    assert captured_stdout.strip() == "Your numbers divided is: 2"
    #Finish this function


@pytest.mark.parametrize("num", [4, 2**2, (True+True)**2, "4"])
def test_sq(num):
    assert sq(num) == 2

#def test_greetUser
