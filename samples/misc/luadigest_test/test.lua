-- test md5 library

print ("begin")

function report(w,s,F)
 print(w,s.."  "..F)
 assert(s==KNOWN)
end

function test(D,known)
 if D==nil then
    print "FAILD"
    return
 end
 
 KNOWN=known
 print""
 print(D.version)
 print""

 assert(io.input(F))
 report("all",D.digest(io.read"*a"),F)

 assert(io.input(F))
 d=D.new()
 while true do
  local c=io.read(1)
  if c==nil then break end
  d:update(c)
 end
 report("loop",d:digest(),F)
 report("again",d:digest(),F)

 assert(io.input(F))
 d:reset()
 while true do
  local c=io.read(math.random(1,16))
  if c==nil then break end
  d:update(c)
 end
 report("reset",d:digest(),F)

 report("known",KNOWN,F)

 local A="hello"
 local B="world"
 local C=A..B
 local a=D.digest(C)
 local b=d:reset():update(C):digest()
 assert(a==b)
 local b=d:reset():update(A,B):digest()
 assert(a==b)
end

F="ENCRYPTME"

test(md4,'0f4cb881959ef738381cc0d10a8c2ad4')
test(md5,'2ce7462b9f6a90fe9de273e461e105c0')
test(sha1,'39de833fa46eee39020a6a9f3aaddfe82b4c3bd2')
test(sha224,'2c706e30148b73f5dfe05cead76118cff842489f7676bf083556b9f4')
test(sha256,'ec88028aff5481f9243b13e3352d3f0494008fe6ac4b49335f0bd1e58b335072')
test(sha384,'245ba4b96ebd000a3d9c8aea8cf5b6996403c7ba9c6607ccc2f6b4460033d963d5cc8089ae961b3435f7b0c734c4a343')
test(sha512,'cf2c0dd42e29c50e1ad74ff318fea37221c0923ae78899e109d1a436e4681c192aac540f9f5a89c3da5bef0d2b8a2ffc3898d3478b68c923830b09972de8aa7f')
test(ripemd160,'fdad406ee66e781a49a7496525edb0653cbf4667')

-- eof
