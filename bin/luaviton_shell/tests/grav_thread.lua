------------------------ THREAD ------------------------
print ("----- testing Thread -----")

local mainLoop=function()
    print("Inside Main Loop")
    return;
end

trd = GraVitoN.Core.Thread(mainLoop);
trd:run();

while trd:isActive() do
    print("*")
    GraVitoN.Core.sleep(10)
end

trd:stop()
