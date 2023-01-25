#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

//removes /n at the end of your inputs
void remove_crlf(char *s) {
    char *t = s + strlen(s);
    t--;

    while ((t >= s) && (*t == '\n' || *t == '\r')){
        *t = '\0'; 
        t--;
    }
}

//Item structure declaration
typedef struct item_struct {
  double n;
  struct item_struct *next;
} item;


//Linked List structure declaration
typedef struct list_struct {
  item *head;
} list;


//Item Constructor 
item *new_item(double n) {
  item *it = malloc(sizeof(item));

  it->n = n;
  it->next = NULL;

  return it;
}


//List Constructor 
list *new_list(void) {
  list *l = malloc(sizeof(list));

  l->head = NULL;

  return l;
}


//Add item to list
void add_item(list *l, item *it) {
  it->next = l->head;
  l->head = it;
}


//List Destructor
void dispose_list(list *l) {
  item *it = l->head;
  item *next;

  while(it != NULL) {
    next = it->next; // Grab it's next pointer
    free(it); // Free it's memory
    it = next; // Go to the next item
  }

  free(l); // Free the list
}


//Prompts for first input
double prompt(FILE *ofp, char *operator, double *sto, list *b) {
    char input[32];
    double num = 9999999999;

    while(num == 9999999999) {
        //Prompts for first number
        printf("First Input: ");
        fgets(input,31,stdin);
        remove_crlf(input);

        //Checks Input
        if(strcasecmp(input, "PI") == 0) {num = M_PI;} 
        
        else if(strcasecmp(input, "E") == 0) {num = M_E;}

        else if(strcasecmp(input, "C") == 0) {
            fprintf(ofp,"0\n");
            strcpy(operator,"ERROR");
        } 

        else if(strcasecmp(input, "CA") == 0) {
            for(int i = 0; i < 10; i++) { sto[i] = 0; }
            strcpy(operator, "ERROR");
        }

        else if(strcasecmp(input, "Q") == 0) {
            fclose(ofp);
            strcpy(operator,"Q");
            return 0;
        }

        else if(isdigit(input[0]) || input[0] == '-') {num = atof(input);}

        //Does the Whole mess regarding B
        else if(strcasecmp(input, "B") == 0) {
            item *it = b->head;
            strcpy(operator, "B");

            while(it != NULL && strcasecmp(operator, "B") == 0) {
                printf("%lf\n", it->n); //prints number

                //Asks user to input B or Enter
                printf("Type B or ENTER: ");
                fgets(input, 9, stdin);
                if(strcasecmp(input, "\n") != 0) {remove_crlf(input);}

                //If user does 'Enter', it exits the loop
                if(strcasecmp(input, "\n") == 0) {
                    strcpy(operator, "Not B");
                    num = it->n;
                    printf("%lf\n", it->n);
                }   
                it = it->next;
            }
        }

        //If input is invalid
        else { 
            printf("ERROR: invalid input\n"); 
            fprintf(ofp, "ERROR\n"); 
        }
    }
    return num;
}


//Math for Unary operations
double unary(double num, char operator[10], FILE *ofp, double *sto, double *result) {
    //Prints Operator
    if(strcasecmp(operator, "rcl") != 0) {
        fprintf(ofp, "%13.6lf (%s)", num, operator);
        for(int i = 0; i < (13-strlen(operator)); i++) { fprintf(ofp," "); }
    }

    //Checks which operator, and does the math
    if(strcasecmp(operator, "sin") == 0) {result[0] = sin(num);}
    
    else if(strcasecmp(operator, "cos") == 0) {result[0] = cos(num);}

    else if(strcasecmp(operator, "tan") == 0) {result[0] = tan(num);}

    else if(strcasecmp(operator, "arcsin") == 0) {result[0] = asin(num);}

    else if(strcasecmp(operator, "arccos") == 0) {result[0] = acos(num);}

    else if(strcasecmp(operator, "arctan") == 0) {result[0] = atan(num);}

    else if(strcasecmp(operator, "root") == 0) {result[0] = sqrt(num);}

    else if(strcasecmp(operator, "abs") == 0) {result[0] = fabs(num);}

    else if(strcasecmp(operator, "inv") == 0) {result[0] = 1/num;}
    
    else if(strcasecmp(operator, "log") == 0) {result[0] = log(num);}

    else if(strcasecmp(operator, "log2") == 0) {result[0] = log2(num);}

    //Recall
    else if(strcasecmp(operator, "rcl") == 0) {
        if(num >= 0 && num <=9) {
            result[0] = sto[(int)num];
            fprintf(ofp, "       %s(%d)                ", operator, (int)num);
        }
        else {
            strcpy(operator, "ERROR");
            fprintf(ofp,"ERROR\n");
            printf("ERROR\n");
        }  
    }

    return result[0];
}


//Math for Binary operations
double binary(double num, char operator[10], FILE *ofp, double *sto, double *result, list *b) {
    char number[32];
    double num2;
    
    //Asks for second number
    if(strcasecmp(operator, "sto") != 0) {fprintf(ofp, "%13.6lf %s ", num, operator);}
    printf("Second number: ");
    fgets(number,31,stdin);
    remove_crlf(number);

    //Checks if second number is PI or E, or a number
    if(strcasecmp(number, "PI") == 0) {num2 = M_PI;} 
    else if(strcasecmp(number, "E") == 0) {num2 = M_E;}
    else {num2 = atof(number);}

    add_item(b, new_item(num2)); //Adds second number to B

    if(strcasecmp(operator, "sto") != 0) {fprintf(ofp, "%13.6lf", num2);} //Prints second number

    //Does the math
    if(strcasecmp(operator, "+") == 0) {result[0] = num + num2;}
    
    else if(strcasecmp(operator, "-") == 0) {result[0] = num - num2;}

    else if(strcasecmp(operator, "x") == 0) {result[0] = num * num2;}

    else if(strcasecmp(operator, "/") == 0) {result[0] = num/num2;}

    else if(strcasecmp(operator, "^") == 0) {result[0] = pow(num,num2);}

    //Storage
    else if(strcasecmp(operator, "sto") == 0) {
        if(num2 >= 0 && num2 <=9) {
            sto[(int)num2] = result[0]; 
        }
        else {
            strcpy(operator, "ERROR");
            printf("ERROR\n");
            fprintf(ofp,"ERROR\n");
        }      
    }
    return result[0]; 
}


//Special operations
void special(char *operator, FILE *ofp, double *sto) {
    if(strcasecmp(operator, "C") == 0) {
        fprintf(ofp,"0\n");
        strcpy(operator,"ERROR");
    } 

    else if(strcasecmp(operator, "CA") == 0) {
        for(int i = 0; i < 10; i++) { sto[i] = 0; }
        strcpy(operator, "ERROR");
    }

    else if(strcasecmp(operator, "Q") == 0) {
        fclose(ofp);
        strcpy(operator,"Q");
    }
}


//Checks what kind of operator the given string is
int operation_type(char operator[10]) {
    //Checks if operator is unary
    if(strcasecmp(operator,"sin")==0 || strcasecmp(operator,"cos")==0 || strcasecmp(operator,"tan")==0 ||
    strcasecmp(operator,"arcsin")==0 || strcasecmp(operator,"arccos")==0 || strcasecmp(operator,"arctan")==0 ||
    strcasecmp(operator,"root")==0 || strcasecmp(operator,"abs")==0 || strcasecmp(operator,"inv")==0 ||
    strcasecmp(operator,"log")==0 || strcasecmp(operator,"log2")==0 || strcasecmp(operator,"rcl")==0) {return 1;}

    //Checks if operator is binary
    else if(strcasecmp(operator,"+")==0 || strcasecmp(operator,"-")==0 || strcasecmp(operator,"x")==0 || 
    strcasecmp(operator,"/")==0 || strcasecmp(operator,"^")==0 || strcasecmp(operator,"sto")==0) {return 2;}

    //Checks if operator is special
    else if(strcasecmp(operator,"c")==0 || strcasecmp(operator,"ca")==0 || strcasecmp(operator,"q")==0) {return 3;}

    //if input is invalid, it returns 0
    return 0;
}

int main(void) {
    double storage[10];
    char operator[10];
    double result;
    FILE *ofp = fopen("calc_output.txt", "w");
    list *b = new_list();

    //Loop for step 1
    while(strcasecmp(operator, "q") != 0) {
        strcpy(operator, "");

        //Prompts for first input/number
        double num = prompt(ofp, operator, storage, b);
        result = num;
        add_item(b, new_item(num));
        fprintf(ofp, "%13.6lf\n", num);

        //Loop for step 2
        while(strcasecmp(operator, "error") != 0 && strcasecmp(operator, "q") != 0 && fpclassify(num) != 1 ) {

            //Prompts for operator
            printf("Operation: ");
            fgets(operator,31,stdin);
            remove_crlf(operator);

            //Checks if operator is Unary, Binary, Special, or None
            if(operation_type(operator) == 1) {
                num = unary(num, operator, ofp, storage, &result);
                add_item(b, new_item(result));
            }

            else if(operation_type(operator) == 2) {
                num = binary(num, operator, ofp, storage, &result, b);
                add_item(b, new_item(result));
            }

            else if(operation_type(operator) == 3) {special(operator, ofp, storage);}

            else {
                printf("ERROR: Invalid input\n");
                fprintf(ofp, "ERROR\n");
                strcpy(operator, "ERROR");
            }

            //Prints results
            if(strcasecmp(operator, "error") != 0 && strcasecmp(operator, "sto") != 0) {fprintf(ofp," = %13.6lf\n", num);}
            if(strcasecmp(operator,"q") != 0 && strcasecmp(operator, "error") != 0 && strcasecmp(operator, "c") != 0) {printf("Result: %lf\n", num);}
        }
    }
    dispose_list(b);
    return 0; 
}