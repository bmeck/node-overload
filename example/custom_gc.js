var OnCollect = require("../lib/overload").OnCollect
var fibonacci_cache = []
function fibonacci(x) {
	//console.log("CALCULATING FIBONACCI OF "+x)
	if(x == 0) {
		return 0
	}
	if(x == 1)  {
		return 1
	}
	if(x in fibonacci_cache) return fibonacci_cache[x]
	return fibonacci_cache[x] = fibonacci(x-1) + fibonacci(x-2)
}
var collected = false
var x = []
setTimeout(function eatMem() {
	x.push({})
	if(!collected)setTimeout(eatMem,0)
}, 0 )
OnCollect({},function clear_cache(obj){
	collected = true
	fibonacci_cache = []
	console.log("CLEARING CACHE")
	//requeue gc behavior as needed
	OnCollect(obj,clear_cache)
})
console.log(fibonacci(200))