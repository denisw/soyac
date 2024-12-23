/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include <cppunit/ui/text/TestRunner.h>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>

#include "unittest/analysis/SymbolTableTest.h"
#include "unittest/ast/ArrayCreationExpressionTest.h"
#include "unittest/ast/ArrayTypeTest.h"
#include "unittest/ast/AssignmentExpressionTest.h"
#include "unittest/ast/BlockTest.h"
#include "unittest/ast/BooleanLiteralTest.h"
#include "unittest/ast/BooleanTypeTest.h"
#include "unittest/ast/BuiltInTypeTest.h"
#include "unittest/ast/CallExpressionTest.h"
#include "unittest/ast/CastExpressionTest.h"
#include "unittest/ast/CharacterLiteralTest.h"
#include "unittest/ast/CharacterTypeTest.h"
#include "unittest/ast/ClassTypeTest.h"
#include "unittest/ast/CombinedRelationalExpressionTest.h"
#include "unittest/ast/CompoundAssignmentExpressionTest.h"
#include "unittest/ast/ConstructorInitializerTest.h"
#include "unittest/ast/ConstructorTest.h"
#include "unittest/ast/DeclarationBlockTest.h"
#include "unittest/ast/DeclarationStatementTest.h"
#include "unittest/ast/DeclaredEntityTest.h"
#include "unittest/ast/DoStatementTest.h"
#include "unittest/ast/EntityImportTest.h"
#include "unittest/ast/EnumConstantTest.h"
#include "unittest/ast/EnumTypeTest.h"
#include "unittest/ast/EnumValueExpressionTest.h"
#include "unittest/ast/ExpressionStatementTest.h"
#include "unittest/ast/ExpressionTest.h"
#include "unittest/ast/FloatingPointLiteralTest.h"
#include "unittest/ast/FloatingPointTypeTest.h"
#include "unittest/ast/FloatingPointValueTest.h"
#include "unittest/ast/ForStatementTest.h"
#include "unittest/ast/FunctionExpressionTest.h"
#include "unittest/ast/FunctionGroupImportTest.h"
#include "unittest/ast/FunctionParameterExpressionTest.h"
#include "unittest/ast/FunctionParameterTest.h"
#include "unittest/ast/FunctionTest.h"
#include "unittest/ast/FunctionTypeTest.h"
#include "unittest/ast/IfStatementTest.h"
#include "unittest/ast/InstanceFunctionExpressionTest.h"
#include "unittest/ast/InstanceVariableExpressionTest.h"
#include "unittest/ast/IntegerLiteralTest.h"
#include "unittest/ast/IntegerTypeTest.h"
#include "unittest/ast/IntegerValueTest.h"
#include "unittest/ast/LinkTest.h"
#include "unittest/ast/LocationTest.h"
#include "unittest/ast/LogicalExpressionTest.h"
#include "unittest/ast/LogicalNotExpressionTest.h"
#include "unittest/ast/ModuleExpressionTest.h"
#include "unittest/ast/ModuleImportTest.h"
#include "unittest/ast/ModuleTest.h"
#include "unittest/ast/NameTest.h"
#include "unittest/ast/NamedEntityTest.h"
#include "unittest/ast/NodeListTest.h"
#include "unittest/ast/NodeTest.h"
#include "unittest/ast/ObjectCreationExpressionTest.h"
#include "unittest/ast/PropertyGetAccessorTest.h"
#include "unittest/ast/PropertySetAccessorTest.h"
#include "unittest/ast/PropertyTest.h"
#include "unittest/ast/ReturnStatementTest.h"
#include "unittest/ast/StatementWithBodyTest.h"
#include "unittest/ast/StructTypeTest.h"
#include "unittest/ast/ThisExpressionTest.h"
#include "unittest/ast/TypeExpressionTest.h"
#include "unittest/ast/TypeTest.h"
#include "unittest/ast/UnknownArrayTypeTest.h"
#include "unittest/ast/UnknownFunctionTypeTest.h"
#include "unittest/ast/UnknownTypeTest.h"
#include "unittest/ast/UnresolvedBaseConstructorInitializerTest.h"
#include "unittest/ast/UnresolvedBinaryExpressionTest.h"
#include "unittest/ast/UnresolvedConstructorInitializerTest.h"
#include "unittest/ast/UnresolvedDefaultConstructorInitializerTest.h"
#include "unittest/ast/UnresolvedElementAccessExpressionTest.h"
#include "unittest/ast/UnresolvedEntityImportTest.h"
#include "unittest/ast/UnresolvedFunctionExpressionTest.h"
#include "unittest/ast/UnresolvedInstanceFunctionExpressionTest.h"
#include "unittest/ast/UnresolvedInstancePropertyExpressionTest.h"
#include "unittest/ast/UnresolvedMemberAccessExpressionTest.h"
#include "unittest/ast/UnresolvedModuleImportTest.h"
#include "unittest/ast/UnresolvedObjectCreationExpressionTest.h"
#include "unittest/ast/UnresolvedSimpleNameExpressionTest.h"
#include "unittest/ast/UnresolvedThisExpressionTest.h"
#include "unittest/ast/UserDefinedTypeTest.h"
#include "unittest/ast/VariableExpressionTest.h"
#include "unittest/ast/VariableTest.h"
#include "unittest/ast/VoidTypeTest.h"
#include "unittest/ast/WhileStatementTest.h"
#include "unittest/common/ProblemTest.h"

namespace fs = std::filesystem;

void
unitTest()
{
    CppUnit::TextUi::TestRunner runner;

    runner.addTest(ArrayCreationExpressionTest::suite());
    runner.addTest(ArrayTypeTest::suite());
    runner.addTest(AssignmentExpressionTest::suite());
    runner.addTest(BlockTest::suite());
    runner.addTest(BooleanLiteralTest::suite());
    runner.addTest(BooleanTypeTest::suite());
    runner.addTest(BuiltInTypeTest::suite());
    runner.addTest(CallExpressionTest::suite());
    runner.addTest(CastExpressionTest::suite());
    runner.addTest(CharacterLiteralTest::suite());
    runner.addTest(CharacterTypeTest::suite());
    runner.addTest(ClassTypeTest::suite());
    runner.addTest(CombinedRelationalExpressionTest::suite());
    runner.addTest(CompoundAssignmentExpressionTest::suite());
    runner.addTest(ConstructorTest::suite());
    runner.addTest(ConstructorInitializerTest::suite());
    runner.addTest(DeclarationBlockTest::suite());
    runner.addTest(DeclarationStatementTest::suite());
    runner.addTest(DeclaredEntityTest::suite());
    runner.addTest(DoStatementTest::suite());
    runner.addTest(EntityImportTest::suite());
    runner.addTest(EnumConstantTest::suite());
    runner.addTest(EnumTypeTest::suite());
    runner.addTest(EnumValueExpressionTest::suite());
    runner.addTest(ExpressionStatementTest::suite());
    runner.addTest(ExpressionTest::suite());
    runner.addTest(FloatingPointLiteralTest::suite());
    runner.addTest(FloatingPointTypeTest::suite());
    runner.addTest(FloatingPointValueTest::suite());
    runner.addTest(ForStatementTest::suite());
    runner.addTest(FunctionExpressionTest::suite());
    runner.addTest(FunctionGroupImportTest::suite());
    runner.addTest(FunctionParameterExpressionTest::suite());
    runner.addTest(FunctionParameterTest::suite());
    runner.addTest(FunctionTest::suite());
    runner.addTest(FunctionTypeTest::suite());
    runner.addTest(IfStatementTest::suite());
    runner.addTest(InstanceFunctionExpressionTest::suite());
    runner.addTest(InstanceVariableExpressionTest::suite());
    runner.addTest(IntegerLiteralTest::suite());
    runner.addTest(IntegerTypeTest::suite());
    runner.addTest(IntegerValueTest::suite());
    runner.addTest(LinkTest::suite());
    runner.addTest(LocationTest::suite());
    runner.addTest(LogicalExpressionTest::suite());
    runner.addTest(LogicalNotExpressionTest::suite());
    runner.addTest(ModuleExpressionTest::suite());
    runner.addTest(ModuleImportTest::suite());
    runner.addTest(ModuleTest::suite());
    runner.addTest(NamedEntityTest::suite());
    runner.addTest(NameTest::suite());
    runner.addTest(NodeListTest::suite());
    runner.addTest(NodeTest::suite());
    runner.addTest(ObjectCreationExpressionTest::suite());
    runner.addTest(ProblemTest::suite());
    runner.addTest(PropertyGetAccessorTest::suite());
    runner.addTest(PropertySetAccessorTest::suite());
    runner.addTest(PropertyTest::suite());
    runner.addTest(ReturnStatementTest::suite());
    runner.addTest(StatementWithBodyTest::suite());
    runner.addTest(StructTypeTest::suite());
    runner.addTest(SymbolTableTest::suite());
    runner.addTest(ThisExpressionTest::suite());
    runner.addTest(TypeExpressionTest::suite());
    runner.addTest(TypeTest::suite());
    runner.addTest(UnknownArrayTypeTest::suite());
    runner.addTest(UnknownFunctionTypeTest::suite());
    runner.addTest(UnknownTypeTest::suite());
    runner.addTest(UnresolvedBaseConstructorInitializerTest::suite());
    runner.addTest(UnresolvedBinaryExpressionTest::suite());
    runner.addTest(UnresolvedConstructorInitializerTest::suite());
    runner.addTest(UnresolvedDefaultConstructorInitializerTest::suite());
    runner.addTest(UnresolvedElementAccessExpressionTest::suite());
    runner.addTest(UnresolvedEntityImportTest::suite());
    runner.addTest(UnresolvedFunctionExpressionTest::suite());
    runner.addTest(UnresolvedInstanceFunctionExpressionTest::suite());
    runner.addTest(UnresolvedInstancePropertyExpressionTest::suite());
    runner.addTest(UnresolvedMemberAccessExpressionTest::suite());
    runner.addTest(UnresolvedModuleImportTest::suite());
    runner.addTest(UnresolvedObjectCreationExpressionTest::suite());
    runner.addTest(UnresolvedSimpleNameExpressionTest::suite());
    runner.addTest(UnresolvedThisExpressionTest::suite());
    runner.addTest(UserDefinedTypeTest::suite());
    runner.addTest(VariableExpressionTest::suite());
    runner.addTest(VariableTest::suite());
    runner.addTest(VoidTypeTest::suite());
    runner.addTest(WhileStatementTest::suite());

    runner.run();
}

void
tortureTest()
{
    std::cout << "Torture Test" << std::endl << "============" << std::endl;

    for (auto& entry : fs::recursive_directory_iterator("torture"))
    {
        if (!entry.is_regular_file() || entry.path().extension() != ".soya")
            continue;

        auto pathString = entry.path().string();
        std::cout << "Processing: " << pathString << "...\n";

        std::string compileCommandPrefix{"../src/soyac -I ../runtime -L ../runtime -s torture/basic/modules --emit-llvm "};
        auto compileCommand = compileCommandPrefix + pathString;
        auto returnCode = std::system(compileCommand.c_str());

        if (returnCode != 0)
        {
            std::cout << ">>> ERROR: soyac exited with error code " << returnCode
                      << "! <<<" << std::endl;

            std::exit(1);
        }

        returnCode = std::system("./a.out");

        if (returnCode != 0)
        {
            std::cout << "*** ERROR: test exited with error code " << returnCode
                      << "! ***" << std::endl;

            std::exit(1);
        }
    }

    std::cout << "=== All tests passed successfully. === " << std::endl;
}


int
main(int argc, char** argv)
{
    unitTest();
    tortureTest();
    return 0;
}
