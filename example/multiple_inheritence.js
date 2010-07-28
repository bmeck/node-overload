var Proxy = require("../lib/proxy")
var PrototypeChain = function(prototypes) {
  return Proxy.create({
  	//GETTER
  	"get": function(holder,property){
  		for(var i=prototypes.length-1;i>=0;i--) {
  			var proto = prototypes[i]
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
  		for(var i=prototypes.length-1;i>=0;i--) {
  			var proto = prototypes[i]
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
  		return prototypes.some(function(item){
  			return property in item
  		} )
  	}
  	, "delete": function(ArgInfo) {
  	}
  } )
}
function mySuper() {}
mySuper.prototype.fly = function() {
  console.log("SOARING AT THE SPEED OF SOUND")
}
mySuper.prototype.run = function() {
  console.log("RUNNING FOR 10 HOURS")
}
function mySuperduper() {}
mySuperduper.prototype = new mySuper
mySuperduper.prototype.fly = function() {
  console.log("SOARING AT THE SPEED OF LIGHT")
}

function myIdentity() {}
myIdentity.prototype.id = function() {
	console.log("I am a mild mannered human")
}
var proto = PrototypeChain([new mySuperduper,new myIdentity])
function myself(){}
myself.prototype = proto

var me = new myself
me.fly()
me.run()
me.id()
