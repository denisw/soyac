/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include <cstdlib>
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <cppunit/ui/text/TestRunner.h>

#include <llvm/System/Program.h>

#include "unittest/analysis/SymbolTableTest.hpp"
#include "unittest/ast/ArrayCreationExpressionTest.hpp"
#include "unittest/ast/ArrayTypeTest.hpp"
#include "unittest/ast/AssignmentExpressionTest.hpp"
#include "unittest/ast/BlockTest.hpp"
#include "unittest/ast/BooleanLiteralTest.hpp"
#include "unittest/ast/BooleanTypeTest.hpp"
#include "unittest/ast/BuiltInTypeTest.hpp"
#include "unittest/ast/CallExpressionTest.hpp"
#include "unittest/ast/CastExpressionTest.hpp"
#include "unittest/ast/CharacterLiteralTest.hpp"
#include "unittest/ast/CharacterTypeTest.hpp"
#include "unittest/ast/ClassTypeTest.hpp"
#include "unittest/ast/CombinedRelationalExpressionTest.hpp"
#include "unittest/ast/CompoundAssignmentExpressionTest.hpp"
#include "unittest/ast/ConstructorTest.hpp"
#include "unittest/ast/ConstructorInitializerTest.hpp"
#include "unittest/ast/DeclarationBlockTest.hpp"
#include "unittest/ast/DeclarationStatementTest.hpp"
#include "unittest/ast/DeclaredEntityTest.hpp"
#include "unittest/ast/DoStatementTest.hpp"
#include "unittest/ast/EntityImportTest.hpp"
#include "unittest/ast/EnumConstantTest.hpp"
#include "unittest/ast/EnumTypeTest.hpp"
#include "unittest/ast/EnumValueExpressionTest.hpp"
#include "unittest/ast/ExpressionStatementTest.hpp"
#include "unittest/ast/ExpressionTest.hpp"
#include "unittest/ast/FloatingPointLiteralTest.hpp"
#include "unittest/ast/FloatingPointTypeTest.hpp"
#include "unittest/ast/FloatingPointValueTest.hpp"
#include "unittest/ast/ForStatementTest.hpp"
#include "unittest/ast/FunctionExpressionTest.hpp"
#include "unittest/ast/FunctionParameterExpressionTest.hpp"
#include "unittest/ast/FunctionParameterTest.hpp"
#include "unittest/ast/FunctionGroupImportTest.hpp"
#include "unittest/ast/FunctionTest.hpp"
#include "unittest/ast/FunctionTypeTest.hpp"
#include "unittest/ast/IfStatementTest.hpp"
#include "unittest/ast/InstanceFunctionExpressionTest.hpp"
#include "unittest/ast/InstanceVariableExpressionTest.hpp"
#include "unittest/ast/IntegerLiteralTest.hpp"
#include "unittest/ast/IntegerTypeTest.hpp"
#include "unittest/ast/IntegerValueTest.hpp"
#include "unittest/ast/LinkTest.hpp"
#include "unittest/ast/LocationTest.hpp"
#include "unittest/ast/LogicalExpressionTest.hpp"
#include "unittest/ast/LogicalNotExpressionTest.hpp"
#include "unittest/ast/ModuleExpressionTest.hpp"
#include "unittest/ast/ModuleImportTest.hpp"
#include "unittest/ast/ModuleTest.hpp"
#include "unittest/ast/NamedEntityTest.hpp"
#include "unittest/ast/NameTest.hpp"
#include "unittest/ast/NodeListTest.hpp"
#include "unittest/ast/NodeTest.hpp"
#include "unittest/ast/ObjectCreationExpressionTest.hpp"
#include "unittest/ast/PropertyGetAccessorTest.hpp"
#include "unittest/ast/PropertySetAccessorTest.hpp"
#include "unittest/ast/PropertyTest.hpp"
#include "unittest/ast/ReturnStatementTest.hpp"
#include "unittest/ast/StatementWithBodyTest.hpp"
#include "unittest/ast/StructTypeTest.hpp"
#include "unittest/ast/ThisExpressionTest.hpp"
#include "unittest/ast/TypeExpressionTest.hpp"
#include "unittest/ast/TypeTest.hpp"
#include "unittest/ast/UnknownArrayTypeTest.hpp"
#include "unittest/ast/UnknownFunctionTypeTest.hpp"
#include "unittest/ast/UnknownTypeTest.hpp"
#include "unittest/ast/UnresolvedBaseConstructorInitializerTest.hpp"
#include "unittest/ast/UnresolvedBinaryExpressionTest.hpp"
#include "unittest/ast/UnresolvedConstructorInitializerTest.hpp"
#include "unittest/ast/UnresolvedDefaultConstructorInitializerTest.hpp"
#include "unittest/ast/UnresolvedElementAccessExpressionTest.hpp"
#include "unittest/ast/UnresolvedEntityImportTest.hpp"
#include "unittest/ast/UnresolvedFunctionExpressionTest.hpp"
#include "unittest/ast/UnresolvedInstanceFunctionExpressionTest.hpp"
#include "unittest/ast/UnresolvedInstancePropertyExpressionTest.hpp"
#include "unittest/ast/UnresolvedMemberAccessExpressionTest.hpp"
#include "unittest/ast/UnresolvedModuleImportTest.hpp"
#include "unittest/ast/UnresolvedObjectCreationExpressionTest.hpp"
#include "unittest/ast/UnresolvedSimpleNameExpressionTest.hpp"
#include "unittest/ast/UnresolvedThisExpressionTest.hpp"
#include "unittest/ast/UserDefinedTypeTest.hpp"
#include "unittest/ast/VariableExpressionTest.hpp"
#include "unittest/ast/VariableTest.hpp"
#include "unittest/ast/VoidTypeTest.hpp"
#include "unittest/ast/WhileStatementTest.hpp"
#include "unittest/common/ProblemTest.hpp"

using namespace boost::filesystem;


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
    const boost::regex sourceFile(".*\\.soya");

    path paths[] = {
      path("test/torture/basic"),
      path("test/torture/basic/conversions"),
      path("test/torture/basic/expressions"),
      path("test/torture/basic/functions"),
      path("test/torture/basic/modules"),
      path("test/torture/basic/properties"),
      path("test/torture/basic/scope"),
      path("test/torture/basic/statements"),
      path("test/torture/basic/types/arrays"),
      path("test/torture/basic/types/class"),
      path("test/torture/basic/types/enums"),
      path("test/torture/basic/types/function"),
      path("test/torture/basic/types/ints"),
      path("test/torture/basic/types/structs"),
      path("")
    };

    std::cout << "Torture Test" << std::endl <<
                 "============" << std::endl;

    for (path* p = paths; p->string() != ""; p++)
    {
        directory_iterator it(*p);
        directory_iterator end;

        for (; it != end; it++)
        {
            if (!regex_match(it->path().string(), sourceFile))
                continue;

            std::cout <<
              "Processing torture test file " <<
              it->path().string().c_str() + 13 << "..." <<
              std::endl;

            llvm::sys::Path soyac("./soyac");

            if (!soyac.exists())
            {
                std::cerr << "cannot find soyac build" << std::endl;
                std::exit(1);
            }

            const char* argv[] = {
                "soyac",
                "-I", "./runtime",
                "-s", p->string().c_str(),
                it->path().string().c_str(),
                NULL
            };

            int code = llvm::sys::Program::ExecuteAndWait(soyac, argv);

            if (code != 0)
            {
                std::cout <<
                  ">>> ERROR: soyac exited with error code " << code <<
                  "! <<<" << std::endl;

                std::exit(1);
            }

            llvm::sys::Path test("./a.out");
            const char* argv2[] = {"a.out", NULL};

            code = llvm::sys::Program::ExecuteAndWait(test, argv2);

            if (code != 0)
            {
                std::cout <<
                  "*** ERROR: test exited with error code " << code <<
                  "! ***" << std::endl;

                std::exit(1);
            }
        }
    }

    std::cout << "=== All tests passed successfully. === " << std::endl;
}


int
main(int argc, char **argv)
{
    unitTest();
    tortureTest();

    return 0;
}
