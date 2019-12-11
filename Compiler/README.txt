Currently, yylex() is written by hand in the utilities section of parse.y. Write a flex program
named scan.l that handles the lexical analysis in the same way. Running flex on scan.l will then produce
the needed yylex() function, so you remove the hand-written one. Note that variable names in SEE are
single letters and case insensitive. (All attributes are integers, which is the default.) Edit the Makefile to
accommodate scan.l (which includes adding lex.yy.o to the list of OBJECTS). To let bison and flex work
together, you need to do two things:
a. Use the "-d" option when running bison. This (along with the "-y" option) generates the header file
y.tab.h for use by scan.l. (Edit the Makefile.)
b. Insert the line
#include "y.tab.h"
inside the %{ and %} delimiters in the preamble of scan.l. (Note: if you include other files in your
scan.l preamble, you should place them above the include directive for y.tab.h, so they are included
first; otherwise, you may get gcc compiler errors.)
c. Update your Makefile to include the new files and their dependencies: The final executable now
also depends on lex.yy.o, which depends on lex.yy.c, which in turn depends on both scan.l and
y.tab.h. Add y.tab.h as an additional target along with y.tab.c.

Alter the grammer in parse.y to allow for unary prefix + and - (the first operator does nothing,
the second negates its argument). The syntax rules are as follows: The first term in an expression may
optionally be preceded with a single '+' or '-', and that is the only place it can appear. The operator applies
to the whole first term. This gives unary + and - the same precedence and associativity as their binary
counterparts. Thus for example,
- 2*x + 5 is evaluated as (-(2x))+5,
- - 3 is a syntax error, but -(-3) is ok,
3*-x is a syntax error, but 3*(-x) is ok.
In particular, you cannot apply two of these operators in a row without anything in between. NOTE: This
is how Pascal parses these operators, but not how C/C++/Java handles them. In C/C++/Java, all unary
operators have precedence higher than all binary operators.

Note: If you decide to implement the last task (full extra credit), then you will need to rewrite some of the
code you use for this task. If you implement the last two tasks fully, you will also get credit for this task as
well. We now want to remember each expression entered (the expression itself, not its value) and be able to
refer to it by number. The first expr entered has number 1, the second has number 2, etc. The idea is to be
able to re-evalate expressions with variables after the variables change. Here is a sample session using my
renamed executable "see2". Each number followed by a colon is a computer-generated prompt giving the
number of the expression to be entered.
