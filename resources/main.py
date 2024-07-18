# Single-line comment

"""
Multi-line string (can be used as a multi-line comment)
This demonstrates the use of various token types in Python.
"""

# Keywords
def example_function(param1, param2):
    # Function definition with parameters
    global_var = 10.0  # Global variable
    if param1 > param2:
        result = param1 + global_var
    else:
        result = param2 - global_var

    for i in range(5):
        print(f"Loop iteration {i}: {result}")

    try:
        result /= param2
    except ZeroDivisionError as e:
        print("Error:", e)
    finally:
        print("Execution completed")

    return result

# Variables and identifiers
my_var = "Hello, World!"  # String literal
another_var = 42  # Integer literal
decimal_var = 3.14  # Floating point literal
boolean_var = True  # Boolean literal

# Data structures
my_list = [1, 2, 3, 4, 5]  # List
my_tuple = (1, 2, 3, 4, 5)  # Tuple
my_set = {1, 2, 3, 4, 5}  # Set
my_dict = {"one": 1, "two": 2, "three": 3}  # Dictionary

# Function call
result = example_function(10, 5)

# Class definition
class ExampleClass:
    # Class variable
    class_var = "I am a class variable"

    def __init__(self, value):
        # Instance variable
        self.instance_var = value

    # Method
    def display(self):
        print(f"Instance variable: {self.instance_var}")
        print(f"Class variable: {ExampleClass.class_var}")

# Object instantiation
example_object = ExampleClass(100)
example_object.display()

# Import statement
import math

# Using an imported module
print("Square root of 16 is:", math.sqrt(16))

# Lambda function
square = lambda x: x ** 2
print("Square of 5 is:", square(5))

# List comprehension
squares = [x ** 2 for x in range(10)]
print("Squares:", squares)

# Generator expression
gen = (x ** 2 for x in range(10))
print("Generator squares:", list(gen))

# Decorators
def decorator_function(func):
    def wrapper(*args, **kwargs):
        print("Function is being called")
        return func(*args, **kwargs)
    return wrapper

@decorator_function
def decorated_function(message):
    print("Message:", message)

decorated_function("Hello from decorated function")

# Context manager
with open("example.txt", "w") as file:
    file.write("This is a test file.")

# End of the script
print("Script executed successfully!")

