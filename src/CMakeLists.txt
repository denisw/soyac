BISON_TARGET(SoyaParser
  parser/soya.yy
  ${CMAKE_CURRENT_SOURCE_DIR}/parser/parser.cpp)

set(COMPILER_SOURCES
  ${BISON_SoyaParser_OUTPUTS}
  analysis/BasicAnalyzer.cpp
  analysis/BasicAnalyzer.h
  analysis/ControlFlowAnalyzer.cpp
  analysis/ControlFlowAnalyzer.h
  analysis/FunctionGroup.cpp
  analysis/FunctionGroup.h
  analysis/ModulesRequiredException.h
  analysis/NavigatingVisitor.cpp
  analysis/NavigatingVisitor.h
  analysis/SymbolTable.cpp
  analysis/SymbolTable.h
  ast/ArrayCreationExpression.cpp
  ast/ArrayCreationExpression.h
  ast/ArrayType.cpp
  ast/ArrayType.h
  ast/AssignmentExpression.cpp
  ast/AssignmentExpression.h
  ast/Block.cpp
  ast/Block.h
  ast/BooleanLiteral.cpp
  ast/BooleanLiteral.h
  ast/BooleanType.cpp
  ast/BooleanType.h
  ast/BuiltInType.cpp
  ast/BuiltInType.h
  ast/CallExpression.cpp
  ast/CallExpression.h
  ast/CastExpression.cpp
  ast/CastExpression.h
  ast/CharacterLiteral.cpp
  ast/CharacterLiteral.h
  ast/CharacterType.cpp
  ast/CharacterType.h
  ast/ClassType.cpp
  ast/ClassType.h
  ast/CombinedRelationalExpression.cpp
  ast/CombinedRelationalExpression.h
  ast/CompoundAssignmentExpression.cpp
  ast/CompoundAssignmentExpression.h
  ast/Constructor.cpp
  ast/Constructor.h
  ast/ConstructorInitializer.cpp
  ast/ConstructorInitializer.h
  ast/DeclarationBlock.cpp
  ast/DeclarationBlock.h
  ast/DeclarationStatement.cpp
  ast/DeclarationStatement.h
  ast/DeclaredEntity.cpp
  ast/DeclaredEntity.h
  ast/DoStatement.cpp
  ast/DoStatement.h
  ast/EntityImport.cpp
  ast/EntityImport.h
  ast/EnumConstant.cpp
  ast/EnumConstant.h
  ast/EnumType.cpp
  ast/EnumType.h
  ast/EnumValueExpression.cpp
  ast/EnumValueExpression.h
  ast/Expression.cpp
  ast/Expression.h
  ast/ExpressionStatement.cpp
  ast/ExpressionStatement.h
  ast/FloatingPointLiteral.cpp
  ast/FloatingPointLiteral.h
  ast/FloatingPointType.cpp
  ast/FloatingPointType.h
  ast/FloatingPointValue.cpp
  ast/FloatingPointValue.h
  ast/ForStatement.cpp
  ast/ForStatement.h
  ast/Function.cpp
  ast/Function.h
  ast/FunctionExpression.cpp
  ast/FunctionExpression.h
  ast/FunctionGroupImport.cpp
  ast/FunctionGroupImport.h
  ast/FunctionParameter.cpp
  ast/FunctionParameter.h
  ast/FunctionParameterExpression.cpp
  ast/FunctionParameterExpression.h
  ast/FunctionType.cpp
  ast/FunctionType.h
  ast/IfStatement.cpp
  ast/IfStatement.h
  ast/Import.cpp
  ast/Import.h
  ast/InstanceFunctionExpression.cpp
  ast/InstanceFunctionExpression.h
  ast/InstanceVariableExpression.cpp
  ast/InstanceVariableExpression.h
  ast/IntegerLiteral.cpp
  ast/IntegerLiteral.h
  ast/IntegerType.cpp
  ast/IntegerType.h
  ast/IntegerValue.cpp
  ast/IntegerValue.h
  ast/Link.h
  ast/Location.cpp
  ast/Location.h
  ast/LogicalExpression.cpp
  ast/LogicalExpression.h
  ast/LogicalNotExpression.cpp
  ast/LogicalNotExpression.h
  ast/Module.cpp
  ast/Module.h
  ast/ModuleExpression.cpp
  ast/ModuleExpression.h
  ast/ModuleImport.cpp
  ast/ModuleImport.h
  ast/Name.cpp
  ast/Name.h
  ast/NamedEntity.cpp
  ast/NamedEntity.h
  ast/Node.cpp
  ast/Node.h
  ast/NodeList.h
  ast/ObjectCreationExpression.cpp
  ast/ObjectCreationExpression.h
  ast/Property.cpp
  ast/Property.h
  ast/PropertyGetAccessor.cpp
  ast/PropertyGetAccessor.h
  ast/PropertySetAccessor.cpp
  ast/PropertySetAccessor.h
  ast/ReturnStatement.cpp
  ast/ReturnStatement.h
  ast/Statement.cpp
  ast/Statement.h
  ast/StatementWithBody.cpp
  ast/StatementWithBody.h
  ast/StructType.cpp
  ast/StructType.h
  ast/ThisExpression.cpp
  ast/ThisExpression.h
  ast/Type.cpp
  ast/Type.h
  ast/TypeExpression.cpp
  ast/TypeExpression.h
  ast/UnknownArrayType.cpp
  ast/UnknownArrayType.h
  ast/UnknownFunctionType.cpp
  ast/UnknownFunctionType.h
  ast/UnknownType.cpp
  ast/UnknownType.h
  ast/UnresolvedBaseConstructorInitializer.cpp
  ast/UnresolvedBaseConstructorInitializer.h
  ast/UnresolvedBinaryExpression.cpp
  ast/UnresolvedBinaryExpression.h
  ast/UnresolvedConstructorInitializer.cpp
  ast/UnresolvedConstructorInitializer.h
  ast/UnresolvedDefaultConstructorInitializer.cpp
  ast/UnresolvedDefaultConstructorInitializer.h
  ast/UnresolvedElementAccessExpression.cpp
  ast/UnresolvedElementAccessExpression.h
  ast/UnresolvedEntityImport.cpp
  ast/UnresolvedEntityImport.h
  ast/UnresolvedFunctionExpression.cpp
  ast/UnresolvedFunctionExpression.h
  ast/UnresolvedInstanceFunctionExpression.cpp
  ast/UnresolvedInstanceFunctionExpression.h
  ast/UnresolvedInstancePropertyExpression.cpp
  ast/UnresolvedInstancePropertyExpression.h
  ast/UnresolvedMemberAccessExpression.cpp
  ast/UnresolvedMemberAccessExpression.h
  ast/UnresolvedModuleImport.cpp
  ast/UnresolvedModuleImport.h
  ast/UnresolvedObjectCreationExpression.cpp
  ast/UnresolvedObjectCreationExpression.h
  ast/UnresolvedSimpleNameExpression.cpp
  ast/UnresolvedSimpleNameExpression.h
  ast/UnresolvedThisExpression.cpp
  ast/UnresolvedThisExpression.h
  ast/UserDefinedType.cpp
  ast/UserDefinedType.h
  ast/Variable.cpp
  ast/Variable.h
  ast/VariableExpression.cpp
  ast/VariableExpression.h
  ast/Visitor.h
  ast/VoidType.cpp
  ast/VoidType.h
  ast/WhileStatement.cpp
  ast/WhileStatement.h
  ast/ast.h
  codegen/CodeGenerator.cpp
  codegen/CodeGenerator.h
  codegen/LLVMTypeMapper.cpp
  codegen/LLVMTypeMapper.h
  codegen/LLValueExpression.cpp
  codegen/LLValueExpression.h
  codegen/mangling.cpp
  codegen/mangling.h
  common/PassResult.cpp
  common/PassResult.h
  common/PassResultBuilder.cpp
  common/PassResultBuilder.h
  common/Problem.cpp
  common/Problem.h
  driver/FileProcessor.cpp
  driver/FileProcessor.h
  driver/ProblemReport.cpp
  driver/ProblemReport.h
  driver/config.cpp
  driver/config.h
  driver/link.cpp
  driver/link.h
  parser/Lexer.cpp
  parser/Lexer.h
  parser/ParserDriver.cpp
  parser/ParserDriver.h
)

source_group("Analysis" REGULAR_EXPRESSION "analysis/.*\\.[ch]pp")
source_group("AST" REGULAR_EXPRESSION "ast/.*\\.[ch]pp")
source_group("CodeGen" REGULAR_EXPRESSION "codegen/.*\\.[ch]pp")
source_group("Common" REGULAR_EXPRESSION "common/.*\\.[ch]pp")
source_group("Driver" REGULAR_EXPRESSION "driver/.*\\.[ch]pp")
source_group("Parser" REGULAR_EXPRESSION "parser.*\\.[ch]pp")

add_library(soya-compiler ${COMPILER_SOURCES})

target_include_directories(soya-compiler PUBLIC
  ${LLVM_INCLUDE_DIRS}
  ${CMAKE_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR})

separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
target_compile_definitions(soya-compiler PUBLIC ${LLVM_DEFINITIONS_LIST})

llvm_map_components_to_libnames(SOYAC_LLVM_LIBS
  core
  mc
  support
  nativecodegen
  target)

target_link_libraries(soya-compiler
  ${SOYAC_LLVM_LIBS}
  Boost::headers
  Boost::program_options
  Boost::regex)

add_executable(soyac driver/main.cpp)
target_link_libraries(soyac soya-compiler)

