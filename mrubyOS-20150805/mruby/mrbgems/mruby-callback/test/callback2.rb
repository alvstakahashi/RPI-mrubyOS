
def handler(n)
	puts "top handler here" 
	puts n.to_s
end


puts "direct call"

handler(109)

puts "from class"

a=Callback.new("handler",5)
b=Callback.new("handler",6)

a.act()
b.act()
