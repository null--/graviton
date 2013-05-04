function asterix(n)
	if n <= 0 then
		print("I need something more than 0!")
	else
		for i = 1,n,1 do
			io.write("*")
		end
		io.write("\n")
	end
end

io.write("Enter a number:")
a = io.read("*number")
asterix(a)

