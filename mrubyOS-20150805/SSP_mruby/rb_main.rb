class Mycyclic < Ssp_cyclic
	def initialize(id,thread)
		super("cyclic",id)
		@thread = thread
	end
	def cyclic(n)
		puts "cyclic here " + n.to_s	
		@thread.iact
	end
end

class Mythread < Ssp_thread
	def initialize(id,led)
		super("thread",id)
		@led = led
	end
	def thread(n)
		puts "thread here " + n.to_s
		@led.toggle
	end
end
class Mythread_gc < Mythread
	def thread(n)
		super(n)
		GC.start
		puts n.to_s + "GC Finish!"
	end
end


  GC.interval_ratio = 20
  GC.step_ratio = 200

  act    = Led.new(Led::ACT)
  green  = Led.new(Led::GREEN)

  act.off
  green.off

  thread_act  = Mythread.new(3,act)
  thread_green = Mythread_gc.new(4,green)
  cyclic_act   = Mycyclic.new(1,thread_act)
  cyclic_green = Mycyclic.new(2,thread_green)
  cyclic_act.start
  cyclic_green.start

  puts "mruby setup finish!"


