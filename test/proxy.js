var Watchable = require("../lib/overload").Watchable
  , sys = require("sys")
var proxied={hello:""}
var debug = Watchable(
	//GETTER
	function(ArgInfo){
		//check the type of the object contained by value
		//undefined if no value, or number if using []'s with a number
		var property=ArgInfo.property
		sys.puts("GET:"+JSON.stringify(property),"VALUE:"+String(proxied[property]));
		return proxied[property];
	}
	//SETTER
	, function(ArgInfo) {
		var property=ArgInfo.property
		var value=ArgInfo.value
		sys.puts("GET:"+JSON.stringify(property),"VALUE:"+String(proxied[property]),"NEW VALUE:"+String(value));
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
		for(var i=proto_chain.length;i>=0;i--) {
			var proto = proto_chain[i]
			if(property in proto) {
				return proto[property]
			}
		}
		return undefined
	}
	//SETTER
	, function(ArgInfo) {
		//DONT SET THE CHAINS VALUES!
	}
	, function(ArgInfo) {

	}
	, function(ArgInfo) {
		var property=ArgInfo.property
		return proto_chain.some(function(item){return property in item})
	}
)

var z = function(){};
z.prototype = multi_proto
z = new z;
console.log(z.A);
console.log(z.B);
console.log(z.C);