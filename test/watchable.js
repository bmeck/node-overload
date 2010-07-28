var Watchable = require("../lib/overload").Watchable
  , sys = require("sys")
  , assert = require("assert")
var proxied={hello:""}
var debug = Watchable(
	//GETTER
	function(ArgInfo){
		//check the type of the object contained by value
		//undefined if no value, or number if using []'s with a number
		var property=ArgInfo.property
		assert.equal(property,"hello","GETTER PROPERTY NAME");
		//sys.puts("GET:"+JSON.stringify(property),"VALUE:"+String(proxied[property]));
		return proxied[property];
	}
	//SETTER
	, function(ArgInfo) {
		var property=ArgInfo.property
		assert.equal(property," world!","SETTER PROPERTY NAME");
		var value=ArgInfo.value
		assert.equal(value,true,"SETTER PROPERTY VALUE");
		//sys.puts("GET:"+JSON.stringify(property),"VALUE:"+String(proxied[property]),"NEW VALUE:"+String(value));
		return ArgInfo.value;
	}
)
debug.hello
debug[" world!"]=true

var x={A:1,B:2},y={B:"B",C:"C"}
var proto_chain = [x,y];
var multi_proto = Watchable(
	//GETTER
	function(ArgInfo){
		//check the type of the object contained by value
		//undefined if no value, or number if using []'s with a number
		var property=ArgInfo.property
		for(var i=proto_chain.length-1;i>=0;i--) {
			var proto = proto_chain[i]
			if(property in proto) {
				return proto[property]
			}
		}
		return null
	}
	//SETTER
	, function(ArgInfo) {
		//DONT SET THE CHAINS VALUES!
	}
	//FOREACH
	, function(ArgInfo) {
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
	, function(ArgInfo) {
		var property=ArgInfo.property
		return proto_chain.some(function(item){
			return property in item
		} )
	}
	//No DELETER
)
var zz = function(){};
zz.prototype = multi_proto
z = new zz;
assert.equal(z.A,1,"2nd Chain");
assert.equal(z.B,"B","1st Chain");
assert.equal(z.C,"C","1st Chain");
z.A = 2
delete z.A
assert.equal(z.A,1,"Setter");
assert.ok("A" in z,"query")
var hold = []
for(var i in z) {
	hold.push(i)
}
assert.deepEqual(hold,["A","C","B"],"foreach")