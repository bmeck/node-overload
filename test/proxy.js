var Proxy = require("../lib/proxy")
  , sys = require("sys")
  , assert = require("assert")
var x={A:1,B:2},y={B:"B",C:"C"}
var proto_chain = [x,y];
var multi_proto = Proxy.create({
	//GETTER
	"get": function(holder,property){
		for(var i=proto_chain.length-1;i>=0;i--) {
			var proto = proto_chain[i]
			if(property in proto) {
				return proto[property]
			}
		}
		return null
	}
	//SETTER
	, "set": function(holder,property,value) {
		//DONT SET THE CHAINS VALUES!
	}
	//FOREACH
	, "enumerate": function() {
		var properties={}
		for(var i=proto_chain.length-1;i>=0;i--) {
			var proto = proto_chain[i]
			var keys = Object.getOwnPropertyNames(proto)
			keys = keys.filter(function(item){
				return !(item in properties)
			})
			keys.forEach(function(item){
				properties[item]=proto[item]
			})
		}
		return Object.getOwnPropertyNames(properties)
	}
	, "has": function(property) {
		return proto_chain.some(function(item){
			return property in item
		} )
	}
	, "delete": function(ArgInfo) {
	}
	//No DELETER
	}
)
var zz = function(){};
zz.prototype = multi_proto
var z = new zz;
assert.equal(z.A,1,"2nd Chain");
assert.equal(z.B,"B","1st Chain");
assert.equal(z.C,"C","1st Chain");
z.A = 2
delete z.A
assert.equal(z.A,1,"Setter");
assert.equal("A" in z,true,"query")
var hold = []
for(var i in z) {
	hold.push(i)
}
assert.deepEqual(hold,["A","C","B"],"foreach")