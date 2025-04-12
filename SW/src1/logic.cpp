#include "logic.h"
#include "VGA.h"
#include <stack>
#include <string>
#include <cmath>

// Scale horizontal 
// Translate horizontal
//GRAPH_Y_SCALE is a constant to scale the Y axis
float scale = 2;
float translate = 0;
const float GRAPH_Y_SCALE = 0.05f;

enum TokenType { NUMBER, VARIABLE, OPERATOR, FUNCTION, PAREN };

struct Token {
    std::string value;
    TokenType type;
};

//draws vertical line between two points
//i is the x coordinate, y1 and y2 are the y coordinates of the two points
// scale and height are used to scale the line to the screen size

void graph(int i,float y1,float y2){

        int off = scale*abs(y1*RHeight - y2*RHeight);
        for(int j = 0 ; j<(1+off) ; j++)
        {
        drawPx(i, (int)(scale*(-y1)*RHeight/2+RHeight/2)+j, colorHex(0, 101, 247, 99));
        }
}

//maps the x coordinate to the screen size
float axis(int i){
 return translate+scale*((float)(2*i)/RWidth-1);
}

//draws the gride on the screen
//i is the x coordinate, off is the y coordinate
// scale and height are used to scale the grid to the screen size

void drawGrid() {
    for (int i = 0; i < RWidth; i++) {
        for (int off = 0; off < RHeight; off++) {
            float ycen = scale * (((float)off) / RHeight - 0.5) * 2.0;
            float ax = axis(i);

            if (fabs(ax) < 0.0025 * scale || fabs(ycen) < 0.0025 * scale) {
                drawPx(i, off, colorHex(0, 70, 77, 129));  // axes
            } else if (fmod(fabs(ax), 0.2) < scale * 0.0025 || fmod(fabs(ycen), 0.2) < scale * 0.0025) {
                drawPx(i, off, colorHex(0, 92, 99, 85));  // grid
            }
        }
    }
    memcpy(vidyaBuffer, drawBuffer, VIDEO_SIZE);
    Xil_DCacheFlush();
}

//updated parser and eval test
//Main changes it that it uses std::vector to generate tokens 

//Converts expression from char buffer and puts it into categories: #, vars, operators, functs
//reads the buffer and creates the appropriate token type

std::vector<Token> tokenize(const std::string& expr) {
    std::vector<Token> tokens;
    size_t i = 0;

    std::string upperExpr = expr;
    for (char& c : upperExpr) c = toupper(c);

    while (i < upperExpr.length()) {
        if (isspace(upperExpr[i])) {
            i++;
            continue;
        }

        if (isdigit(upperExpr[i]) || upperExpr[i] == '.') {
            std::string num;
            while (i < upperExpr.length() && (isdigit(upperExpr[i]) || upperExpr[i] == '.'))
                num += upperExpr[i++];
            tokens.push_back({num, NUMBER});
        } else if (upperExpr.substr(i, 4) == "LOG2") {
            tokens.push_back({"LOG2", FUNCTION}); i += 4;
        } else if (upperExpr.substr(i, 3) == "SIN") {
            tokens.push_back({"SIN", FUNCTION}); i += 3;
        } else if (upperExpr.substr(i, 3) == "COS") {
            tokens.push_back({"COS", FUNCTION}); i += 3;
        } else if (upperExpr.substr(i, 3) == "EXP") {
            tokens.push_back({"EXP", FUNCTION}); i += 3;
        } else if (upperExpr[i] == 'X') {
            tokens.push_back({"X", VARIABLE}); i++;
        } else if (strchr("+-*/", upperExpr[i])) {
            tokens.push_back({std::string(1, upperExpr[i]), OPERATOR}); i++;
        } else if (upperExpr[i] == '[' || upperExpr[i] == ']') {
            tokens.push_back({std::string(1, upperExpr[i]), PAREN}); i++;
        } else {
            xil_printf("Unknown token: %c\n\r", upperExpr[i]);
            AUDIOERROR = TRUE;
            i++;
        }
    }

    return tokens;
}

//Converts the tokens to postfix notation using the shunting yard algorithm
//uses a stack to store the operators and functions, and outputs the tokens in postfix order

std::vector<Token> toPostfix(const std::vector<Token>& tokens) {
    std::vector<Token> output;
    std::stack<Token> ops;

    for (const auto& tok : tokens) {
        if (tok.type == NUMBER || tok.type == VARIABLE) {
            output.push_back(tok);
        } else if (tok.type == FUNCTION) {
            ops.push(tok);
        } else if (tok.type == OPERATOR) {
            while (!ops.empty() && ops.top().type != PAREN && ops.top().type != FUNCTION)
                output.push_back(ops.top()), ops.pop();
            ops.push(tok);
        } else if (tok.value == "[") {
            ops.push(tok);
        } else if (tok.value == "]") {
            while (!ops.empty() && ops.top().value != "[") {
                output.push_back(ops.top());
                ops.pop();
            }
            if (!ops.empty()) ops.pop(); // Remove "["
            if (!ops.empty() && ops.top().type == FUNCTION) {
                output.push_back(ops.top());
                ops.pop();
            }
        }
    }
    while (!ops.empty()) {
        output.push_back(ops.top());
        ops.pop();
    }
    return output;
}

//Evaluates the postfix expression using a stack
//uses a stack to store the operands and functions, and evaluates the expression in postfix order

float evalPostfix(const std::vector<Token>& postfix, float x) {
    std::stack<float> stack;

    for (const auto& tok : postfix) {
        if (tok.type == NUMBER) {
            try {
                stack.push(std::stof(tok.value));
            } catch (...) {
                xil_printf("Invalid number format: %s\n\r", tok.value.c_str());
                AUDIOERROR = TRUE;
                return 0;
            }
        } else if (tok.type == VARIABLE) {
            stack.push(x);
        } else if (tok.type == OPERATOR) {
            if (stack.size() < 2) {
                xil_printf("Operator error: not enough operands for %s\n\r", tok.value.c_str());
                AUDIOERROR = TRUE;
                return 0;
            }
            float b = stack.top(); stack.pop();
            float a = stack.top(); stack.pop();
            if (tok.value == "+") stack.push(a + b);
            else if (tok.value == "-") stack.push(a - b);
            else if (tok.value == "*") stack.push(a * b);
            else if (tok.value == "/") {
                if (b == 0) {
                    xil_printf("Division by zero error\n\r");
                    AUDIOERROR = TRUE;
                    return 0;
                }
                stack.push(a / b);
            }
        } else if (tok.type == FUNCTION) {
            if (stack.empty()) {
                xil_printf("Function error: no operand for %s\n\r", tok.value.c_str());
                AUDIOERROR = TRUE;
                return 0;
            }
            float val = stack.top(); stack.pop();
            if (tok.value == "SIN") {
                int test = int(val * 10e8);
                Xil_Out32(0x43C20004, test);
                float out = 0.6072529353859135 * ((float)(int)Xil_In32(0x43C20000)) / 1e7;
                stack.push(out);
            } else if (tok.value == "COS") {
                stack.push(cosf(val));
            } else if (tok.value == "LOG2") {
                val += -0.0001f; 
                float *inVal = (float *)0x43C30004;
                inVal[0] = val;
                float out = ((float)(int)Xil_In32(0x43C30000)) / pow(2, 23);
                stack.push(out + 5.0f); 
            } else if (tok.value == "EXP") {
                stack.push(expf(val));
            }
        }
    }

    if (stack.empty()) {
        xil_printf("Evaluation error: empty stack at end\n\r");
       AUDIOERROR = TRUE;
        return 0;
    }

    return stack.top() * GRAPH_Y_SCALE;
}

//uses the tokenizing function to generate tokens
//uses the postfix function to convert the tokens to postfix notation
//uses the eval function to evaluate the postfix expression
//uses the graph function to draw the graph on the screen

void readAndDraw(const char* expr) {
    std::vector<Token> tokens = tokenize(expr);
    std::vector<Token> postfix = toPostfix(tokens);

    for (int i = 0; i < RWidth; i++) {
        float x1 = axis(i);       //translate
        float x2 = axis(i + 1);

        float y1 = evalPostfix(postfix, x1);  // still scales Y using GRAPH_Y_SCALE
        float y2 = evalPostfix(postfix, x2);

        graph(i, y1, y2);
    }
}
