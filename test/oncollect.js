var assert = require("assert")
var OnCollect = require("../lib/overload").OnCollect;
var b = {}
  , c = {}
  , waiting = true
b.a = c

OnCollect(b,function(obj) {
	assert.equal(obj.a,c,"revival object validation")
	waiting = false
} )

b = null
assert.equal(b,null,"removal")

setTimeout(
  function wait(){if(waiting)setTimeout(wait,1000)}
  ,1000
)