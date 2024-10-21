cmake_minimum_required(VERSION 3.20)
project(Chinstrap)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_BUILD_TYPE Debug)

add_executable(Chinstrap src/main.cpp
        src/Lexer.cpp
        src/Lexer.h
        src/Parser.cpp
        src/Parser.h
        src/ASTNode.h
        src/ASTNode.cpp
        src/Visitor.cpp
        src/Visitor.h
        src/parslet/Parslet.h
        src/parslet/Parslet.cpp
        src/Precedence.h
        src/parslet/InfixParslet.cpp
        src/parslet/InfixParslet.h
        src/parslet/PrefixParslet.cpp
        src/parslet/PrefixParslet.h
        src/parslet/GroupParslet.cpp
        src/parslet/GroupParslet.h
        src/parslet/PostfixParslet.cpp
        src/parslet/PostfixParslet.h
        src/parslet/BraceParslet.h
        src/parslet/BraceParslet.cpp
        src/parslet/StatementParslet.h
        src/parslet/StatementParslet.cpp
        src/parslet/FunctionDefinitionParslet.h
        src/parslet/FunctionDefinitionParslet.cpp
        src/Exception.h
        src/Units.h
        src/parslet/ListParslet.cpp
        src/parslet/ListParslet.h
        src/types/Literal.h
        src/NodeForward.h
        src/types/Collection.h
        src/Visit.h
        src/repl/Cli.cpp
        src/repl/Cli.h
        src/function/Function.h
        src/function/Function.cpp
        src/function/NativeFunction.h
        src/function/PrintFunction.h
        src/function/PrintFunction.cpp
        src/function/SingleParameterMathFunction.h
        src/function/SingleParameterMathFunction.cpp
        src/types/Literal.cpp
        src/util/StringFormat.h
        src/types/OperationVisitor.h
)
