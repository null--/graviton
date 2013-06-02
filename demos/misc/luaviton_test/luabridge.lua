print("Beginning Test...")

print ("----- testing callback -----")
print(func)
f1 = func
-- callback_func(f1)

print ("----- testing A -----")
gvn.A.testStatic()
print(gvn.A.testStaticProp)

a_obj = A("HELL'o C")
a_obj:testVirtual()

name = a_obj:getName()
print(name)

print(a_obj.testProp)
a_obj:testPropSet(23)
print(a_obj:testPropGet())

print ("----- testing B -----")
B.testStatic()
b_obj = B("I'm B!")
b_obj:testVirtual()
b_obj:testPropSet(24)
print(b_obj:testPropGet())

print ("----- testing funcs -----")
print( testRetStdString() )
testParamStdStringRef("Back to C")

testData(a_obj)
print(a_obj.testProp)
