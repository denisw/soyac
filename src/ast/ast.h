/*
 * soyac - Soya Programming Language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#include "ArrayCreationExpression.h"
#include "ArrayType.h"
#include "AssignmentExpression.h"
#include "Block.h"
#include "BooleanLiteral.h"
#include "BooleanType.h"
#include "CallExpression.h"
#include "CastExpression.h"
#include "CharacterLiteral.h"
#include "CharacterType.h"
#include "ClassType.h"
#include "CombinedRelationalExpression.h"
#include "CompoundAssignmentExpression.h"
#include "Constructor.h"
#include "DeclarationBlock.h"
#include "DeclarationStatement.h"
#include "DeclaredEntity.h"
#include "DoStatement.h"
#include "EntityImport.h"
#include "EnumConstant.h"
#include "EnumType.h"
#include "EnumValueExpression.h"
#include "Expression.h"
#include "ExpressionStatement.h"
#include "FloatingPointLiteral.h"
#include "FloatingPointType.h"
#include "ForStatement.h"
#include "Function.h"
#include "FunctionExpression.h"
#include "FunctionGroupImport.h"
#include "FunctionParameter.h"
#include "FunctionParameterExpression.h"
#include "FunctionType.h"
#include "IfStatement.h"
#include "InstanceFunctionExpression.h"
#include "InstanceVariableExpression.h"
#include "IntegerLiteral.h"
#include "IntegerType.h"
#include "Location.h"
#include "LogicalExpression.h"
#include "LogicalNotExpression.h"
#include "Module.h"
#include "ModuleExpression.h"
#include "ModuleImport.h"
#include "Name.h"
#include "NamedEntity.h"
#include "Node.h"
#include "ObjectCreationExpression.h"
#include "Property.h"
#include "PropertyGetAccessor.h"
#include "PropertySetAccessor.h"
#include "ReturnStatement.h"
#include "Statement.h"
#include "StructType.h"
#include "ThisExpression.h"
#include "Type.h"
#include "TypeExpression.h"
#include "UnknownArrayType.h"
#include "UnknownFunctionType.h"
#include "UnknownType.h"
#include "UnresolvedBaseConstructorInitializer.h"
#include "UnresolvedBinaryExpression.h"
#include "UnresolvedConstructorInitializer.h"
#include "UnresolvedDefaultConstructorInitializer.h"
#include "UnresolvedElementAccessExpression.h"
#include "UnresolvedEntityImport.h"
#include "UnresolvedFunctionExpression.h"
#include "UnresolvedInstanceFunctionExpression.h"
#include "UnresolvedInstancePropertyExpression.h"
#include "UnresolvedMemberAccessExpression.h"
#include "UnresolvedModuleImport.h"
#include "UnresolvedObjectCreationExpression.h"
#include "UnresolvedSimpleNameExpression.h"
#include "UnresolvedThisExpression.h"
#include "Variable.h"
#include "VariableExpression.h"
#include "Visitor.h"
#include "VoidType.h"
#include "WhileStatement.h"
