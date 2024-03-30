package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
)

func main() {
	fmt.Println("Welcome to the Simple Calculator!")
	fmt.Println("Enter two numbers and choose an operation.")

	reader := bufio.NewReader(os.Stdin)

	fmt.Print("Enter the first number: ")
	num1, err := getUserInput(reader)
	if err != nil {
		fmt.Println("Error:", err)
		return
	}

	fmt.Print("Enter the second number: ")
	num2, err := getUserInput(reader)
	if err != nil {
		fmt.Println("Error:", err)
		return
	}

	fmt.Println("Choose an operation:")
	fmt.Println("1. Add")
	fmt.Println("2. Subtract")
	fmt.Println("3. Multiply")
	fmt.Println("4. Divide")

	operation, err := getUserInput(reader)
	if err != nil {
		fmt.Println("Error:", err)
		return
	}

	result := performOperation(num1, num2, operation)
	fmt.Printf("Result: %.2f\n", result)
}

func getUserInput(reader *bufio.Reader) (float64, error) {
	input, err := reader.ReadString('\n')
	if err != nil {
		return 0, err
	}

	num, err := strconv.ParseFloat(input[:len(input)-1], 64)
	if err != nil {
		return 0, err
	}

	return num, nil
}

func performOperation(num1, num2, operation float64) float64 {
	var result float64

	switch operation {
	case 1:
		result = num1 + num2
	case 2:
		result = num1 - num2
	case 3:
		result = num1 * num2
	case 4:
		if num2 != 0 {
			result = num1 / num2
		} else {
			fmt.Println("Error: Cannot divide by zero.")
			os.Exit(1)
		}
	default:
		fmt.Println("Error: Invalid operation.")
		os.Exit(1)
	}

	return result
}
