/*
 * soyac - The Soya programming language compiler
 * Copyright (c) 2009 Denis Washington <dwashington@gmx.net>
 *
 * This file is distributed under the terms of the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef SOYA_INTEGER_TYPE_TEST_H_
#define SOYA_INTEGER_TYPE_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <stdint.h>
#include <ast/BooleanType.h>
#include <ast/IntegerType.h>
#include "UnitTestUtil.h"

using namespace soyac::ast;

class IntegerTypeTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE (IntegerTypeTest);
    CPPUNIT_TEST (testVisit);
    CPPUNIT_TEST (testGet);
    CPPUNIT_TEST (testSize);
    CPPUNIT_TEST (testIsSigned);
    CPPUNIT_TEST (testMin);
    CPPUNIT_TEST (testMinUnsigned);
    CPPUNIT_TEST (testMax);
    CPPUNIT_TEST (testMaxUnsigned);
    CPPUNIT_TEST (testNameSigned);
    CPPUNIT_TEST (testNameUnsigned);
    CPPUNIT_TEST (testNameOfInt);
    CPPUNIT_TEST (testNameOfUInt);
    CPPUNIT_TEST (testConvertable);
    CPPUNIT_TEST (testNotConvertable);
    CPPUNIT_TEST (testImplicitlyConvertable);
    CPPUNIT_TEST (testNotImplicitlyConvertable);
    //CPPUNIT_TEST (testLookupArithmeticMethod);
    //CPPUNIT_TEST (testLookupRelationalMethod);
    //CPPUNIT_TEST (testLookupBitShiftMethod);
    CPPUNIT_TEST_SUITE_END ();

    TEST_VISITOR (IntegerType);

public:
    void setUp()
    {
        mType = IntegerType::get(32, true);
    }

    void tearDown()
    {
    }

    void testVisit()
    {
        IntegerTypeTestVisitor v;
        CPPUNIT_ASSERT (((bool) mType->visit(&v)) == true);
    }

    void testGet()
    {
        CPPUNIT_ASSERT (IntegerType::get(32, true) == mType);
        CPPUNIT_ASSERT (IntegerType::get(32, false) != mType);
        CPPUNIT_ASSERT (IntegerType::get(64, true) != mType);
    }

    void testSize()
    {
        CPPUNIT_ASSERT (mType->size() == 32);
    }

    void testIsSigned()
    {
        CPPUNIT_ASSERT (mType->isSigned() == true);
    }

    void testMin()
    {
        CPPUNIT_ASSERT (mType->min() == IntegerValue(INT32_MIN, true));
    }

    void testMinUnsigned()
    {
        IntegerType* u32 = IntegerType::get(32, false);
        CPPUNIT_ASSERT (u32->min() == IntegerValue(0, false));
    }

    void testMax()
    {
        CPPUNIT_ASSERT (mType->max() == IntegerValue(INT32_MAX, true));
    }

    void testMaxUnsigned()
    {
        IntegerType* u32 = IntegerType::get(32, false);
        CPPUNIT_ASSERT (u32->max() == IntegerValue(UINT32_MAX, false));
    }

    void testNameSigned()
    {
        CPPUNIT_ASSERT (mType->name() == Name("int32"));
    }

    void testNameUnsigned()
    {
        IntegerType* type2 = IntegerType::get(32, false);
        CPPUNIT_ASSERT (type2->name() == Name("uint32"));
    }

    void testNameOfInt()
    {
        CPPUNIT_ASSERT (TYPE_INT->name() == "int");
    }

    void testNameOfUInt()
    {
        CPPUNIT_ASSERT (TYPE_UINT->name() == "uint");
    }

    void testConvertable()
    {
        IntegerType* i32 = IntegerType::get(32, true);
        IntegerType* u32 = IntegerType::get(32, false);
        IntegerType* i64 = IntegerType::get(64, true);

        CPPUNIT_ASSERT (i32->isConvertableTo(i32));
        CPPUNIT_ASSERT (i32->isConvertableTo(u32));
        CPPUNIT_ASSERT (i32->isConvertableTo(i64));
        CPPUNIT_ASSERT (u32->isConvertableTo(i32));
        CPPUNIT_ASSERT (u32->isConvertableTo(i64));
        CPPUNIT_ASSERT (i64->isConvertableTo(i32));
        CPPUNIT_ASSERT (i64->isConvertableTo(u32));
    }

    void testNotConvertable()
    {
        DummyType other;
        CPPUNIT_ASSERT (!mType->isConvertableTo(&other));
    }

    void testImplicitlyConvertable()
    {
        IntegerType* i32 = IntegerType::get(32, true);
        IntegerType* u32 = IntegerType::get(32, false);
        IntegerType* i64 = IntegerType::get(64, true);

        CPPUNIT_ASSERT (i32->isImplicitlyConvertableTo(i32));
        CPPUNIT_ASSERT (i32->isImplicitlyConvertableTo(i64));
        CPPUNIT_ASSERT (u32->isImplicitlyConvertableTo(i64));

    }

    void testNotImplicitlyConvertable()
    {
        IntegerType* i32 = IntegerType::get(32, true);
        IntegerType* u32 = IntegerType::get(32, false);
        IntegerType* i64 = IntegerType::get(64, true);
        DummyType other;

        CPPUNIT_ASSERT (!i64->isImplicitlyConvertableTo(i32));
        CPPUNIT_ASSERT (!i32->isImplicitlyConvertableTo(u32));
        CPPUNIT_ASSERT (!u32->isImplicitlyConvertableTo(i32));
        CPPUNIT_ASSERT (!i32->isImplicitlyConvertableTo(&other));
    }
/*
    void testLookupArithmeticMethod()
    {
        IntegerType* i60 = IntegerType::get(60, true);

        const NodeList<Function> methods = i60->getMethod("plus");
        NodeList<Function>::const_iterator it = methods->begin();

        Function* m;

        m = (Function*) *it;
        CPPUNIT_ASSERT (m->str() == "plus(int60)");
        CPPUNIT_ASSERT (m->returnType() == i60);
        it++;
        m = (Function*) *it;
        CPPUNIT_ASSERT (m->str() == "plus(int61)");
        CPPUNIT_ASSERT (m->returnType() == IntegerType::get(61, true));
        it++;
        m = (Function*) *it;
        CPPUNIT_ASSERT (m->str() == "plus(int62)");
        CPPUNIT_ASSERT (m->returnType() == IntegerType::get(62, true));
        it++;
        m = (Function*) *it;
        CPPUNIT_ASSERT (m->str() == "plus(int63)");
        CPPUNIT_ASSERT (m->returnType() == IntegerType::get(63, true));
        it++;
        m = (Function*) *it;
        CPPUNIT_ASSERT (m->str() == "plus(int64)");
        CPPUNIT_ASSERT (m->returnType() == IntegerType::get(64, true));
        it++;
        CPPUNIT_ASSERT (it == methods->end());
    }

    void testLookupRelationalMethod()
    {
        IntegerType* i60 = IntegerType::get(60, true);

        const NodeList<Function>* methods = i60->getMethod("equals");
        NodeList<Function>::const_iterator it = methods->begin();

        Function* m;

        m = (Function*) *it;
        CPPUNIT_ASSERT (m->str() == "equals(int60)");
        CPPUNIT_ASSERT (m->returnType() == TYPE_BOOL);
        it++;
        m = (Function*) *it;
        CPPUNIT_ASSERT (m->str() == "equals(int61)");
        CPPUNIT_ASSERT (m->returnType() == TYPE_BOOL);
        it++;
        m = (Function*) *it;
        CPPUNIT_ASSERT (m->str() == "equals(int62)");
        CPPUNIT_ASSERT (m->returnType() == TYPE_BOOL);
        it++;
        m = (Function*) *it;
        CPPUNIT_ASSERT (m->str() == "equals(int63)");
        CPPUNIT_ASSERT (m->returnType() == TYPE_BOOL);
        it++;
        m = (Function*) *it;
        CPPUNIT_ASSERT (m->str() == "equals(int64)");
        CPPUNIT_ASSERT (m->returnType() == TYPE_BOOL);
        it++;
        CPPUNIT_ASSERT (it == methods->end());
    }

    void testLookupBitShiftMethod()
    {
        IntegerType* i60 = IntegerType::get(60, true);

        const NodeList<Function>* methods = i60->getMethod("lshift");
        NodeList<Function>::const_iterator it = methods->begin();

        Function* m;

        m = (Function*) *it;
        CPPUNIT_ASSERT (m->str() == "lshift(int)");
        CPPUNIT_ASSERT (m->returnType() == i60);
        it++;
        CPPUNIT_ASSERT (it == methods->end());
    }
*/
private:
    IntegerType* mType;
};

#endif
