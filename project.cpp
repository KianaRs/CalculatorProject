#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <cctype>
#include <cmath>
#include <stdexcept>
#include <sstream>
class Stack{
private:
    std::vector<double>elements;
public:
    void push(double value){
        elements.push_back(value);
    }
    
    double pop() {
        if (elements.empty()) {
            throw std::out_of_range("Stack is empty");
        }
        double value = elements.back();
        elements.pop_back();
        return value;
    }
    
    double top(){
        if (elements.empty()){
            throw std::out_of_range("Stack is empty");
        }
        return elements.back();
    }
    
    bool isEmpty(){
        return elements.empty();
   }
};

int precedence(char op){
    if(op=='+'||op=='-'){
        return 1;
    }
    if(op=='*'||op=='/') {
        return 2;
    }
    if(op=='^') {
        return 3;
    }
    return 0;
}

std::string infixToPostfix(const std::string& infix) {
    std::stack<char> opStack;
    std::string postfix;
    for (size_t i=0; i< infix.size(); i++){
        if (isspace(infix[i])) continue;
        
        if (isdigit(infix[i]) || infix[i]== '.') {
            postfix +=infix[i];
            if (i== infix.size() - 1 || (!isdigit(infix[i + 1]) && infix[i + 1] != '.')){
                postfix +=' ';
            }
        }
        else if (infix[i]== '('){
            opStack.push(infix[i]);
        }
        else if (infix[i]== ')') {
            while (!opStack.empty() && opStack.top() != '(') {
                postfix +=opStack.top();
                postfix += ' ';
                opStack.pop();
            }
            if (!opStack.empty()) opStack.pop();
        }
        else {
            while (!opStack.empty() && precedence(opStack.top())>= precedence(infix[i]))
            {
                postfix += opStack.top();
                postfix += ' ';
                opStack.pop();
            }
            opStack.push(infix[i]);
        }
    }
    while (!opStack.empty())
    {
        postfix += opStack.top();
        postfix += ' ';
        opStack.pop();
    }
    return postfix;
}

double evaluatePostfix(const std::string& postfix)
{
    Stack stack;
    std::istringstream iss(postfix);
    std::string token;
    while (iss >> token) {
        if (isdigit(token[0]) || (token.size() > 1 && token[0]== '-')) {
            stack.push(std::stod(token));
        } else 
        {
            double b = stack.pop();
            double a = stack.pop();
            if (token== "+") stack.push(a + b);
            else if (token== "-") stack.push(a - b);
            else if (token== "*") stack.push(a * b);
            else if (token== "/") 
            {
                if (b==0) throw std::runtime_error("Division by zero");
                stack.push(a / b);
            }
            else if (token== "^") stack.push(std::pow(a, b));
        }
    }
    return stack.pop();
}

bool isValidExpression(const std::string& expr) {
    std::stack<char> parentheses;
    bool expectOperator = false;
    for (size_t i= 0; i < expr.size(); i++) {
        if (isspace(expr[i])) continue;
        
        if (isdigit(expr[i]) || expr[i]== '.') {
            expectOperator = true;
            continue;
        }
        
        if (expr[i]== '(') {
            parentheses.push('(');
        } else if (expr[i]== ')') 
        {
            if (parentheses.empty()) return false;
            parentheses.pop();
        } else if (strchr("+-*/^", expr[i])) 
        {
            if (!expectOperator && (i== 0 || expr[i-1] != ')')) return false;
            expectOperator = false;
        } else 
        {
            return false;
        }
    }
    return parentheses.empty() && expectOperator;
}

int main() 
{
    std::string input;
    std::cout<< "Enter a mathematical expression: ";
    std::getline(std::cin, input);
    
    try {
        if (!isValidExpression(input)) 
        {
            std::cout <<"Error: Invalid expression"<< std::endl;
            return 1;
        }
        
        std::string postfix = infixToPostfix(input);
        double result = evaluatePostfix(postfix);
        std::cout <<"Result: " << result << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Error: "<< e.what() << std::endl;
    }
}